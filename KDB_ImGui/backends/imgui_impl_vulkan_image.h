///COPYRIGHT: Yan Chernikov Walnut Engine
//MIT License
#pragma once

#include <filesystem>
#include <vulkan/vulkan.h>


enum class ImageFormat
{
None = 0,
RGBA,
RGBA32F
};

class Image{
public:
    Image() = default;
    Image(std::string_view path);
    Image(uint32_t width, uint32_t height, ImageFormat format, const void* data = nullptr);
    ~Image();

    bool LoadImageFromPath();
    bool LoadImageFromPath(std::string filePath);
    void SetData(const void* data);

    VkDescriptorSet GetDescriptorSet() const { return m_DescriptorSet; }

    void Zoom(float zoom);

    void Resize(uint32_t width, uint32_t height);

    uint32_t GetWidth() const { return m_Width; }
    uint32_t GetHeight() const { return m_Height; }


private:
    void AllocateMemory(uint64_t size);
    void Release();
private:
    uint32_t m_Width = 32, m_Height = 32;

    VkImage m_Image = nullptr;
    VkImageView m_ImageView = nullptr;
    VkDeviceMemory m_Memory = nullptr;
    VkSampler m_Sampler = nullptr;

    ImageFormat m_Format = ImageFormat::None;

    VkBuffer m_StagingBuffer = nullptr;
    VkDeviceMemory m_StagingBufferMemory = nullptr;

    size_t m_AlignedSize = 0;

    VkDescriptorSet m_DescriptorSet = nullptr;

    std::string m_Filepath;
};