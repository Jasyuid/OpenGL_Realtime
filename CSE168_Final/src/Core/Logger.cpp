#include "oglpch.h"
#include "Logger.h"

// Set Windows console color
#define SET_CONSOLE_COLOR(x) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);

namespace Logger {

	// Window console colors
	enum WIN_FG_COLORS
	{
		FG_BLACK = 0,
		FG_BLUE = 1,
		FG_GREEN = 2,
		FG_CYAN = 3,
		FG_RED = 4,
		FG_MAGENTA = 5,
		FG_BROWN = 6,
		FG_WHITE = 7,
		FG_GRAY = 8,
		FG_LIGHTBLUE = 9,
		FG_LIGHTGREEN = 10,
		FG_LIGHTCYAN = 11,
		FG_LIGHTRED = 12,
		FG_PINK = 13,
		FG_YELLOW = 14,
		FG_LIGHTWHITE = 15
	};

	// Prints out log message
	void printMessage(LogCategory type, std::string m)
	{
		// Set the text color
		switch (type)
		{
		case LogCategory::Trace:
			SET_CONSOLE_COLOR(FG_GREEN);
			break;
		case LogCategory::Info:
			SET_CONSOLE_COLOR(FG_WHITE);
			break;
		case LogCategory::Warn:
			SET_CONSOLE_COLOR(FG_YELLOW);
			break;
		case LogCategory::Error:
			SET_CONSOLE_COLOR(FG_RED);
			break;
		}


		// Get the current time
		time_t now = time(0);
		struct tm time_info;
		localtime_s(&time_info, &now);
		// Print the time
		if (time_info.tm_hour < 10)
			std::cout << "[0" << time_info.tm_hour << ":";
		else
			std::cout << "[" << time_info.tm_hour << ":";
		if (time_info.tm_min < 10)
			std::cout << "0" << time_info.tm_min << ":";
		else
			std::cout << time_info.tm_min << ":";
		if (time_info.tm_sec < 10)
			std::cout << "0" << time_info.tm_sec << "] ";
		else
			std::cout << time_info.tm_sec << "] ";

		// Print the message
		std::cout << m << std::endl;

		// Reset the text color
		SET_CONSOLE_COLOR(FG_WHITE);
	}

	void formatMessage(int count, LogCategory type, std::string m)
	{
		printMessage(type, m);
	}

}
