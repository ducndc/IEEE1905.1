/**
 * logger.cpp
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#include "ieee1905_1/logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace ieee1905_1 {

static std::shared_ptr<spdlog::logger> g_logger;

void Logger::init() 
{
    if (!g_logger) {
        g_logger = spdlog::stdout_color_mt("ieee1905_1");
        spdlog::set_level(spdlog::level::debug);   // default = debug
        spdlog::flush_on(spdlog::level::err);      // flush when error
    }
}

std::shared_ptr<spdlog::logger>& Logger::get() 
{
    if (!g_logger) {
        init();
    }

    return g_logger;
}

} // namespace ieee1905_1


