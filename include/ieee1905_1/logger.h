/**
 * logger.h
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#pragma once

#include <memory>
#include <spdlog/spdlog.h>

namespace ieee1905_1 {

class Logger {
public:
    static void init();
    static std::shared_ptr<spdlog::logger>& get();
};

} // namespace ieee1905_1

#define EM_TRACE(...)  ::ieee1905_1::Logger::get()->trace(__VA_ARGS__)
#define EM_DEBUG(...)  ::ieee1905_1::Logger::get()->debug(__VA_ARGS__)
#define EM_INFO(...)   ::ieee1905_1::Logger::get()->info(__VA_ARGS__)
#define EM_WARN(...)   ::ieee1905_1::Logger::get()->warn(__VA_ARGS__)
#define EM_ERROR(...)  ::ieee1905_1::Logger::get()->error(__VA_ARGS__)
#define EM_CRITICAL(...) ::ieee1905_1::Logger::get()->critical(__VA_ARGS__)
