#pragma once

// Log macros
#define LOG_TRACE(s, ...)	::Logger::formatMessage(0, LogCategory::Trace, s, __VA_ARGS__)
#define LOG_INFO(s, ...)	::Logger::formatMessage(0, LogCategory::Info, s, __VA_ARGS__)
#define LOG_WARN(s, ...)	::Logger::formatMessage(0, LogCategory::Warn, s, __VA_ARGS__)
#define LOG_ERROR(s, ...)	::Logger::formatMessage(0, LogCategory::Error, s, __VA_ARGS__)


// Log categories
enum class LogCategory
{
	Trace,
	Info,
	Warn,
	Error
};

namespace Logger {
	// Final format loop, move to printing
	void formatMessage(int count, LogCategory type, std::string m);

	// Add variables into string.
	template <typename T, typename... Types>
	void formatMessage(int count, LogCategory type, std::string m, const T& arg1, const Types&... arg2)
	{
		// Go through and find all input spots
		std::stringstream ss;
		ss << "{" << count << "}";
		size_t index = m.find(ss.str());
		// Print error if more argument exist then message slots available
		if (index == std::string::npos)
		{
			int prev_count = count + 1;
			count = 0;
			LOG_ERROR("Logger missing argument {0} in string '{1}'!", prev_count, m);
			return;
		}

		// Insert variable into message
		std::stringstream ss2;
		ss2 << arg1;
		m.replace(index, ss.str().length(), ss2.str());

		// Move on to next value
		formatMessage(count + 1, type, m, arg2...);
	}

}


