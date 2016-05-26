/*!
	\file Progress.h
	\brief Definitions of progress reporting functions.
*/

#pragma once

#include <Platform/Fwd.h>
#include <string>

namespace Platform {

	// Helper object to manage the progress display.
	// {
	//   Progress p1("Solving", 10);
	//   for (int i = 0; i < 10; ++i, ++p1) 
	//   {
	//     // do something
	//   }
	// } // end progress p1
	class Progress {
		public:
			Progress(const std::string& str, int max);
			virtual ~Progress();

			void setNumSteps(int);
			void step(int steps = 1);
			void done();

			int progress() const { return m_curr; }

			Progress& operator++ () { step(); return *this; }
			Progress& operator+= (int steps) { step(steps); return *this; }

		private:
			std::string m_str;
			int m_curr, m_max;
			float m_frac;
			bool m_done;
	};

	void consoleProgress(float amountDone, const char message[]);
	void consoleStartProgress(const char message[]);
	void consoleEndProgress(const char message[]);
	void consoleStartBusy(const char message[]);
	void consoleBusy();
	void consoleEndBusy();

} // namespace Platform
