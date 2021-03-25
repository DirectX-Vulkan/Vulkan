#pragma once

#include "Resources.h"
#include <vector>

#define vector std::vector
#define runtime_error std::runtime_error

const vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

class Debug {
public:
	Debug();
	void CleanUp();
	
	void InitialiseDebug(VkInstance instance);
	bool CheckValidationLayerSupport();
	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

private:
	VkInstance* m_instanceptr = nullptr;
	VkDebugUtilsMessengerEXT m_debugMessenger;
};

