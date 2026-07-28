/**
 * @file    DebuggingInstance.h
 * @brief   Wrap up for personal debugger
 *
 * @details  It wraps up a debugger just to log trace
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */
 
#pragma once

#pragma warning(push)
#pragma warning(disable: 4996)  // Para deprecation MSG_WARNINGs generales
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#include <vulkan/vulkan.hpp>  
#include <vector>
#include <string>
#include <mutex>
#include <memory>

#pragma warning(pop)


namespace Engine
{
	class ISink;

	class DebuggingInstance
	{
	public:


        /**
        * @brief  It retrieves the static reference for the debugger  
        * @return the debuggimg instance
        */
		static DebuggingInstance* get()
		{
			static std::unique_ptr<DebuggingInstance> instance;

			if (!instance)
				instance = std::make_unique<DebuggingInstance>();
			return instance.get();
		}

		/**
        * @brief  It destroys the debugging instance  
        * @return void
        */
	    void destroy()
		{
			VkInstance instance = reinterpret_cast<VkInstance>(appInstance);

			if (instance) {

				auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
				if (func != nullptr && mDebugMessenger)
				{
					func(instance, mDebugMessenger, nullptr);
				}
			}
		}

		enum class LogLevel
		{
			Trace,
			Info,
			Warning,
			Error,
			Fatal
		};


		/**
        * @brief  Constructs the instance  
        * @return none 
        */
		DebuggingInstance();

		/**
        * @brief  Destroys the instance  
        * @return none 
        */
		~DebuggingInstance();

		DebuggingInstance(const DebuggingInstance&) = delete;
		DebuggingInstance& operator=(const DebuggingInstance&) = delete;

		/**
        * @brief  It returns the timestamp
        * @param[in] computePath: the compute shader path
        * @return none 
        */
		std::string timestamp() const;
		
		/**
        * @brief  It returns changes from level to string
        * @param[in] level: the level of the logger
        * @return the level as a string
        */
		std::string levelToString(LogLevel level) const;

		/**
        * @brief  It dumps the log of the message
        * @param[in] level: the level of the logger
        * @param[in] msg: the message 
        * @param[in] file: where to dump
        * @param[in] line: line of the code
        * @return none
        */
		void log(LogLevel level, const std::string& msg, const char* file, int line);

		/**
        * @brief  checks the support of the validation layers IN VULKAN
        * @return none
        */
		bool checkValidationLayerSupport();

#ifdef  VULKAN_API // for future vulkan compatibility

		bool CheckGLFWExtensions(const std::vector<VkExtensionProperties>& extensions) const;
		const std::vector<const char*>& GetValidationLayers() { return mValidationLayers; }
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void setupDebugMessenger(VkInstance instance, bool eval);
#endif 

	private:
		//validation layers vector
		const std::vector<const char*> mValidationLayers = { "VK_LAYER_KHRONOS_validation" };

		//debug messenger for vulkan 
		VkDebugUtilsMessengerEXT mDebugMessenger;

		//sink format
		std::vector<std::unique_ptr<ISink>> mSinks;
		std::mutex mMutex;

		void* appInstance;
	};
}
