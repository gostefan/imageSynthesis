// UnitTests.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "TestResult.h"

#include <iostream>

int main()
{
	test::TestResult result;
	std::cout << result << std::endl;

	// used to keep the command line open until user input
	int i;
	std::cin >> i;
	return 0;
}

