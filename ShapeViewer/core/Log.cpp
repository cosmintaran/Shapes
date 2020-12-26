#include "stdafx.h"
#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace SV::CORE {

	std::shared_ptr<spdlog::logger> Log::_logger;

	void Log::Init()
	{
		std::array<spdlog::sink_ptr,2> logSinks;

		logSinks[0] = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		logSinks[1] = std::make_shared<spdlog::sinks::basic_file_sink_mt>("EngineLog.log", true);

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		_logger = std::make_shared<spdlog::logger>("Engine", begin(logSinks), end(logSinks));
		spdlog::register_logger(_logger);
		_logger->set_level(spdlog::level::trace);
		_logger->flush_on(spdlog::level::trace);
	}
}
