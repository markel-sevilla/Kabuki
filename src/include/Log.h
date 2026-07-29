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

#define LOG_TRACE(msg)   Debug::DebuggingInstance::get()->log(Debug::DebuggingInstance::LogLevel::Trace,   msg, __FILE__, __LINE__)
#define LOG_INFO(msg)    Debug::DebuggingInstance::get()->log(Debug::DebuggingInstance::LogLevel::Info,     msg, __FILE__, __LINE__)
#define LOG_WARNING(msg) Debug::DebuggingInstance::get()->log(Debug::DebuggingInstance::LogLevel::Warning,  msg, __FILE__, __LINE__)
#define LOG_ERROR(msg)   Debug::DebuggingInstance::get()->log(Debug::DebuggingInstance::LogLevel::Error,    msg, __FILE__, __LINE__)
#define LOG_FATAL(msg)   Debug::DebuggingInstance::get()->log(Debug::DebuggingInstance::LogLevel::Fatal,    msg, __FILE__, __LINE__)