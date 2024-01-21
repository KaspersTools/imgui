//
// Adapted from Dear ImGui Vulkan example
//

#include "imgui_impl_glfw_vulkan_window.h"
#include "imgui_internal.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

#include <functional>
#include <math.h>
#include <memory>
#include <mutex>
#include <queue>
#include <stdio.h> // printf, fprintf
#include <stdlib.h>// abort
#include <thread>
#include <unordered_map>
#include <vector>

#include <iostream>


// Emedded font
GLFWwindow *m_WindowHandle = NULL;

std::mutex m_EventQueueMutex;
std::queue<std::function<void()>> m_EventQueue;

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

//#define IMGUI_UNLIMITED_FRAME_RATE
#ifdef DEBUG
#define IMGUI_VULKAN_DEBUG_REPORT
#endif

#include <iostream>
#include <map>

static VkAllocationCallbacks *g_Allocator = NULL;
static VkInstance g_Instance = VK_NULL_HANDLE;
static VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;
static VkDevice g_Device = VK_NULL_HANDLE;
static uint32_t g_QueueFamily = (uint32_t) -1;
static VkQueue g_Queue = VK_NULL_HANDLE;
static VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
static VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
static VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE;
static int g_MinImageCount = 2;
static bool g_SwapChainRebuild = false;

static ImGui_ImplVulkanH_Window g_MainWindowData;
static ImGui_ImplVulkanH_Window *g_wd = &g_MainWindowData;

// Per-frame-in-flight
static std::vector<std::vector<VkCommandBuffer>> s_AllocatedCommandBuffers = {};
static std::vector<std::vector<std::function<void()>>> s_ResourceFreeQueue = {};
static uint32_t s_CurrentFrameIndex = 0;
static ImVec4 g_ClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

static RenderStats s_RenderStats = {};
static ApplicationSpecification m_Specification;

static std::map<std::string, std::string> g_debugStats = {};

#ifdef IMGUI_VULKAN_DEBUG_REPORT
static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char *pLayerPrefix, const char *pMessage, void *pUserData) {
  (void) flags;
  (void) object;
  (void) location;
  (void) messageCode;
  (void) pUserData;
  (void) pLayerPrefix;// Unused arguments
  fprintf(stderr, "[vulkan] Debug report from ObjectType: %i\nMessage: %s\n\n", objectType, pMessage);
  return VK_FALSE;
}
#endif// IMGUI_VULKAN_DEBUG_REPORT

//function pointer callbacks for errors etc with default fprintf
static std::function<void(int error, const char *description)> *g_GlfwErrorCallback;

//Vulkan functions
//todo: maybe move to seperate file
void check_vk_result(VkResult err) {
  if (err == 0)
    return;
  fprintf(stderr, "[vulkan] [Error]: VkResult = %d\n", err);
  if (err < 0)
    abort();
}
static bool IsExtensionAvailable(const ImVector<VkExtensionProperties> &properties, const char *extension) {
  for (const VkExtensionProperties &p: properties)
    if (strcmp(p.extensionName, extension) == 0)
      return true;
  return false;
}
static VkPhysicalDevice SetupVulkan_SelectPhysicalDevice() {
  uint32_t gpu_count;
  VkResult err = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, nullptr);
  check_vk_result(err);
  IM_ASSERT(gpu_count > 0);

  ImVector<VkPhysicalDevice> gpus;
  gpus.resize(gpu_count);
  err = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, gpus.Data);
  check_vk_result(err);

  // If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
  // most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
  // dedicated GPUs) is out of scope of this sample.
  for (VkPhysicalDevice &device: gpus) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);
    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
      return device;
  }

  // Use first GPU (Integrated) is a Discrete one is not available.
  if (gpu_count > 0)
    return gpus[0];
  return VK_NULL_HANDLE;
}
static void SetupVulkan(ImVector<const char *> instance_extensions) {
  VkResult err;

  // Create Vulkan Instance
  // Create Vulkan Instance
  {
    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    // Enumerate available extensions
    uint32_t properties_count;
    ImVector<VkExtensionProperties> properties;
    vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, nullptr);
    properties.resize(properties_count);
    err = vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, properties.Data);
    check_vk_result(err);

    // Enable required extensions
    if (IsExtensionAvailable(properties, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
      instance_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
#ifdef VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
    if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME)) {
      instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
      create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    }
#endif

    // Enabling validation layers
#ifdef IMGUI_VULKAN_DEBUG_REPORT
    const char *layers[] = {"VK_LAYER_KHRONOS_validation"};
    create_info.enabledLayerCount = 1;
    create_info.ppEnabledLayerNames = layers;
    instance_extensions.push_back("VK_EXT_debug_report");
#endif

    // Create Vulkan Instance
    create_info.enabledExtensionCount = (uint32_t) instance_extensions.Size;
    create_info.ppEnabledExtensionNames = instance_extensions.Data;
    err = vkCreateInstance(&create_info, g_Allocator, &g_Instance);
    check_vk_result(err);

    // Setup the debug report callback
#ifdef IMGUI_VULKAN_DEBUG_REPORT
    auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(g_Instance, "vkCreateDebugReportCallbackEXT");
    IM_ASSERT(vkCreateDebugReportCallbackEXT != nullptr);
    VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
    debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
    debug_report_ci.pfnCallback = debug_report;
    debug_report_ci.pUserData = nullptr;
    err = vkCreateDebugReportCallbackEXT(g_Instance, &debug_report_ci, g_Allocator, &g_DebugReport);
    check_vk_result(err);
#endif
  }
  // Select Physical Device (GPU)
  g_PhysicalDevice = SetupVulkan_SelectPhysicalDevice();

  // Select graphics queue family
  {
    uint32_t count;
    vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, NULL);
    VkQueueFamilyProperties *queues = (VkQueueFamilyProperties *) malloc(sizeof(VkQueueFamilyProperties) * count);
    vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, queues);
    for (uint32_t i = 0; i < count; i++)
      if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        g_QueueFamily = i;
        break;
      }
    free(queues);
    IM_ASSERT(g_QueueFamily != (uint32_t) -1);
  }

  // Create Logical Device (with 1 queue)
  {
    int device_extension_count = 1;
    const char *device_extensions[] = {"VK_KHR_swapchain"};
    const float queue_priority[] = {1.0f};
    VkDeviceQueueCreateInfo queue_info[1] = {};
    queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_info[0].queueFamilyIndex = g_QueueFamily;
    queue_info[0].queueCount = 1;
    queue_info[0].pQueuePriorities = queue_priority;
    VkDeviceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
    create_info.pQueueCreateInfos = queue_info;
    create_info.enabledExtensionCount = device_extension_count;
    create_info.ppEnabledExtensionNames = device_extensions;


    err = vkCreateDevice(g_PhysicalDevice, &create_info, g_Allocator, &g_Device);
    check_vk_result(err);
    vkGetDeviceQueue(g_Device, g_QueueFamily, 0, &g_Queue);
  }

  // Create Descriptor Pool
  {
    VkDescriptorPoolSize pool_sizes[] =
            {
                    {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
                    {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
                    {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
                    {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
                    {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
                    {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
                    {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
                    {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
                    {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
                    {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
                    {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};
    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
    pool_info.poolSizeCount = (uint32_t) IM_ARRAYSIZE(pool_sizes);
    pool_info.pPoolSizes = pool_sizes;
    err = vkCreateDescriptorPool(g_Device, &pool_info, g_Allocator, &g_DescriptorPool);
    check_vk_result(err);
  }
}
static void SetupVulkanWindow(ImGui_ImplVulkanH_Window *wd, VkSurfaceKHR surface, int width, int height) {
  wd->Surface = surface;

  // Check for WSI support
  VkBool32 res;
  vkGetPhysicalDeviceSurfaceSupportKHR(g_PhysicalDevice, g_QueueFamily, wd->Surface, &res);
  if (res != VK_TRUE) {
    fprintf(stderr, "Error no WSI support on physical device 0\n");
    exit(-1);
  }

  // Select Surface Format
  const VkFormat requestSurfaceImageFormat[] = {VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM};
  const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
  wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(g_PhysicalDevice, wd->Surface, requestSurfaceImageFormat, (size_t) IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

  // Select Present Mode
#ifdef IMGUI_UNLIMITED_FRAME_RATE
  VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR};
#else
  VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_FIFO_KHR};
#endif
  wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(g_PhysicalDevice, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
  //printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

  // Create SwapChain, RenderPass, Framebuffer, etc.
  IM_ASSERT(g_MinImageCount >= 2);
  ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, wd, g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
}
static void CleanupVulkan() {
  vkDestroyDescriptorPool(g_Device, g_DescriptorPool, g_Allocator);

#ifdef IMGUI_VULKAN_DEBUG_REPORT
  // Remove the debug report callback
  auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(g_Instance, "vkDestroyDebugReportCallbackEXT");
  vkDestroyDebugReportCallbackEXT(g_Instance, g_DebugReport, g_Allocator);
#endif// IMGUI_VULKAN_DEBUG_REPORT

  vkDestroyDevice(g_Device, g_Allocator);
  vkDestroyInstance(g_Instance, g_Allocator);
}
static void CleanupVulkanWindow() {
  ImGui_ImplVulkanH_DestroyWindow(g_Instance, g_Device, &g_MainWindowData, g_Allocator);
}
//ImGui Helper Functions
static void FrameRender(ImGui_ImplVulkanH_Window *wd, ImDrawData *draw_data) {
  VkResult err;

  VkSemaphore image_acquired_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
  VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
  err = vkAcquireNextImageKHR(g_Device, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
  if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
    g_SwapChainRebuild = true;
    return;
  }
  check_vk_result(err);

  s_CurrentFrameIndex = (s_CurrentFrameIndex + 1) % g_MainWindowData.ImageCount;

  ImGui_ImplVulkanH_Frame *fd = &wd->Frames[wd->FrameIndex];
  {
    err = vkWaitForFences(g_Device, 1, &fd->Fence, VK_TRUE, UINT64_MAX);// wait indefinitely instead of periodically checking
    check_vk_result(err);

    err = vkResetFences(g_Device, 1, &fd->Fence);
    check_vk_result(err);
  }

  {
    // Free resources in queue
    for (auto &func: s_ResourceFreeQueue[s_CurrentFrameIndex])
      func();
    s_ResourceFreeQueue[s_CurrentFrameIndex].clear();
  }
  {
    // Free command buffers allocated by Application::GetCommandBuffer
    // These use g_MainWindowData.FrameIndex and not s_CurrentFrameIndex because they're tied to the swapchain image index
    auto &allocatedCommandBuffers = s_AllocatedCommandBuffers[wd->FrameIndex];
    if (allocatedCommandBuffers.size() > 0) {
      vkFreeCommandBuffers(g_Device, fd->CommandPool, (uint32_t) allocatedCommandBuffers.size(), allocatedCommandBuffers.data());
      allocatedCommandBuffers.clear();
    }

    err = vkResetCommandPool(g_Device, fd->CommandPool, 0);
    check_vk_result(err);
    VkCommandBufferBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
    check_vk_result(err);
  }
  {
    VkRenderPassBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.renderPass = wd->RenderPass;
    info.framebuffer = fd->Framebuffer;
    info.renderArea.extent.width = wd->Width;
    info.renderArea.extent.height = wd->Height;
    info.clearValueCount = 1;
    info.pClearValues = &wd->ClearValue;
    vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
  }

  // Record dear imgui primitives into command buffer
  ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

  // Submit command buffer
  vkCmdEndRenderPass(fd->CommandBuffer);
  {
    VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &image_acquired_semaphore;
    info.pWaitDstStageMask = &wait_stage;
    info.commandBufferCount = 1;
    info.pCommandBuffers = &fd->CommandBuffer;
    info.signalSemaphoreCount = 1;
    info.pSignalSemaphores = &render_complete_semaphore;

    err = vkEndCommandBuffer(fd->CommandBuffer);
    check_vk_result(err);
    err = vkQueueSubmit(g_Queue, 1, &info, fd->Fence);
    check_vk_result(err);
  }
}

static void FramePresent(ImGui_ImplVulkanH_Window *wd) {
  if (g_SwapChainRebuild)
    return;
  VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
  VkPresentInfoKHR info = {};
  info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  info.waitSemaphoreCount = 1;
  info.pWaitSemaphores = &render_complete_semaphore;
  info.swapchainCount = 1;
  info.pSwapchains = &wd->Swapchain;
  info.pImageIndices = &wd->FrameIndex;
  VkResult err = vkQueuePresentKHR(g_Queue, &info);
  if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
    g_SwapChainRebuild = true;
    return;
  }
  check_vk_result(err);
  wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount;// Now we can use the next set of semaphores
}

namespace KDB_IMGUI_EXTENSION {

  void ShiftCursorY(float distance) {
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + distance);
  }
  ImRect GetItemRect() {
    return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
  }

  ImRect RectExpanded(const ImRect &rect, float x, float y) {
    ImRect result = rect;
    result.Min.x -= x;
    result.Min.y -= y;
    result.Max.x += x;
    result.Max.y += y;
    return result;
  }

  ImRect RectOffset(const ImRect &rect, float x, float y) {
    ImRect result = rect;
    result.Min.x += x;
    result.Min.y += y;
    result.Max.x += x;
    result.Max.y += y;
    return result;
  }

  ImVec2 GetScaleFactor() {

    float dpiScale = ImGui::GetWindowDpiScale();

    //Get Scale of the glfw window
    ImVec2 screenSize = ImGui::GetMainViewport()->Size;

    // Define a base resolution
    ImVec2 baseResolution = ImVec2(1600, 900);

    // Calculate scale factor
    float scaleX = screenSize.x / baseResolution.x;
    float scaleY = screenSize.y / baseResolution.y;

    return ImVec2(scaleX, scaleY);
  }

  // Exposed to be used for window with disabled decorations
  // This border is going to be drawn even if window border size is set to 0.0f
  void RenderWindowOuterBorders(ImGuiWindow *window) {
    struct ImGuiResizeBorderDef {
      ImVec2 InnerDir;
      ImVec2 SegmentN1, SegmentN2;
      float OuterAngle;
    };

    static const ImGuiResizeBorderDef resize_border_def[4] =
            {
                    {ImVec2(+1, 0), ImVec2(0, 1), ImVec2(0, 0), IM_PI * 1.00f},// Left
                    {ImVec2(-1, 0), ImVec2(1, 0), ImVec2(1, 1), IM_PI * 0.00f},// Right
                    {ImVec2(0, +1), ImVec2(0, 0), ImVec2(1, 0), IM_PI * 1.50f},// Up
                    {ImVec2(0, -1), ImVec2(1, 1), ImVec2(0, 1), IM_PI * 0.50f} // Down
            };

    auto GetResizeBorderRect = [](ImGuiWindow *window, int border_n, float perp_padding, float thickness) {
      ImRect rect = window->Rect();
      if (thickness == 0.0f) {
        rect.Max.x -= 1;
        rect.Max.y -= 1;
      }
      if (border_n == ImGuiDir_Left) { return ImRect(rect.Min.x - thickness, rect.Min.y + perp_padding, rect.Min.x + thickness, rect.Max.y - perp_padding); }
      if (border_n == ImGuiDir_Right) { return ImRect(rect.Max.x - thickness, rect.Min.y + perp_padding, rect.Max.x + thickness, rect.Max.y - perp_padding); }
      if (border_n == ImGuiDir_Up) { return ImRect(rect.Min.x + perp_padding, rect.Min.y - thickness, rect.Max.x - perp_padding, rect.Min.y + thickness); }
      if (border_n == ImGuiDir_Down) { return ImRect(rect.Min.x + perp_padding, rect.Max.y - thickness, rect.Max.x - perp_padding, rect.Max.y + thickness); }
      IM_ASSERT(0);
      return ImRect();
    };


    ImGuiContext &g = *GImGui;
    float rounding = window->WindowRounding;
    float border_size = 1.0f;// window->WindowBorderSize;
    if (border_size > 0.0f && !(window->Flags & ImGuiWindowFlags_NoBackground))
      window->DrawList->AddRect(window->Pos, {window->Pos.x + window->Size.x, window->Pos.y + window->Size.y}, ImGui::GetColorU32(ImGuiCol_Border), rounding, 0, border_size);

    int border_held = window->ResizeBorderHeld;
    if (border_held != -1) {
      const ImGuiResizeBorderDef &def = resize_border_def[border_held];
      ImRect border_r = GetResizeBorderRect(window, border_held, rounding, 0.0f);
      ImVec2 p1 = ImLerp(border_r.Min, border_r.Max, def.SegmentN1);
      const float offsetX = def.InnerDir.x * rounding;
      const float offsetY = def.InnerDir.y * rounding;
      p1.x += 0.5f + offsetX;
      p1.y += 0.5f + offsetY;

      ImVec2 p2 = ImLerp(border_r.Min, border_r.Max, def.SegmentN2);
      p2.x += 0.5f + offsetX;
      p2.y += 0.5f + offsetY;

      window->DrawList->PathArcTo(p1, rounding, def.OuterAngle - IM_PI * 0.25f, def.OuterAngle);
      window->DrawList->PathArcTo(p2, rounding, def.OuterAngle, def.OuterAngle + IM_PI * 0.25f);
      window->DrawList->PathStroke(ImGui::GetColorU32(ImGuiCol_SeparatorActive), 0, ImMax(2.0f, border_size));// Thicker than usual
    }
    if (g.Style.FrameBorderSize > 0 && !(window->Flags & ImGuiWindowFlags_NoTitleBar) && !window->DockIsActive) {
      float y = window->Pos.y + window->TitleBarHeight() - 1;
      window->DrawList->AddLine(ImVec2(window->Pos.x + border_size, y), ImVec2(window->Pos.x + window->Size.x - border_size, y), ImGui::GetColorU32(ImGuiCol_Border), g.Style.FrameBorderSize);
    }
  }

  // Exposed resize behavior for native OS windows
  bool UpdateWindowManualResize(ImGuiWindow *window, ImVec2 &newSize, ImVec2 &newPosition) {
    auto CalcWindowSizeAfterConstraint = [](ImGuiWindow *window, const ImVec2 &size_desired) {
      ImGuiContext &g = *GImGui;
      ImVec2 new_size = size_desired;
      if (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint) {
        // Using -1,-1 on either X/Y axis to preserve the current size.
        ImRect cr = g.NextWindowData.SizeConstraintRect;
        new_size.x = (cr.Min.x >= 0 && cr.Max.x >= 0) ? ImClamp(new_size.x, cr.Min.x, cr.Max.x) : window->SizeFull.x;
        new_size.y = (cr.Min.y >= 0 && cr.Max.y >= 0) ? ImClamp(new_size.y, cr.Min.y, cr.Max.y) : window->SizeFull.y;
        if (g.NextWindowData.SizeCallback) {
          ImGuiSizeCallbackData data;
          data.UserData = g.NextWindowData.SizeCallbackUserData;
          data.Pos = window->Pos;
          data.CurrentSize = window->SizeFull;
          data.DesiredSize = new_size;
          g.NextWindowData.SizeCallback(&data);
          new_size = data.DesiredSize;
        }
        new_size.x = IM_FLOOR(new_size.x);
        new_size.y = IM_FLOOR(new_size.y);
      }

      // Minimum size
      if (!(window->Flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_AlwaysAutoResize))) {
        ImGuiWindow *window_for_height = (window->DockNodeAsHost && window->DockNodeAsHost->VisibleWindow) ? window->DockNodeAsHost->VisibleWindow : window;
        const float decoration_up_height = window_for_height->TitleBarHeight() + window_for_height->MenuBarHeight();
        new_size = ImMax(new_size, g.Style.WindowMinSize);
        new_size.y = ImMax(new_size.y, decoration_up_height + ImMax(0.0f, g.Style.WindowRounding - 1.0f));// Reduce artifacts with very small windows
      }
      return new_size;
    };

    auto CalcWindowAutoFitSize = [CalcWindowSizeAfterConstraint](ImGuiWindow *window, const ImVec2 &size_contents) {
      ImGuiContext &g = *GImGui;
      ImGuiStyle &style = g.Style;
      const float decoration_up_height = window->TitleBarHeight() + window->MenuBarHeight();
      ImVec2 size_pad{window->WindowPadding.x * 2.0f, window->WindowPadding.y * 2.0f};
      ImVec2 size_desired = {size_contents.x + size_pad.x + 0.0f, size_contents.y + size_pad.y + decoration_up_height};
      if (window->Flags & ImGuiWindowFlags_Tooltip) {
        // Tooltip always resize
        return size_desired;
      } else {
        // Maximum window size is determined by the viewport size or monitor size
        const bool is_popup = (window->Flags & ImGuiWindowFlags_Popup) != 0;
        const bool is_menu = (window->Flags & ImGuiWindowFlags_ChildMenu) != 0;
        ImVec2 size_min = style.WindowMinSize;
        if (is_popup || is_menu)// Popups and menus bypass style.WindowMinSize by default, but we give then a non-zero minimum size to facilitate understanding problematic cases (e.g. empty popups)
          size_min = ImMin(size_min, ImVec2(4.0f, 4.0f));

        // FIXME-VIEWPORT-WORKAREA: May want to use GetWorkSize() instead of Size depending on the type of windows?
        ImVec2 avail_size = window->Viewport->Size;
        if (window->ViewportOwned)
          avail_size = ImVec2(FLT_MAX, FLT_MAX);
        const int monitor_idx = window->ViewportAllowPlatformMonitorExtend;
        if (monitor_idx >= 0 && monitor_idx < g.PlatformIO.Monitors.Size)
          avail_size = g.PlatformIO.Monitors[monitor_idx].WorkSize;
        ImVec2 size_auto_fit = ImClamp(size_desired, size_min, ImMax(size_min, {avail_size.x - style.DisplaySafeAreaPadding.x * 2.0f, avail_size.y - style.DisplaySafeAreaPadding.y * 2.0f}));

        // When the window cannot fit all contents (either because of constraints, either because screen is too small),
        // we are growing the size on the other axis to compensate for expected scrollbar. FIXME: Might turn bigger than ViewportSize-WindowPadding.
        ImVec2 size_auto_fit_after_constraint = CalcWindowSizeAfterConstraint(window, size_auto_fit);
        bool will_have_scrollbar_x = (size_auto_fit_after_constraint.x - size_pad.x - 0.0f < size_contents.x && !(window->Flags & ImGuiWindowFlags_NoScrollbar) && (window->Flags & ImGuiWindowFlags_HorizontalScrollbar)) || (window->Flags & ImGuiWindowFlags_AlwaysHorizontalScrollbar);
        bool will_have_scrollbar_y = (size_auto_fit_after_constraint.y - size_pad.y - decoration_up_height < size_contents.y && !(window->Flags & ImGuiWindowFlags_NoScrollbar)) || (window->Flags & ImGuiWindowFlags_AlwaysVerticalScrollbar);
        if (will_have_scrollbar_x)
          size_auto_fit.y += style.ScrollbarSize;
        if (will_have_scrollbar_y)
          size_auto_fit.x += style.ScrollbarSize;
        return size_auto_fit;
      }
    };

    ImGuiContext &g = *GImGui;

    // Decide if we are going to handle borders and resize grips
    const bool handle_borders_and_resize_grips = (window->DockNodeAsHost || !window->DockIsActive);

    if (!handle_borders_and_resize_grips || window->Collapsed)
      return false;

    const ImVec2 size_auto_fit = CalcWindowAutoFitSize(window, window->ContentSizeIdeal);

    // Handle manual resize: Resize Grips, Borders, Gamepad
    int border_held = -1;
    ImU32 resize_grip_col[4] = {};
    const int resize_grip_count = g.IO.ConfigWindowsResizeFromEdges ? 2 : 1;// Allow resize from lower-left if we have the mouse cursor feedback for it.
    const float resize_grip_draw_size = IM_FLOOR(ImMax(g.FontSize * 1.10f, window->WindowRounding + 1.0f + g.FontSize * 0.2f));
    window->ResizeBorderHeld = (signed char) border_held;

    //const ImRect& visibility_rect;

    struct ImGuiResizeBorderDef {
      ImVec2 InnerDir;
      ImVec2 SegmentN1, SegmentN2;
      float OuterAngle;
    };
    static const ImGuiResizeBorderDef resize_border_def[4] =
            {
                    {ImVec2(+1, 0), ImVec2(0, 1), ImVec2(0, 0), IM_PI * 1.00f},// Left
                    {ImVec2(-1, 0), ImVec2(1, 0), ImVec2(1, 1), IM_PI * 0.00f},// Right
                    {ImVec2(0, +1), ImVec2(0, 0), ImVec2(1, 0), IM_PI * 1.50f},// Up
                    {ImVec2(0, -1), ImVec2(1, 1), ImVec2(0, 1), IM_PI * 0.50f} // Down
            };

    // Data for resizing from corner
    struct ImGuiResizeGripDef {
      ImVec2 CornerPosN;
      ImVec2 InnerDir;
      int AngleMin12, AngleMax12;
    };
    static const ImGuiResizeGripDef resize_grip_def[4] =
            {
                    {ImVec2(1, 1), ImVec2(-1, -1), 0, 3},// Lower-right
                    {ImVec2(0, 1), ImVec2(+1, -1), 3, 6},// Lower-left
                    {ImVec2(0, 0), ImVec2(+1, +1), 6, 9},// Upper-left (Unused)
                    {ImVec2(1, 0), ImVec2(-1, +1), 9, 12}// Upper-right (Unused)
            };

    auto CalcResizePosSizeFromAnyCorner = [CalcWindowSizeAfterConstraint](ImGuiWindow *window, const ImVec2 &corner_target, const ImVec2 &corner_norm, ImVec2 *out_pos, ImVec2 *out_size) {
      ImVec2 pos_min = ImLerp(corner_target, window->Pos, corner_norm);                                                     // Expected window upper-left
      ImVec2 pos_max = ImLerp({window->Pos.x + window->Size.x, window->Pos.y + window->Size.y}, corner_target, corner_norm);// Expected window lower-right
      ImVec2 size_expected = {pos_max.x - pos_min.x, pos_max.y - pos_min.y};
      ImVec2 size_constrained = CalcWindowSizeAfterConstraint(window, size_expected);
      *out_pos = pos_min;
      if (corner_norm.x == 0.0f)
        out_pos->x -= (size_constrained.x - size_expected.x);
      if (corner_norm.y == 0.0f)
        out_pos->y -= (size_constrained.y - size_expected.y);
      *out_size = size_constrained;
    };

    auto GetResizeBorderRect = [](ImGuiWindow *window, int border_n, float perp_padding, float thickness) {
      ImRect rect = window->Rect();
      if (thickness == 0.0f) {
        rect.Max.x -= 1;
        rect.Max.y -= 1;
      }
      if (border_n == ImGuiDir_Left) { return ImRect(rect.Min.x - thickness, rect.Min.y + perp_padding, rect.Min.x + thickness, rect.Max.y - perp_padding); }
      if (border_n == ImGuiDir_Right) { return ImRect(rect.Max.x - thickness, rect.Min.y + perp_padding, rect.Max.x + thickness, rect.Max.y - perp_padding); }
      if (border_n == ImGuiDir_Up) { return ImRect(rect.Min.x + perp_padding, rect.Min.y - thickness, rect.Max.x - perp_padding, rect.Min.y + thickness); }
      if (border_n == ImGuiDir_Down) { return ImRect(rect.Min.x + perp_padding, rect.Max.y - thickness, rect.Max.x - perp_padding, rect.Max.y + thickness); }
      IM_ASSERT(0);
      return ImRect();
    };

    static const float WINDOWS_HOVER_PADDING = 4.0f;                    // Extend outside window for hovering/resizing (maxxed with TouchPadding) and inside windows for borders. Affect FindHoveredWindow().
    static const float WINDOWS_RESIZE_FROM_EDGES_FEEDBACK_TIMER = 0.04f;// Reduce visual noise by only highlighting the border after a certain time.

    auto &style = g.Style;
    ImGuiWindowFlags flags = window->Flags;

    if (/*(flags & ImGuiWindowFlags_NoResize) || */ (flags & ImGuiWindowFlags_AlwaysAutoResize) || window->AutoFitFramesX > 0 || window->AutoFitFramesY > 0)
      return false;
    if (window->WasActive == false)// Early out to avoid running this code for e.g. an hidden implicit/fallback Debug window.
      return false;

    bool ret_auto_fit = false;
    const int resize_border_count = g.IO.ConfigWindowsResizeFromEdges ? 4 : 0;
    const float grip_draw_size = IM_FLOOR(ImMax(g.FontSize * 1.35f, window->WindowRounding + 1.0f + g.FontSize * 0.2f));
    const float grip_hover_inner_size = IM_FLOOR(grip_draw_size * 0.75f);
    const float grip_hover_outer_size = g.IO.ConfigWindowsResizeFromEdges ? WINDOWS_HOVER_PADDING : 0.0f;

    ImVec2 pos_target(FLT_MAX, FLT_MAX);
    ImVec2 size_target(FLT_MAX, FLT_MAX);

    // Calculate the range of allowed position for that window (to be movable and visible past safe area padding)
    // When clamping to stay visible, we will enforce that window->Pos stays inside of visibility_rect.
    ImRect viewport_rect(window->Viewport->GetMainRect());
    ImRect viewport_work_rect(window->Viewport->GetWorkRect());
    ImVec2 visibility_padding = ImMax(style.DisplayWindowPadding, style.DisplaySafeAreaPadding);
    ImRect visibility_rect({viewport_work_rect.Min.x + visibility_padding.x, viewport_work_rect.Min.y + visibility_padding.y},
                           {viewport_work_rect.Max.x - visibility_padding.x, viewport_work_rect.Max.y - visibility_padding.y});

    // Clip mouse interaction rectangles within the viewport rectangle (in practice the narrowing is going to happen most of the time).
    // - Not narrowing would mostly benefit the situation where OS windows _without_ decoration have a threshold for hovering when outside their limits.
    //   This is however not the case with current backends under Win32, but a custom borderless window implementation would benefit from it.
    // - When decoration are enabled we typically benefit from that distance, but then our resize elements would be conflicting with OS resize elements, so we also narrow.
    // - Note that we are unable to tell if the platform setup allows hovering with a distance threshold (on Win32, decorated window have such threshold).
    // We only clip interaction so we overwrite window->ClipRect, cannot call PushClipRect() yet as DrawList is not yet setup.
    const bool clip_with_viewport_rect = !(g.IO.BackendFlags & ImGuiBackendFlags_HasMouseHoveredViewport) || (g.IO.MouseHoveredViewport != window->ViewportId) || !(window->Viewport->Flags & ImGuiViewportFlags_NoDecoration);
    if (clip_with_viewport_rect)
      window->ClipRect = window->Viewport->GetMainRect();

    // Resize grips and borders are on layer 1
    window->DC.NavLayerCurrent = ImGuiNavLayer_Menu;

    // Manual resize grips
    ImGui::PushID("#RESIZE");
    for (int resize_grip_n = 0; resize_grip_n < resize_grip_count; resize_grip_n++) {
      const ImGuiResizeGripDef &def = resize_grip_def[resize_grip_n];

      const ImVec2 corner = ImLerp(window->Pos, {window->Pos.x + window->Size.x, window->Pos.y + window->Size.y}, def.CornerPosN);

      // Using the FlattenChilds button flag we make the resize button accessible even if we are hovering over a child window
      bool hovered, held;
      const ImVec2 min = {corner.x - def.InnerDir.x * grip_hover_outer_size, corner.y - def.InnerDir.y * grip_hover_outer_size};
      const ImVec2 max = {corner.x + def.InnerDir.x * grip_hover_outer_size, corner.y + def.InnerDir.y * grip_hover_outer_size};
      ImRect resize_rect(min, max);

      if (resize_rect.Min.x > resize_rect.Max.x) ImSwap(resize_rect.Min.x, resize_rect.Max.x);
      if (resize_rect.Min.y > resize_rect.Max.y) ImSwap(resize_rect.Min.y, resize_rect.Max.y);
      ImGuiID resize_grip_id = window->GetID(resize_grip_n);// == GetWindowResizeCornerID()
      ImGui::ButtonBehavior(resize_rect, resize_grip_id, &hovered, &held, ImGuiButtonFlags_FlattenChildren | ImGuiButtonFlags_NoNavFocus);
      //GetForegroundDrawList(window)->AddRect(resize_rect.Min, resize_rect.Max, IM_COL32(255, 255, 0, 255));
      if (hovered || held)
        g.MouseCursor = (resize_grip_n & 1) ? ImGuiMouseCursor_ResizeNESW : ImGuiMouseCursor_ResizeNWSE;

      if (held && g.IO.MouseDoubleClicked[0] && resize_grip_n == 0) {
        // Manual auto-fit when double-clicking
        size_target = CalcWindowSizeAfterConstraint(window, size_auto_fit);
        ret_auto_fit = true;
        ImGui::ClearActiveID();
      } else if (held) {
        // Resize from any of the four corners
        // We don't use an incremental MouseDelta but rather compute an absolute target size based on mouse position
        ImVec2 clamp_min = ImVec2(def.CornerPosN.x == 1.0f ? visibility_rect.Min.x : -FLT_MAX, def.CornerPosN.y == 1.0f ? visibility_rect.Min.y : -FLT_MAX);
        ImVec2 clamp_max = ImVec2(def.CornerPosN.x == 0.0f ? visibility_rect.Max.x : +FLT_MAX, def.CornerPosN.y == 0.0f ? visibility_rect.Max.y : +FLT_MAX);

        const float x = g.IO.MousePos.x - g.ActiveIdClickOffset.x + ImLerp(def.InnerDir.x * grip_hover_outer_size, def.InnerDir.x * -grip_hover_inner_size, def.CornerPosN.x);
        const float y = g.IO.MousePos.y - g.ActiveIdClickOffset.y + ImLerp(def.InnerDir.y * grip_hover_outer_size, def.InnerDir.y * -grip_hover_inner_size, def.CornerPosN.y);

        ImVec2 corner_target(x, y);// Corner of the window corresponding to our corner grip
        corner_target = ImClamp(corner_target, clamp_min, clamp_max);
        CalcResizePosSizeFromAnyCorner(window, corner_target, def.CornerPosN, &pos_target, &size_target);
      }

      // Only lower-left grip is visible before hovering/activating
      if (resize_grip_n == 0 || held || hovered)
        resize_grip_col[resize_grip_n] = ImGui::GetColorU32(held ? ImGuiCol_ResizeGripActive : hovered ? ImGuiCol_ResizeGripHovered
                                                                                                       : ImGuiCol_ResizeGrip);
    }
    for (int border_n = 0; border_n < resize_border_count; border_n++) {
      const ImGuiResizeBorderDef &def = resize_border_def[border_n];
      const ImGuiAxis axis = (border_n == ImGuiDir_Left || border_n == ImGuiDir_Right) ? ImGuiAxis_X : ImGuiAxis_Y;

      bool hovered, held;
      ImRect border_rect = GetResizeBorderRect(window, border_n, grip_hover_inner_size, WINDOWS_HOVER_PADDING);
      ImGuiID border_id = window->GetID(border_n + 4);// == GetWindowResizeBorderID()
      ImGui::ButtonBehavior(border_rect, border_id, &hovered, &held, ImGuiButtonFlags_FlattenChildren);
      //GetForegroundDrawLists(window)->AddRect(border_rect.Min, border_rect.Max, IM_COL32(255, 255, 0, 255));
      if ((hovered && g.HoveredIdTimer > WINDOWS_RESIZE_FROM_EDGES_FEEDBACK_TIMER) || held) {
        g.MouseCursor = (axis == ImGuiAxis_X) ? ImGuiMouseCursor_ResizeEW : ImGuiMouseCursor_ResizeNS;
        if (held)
          border_held = border_n;
      }
      if (held) {
        ImVec2 clamp_min(border_n == ImGuiDir_Right ? visibility_rect.Min.x : -FLT_MAX, border_n == ImGuiDir_Down ? visibility_rect.Min.y : -FLT_MAX);
        ImVec2 clamp_max(border_n == ImGuiDir_Left ? visibility_rect.Max.x : +FLT_MAX, border_n == ImGuiDir_Up ? visibility_rect.Max.y : +FLT_MAX);
        ImVec2 border_target = window->Pos;
        border_target[axis] = g.IO.MousePos[axis] - g.ActiveIdClickOffset[axis] + WINDOWS_HOVER_PADDING;
        border_target = ImClamp(border_target, clamp_min, clamp_max);
        CalcResizePosSizeFromAnyCorner(window, border_target, ImMin(def.SegmentN1, def.SegmentN2), &pos_target, &size_target);
      }
    }
    ImGui::PopID();

    bool changed = false;
    newSize = window->Size;
    newPosition = window->Pos;

    // Apply back modified position/size to window
    if (size_target.x != FLT_MAX) {
      //window->SizeFull = size_target;
      //MarkIniSettingsDirty(window);
      newSize = size_target;
      changed = true;
    }
    if (pos_target.x != FLT_MAX) {
      //window->Pos = ImFloor(pos_target);
      //MarkIniSettingsDirty(window);
      newPosition = pos_target;
      changed = true;
    }

    //window->Size = window->SizeFull;
    return changed;
  }

  static bool IsRootOfOpenMenuSet() {
    ImGuiContext &g = *GImGui;
    ImGuiWindow *window = g.CurrentWindow;
    if ((g.OpenPopupStack.Size <= g.BeginPopupStack.Size) || (window->Flags & ImGuiWindowFlags_ChildMenu))
      return false;

    // Initially we used 'upper_popup->OpenParentId == window->IDStack.back()' to differentiate multiple menu sets from each others
    // (e.g. inside menu bar vs loose menu items) based on parent ID.
    // This would however prevent the use of e.g. PushID() user code submitting menus.
    // Previously this worked between popup and a first child menu because the first child menu always had the _ChildWindow flag,
    // making hovering on parent popup possible while first child menu was focused - but this was generally a bug with other side effects.
    // Instead we don't treat Popup specifically (in order to consistently support menu features in them), maybe the first child menu of a Popup
    // doesn't have the _ChildWindow flag, and we rely on this IsRootOfOpenMenuSet() check to allow hovering between root window/popup and first child menu.
    // In the end, lack of ID check made it so we could no longer differentiate between separate menu sets. To compensate for that, we at least check parent window nav layer.
    // This fixes the most common case of menu opening on hover when moving between window content and menu bar. Multiple different menu sets in same nav layer would still
    // open on hover, but that should be a lesser problem, because if such menus are close in proximity in window content then it won't feel weird and if they are far apart
    // it likely won't be a problem anyone runs into.
    const ImGuiPopupData *upper_popup = &g.OpenPopupStack[g.BeginPopupStack.Size];
    if (window->DC.NavLayerCurrent != upper_popup->ParentNavLayer)
      return false;
    return upper_popup->Window && (upper_popup->Window->Flags & ImGuiWindowFlags_ChildMenu) && ImGui::IsWindowChildOf(upper_popup->Window, window, true, false);
  }


  bool BeginMenuEx(const char *label, const char *icon, bool enabled) {
    ImGuiWindow *window = ImGui::GetCurrentWindow();

    if (window->SkipItems)
      return false;
    // The reference position stored in popup_pos will be used by Begin() to find a suitable position for the child menu,
    // However the final position is going to be different! It is chosen by FindBestWindowPosForPopup().
    // e.g. Menus tend to overlap each other horizontally to amplify relative Z-ordering.
    //    if (window->DC.CursorPos.x > KDB_IMGUI_EXTENSION::CurrentMenuXScreenEnd) {
    //      //to new line
    //      window->DC.CursorPos.x = KDB_IMGUI_EXTENSION::CurrentMenuXScreenStart;
    //      window->DC.CursorPos.y += window->DC.CurrLineSize.y;
    //    }
    ImGuiContext &g = *GImGui;
    const ImGuiStyle &style = g.Style;
    const ImGuiID id = window->GetID(label);
    bool menu_is_open = ImGui::IsPopupOpen(id, ImGuiPopupFlags_None);

    // Sub-menus are ChildWindow so that mouse can be hovering across them (otherwise top-most popup menu would steal focus and not allow hovering on parent menu)
    // The first menu in a hierarchy isn't so hovering doesn't get across (otherwise e.g. resizing borders with ImGuiButtonFlags_FlattenChildren would react), but top-most BeginMenu() will bypass that limitation.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_ChildMenu | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNavFocus;
    if (window->Flags & ImGuiWindowFlags_ChildMenu)
      window_flags |= ImGuiWindowFlags_ChildWindow;

    // If a menu with same the ID was already submitted, we will append to it, matching the behavior of Begin().
    // We are relying on a O(N) search - so O(N log N) over the frame - which seems like the most efficient for the expected small amount of BeginMenu() calls per frame.
    // If somehow this is ever becoming a problem we can switch to use e.g. ImGuiStorage mapping key to last frame used.
    if (g.MenusIdSubmittedThisFrame.contains(id)) {
      if (menu_is_open)
        menu_is_open = ImGui::BeginPopupEx(id, window_flags);// menu_is_open can be 'false' when the popup is completely clipped (e.g. zero size display)
      else
        g.NextWindowData.ClearFlags();// we behave like Begin() and need to consume those values
      return menu_is_open;
    }

    // Tag menu as used. Next time BeginMenu() with same ID is called it will append to existing menu
    g.MenusIdSubmittedThisFrame.push_back(id);

    ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    // Odd hack to allow hovering across menus of a same menu-set (otherwise we wouldn't be able to hover parent without always being a Child window)
    // This is only done for items for the menu set and not the full parent window.
    const bool menuset_is_open = IsRootOfOpenMenuSet();
    if (menuset_is_open)
      ImGui::PushItemFlag(ImGuiItemFlags_NoWindowHoverableCheck, true);

    ImVec2 popup_pos, pos = window->DC.CursorPos;
    ImGui::PushID(label);
    if (!enabled)
      ImGui::BeginDisabled();
    const ImGuiMenuColumns *offsets = &window->DC.MenuColumns;
    bool pressed;

    // We use ImGuiSelectableFlags_NoSetKeyOwner to allow down on one menu item, move, up on another.
    const ImGuiSelectableFlags selectable_flags = ImGuiSelectableFlags_NoHoldingActiveID | ImGuiSelectableFlags_NoSetKeyOwner | ImGuiSelectableFlags_SelectOnClick | ImGuiSelectableFlags_DontClosePopups;
    if (window->DC.LayoutType == ImGuiLayoutType_Horizontal) {
      // Menu inside an horizontal menu bar
      // Selectable extend their highlight by half ItemSpacing in each direction.
      // For ChildMenu, the popup position will be overwritten by the call to FindBestWindowPosForPopup() in Begin()
      popup_pos = ImVec2(pos.x - 1.0f - IM_TRUNC(style.ItemSpacing.x * 0.5f), pos.y - style.FramePadding.y + window->MenuBarHeight());

      window->DC.CursorPos.x += IM_TRUNC(style.ItemSpacing.x * 0.5f);
      //check if we exceed the KDB_IMGUI_EXTENSION::CurrentMenuXScreenEnd
      //TODO: Implement

      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x * 2.0f, style.ItemSpacing.y));
      float w = label_size.x;

      ImVec2 text_pos(window->DC.CursorPos.x + offsets->OffsetLabel,
                      window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
      pressed = ImGui::Selectable("", menu_is_open, selectable_flags, ImVec2(w, label_size.y));
      ImGui::RenderText(text_pos, label);
      ImGui::PopStyleVar();
      window->DC.CursorPos.x += IM_TRUNC(style.ItemSpacing.x * (-1.0f + 0.5f));// -1 spacing to compensate the spacing added when Selectable() did a SameLine(). It would also work to call SameLine() ourselves after the PopStyleVar().
    }
    if (!enabled)
      ImGui::EndDisabled();

    const bool hovered = (g.HoveredId == id) && enabled && !g.NavDisableMouseHover;
    if (menuset_is_open)
      ImGui::PopItemFlag();

    bool want_open = false;
    bool want_close = false;
    if (window->DC.LayoutType == ImGuiLayoutType_Vertical)// (window->Flags & (ImGuiWindowFlags_Popup|ImGuiWindowFlags_ChildMenu))
    {
      //todo: implement
    } else {
      // Menu bar
      if (menu_is_open && pressed && menuset_is_open)// Click an open menu again to close it
      {
        want_close = true;
        want_open = menu_is_open = false;
      } else if (pressed || (hovered && menuset_is_open && !menu_is_open))// First click to open, then hover to open others
      {
        want_open = true;
      } else if (g.NavId == id && g.NavMoveDir == ImGuiDir_Down)// Nav-Down to open
      {
        want_open = true;
        ImGui::NavMoveRequestCancel();
      }
    }

    if (!enabled)// explicitly close if an open menu becomes disabled, facilitate users code a lot in pattern such as 'if (BeginMenu("options", has_object)) { ..use object.. }'
      want_close = true;
    if (want_close && ImGui::IsPopupOpen(id, ImGuiPopupFlags_None))
      ImGui::ClosePopupToLevel(g.BeginPopupStack.Size, true);

    ImGui::PopID();

    if (want_open && !menu_is_open && g.OpenPopupStack.Size > g.BeginPopupStack.Size) {
      // Don't reopen/recycle same menu level in the same frame, first close the other menu and yield for a frame.
      ImGui::OpenPopup(label);
    } else if (want_open) {
      menu_is_open = true;
      ImGui::OpenPopup(label);
    }

    if (menu_is_open) {
      ImGuiLastItemData last_item_in_parent = g.LastItemData;
      ImGui::SetNextWindowPos(popup_pos, ImGuiCond_Always);                 // Note: misleading: the value will serve as reference for FindBestWindowPosForPopup(), not actual pos.
      ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, style.PopupRounding);// First level will use _PopupRounding, subsequent will use _ChildRounding
      menu_is_open = ImGui::BeginPopupEx(id, window_flags);                 // menu_is_open can be 'false' when the popup is completely clipped (e.g. zero size display)
      ImGui::PopStyleVar();
      if (menu_is_open) {
        // Restore LastItemData so IsItemXXXX functions can work after BeginMenu()/EndMenu()
        // (This fixes using IsItemClicked() and IsItemHovered(), but IsItemHovered() also relies on its support for ImGuiItemFlags_NoWindowHoverableCheck)
        g.LastItemData = last_item_in_parent;
        if (g.HoveredWindow == window)
          g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;
      }
    } else {
      g.NextWindowData.ClearFlags();// We behave like Begin() and need to consume those values
    }

    return menu_is_open;
  }

  bool KDB_IMGUI_EXTENSION::BeginMenu(const char *label, bool enabled) {
    return ImGui::BeginMenu(label, true);
    //    return BeginMenuEx(label, NULL, enabled);
  }

  void KDB_IMGUI_EXTENSION::EndMenu() {
    return ImGui::EndMenu();
  }

  ImRect MenuBarRect(ImVec2 windowScreenPos, float menuBarWidth, float menuBarHeight) {
    float y1 = windowScreenPos.y;
    return ImRect(windowScreenPos.x,
                  y1,
                  windowScreenPos.x + menuBarWidth,
                  y1 + ImGui::GetFrameHeight());
  }

  bool BeginMenubar(ImVec2 windowScreenPos, float menuBarWidth, float menuBarHeight) {
    ImGuiWindow *window = ImGui::GetCurrentWindow();

    if (window->SkipItems)
      return false;
    /*if (!(window->Flags & ImGuiWindowFlags_MenuBar))
			return false;*/

    IM_ASSERT(!window->DC.MenuBarAppending);
    ImGui::BeginGroup();// Backup position on layer 0 // FIXME: Misleading to use a group for that backup/restore
    ImGui::PushID("##menubar");

    const ImVec2 padding = window->WindowPadding;

    // We don't clip with current window clipping rectangle as it is already set to the area below. However we clip with window full rect.
    // We remove 1 worth of rounding to Max.x to that text in long menus and small windows don't tend to display over the lower-right rounded area, which looks particularly glitchy.
    ImRect MenuBarRect = KDB_IMGUI_EXTENSION::MenuBarRect(windowScreenPos, menuBarWidth, menuBarHeight);

    ImRect bar_rect = MenuBarRect;
    ImRect clip_rect(
            IM_ROUND(bar_rect.Min.x + window->WindowBorderSize),
            IM_ROUND(bar_rect.Min.y + window->WindowBorderSize),
            IM_ROUND(ImMax(bar_rect.Min.x, bar_rect.Max.x - ImMax(window->WindowRounding, window->WindowBorderSize))),
            IM_ROUND(bar_rect.Max.y));

    ImGui::GetForegroundDrawList()->AddRect(
            clip_rect.Min,
            clip_rect.Max,
            ImColor(155, 155, 0, 255));

    KDB_IMGUI_EXTENSION::CurrentMenuXScreenStart = clip_rect.Min.x;
    KDB_IMGUI_EXTENSION::CurrentMenuXScreenEnd = clip_rect.Max.x;

    clip_rect.ClipWith(window->OuterRectClipped);
    ImGui::PushClipRect(clip_rect.Min, clip_rect.Max, false);

    // We overwrite CursorMaxPos because BeginGroup sets it to CursorPos (essentially the .EmitItem hack in EndMenuBar() would need something analogous here, maybe a BeginGroupEx() with flags).
    window->DC.CursorPos = window->DC.CursorMaxPos =
            ImVec2(bar_rect.Min.x + window->DC.MenuBarOffset.x,
                   bar_rect.Min.y + window->DC.MenuBarOffset.y);

    window->DC.LayoutType = ImGuiLayoutType_Horizontal;
    window->DC.NavLayerCurrent = ImGuiNavLayer_Menu;
    window->DC.MenuBarAppending = true;
    ImGui::AlignTextToFramePadding();
    return true;
  }// namespace KDB_IMGUI_EXTENSION

  void EndMenubar() {
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
      return;
    ImGuiContext &g = *GImGui;

    // Nav: When a move request within one of our child menu failed, capture the request to navigate among our siblings.
    if (ImGui::NavMoveRequestButNoResultYet() && (g.NavMoveDir == ImGuiDir_Left || g.NavMoveDir == ImGuiDir_Right) && (g.NavWindow->Flags & ImGuiWindowFlags_ChildMenu)) {
      // Try to find out if the request is for one of our child menu
      ImGuiWindow *nav_earliest_child = g.NavWindow;
      while (nav_earliest_child->ParentWindow && (nav_earliest_child->ParentWindow->Flags & ImGuiWindowFlags_ChildMenu))
        nav_earliest_child = nav_earliest_child->ParentWindow;
      if (nav_earliest_child->ParentWindow == window && nav_earliest_child->DC.ParentLayoutType == ImGuiLayoutType_Horizontal && (g.NavMoveFlags & ImGuiNavMoveFlags_Forwarded) == 0) {
        // To do so we claim focus back, restore NavId and then process the movement request for yet another frame.
        // This involve a one-frame delay which isn't very problematic in this situation. We could remove it by scoring in advance for multiple window (probably not worth bothering)
        const ImGuiNavLayer layer = ImGuiNavLayer_Menu;
        IM_ASSERT(window->DC.NavLayersActiveMaskNext & (1 << layer));// Sanity check
        ImGui::FocusWindow(window);
        ImGui::SetNavID(window->NavLastIds[layer], layer, 0, window->NavRectRel[layer]);

        g.NavDisableHighlight = true;// Hide highlight for the current frame so we don't see the intermediary selection.
        g.NavDisableMouseHover = g.NavMousePosDirty = true;

        ImGui::NavMoveRequestForward(g.NavMoveDir, g.NavMoveClipDir, g.NavMoveFlags, g.NavMoveScrollFlags);// Repeat
      }
    }

    IM_MSVC_WARNING_SUPPRESS(6011);// Static Analysis false positive "warning C6011: Dereferencing NULL pointer 'window'"
    // IM_ASSERT(window->Flags & ImGuiWindowFlags_MenuBar); // NOTE(Yan): Needs to be commented out because Jay
    IM_ASSERT(window->DC.MenuBarAppending);
    ImGui::PopClipRect();
    ImGui::PopID();
    window->DC.MenuBarOffset.x = window->DC.CursorPos.x - window->Pos.x;// Save horizontal position so next append can reuse it. This is kinda equivalent to a per-layer CursorPos.
    g.GroupStack.back().EmitItem = false;
    ImGui::EndGroup();// Restore position on layer 0
    window->DC.LayoutType = ImGuiLayoutType_Vertical;
    window->DC.NavLayerCurrent = ImGuiNavLayer_Main;
    window->DC.MenuBarAppending = false;
  }

}// namespace KDB_IMGUI_EXTENSION

////----------LOCAL FUNCTIONS----------------------------------------------------------------------------------------------
//Dockspace

static void renderMainMenuBar(ImVec2 startScreen, float width, float height) {
  const ApplicationSpecification &appSpec = m_Specification;
  const ApplicationTitleBarSettings &titleBarSettings = appSpec.TitleBarSettings;
  const ApplicationWindowSettings &windowSettings = appSpec.WindowSettings;
  auto cursorBeginScreenPos = ImGui::GetCursorScreenPos();
  ImGui::SetCursorScreenPos(startScreen);

  if (titleBarSettings.CustomTitleBar) {
    if (titleBarSettings.MainMenuBarCallback != nullptr) {
      ImVec2 cursPos = ImGui::GetCursorPos();
      const ImVec2 screenSpaceStart = KDB_IMGUI_EXTENSION::windowToScreenSpace(cursPos);
      const ImVec2 screenSpaceEnd = KDB_IMGUI_EXTENSION::windowToScreenSpace(ImVec2(cursPos.x + width, cursPos.y + height));
      ImGui::BeginGroup();

      if (KDB_IMGUI_EXTENSION::BeginMenubar(screenSpaceStart, width, height)) {
        (*titleBarSettings.MainMenuBarCallback)();
      }

      KDB_IMGUI_EXTENSION::EndMenubar();
      ImGui::EndGroup();
    }
  }

  ImGui::SetCursorScreenPos(cursorBeginScreenPos);
}

static void renderTitleBar(float &outHeight) {
  const ApplicationSpecification &appSpec = m_Specification;
  const ApplicationTitleBarSettings &titleBarSettings = appSpec.TitleBarSettings;
  const ApplicationWindowSettings &windowSettings = appSpec.WindowSettings;

  const float titlebarHeight = titleBarSettings.Height;
  outHeight = titlebarHeight;
  const bool isMaximized = ImGui_ImplVKGlfw_isWindowMaximized();

  float titlebarHorizontalOffset = isMaximized ? titleBarSettings.StartMaximized.x : titleBarSettings.StartWindowed.x;
  float titlebarVerticalOffset = isMaximized ? titleBarSettings.StartMaximized.y : titleBarSettings.StartWindowed.y;

  const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

  //set curstor to 0,0 off the window
  ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));

  const ImVec2 titlebarMin =
          {ImGui::GetCursorScreenPos().x + titlebarHorizontalOffset,
           ImGui::GetCursorScreenPos().y - titlebarVerticalOffset};

  const ImVec2 titlebarMax = {
          ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - titlebarVerticalOffset,
          ImGui::GetCursorScreenPos().y + titlebarHeight - titlebarVerticalOffset};

  auto *bgDrawList = ImGui::GetBackgroundDrawList();
  auto *fgDrawList = ImGui::GetForegroundDrawList();

  bgDrawList->AddRectFilled(titlebarMin, titlebarMax, KDB_IMGUI_EXTENSION::Colors::Theme::titlebar);
  fgDrawList->AddRect(titlebarMin, titlebarMax, KDB_IMGUI_EXTENSION::Colors::Theme::titlebar);

  //set cursor to the start of the titlebar
  ImGui::SetCursorPos(ImVec2(titlebarHorizontalOffset, titlebarVerticalOffset));

  ImVec2 titleBarStartScreen = {
          ImGui::GetCursorScreenPos().x,
          ImGui::GetCursorScreenPos().y};
  ImVec2 titleBarStartWindow = KDB_IMGUI_EXTENSION::screenToWindowSpace(titleBarStartScreen);
  ImVec2 defaultSpacing = ImGui::GetStyle().ItemSpacing;

  //draw logo
  ImVec2 logoStartScreen = {
          titleBarStartScreen.x + defaultSpacing.x,
          titleBarStartScreen.y + defaultSpacing.y};
  ImVec2 logoStartWindow = KDB_IMGUI_EXTENSION::screenToWindowSpace(logoStartScreen);

  ImVec2 logoEndScreen = {
          logoStartScreen.x + titleBarSettings.LogoSize.x,
          logoStartScreen.y + titleBarSettings.LogoSize.y};
  ImVec2 logoEndWindow = KDB_IMGUI_EXTENSION::screenToWindowSpace(logoEndScreen);
  fgDrawList->AddRect(logoStartScreen, logoEndScreen, KDB_IMGUI_EXTENSION::Colors::Theme::invalidLogo);

  //centered draw title
  ImVec2 titleStartScreen;
  ImVec2 titleStartWindow;
  {
    float windowHalfSize = ImGui::GetWindowWidth() * 0.5f;
    float textHalfSize = ImGui::CalcTextSize(appSpec.Name.c_str()).x * 0.5f;
    float startX = windowHalfSize - textHalfSize;
    float startY = ImGui::CalcTextSize(appSpec.Name.c_str()).y * 0.5f;
    titleStartScreen = {
            ImGui::GetWindowPos().x + startX,
            ImGui::GetWindowPos().y + startY};
    titleStartWindow = KDB_IMGUI_EXTENSION::screenToWindowSpace(titleStartScreen);
  }

  fgDrawList->AddText(titleStartScreen,
                      KDB_IMGUI_EXTENSION::Colors::Theme::text, appSpec.Name.c_str());


  ImVec2 menuBarStartScreen;
  ImVec2 menuBarStartWindow;
  ImVec2 menuBarEndScreen;
  ImVec2 menuBarEndWindow;
  //draw menu bar between centered text and logo
  {
    const float width = titleStartScreen.x - logoEndScreen.x;
    const float height = titlebarHeight;
    menuBarStartScreen = {
            logoEndScreen.x + defaultSpacing.x,
            titleBarStartScreen.y};
    menuBarStartWindow = KDB_IMGUI_EXTENSION::screenToWindowSpace(menuBarStartScreen);
    menuBarEndScreen = {
            menuBarStartScreen.x + width,
            menuBarStartScreen.y + height};
    menuBarEndWindow = KDB_IMGUI_EXTENSION::screenToWindowSpace(menuBarEndScreen);

    //debug draw
    fgDrawList->AddRect(menuBarStartScreen, menuBarEndScreen, KDB_IMGUI_EXTENSION::Colors::Theme::mainMenuBarOutLine);
    renderMainMenuBar(menuBarStartScreen, width, height);
  }
}

static void renderFullScreenDockspace() {
  const ApplicationSpecification &appSpec = m_Specification;
  const ApplicationWindowSettings &windowSettings = appSpec.WindowSettings;
  const ApplicationTitleBarSettings &titleBarSettings = appSpec.TitleBarSettings;


  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

  ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::SetNextWindowViewport(viewport->ID);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  if (!titleBarSettings.CustomTitleBar && titleBarSettings.MainMenuBarCallback != nullptr) {
    window_flags |= ImGuiWindowFlags_MenuBar;
  }

  const bool isMaximized = ImGui_ImplVKGlfw_isWindowMaximized();

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, isMaximized ? ImVec2(6.0f, 6.0f) : ImVec2(1.0f, 1.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);

  ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{0.0f, 0.0f, 0.0f, 0.0f});
  ImGui::Begin("DockSpaceWindow", nullptr, window_flags);
  ImGui::PopStyleColor();// MenuBarBg
  ImGui::PopStyleVar(2);
  ImGui::PopStyleVar(2);

  {
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(50, 50, 50, 255));
    // Draw window border if the window is not maximized
    if (!isMaximized)
      KDB_IMGUI_EXTENSION::RenderWindowOuterBorders(ImGui::GetCurrentWindow());

    ImGui::PopStyleColor();// ImGuiCol_Border
  }

  if (titleBarSettings.CustomTitleBar) {
    float titleBarHeight;
    renderTitleBar(titleBarHeight);
    ImGui::SetCursorPosY(titleBarHeight);
    ImGui::SetCursorPosX(0.0f);
  }

  // Dockspace
  ImGuiIO &io = ImGui::GetIO();
  ImGuiStyle &style = ImGui::GetStyle();
  float minWinSizeX = style.WindowMinSize.x;
  style.WindowMinSize.x = 370.0f;
  ImGui::DockSpace(ImGui::GetID("MyDockspace"));
  style.WindowMinSize.x = minWinSizeX;
}


static void endFullScreenDockSpace() {
  ImGui::End();
}


void ImGui_ImplVKGlfw_init(ApplicationSpecification specification) {
  if (g_GlfwErrorCallback == nullptr) {
    g_GlfwErrorCallback = new std::function<void(int error, const char *description)>([](int error, const char *description) {
      fprintf(stderr, "GLFW error %d: %s\n", error, description);
    });
  }

  m_Specification = specification;
  // Setup GLFW window
  glfwSetErrorCallback([](int error, const char *description) {
    (*g_GlfwErrorCallback)(error, description);
  });

  if (!glfwInit()) {
    (*g_GlfwErrorCallback)(800, "GLFW: Failed to initialize GLFW");
    return;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  if (m_Specification.TitleBarSettings.CustomTitleBar) {
    //TODO: fixme in glfw submodule not working yet
    //      glfwWindowHint(GLFW_TITLEBAR, false);
  }

  if (!ImGui_ImplVKGlfw_getWindowSettings().WindowDecorated) {
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  }

  GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *videoMode = glfwGetVideoMode(primaryMonitor);

  int monitorX, monitorY;
  glfwGetMonitorPos(primaryMonitor, &monitorX, &monitorY);

  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  m_WindowHandle = glfwCreateWindow(ImGui_ImplVKGlfw_getWindowSettings().Width, ImGui_ImplVKGlfw_getWindowSettings().Height, m_Specification.Name.c_str(), NULL, NULL);

  if (ImGui_ImplVKGlfw_getWindowSettings().CenterWindow) {
    glfwSetWindowPos(m_WindowHandle,
                     monitorX + (videoMode->width - ImGui_ImplVKGlfw_getWindowSettings().Width) / 2,
                     monitorY + (videoMode->height - ImGui_ImplVKGlfw_getWindowSettings().Height) / 2);

    glfwSetWindowAttrib(m_WindowHandle, GLFW_RESIZABLE, ImGui_ImplVKGlfw_getWindowSettings().WindowResizeable ? GLFW_TRUE : GLFW_FALSE);
  }

  glfwShowWindow(m_WindowHandle);

  // Setup Vulkan
  if (!glfwVulkanSupported()) {
    (*g_GlfwErrorCallback)(900, "GLFW: Vulkan Not Supported");
    return;
  } else {
  }

  // Set icon
  //todo: implement
  //  GLFWimage icon;
  //  int channels;
  //  if (!m_Specification.IconPath.empty()) {
  //    std::string iconPathStr = m_Specification.IconPath.string();
  //    icon.pixels = stbi_load(iconPathStr.c_str(), &icon.width, &icon.height, &channels, 4);
  //    glfwSetWindowIcon(m_WindowHandle, 1, &icon);
  //    stbi_image_free(icon.pixels);
  //  }

  //TODO: remove me when glfw set hint in submodule is fixed
  glfwSetWindowAttrib(m_WindowHandle, GLFW_TITLEBAR, !m_Specification.TitleBarSettings.CustomTitleBar);


  ImVector<const char *> extensions;
  uint32_t extensions_count = 0;
  const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
  for (uint32_t i = 0; i < extensions_count; i++)
    extensions.push_back(glfw_extensions[i]);

  SetupVulkan(extensions);


  // Create Window Surface
  VkSurfaceKHR surface;
  VkResult err = glfwCreateWindowSurface(g_Instance, m_WindowHandle, g_Allocator, &surface);
  check_vk_result(err);

  // Create Framebuffers
  int w, h;
  glfwGetFramebufferSize(m_WindowHandle, &w, &h);
  g_wd = &g_MainWindowData;
  SetupVulkanWindow(g_wd, surface, w, h);

  s_AllocatedCommandBuffers.resize(g_wd->ImageCount);
  s_ResourceFreeQueue.resize(g_wd->ImageCount);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void) io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport / Platform Windows
  io.ConfigViewportsNoAutoMerge = false;
  io.ConfigViewportsNoTaskBarIcon = true;

  // Theme colors
  //  UI::SetHazelTheme();
  ImGui::StyleColorsDark();

  // Style
  ImGuiStyle &style = ImGui::GetStyle();
  style.WindowPadding = ImVec2(10.0f, 10.0f);
  style.FramePadding = ImVec2(8.0f, 6.0f);
  style.ItemSpacing = ImVec2(6.0f, 6.0f);
  style.ChildRounding = 6.0f;
  style.PopupRounding = 6.0f;
  style.FrameRounding = 6.0f;
  style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForVulkan(m_WindowHandle, true);
  ImGui_ImplVulkan_InitInfo init_info = {};
  init_info.Instance = g_Instance;
  init_info.PhysicalDevice = g_PhysicalDevice;
  init_info.Device = g_Device;
  init_info.QueueFamily = g_QueueFamily;
  init_info.Queue = g_Queue;
  init_info.PipelineCache = g_PipelineCache;
  init_info.DescriptorPool = g_DescriptorPool;
  init_info.Subpass = 0;
  init_info.MinImageCount = g_MinImageCount;
  init_info.ImageCount = g_wd->ImageCount;
  init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
  init_info.Allocator = g_Allocator;
  init_info.CheckVkResultFn = check_vk_result;
  ImGui_ImplVulkan_Init(&init_info, g_wd->RenderPass);

  // Load default font
  //todo: implement


  // Upload Fonts
  {
    // Use any command queue
    VkCommandPool command_pool = g_wd->Frames[g_wd->FrameIndex].CommandPool;
    VkCommandBuffer command_buffer = g_wd->Frames[g_wd->FrameIndex].CommandBuffer;

    err = vkResetCommandPool(g_Device, command_pool, 0);
    check_vk_result(err);
    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    err = vkBeginCommandBuffer(command_buffer, &begin_info);
    check_vk_result(err);

    //      ImGui_ImplVulkan_CreateFontsTexture();

    VkSubmitInfo end_info = {};
    end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    end_info.commandBufferCount = 1;
    end_info.pCommandBuffers = &command_buffer;
    err = vkEndCommandBuffer(command_buffer);
    check_vk_result(err);
    err = vkQueueSubmit(g_Queue, 1, &end_info, VK_NULL_HANDLE);
    check_vk_result(err);

    err = vkDeviceWaitIdle(g_Device);
    check_vk_result(err);
  }
}

void ImGui_ImplVKGlfw_startRender() {
  // Poll and handle events (inputs, window resize, etc.)
  // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
  // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
  // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
  // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
  glfwPollEvents();

  {
    std::scoped_lock<std::mutex> lock(m_EventQueueMutex);

    // Process custom event queue
    while (m_EventQueue.size() > 0) {
      auto &func = m_EventQueue.front();
      func();
      m_EventQueue.pop();
    }
  }

  // Resize swap chain?
  if (g_SwapChainRebuild) {
    int width, height;
    glfwGetFramebufferSize(m_WindowHandle, &width, &height);
    if (width > 0 && height > 0) {
      ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
      ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, &g_MainWindowData, g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
      g_MainWindowData.FrameIndex = 0;

      // Clear allocated command buffers from here since entire pool is destroyed
      s_AllocatedCommandBuffers.clear();
      s_AllocatedCommandBuffers.resize(g_MainWindowData.ImageCount);

      g_SwapChainRebuild = false;
    }
  }

  // Start the Dear ImGui frame
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  //create dockspace
  if (ImGui_ImplVKGlfw_getWindowSettings().CreateDefaultDockSpace) {
    renderFullScreenDockspace();
  }

  ImGui::Begin("Logs");
  for (auto &log: g_debugStats) {
    ImGui::Text("%s: %s", log.first.c_str(), log.second.c_str());
  }
  ImGui::End();
}

//ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//ImGuiIO& io = ImGui::GetIO();
void ImGui_ImplVKGlfw_endRender() {
  ImGuiIO &io = ImGui::GetIO();
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  ImGui_ImplVulkanH_Window *wd = g_wd;

  if (ImGui_ImplVKGlfw_getWindowSettings().CreateDefaultDockSpace) {
    endFullScreenDockSpace();
  }
  // Rendering
  ImGui::Render();
  ImDrawData *main_draw_data = ImGui::GetDrawData();
  const bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);
  wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
  wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
  wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
  wd->ClearValue.color.float32[3] = clear_color.w;
  if (!main_is_minimized)
    FrameRender(wd, main_draw_data);

  // Update and Render additional Platform Windows
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
  }

  // Present Main Platform Window
  if (!main_is_minimized)
    FramePresent(wd);
  else
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

  //  float time = GetTime();
  //  g_= time - m_LastFrameTime;
  //  m_TimeStep = glm::min<float>(m_FrameTime, 0.0333f);
  //  m_LastFrameTime = time;
}

void ImGui_ImplVKGlfw_shutdown() {
  // Cleanup
  VkResult err = vkDeviceWaitIdle(g_Device);
  check_vk_result(err);

  // Free resources in queue
  for (auto &queue: s_ResourceFreeQueue) {
    for (auto &func: queue)
      func();
  }
  s_ResourceFreeQueue.clear();

  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  //function pointers
  {
    delete g_GlfwErrorCallback;
    delete m_Specification.TitleBarSettings.MainMenuBarCallback;
  }

  CleanupVulkanWindow();
  CleanupVulkan();

  glfwDestroyWindow(m_WindowHandle);
  glfwTerminate();
}

RenderStats &ImGui_ImplVKGlfw_getRenderStats() {
  return s_RenderStats;
}

const float ImGui_ImplVKGlfw_getTime() {
  return (float) glfwGetTime();
}

//SPECIFICATIONS
ApplicationSpecification &ImGui_ImplVKGlfw_getApplicationSpecification() {
  return m_Specification;
}
const ApplicationTitleBarSettings &ImGui_ImplVKGlfw_getTitleBarSpecification() {
  return m_Specification.TitleBarSettings;
}
const ApplicationWindowSettings &ImGui_ImplVKGlfw_getWindowSettings() {
  return m_Specification.WindowSettings;
}

//LOGGING
void ImGui_ImplVKGlfw_setImplErrorCallback(const std::function<void(int error, const char *description)> *func) {
  (*g_GlfwErrorCallback) = *func;
}

const std::map<std::string, std::string> ImGui_ImplVKGlfw_getLog() {
  return g_debugStats;
}

void ImGui_ImplVKGlfw_addLog(const std::string &logMsg, const std::string &logType) {
  g_debugStats[logType] = logMsg;
}

//VULKAN
VkInstance ImGui_ImplVKGlfw_getInstance() {
  return g_Instance;
}
VkPhysicalDevice ImGui_ImplVKGlfw_getPhysicalDevice() {
  return g_PhysicalDevice;
}
VkDevice ImGui_ImplVKGlfw_getDevice() {
  return g_Device;
}
VkCommandBuffer ImGui_ImplVKGlfw_getCommandBuffer(bool begin) {
  ImGui_ImplVulkanH_Window *wd = &g_MainWindowData;

  // Use any command queue
  VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;

  VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
  cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  cmdBufAllocateInfo.commandPool = command_pool;
  cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  cmdBufAllocateInfo.commandBufferCount = 1;

  VkCommandBuffer &command_buffer = s_AllocatedCommandBuffers[wd->FrameIndex].emplace_back();
  auto err = vkAllocateCommandBuffers(g_Device, &cmdBufAllocateInfo, &command_buffer);

  VkCommandBufferBeginInfo begin_info = {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  err = vkBeginCommandBuffer(command_buffer, &begin_info);
  check_vk_result(err);

  return command_buffer;
}
void ImGui_ImplVKGlfw_flushCommandBuffer(VkCommandBuffer commandBuffer) {
  const uint64_t DEFAULT_FENCE_TIMEOUT = 100000000000;

  VkSubmitInfo end_info = {};
  end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  end_info.commandBufferCount = 1;
  end_info.pCommandBuffers = &commandBuffer;
  auto err = vkEndCommandBuffer(commandBuffer);
  check_vk_result(err);
  // Create fence to ensure that the command buffer has finished executing
  VkFenceCreateInfo fenceCreateInfo = {};
  fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceCreateInfo.flags = 0;
  VkFence fence;
  err = vkCreateFence(g_Device, &fenceCreateInfo, nullptr, &fence);
  check_vk_result(err);

  err = vkQueueSubmit(g_Queue, 1, &end_info, fence);
  check_vk_result(err);

  err = vkWaitForFences(g_Device, 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT);
  check_vk_result(err);

  vkDestroyFence(g_Device, fence, nullptr);
}
void submitResourceFree(std::function<void()> &&func) {
  s_ResourceFreeQueue[s_CurrentFrameIndex].emplace_back(func);
}


//callbacks
//--glfw
void ImGui_ImplVKGlfw_setImplErrorCallback(const std::function<void(int error, const char *description)> &func) {
  g_GlfwErrorCallback = new std::function<void(int error, const char *description)>(func);
}

//Glfw Window
GLFWwindow *ImGui_ImplVKGlfw_getMainWindowHandle() {
  return m_WindowHandle;
}
const bool ImGui_ImplVKGlfw_isWindowMaximized() {
  ImVec2 size = ImGui_ImplVKGlfw_getWindowSize();
  ImVec2 frameSize = ImGui_ImplVKGlfw_getWindowFrameSize();

  return (size.x + frameSize.x == ImGui_ImplVKGlfw_getMonitorWidth() &&
          size.y + frameSize.y == ImGui_ImplVKGlfw_getMonitorHeight());
}
float ImGui_ImplVKGlfw_getMonitorWidth() {
  GLFWmonitor *currentMonior = glfwGetWindowMonitor(m_WindowHandle);
  if (currentMonior == nullptr) {
    currentMonior = glfwGetPrimaryMonitor();
  }
  if (currentMonior == nullptr) {
    return 0.0f;
  }
  const GLFWvidmode *mode = glfwGetVideoMode(currentMonior);
  return mode->width;
}
float ImGui_ImplVKGlfw_getMonitorHeight() {
  GLFWmonitor *currentMonior = glfwGetWindowMonitor(m_WindowHandle);
  if (currentMonior == nullptr) {
    currentMonior = glfwGetPrimaryMonitor();
  }
  if (currentMonior == nullptr) {
    return 0.0f;
  }
  const GLFWvidmode *mode = glfwGetVideoMode(currentMonior);
  return mode->height;
}
ImVec2 ImGui_ImplVKGlfw_getWindowFrameSize() {
  int left, top, right, bottom;
  glfwGetWindowFrameSize(m_WindowHandle, &left, &top, &right, &bottom);
  return ImVec2(left, top);
}
ImVec2 ImGui_ImplVKGlfw_getWindowSize() {
  int width, height;
  glfwGetWindowSize(m_WindowHandle, &width, &height);
  return ImVec2(width, height);
}
ImVec2 ImGui_ImplVKGlfw_getWindowPos() {
  int x, y;
  glfwGetWindowPos(m_WindowHandle, &x, &y);
  return ImVec2(x, y);
}
void ImGui_ImplVKGlfw_setWindowSize(const ImVec2 &size) {
  glfwSetWindowSize(m_WindowHandle, size.x, size.y);
}
void ImGui_ImplVKGlfw_setWindowPos(const ImVec2 &pos) {
  glfwSetWindowPos(m_WindowHandle, pos.x, pos.y);
}


//Title bar
void ImGui_ImplVKGlfw_setTitleBarCallback(std::function<void()> &func) {
  //  m_ImplErrorCallback(404, "Set Title Bar Callback not implemented yet!");
  (*g_GlfwErrorCallback)(404, "Set Title Bar Callback not implemented yet!");
}