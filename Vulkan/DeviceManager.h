#pragma once

#include "Resources.h"

class DeviceManager {
public:
	DeviceManager();
	~DeviceManager();

	void PickPhysicalDevice(VkInstance& instance, VkPhysicalDevice& device, VkSurfaceKHR& surface);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice& device, VkSurfaceKHR& surface);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR& surface);

private:
	VkPhysicalDeviceProperties m_deviceProps;
	VkPhysicalDeviceFeatures m_deviceFeatures;

	bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR& surface);
	int RateDeviceSuitability(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
};