/*
	\file Progress.cpp
	\brief Functions to display a progress bar in the commandline
	\author Wojciech Jarosz

	A command-line implementation of the progress bar. The code tries to figure
	out the size of the terminal window to allow for the largest possible
	progress bar. This portion of the code is taken from the wget project. The
	original license follows below.
 
	------------------------------------------------------------------------

	Download progress.
	Copyright (C) 2001, 2002 Free Software Foundation, Inc.

	This file is part of GNU Wget.

	GNU Wget is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	GNU Wget is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Wget; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

	In addition, as a special exception, the Free Software Foundation
	gives permission to link the code of its release of Wget with the
	OpenSSL project's "OpenSSL" library (or with modified versions of it
	that use the same license as the "OpenSSL" library), and distribute
	the linked executables.  You must obey the GNU General Public License
	in all respects for all of the code used other than "OpenSSL".  If you
	modify this file, you may extend this exception to your version of the
	file, but you are not obligated to do so.  If you do not wish to do
	so, delete this exception statement from your version.
*/

#if HAVE_CONFIG_H
#  include <config.h>
#endif // HAVE_CONFIG_H

#include "Progress.h"
#include "Stopwatch.h"

#include <vector>
#include <string>
#include <iostream>
#include <string.h>

#include <string.h>

#ifndef _WIN32

// For TIOCGWINSZ and friends:
#ifdef HAVE_SYS_IOCTL_H
# include <sys/ioctl.h>
#endif
#ifdef HAVE_TERMIOS_H
# include <termios.h>
#endif

#else

#ifdef NOMINMAX
    #undef NOMINMAX
#endif
#define NOMINMAX
#include <windows.h>

#ifndef snprintf
#define snprintf _snprintf
#endif 

#endif


#include <signal.h>

namespace Platform
{

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::flush;

namespace
{

// Assumed screen width if we can't find the real value.
#define DEFAULT_SCREEN_WIDTH 79

// The last known screen width.  This can be updated by the code that
// detects that SIGWINCH was received (but it's never updated from the
// signal handler).
int screenWidth = 0;

// A flag that, when set, means SIGWINCH was received.
bool receivedSIGWINCH = false;
bool monitoringSIGWINCH = false;

vector<char> progressBuffer;
string ticks;
int currentTick = 0;
Stopwatch progressTimer;
Stopwatch progressUpdateTimer;

string    busyMessage;
Stopwatch busyTimer;
Stopwatch busyUpdateTimer;
unsigned  busyRotation;

// Determine the width of the terminal we're running on.  If that's
// not possible, return 0.
int
determineScreenWidth()
{
    // If there's a way to get the terminal size using POSIX
    // tcgetattr(), somebody please tell me.
#ifdef TIOCGWINSZ
    int fd;
    struct winsize wsz;
    
    fd = fileno (stderr);
    if (ioctl (fd, TIOCGWINSZ, &wsz) < 0)
        return 0;                   /* most likely ENOTTY */
    
    return wsz.ws_col;
#elif defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &csbi))
        return 0;
    return csbi.dwSize.X - 1;
#else /* neither _WIN32 nor TIOCGWINSZ */
    return 0;
#endif /* not TIOCGWINSZ */
}

#ifdef SIGWINCH
void
progressHandleSIGWINCH (int sig)
{
    receivedSIGWINCH = true;
    signal(SIGWINCH, progressHandleSIGWINCH);
}
#endif


inline void
resetPlusses(const char message[])
{
    // Initialize screenWidth if this hasn't been done or if it might
    // have changed, as indicated by receiving SIGWINCH.
    if (!screenWidth || receivedSIGWINCH)
    {
        screenWidth = determineScreenWidth();
        if (!screenWidth)
            screenWidth = DEFAULT_SCREEN_WIDTH;
        receivedSIGWINCH = false;
    }

    size_t msgLen = strlen(message);
    size_t progLen = std::max(size_t(screenWidth+1), size_t(62));
    size_t tickLen = std::max(size_t(1), size_t(screenWidth - 52 - msgLen));

	progressBuffer = vector<char>(progLen, ' ');
	ticks = string(tickLen, ' ');
	currentTick = 0;
}


inline void
printTime(int s, int & i)
{
    int hrs = s / 3600;
    s %= 3600;
    int mins = s / 60;
    s %= 60;
    int secs = s;
            
    if (hrs == 0)
        i += snprintf(&progressBuffer[i], progressBuffer.size()-i, "%02d:%02d   ", mins, secs);
    else
        i += snprintf(&progressBuffer[i], progressBuffer.size()-i, "%02d:%02d:%02d", hrs, mins, secs);
}

} // namespace


void
consoleProgress(float amountDone, const char message[])
{
    // The progress bar should look like this:
    // message:  xx% [=======>   ]  ELP xx:xx:xx  ETA xx:xx:xx  TOT xx:xx:xx
    // 
    // Calculate the geometry.  The idea is to assign as much room as
    // possible to the progress bar.  The other idea is to never let
    // things "jitter", i.e. pad elements that vary in size so that
    // their variance does not affect the placement of other elements.
    // It would be especially bad for the progress bar to be resized
    // randomly.
    // 
    // Element                 Description                Length (in chars)
    // ---------------------------------------------------------------------
    // message:              - progress message         - strlen(message) + 2
    // " xx% " or "100% "    - percentage               - 5
    // "[]"                  - progress bar decorations - 2
    // "  ELP xx:xx:xx"      - elapsed time             - 14
    // "  ETA xx:xx:xx"      - estimated remaining time - 14
    // "  TOT xx:xx:xx"      - estimated total time     - 14
    // 
    // "=====>..."           - progress bar             - the rest


    resetPlusses(message);

    amountDone = std::min(std::max(amountDone, 0.0f), 1.0f);
    int percentage = int(amountDone*100);
        
    int oldPlus = currentTick;
    while (currentTick < amountDone * (int(ticks.size())-1))
        ticks[currentTick++] = '=';
    ticks[currentTick] = (percentage < 100) ? '>' : '=';
        
    if (oldPlus != currentTick || progressUpdateTimer.elapsedSeconds() > 2.0f ||
        amountDone == 0.0f)
    {
        // Update elapsed time and estimated time to completion
        int elp = int(progressTimer.elapsedSeconds());
//         int cpu = int(progressTimer.cpuSeconds());
        int eta = int(elp/amountDone - elp);

        printf("\r");
        int i = snprintf(&progressBuffer[0], progressBuffer.size(), "%s: ", message);
    
        int j;
        if (percentage < 100)
            j = snprintf(&progressBuffer[i], progressBuffer.size()-i, " %2d%% ", percentage);
        else
            j = snprintf(&progressBuffer[i], progressBuffer.size()-i, "100%% ");
        i += j;

        j = snprintf(&progressBuffer[i], progressBuffer.size()-i, "[%s]", ticks.c_str());
        i += j;

        j = snprintf(&progressBuffer[i], progressBuffer.size()-i, "  ELP ");
        i += j;
        printTime(elp, i);
        
        j = snprintf(&progressBuffer[i], progressBuffer.size()-i, "  ETA ");
        i += j;
        printTime(eta, i);
        
        j = snprintf(&progressBuffer[i], progressBuffer.size()-i, "  TOT ");
        i += j;
        printTime(elp + eta, i);

        int len = std::min(screenWidth, int(progressBuffer.size()));
        for (int j = 0; j < len; ++j)
            printf("%c", progressBuffer[j]);
        fflush(stdout);
        progressUpdateTimer.reset();
    }
}


void
consoleStartProgress(const char message[])
{
#ifdef SIGWINCH
    if (!monitoringSIGWINCH)
    {
        signal(SIGWINCH, progressHandleSIGWINCH);
        monitoringSIGWINCH = true;
    }
#endif

    progressTimer.start();
    progressUpdateTimer.start();
    
    resetPlusses(message);
    snprintf(&progressBuffer[0], progressBuffer.size(),
            "\r%s:   0%% [%s]  ELP --:--     ETA --:--     TOT --:--   ",
           message, ticks.c_str());
           
    int len = std::min(screenWidth, int(progressBuffer.size()));
    for (int j = 0; j < len; ++j)
        printf("%c", progressBuffer[j]);

    fflush(stdout);
}


void
consoleEndProgress(const char message[])
{
    resetPlusses(message);

    while (currentTick < int(ticks.size()))
        ticks[currentTick++] = '=';
        
    // Update elapsed time and estimated time to completion
    int elp = int(progressTimer.elapsedSeconds());
    int cpu = int(progressTimer.cpuSeconds());

    int i = 0;
    i += snprintf(&progressBuffer[i], progressBuffer.size()-i,
                  "\r%s: 100%% [%s]", message, ticks.c_str());
    i += snprintf(&progressBuffer[i], progressBuffer.size()-i, "  ELP ");
    printTime(elp, i);
    i += snprintf(&progressBuffer[i], progressBuffer.size()-i, "  CPU ");
    printTime(cpu, i);

    for (size_t j = i; j < progressBuffer.size(); ++j)
        progressBuffer[j] = ' ';

    int len = std::min(screenWidth, int(progressBuffer.size()));
    for (int j = 0; j < len; ++j)
        printf("%c", progressBuffer[j]);
    printf("\n");
    fflush(stdout);
}


void
consoleStartBusy(const char message[])
{
    busyMessage = message;
    busyTimer.start();
    busyUpdateTimer.start();
    busyRotation = 0;

    cout << "\r" << message << "... |" << flush;
}


void
consoleBusy()
{
    static const char chars[4] = {'/', '-', '\\', '|'};
    if (busyUpdateTimer.elapsedSeconds() > 0.5f)
    {
        ++busyRotation;
        busyRotation = busyRotation % 4;
        cout << "\r" << busyMessage << "... " << chars[busyRotation] << flush;
        busyUpdateTimer.reset();
    }
}


void
consoleEndBusy()
{
    cout << "\r" << busyMessage << "... done ";
    float secondsCPU = (float) busyTimer.cpuSeconds();
    float secondsElapsed = (float) busyTimer.elapsedSeconds();
    printf("(%.2f cpu s, %.2f elapsed s)\n", secondsCPU, secondsElapsed);
    fflush(stdout);
}


Progress::Progress(const string & str, int max) :
    m_str(str), m_curr(0), m_max(max), m_frac(max == 0 ? 1.0f : 1.0f / max),
    m_done(false)
{
    consoleStartProgress(m_str.c_str());
}
    

Progress::~Progress() 
{
    if (!m_done)
        done();
}


void
Progress::done()
{
    consoleEndProgress(m_str.c_str());
    m_done = true;
}
    
void
Progress::setNumSteps(int steps)
{
    m_max = steps;
    
    // recompute other variables
    m_frac = m_max == 0 ? 1.0f : 1.0f / m_max;
}


void 
Progress::step(int steps)
{
    m_curr += steps;

    // check if the counter is out of range.
    if (m_curr > m_max)
		std::cout << "Error: Progress out of range " << m_str << " " << m_curr << " " << m_max << std::endl;

    consoleProgress(m_curr * m_frac, m_str.c_str());
}

} // namespace Platform
