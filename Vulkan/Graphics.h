#pragma once

#include "Resources.h"
#include "Debug.h"
#include "ModelImport.h"
#include "DeviceManager.h"

class Graphics {
public:
	Graphics(GLFWwindow* window, string model_path, string texture_path);
	void CleanUp();

    void DrawFrame();

    VkInstance GetInstance();
    VkDevice GetDevice();
    VkDebugUtilsMessengerCreateInfoEXT GetDebugCreateInfo();

    bool m_framebufferResized = false;

private:   
    DeviceManager m_devicemanager;
    ModelImport m_modelImport;

    const int MAX_FRAMES_IN_FLIGHT = 2;

    GLFWwindow* m_windowPtr;
    Debug m_debug;

    string m_texture_path;
    string m_model_path;


    vector<Vertex> m_vertices;
    vector<uint32_t> m_indices; //uint32 because there will be more then 65K vertices

    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;
    VkBuffer m_indexBuffer;
    VkDeviceMemory m_indexBufferMemory;

    //images
    VkBuffer m_stagingBuffer;
    VkDeviceMemory m_stagingBufferMemory;

    //texture image
    VkImage m_textureImage;
    VkDeviceMemory m_textureImageMemory;

    VkImageView m_textureImageView;
    VkSampler m_textureSampler;
    vector<VkBuffer> m_uniformBuffers;
    vector<VkDeviceMemory> m_uniformBuffersMemory;

    VkInstance m_instance;
    VkDebugUtilsMessengerCreateInfoEXT m_debugCreateInfo;

    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

    // logical device
    VkDevice m_device;
    VkQueue m_gfxQueue;
    VkSurfaceKHR m_surface;
    VkQueue m_presentQueue;
    VkSwapchainKHR m_swapChain;

    // swap chain image handles
    vector<VkImage> m_swapChainImgs;
    VkFormat m_swapChainImgFormat;
    VkExtent2D m_swapChainExtent;

    // the image views
    vector<VkImageView> m_swapChainImgViews;
    
    VkDescriptorSetLayout m_descriptorSetLayout;
    VkPipelineLayout m_pipelineLayout;

    VkRenderPass m_renderPass;

    VkPipeline m_graphicsPipeline;


    vector<VkFramebuffer> m_swapChainFramebuffers;

    VkCommandPool m_commandPool;
    vector<VkCommandBuffer> m_commandBuffers;

    VkSemaphore m_imageAvailableSemaphore;
    VkSemaphore m_renderFinishedSemaphore;

    vector<VkSemaphore> m_imageAvailableSemaphores;
    vector<VkSemaphore> m_renderFinishedSemaphores;

    size_t m_currentFrame = 0;
    vector<VkFence> m_inFlightFences;
    vector<VkFence> m_imagesInFlight;

    VkDescriptorPool m_descriptorPool;
    vector<VkDescriptorSet> m_descriptorSets;


    VkImage m_depthImage;
    VkDeviceMemory m_depthImageMemory;
    VkImageView m_depthImageView;

    ///////////////
    /// Methods ///
    ///////////////
    void CreateInstance();                        
    vector<const char*> GetRequiredExtensions();

    void CreateLogicalDevice();

    void CreateSurface();
    void CreateSwapChain();
    VkPresentModeKHR ChooseSwapPresentMode(const vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR>& availableFormats);

    void CreateImageViews(); 
    void CreateRenderPass();
    void CreateDescriptorSetLayout();
    void CreateGraphicsPipeline(); 
    VkShaderModule CreateShaderModule(const vector<char>& code);

    void CreateFramebuffers();
    void CreateCommandPool();
    void CreateDepthResources();
    VkFormat FindSupportedFormat(const vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    VkFormat FindDepthFormat();
    bool HasStencilComponent(VkFormat format);

    void CreateTextureImage();
    void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void CreateTextureImageView();
    VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    void CreateTextureSampler();

    void CreateVertexBuffer();
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void CreateIndexBuffer();
    void CreateUniformBuffers();
    void CreateDescriptorPool();
    void CreateDescriptorSets();

    void CreateCommandBuffers();
    void CreateSemaphores();
    void CreateSyncObjects();

    void UpdateUniformBuffer(uint32_t currentImage);
    float GetFarestPoint();

    //swap chain recreation
    void RecreateSwapChain();
    void CleanupSwapChain();   

    VkCommandBuffer BeginSingleTimeCommands();
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
};