#include "DeviceManager.h"


DeviceManager::DeviceManager(){
}

DeviceManager::~DeviceManager(){
}

void DeviceManager::PickPhysicalDevice(VkInstance& instance, VkPhysicalDevice& device, VkSurfaceKHR& surface)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		throw runtime_error("failed to find GPUs with Vulkan support!");
	}

	vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	multimap<VkPhysicalDevice, int> candidates;

	for (const auto& device : devices) {
		int score = RateDeviceSuitability(device);
		candidates.insert(make_pair(device, score));
	}

	for (const auto& device : devices) {
		if (IsDeviceSuitable(device, surface)) {
			int score = RateDeviceSuitability(device);
			candidates.insert(make_pair(device, score));
		}
	}

	// Check if the best candidate is suitable at all
	if (candidates.rbegin()->first > 0)
		device = candidates.rbegin()->first;
	else
		throw runtime_error("failed to find a suitable GPU!");
}

bool DeviceManager::IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR& surface) {
	QueueFamilyIndices indices = FindQueueFamilies(device, surface);

	bool extensionsSupported = CheckDeviceExtensionSupport(device);

	// check if the swapchain is adequately supported
	bool swapChainAdequate = false;
	if (extensionsSupported)
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device, surface);

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return m_deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		m_deviceFeatures.geometryShader &&
		indices.isComplete() &&
		extensionsSupported &&
		swapChainAdequate;
}

int DeviceManager::RateDeviceSuitability(VkPhysicalDevice device) {
	vkGetPhysicalDeviceProperties(device, &m_deviceProps);
	vkGetPhysicalDeviceFeatures(device, &m_deviceFeatures);
	int score = 0;

	// Discrete GPUs have a significant performance advantage
	if (m_deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1000;

	// Maximum possible size of textures affects graphics quality
	score += m_deviceProps.limits.maxImageDimension2D;

	// Application can't function without geometry shaders
	if (!m_deviceFeatures.geometryShader)
		return 0;

	return score;
}

bool DeviceManager::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	set<string> requiredExtensions(m_deviceExtensions.begin(), m_deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		// if the extension name is found in the requiredExtensions set,
		// take it out of the set
		requiredExtensions.erase(extension.extensionName);
	}

	// return if all required extensions are available
	// by checking if the for loop erased every single one of them
	return requiredExtensions.empty();
}

QueueFamilyIndices DeviceManager::FindQueueFamilies(VkPhysicalDevice& device, VkSurfaceKHR& surface) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		//check graphics layer
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.graphicsFamily = i;
		
		//check presentation layer ---
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (presentSupport)
			indices.presentFamily = i;

		// quit loop if indices are complete
		if (indices.isComplete())
			break;

		i++;
	}
	return indices;
}

SwapChainSupportDetails DeviceManager::QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR& surface) {
	SwapChainSupportDetails details;

	// get the capabilities shared by the device & drawing surface
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	// surface formats
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	// presentation modes
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}
	return details;
}