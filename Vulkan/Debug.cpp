#include "Debug.h"


// debug severities
#define VK_VERBOSE VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
#define VK_INFO VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
#define VK_WARNING VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
#define VK_ERROR VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT

// debug types
#define VK_DEBUG_GENERAL VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
#define VK_DEBUG_VALIDATION VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
#define VK_DEBUG_PERFORMANCE VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT

Debug::Debug()
{
  
}

void Debug::CleanUp()
{
    //check if the instance isn't already destoryed
    if ((void *) *m_instanceptr != (void *) 0xcccccccccccccccc) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*m_instanceptr, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
            func(*m_instanceptr, m_debugMessenger, nullptr);
    }
}

void Debug::InitialiseDebug(VkInstance instance)
{
    if (!CheckValidationLayerSupport())
        throw runtime_error("Validation layers requested, but not available!");

    m_instanceptr = &instance;
    print << "available extensions:" << newLine;

    // debugger creatInfo struct
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    PopulateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS)
        throw runtime_error("failed to set up debug messenger!");
}

void Debug::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    // debug severity filter
    createInfo.messageSeverity = VK_VERBOSE | VK_WARNING | VK_ERROR;
    // debug type filter
    createInfo.messageType = VK_DEBUG_GENERAL | VK_DEBUG_VALIDATION | VK_DEBUG_PERFORMANCE;
    // pointer to callback function
    createInfo.pfnUserCallback = DebugCallback;
}


// check if all requested layers are available
bool Debug::CheckValidationLayerSupport() {
    // count all available layers
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    // list all available layers
    vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    // check if all requested validation layers are available
    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) 
            return false;       
    }
    return true;
}


VkResult Debug::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    else
        return VK_ERROR_EXTENSION_NOT_PRESENT;
}

VKAPI_ATTR VkBool32 VKAPI_CALL Debug::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    printError << "validation layer: " << pCallbackData->pMessage << newLine;
    return VK_FALSE;
}