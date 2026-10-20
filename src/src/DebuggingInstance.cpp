/**
 * @file    DebuggingInstance.cpp
 * @brief   Wrap up for personal debugger
 *
 * @details  It wraps up a debugger just to log trace
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */

#include "DebuggingInstance.h"
#include "Log.h"
#include "Sink.h"
//#include <cerrno>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <fmt/color.h>

namespace Debug
{
    std::string DebuggingInstance::levelToString(LogLevel level) const
    {
        switch (level)
        {
        case LogLevel::Trace:
            return "TRACE";
        case LogLevel::Info:
            return "INFO";
        case LogLevel::Warning:
            return "WARNING";
        case LogLevel::Error:
            return "ERROR";
        case LogLevel::Fatal:
            return "FATAL";
        default:
            return "UNKNOWN";
        }
    }
    
    DebuggingInstance::DebuggingInstance() : appInstance(nullptr)
    {
        
    #ifdef VULKAN_API
        mDebugMessenger = nullptr;
    #endif

        // we add a console sink by default, but we could add more sinks here (like file sink)
        mSinks.push_back(std::make_unique<ConsoleSink>());
        mSinks.push_back(std::make_unique<FileSink>("log.txt"));
    }
    
    DebuggingInstance::~DebuggingInstance() = default;
    
    std::string DebuggingInstance::timestamp() const {
    
        auto now = std::chrono::system_clock::now();
        auto local = std::chrono::zoned_time{ std::chrono::current_zone(), now };
        return std::format("{:%H:%M:%S}", local);
    }
    
    void DebuggingInstance::log(LogLevel level, const std::string& msg,
                     const char* file, int line)
    {
    
        // construimos el mensaje formateado
        std::ostringstream ss;
        ss << "[" << timestamp()            << "]"
           << "[" << levelToString(level)   << "] "
           << msg
           << "  (" << file << ":" << line << ")";
    
        std::string formatted = ss.str();
    
        // escribimos a todos los sinks de forma thread-safe
        {
            std::lock_guard<std::mutex> lock(mMutex);
            for (auto& sink : mSinks)
                sink->write(formatted, level);
        }

        if(level == LogLevel::Error || level == LogLevel::Fatal)
            throw std::runtime_error{formatted.c_str()};
    }
    
#ifdef VULKAN_API

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) 
    {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    
        return VK_FALSE;
    }


    bool DebuggingInstance::CheckGLFWExtensions(const std::vector<VkExtensionProperties>& extensions) const
    {
        unsigned extensionNumber = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionNumber);
    
        if (!glfwVulkanSupported())
        {
            LOG_ERROR("GLFW reports Vulkan is not supported");
        }
    
        // if we dont hace a extension lost of glfw then dont do it
        if (glfwExtensions == nullptr || extensionNumber == 0)
        {
            LOG_ERROR("No GLFW extensions found - AN ERROR OCCURRED");
        }
    
        //for each extension
        for (int i = 0; i < extensionNumber; i++)
        {
            bool value = false;
    
            // see if all the GLFW needed extenions are found in my extensions 
            for (auto& extension : extensions)
            {
                if (!strcmp(extension.extensionName, glfwExtensions[i]))
                {
                    value = true;
                    LOG_INFO("\tGLFW needed extension " + std::string(extension.extensionName) + " was found\n");
                }
            }
    
            //is it found? if not abort
            if (!value)
            {
                try {
                    LOG_ERROR("\tERROR - GLFW needed extension " + std::string(glfwExtensions[i]) + " was not found");
                }
                catch (const std::runtime_error& e)
                {
                    fmt::print(fg(fmt::color::red) | fmt::emphasis::bold, "\tRUNTIME ERROR - {} \n", e.what());
                }
            }
        }
    
        return true;
    }
    
    void DebuggingInstance::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
        createInfo.pUserData = nullptr; // Optional
    
    }
    
    void DebuggingInstance::setupDebugMessenger(VkInstance instance, bool eval)
    {
        //don`t need to debug
        if (!eval)
            return;
    
        //check for the severuty, message types and suchh
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        populateDebugMessengerCreateInfo(createInfo);
    
        if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &mDebugMessenger) != VK_SUCCESS) {
            LOG_ERROR("failed to set up debug messenger!");
        }
    
        appInstance = instance;
    }

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) 
    {
        // catch the original function
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        
        //if that function exists then we use it
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }
    
    
#endif
    
}
