#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace SV::CORE {

	class Log {
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger>& GetLogger() { return _logger; }
	private:
		static std::shared_ptr<spdlog::logger> _logger;
	};
}

#ifdef _DEBUG
#define CAD_TRACE(...)    ::SV::CORE::Log::GetLogger()->trace(__VA_ARGS__)
#define CAD_INFO(...)     ::SV::CORE::Log::GetLogger()->info(__VA_ARGS__)
#define CAD_WARN(...)     ::SV::CORE::Log::GetLogger()->warn(__VA_ARGS__)
#define CAD_ERROR(...)    ::SV::CORE::Log::GetLogger()->error(__VA_ARGS__)
#define CAD_CRITICAL(...) ::SV::CORE::Log::GetLogger()->critical(__VA_ARGS__)
#else 
#define CAD_TRACE(...)    
#define CAD_INFO(...)     
#define CAD_WARN(...)     
#define CAD_ERROR(...)    
#define CAD_CRITICAL(...) 
#endif