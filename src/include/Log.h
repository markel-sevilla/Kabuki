// Log.h

/**
 * @file    Log.h
 * @brief   Static library for debugging
 *
 * @details  It wraps up a debugger just to log trace
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */

#pragma once
#include "DebuggingInstance.h"

#define LOG_TRACE(msg)   Engine::DebuggingInstance::get()->log(Engine::DebuggingInstance::LogLevel::Trace,   msg, __FILE__, __LINE__)
#define LOG_INFO(msg)    Engine::DebuggingInstance::get()->log(Engine::DebuggingInstance::LogLevel::Info,     msg, __FILE__, __LINE__)
#define LOG_WARNING(msg) Engine::DebuggingInstance::get()->log(Engine::DebuggingInstance::LogLevel::Warning,  msg, __FILE__, __LINE__)
#define LOG_ERROR(msg)   Engine::DebuggingInstance::get()->log(Engine::DebuggingInstance::LogLevel::Error,    msg, __FILE__, __LINE__)
#define LOG_FATAL(msg)   Engine::DebuggingInstance::get()->log(Engine::DebuggingInstance::LogLevel::Fatal,    msg, __FILE__, __LINE__)