/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

Copyright (c) 2019-2026, PardCode

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#pragma once
#include <DX3D/Core/Core.h>
#include <format>

namespace dx3d
{
	class Logger final
	{
		dx3d_disable_copy_and_move(Logger)
	public:
		enum class LogLevel
		{
			Error = 0,
			Warning,
			Info
		};

		explicit Logger(LogLevel logLevel = LogLevel::Error);
		~Logger();

		template<typename... Args>
		void log(LogLevel level, std::format_string<Args...> fmt, Args&&... args)
		{
			auto str = std::format(fmt, std::forward<Args>(args)...);
			_log(level,
				str.c_str()
			);
		}
	private:
		void _log(LogLevel level, const char* message);
	private:
		LogLevel m_logLevel = LogLevel::Error;
	};
}

#define DX3DLog(logger, type, message,...)\
	logger.log((type), {message} __VA_OPT__(,) __VA_ARGS__);

#define DX3DLogThrow(logger, exception, type, message, ...)\
{\
DX3DLog(logger,type,message, __VA_ARGS__);\
throw exception(message);\
}

#define DX3DLogInfo(message,...)\
	DX3DLog(getLogger(), Logger::LogLevel::Info, message, __VA_ARGS__)

#define DX3DLogWarning(message,...)\
	DX3DLog(getLogger(), Logger::LogLevel::Warning, message, __VA_ARGS__)

#define DX3DLogError(message,...)\
	DX3DLog(getLogger(), Logger::LogLevel::Error, message, __VA_ARGS__)

#define DX3DLogThrowError(message,...)\
	DX3DLogThrow(getLogger(), std::runtime_error, Logger::LogLevel::Error, message, __VA_ARGS__)

#define DX3DLogThrowInvalidArg(message,...)\
	DX3DLogThrow(getLogger(), std::invalid_argument, Logger::LogLevel::Error, message, __VA_ARGS__)