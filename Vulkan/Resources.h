#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "Stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "Tiny_obj_loader.h"


#include <algorithm>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <optional>
#include <set>
#include <stdexcept>
#include <vector>
#include <array>
#include <chrono>

#define VK_DEBUG_UTILS VK_EXT_DEBUG_UTILS_EXTENSION_NAME

#define exceptionn std::exception
#define ifstream std::ifstream
#define ios std::ios
#define make_pair std::make_pair
#define multimap std::multimap
#define newLine std::endl
#define optional std::optional
#define print std::cout
#define printError std::cerr
#define runtime_error std::runtime_error
#define set std::set
#define unordered_map std::unordered_map
#define string std::string
#define vector std::vector
#define array std::array
#define invalid_argument std::invalid_argument

const vector<const char*> m_deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

static vector<char> readFile(const string& filename) {
    // read all bytes of specified file
    // ate   : start At The End of the file
    // binary: read as binary file 
    ifstream file(filename, ios::ate | ios::binary);

    // check if the file could be opened
    if (!file.is_open())
    {
        throw runtime_error("failed to open file!");
    }

    // get the current position to read the file size
    size_t fileSize = (size_t)file.tellg();
    vector<char> buffer(fileSize);

    // move to the front of the file
    file.seekg(0);
    // read the file
    file.read(buffer.data(), fileSize);

    // close file
    file.close();

    // return file contents
    return buffer;
}

struct QueueFamilyIndices {
    // graphics family
    optional<uint32_t> graphicsFamily;
    // presentation family
    optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() &&
            presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    vector<VkSurfaceFormatKHR> formats;
    vector<VkPresentModeKHR> presentModes;
};

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription GetBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions() {
        array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}
