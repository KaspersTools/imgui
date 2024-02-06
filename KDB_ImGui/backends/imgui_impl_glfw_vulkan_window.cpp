//
// Adapted from Dear ImGui Vulkan example
//
#include <map>
#include <stdio.h> // printf, fprintf
#include <stdlib.h>// abort


#define GLFW_INCLUDE_NONE  // GLFW including its own vulkan will break the build
#define GLFW_INCLUDE_VULKAN// GLFW including its own vulkan will break the build
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <imconfig.h>
#include <imgui.h>

#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "misc/stb_image/stb_image.h"
#endif

#include <KDB_ImGui/Extension.h>
#include <KDB_ImGui/backends/imgui_impl_glfw_vulkan_window.h>

#include <KDB_ImGui/themes/ThemeManager.h>
#include <KDB_ImGui/themes/Themes.h>

#include <mutex>
#include <queue>
#include <thread>


// Emedded font
GLFWwindow *m_WindowHandle = NULL;

std::mutex m_EventQueueMutex;
std::queue<std::function<void()>> m_EventQueue;

static VkAllocationCallbacks *g_Allocator = NULL;
static VkInstance g_Instance = VK_NULL_HANDLE;
static VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;
static VkDevice g_Device = VK_NULL_HANDLE;
static uint32_t g_QueueFamily = (uint32_t) -1;
static VkQueue g_Queue = VK_NULL_HANDLE;
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
  const VkFormat requestSurfaceImageFormat[] = {VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM,
                                                VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM};
  const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
  wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(g_PhysicalDevice, wd->Surface, requestSurfaceImageFormat,
                                                            (size_t) IM_ARRAYSIZE(requestSurfaceImageFormat),
                                                            requestSurfaceColorSpace);

  // Select Present Mode
#ifdef IMGUI_UNLIMITED_FRAME_RATE
  VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR};
#else
  VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_FIFO_KHR};
#endif
  wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(g_PhysicalDevice, wd->Surface, &present_modes[0],
                                                        IM_ARRAYSIZE(present_modes));
  //printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

  // Create SwapChain, RenderPass, Framebuffer, etc.
  IM_ASSERT(g_MinImageCount >= 2);
  ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, wd, g_QueueFamily, g_Allocator,
                                         width, height, g_MinImageCount);
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
  err = vkAcquireNextImageKHR(g_Device, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE,
                              &wd->FrameIndex);
  if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
    g_SwapChainRebuild = true;
    return;
  }
  check_vk_result(err);

  s_CurrentFrameIndex = (s_CurrentFrameIndex + 1) % g_MainWindowData.ImageCount;

  ImGui_ImplVulkanH_Frame *fd = &wd->Frames[wd->FrameIndex];
  {
    err = vkWaitForFences(g_Device, 1, &fd->Fence, VK_TRUE,
                          UINT64_MAX);// wait indefinitely instead of periodically checking
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
      vkFreeCommandBuffers(g_Device, fd->CommandPool, (uint32_t) allocatedCommandBuffers.size(),
                           allocatedCommandBuffers.data());
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

static void renderFullScreenDockspace() {
  const ApplicationSpecification &appSpec = m_Specification;
  const ApplicationTitleBarSettings &titleBarSettings = appSpec.TitleBarSettings;

  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

  ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::SetNextWindowViewport(viewport->ID);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

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
      HBUI::renderWindowOuterBorders(ImGui::GetCurrentWindow());

    ImGui::PopStyleColor();// ImGuiCol_Border
  }

  if (titleBarSettings.CustomTitleBar) {


    HBUI::beginTitleBar(
            titleBarSettings.DrawTitleCentered,
            appSpec.Name,
            titleBarSettings.HasLogo,
            titleBarSettings.Logo->GetDescriptorSet(),
            titleBarSettings.LogoDrawSize,
            ImVec2(titleBarSettings.StartMaximized.x, titleBarSettings.StartMaximized.y),
            ImVec2(titleBarSettings.StartWindowed.x, titleBarSettings.StartWindowed.y),
            titleBarSettings.Height,
            titleBarSettings.MainMenuBarExtraHeight,
            isMaximized,
            titleBarSettings.DebugInfo);
    HBUI::beginMainMenuBar();

    //render itemsi
    if (titleBarSettings.MainMenuBarCallback != nullptr)
      (*titleBarSettings.MainMenuBarCallback)();

    HBUI::endMainMenuBar();
    HBUI::endTitleBar();
  }

  //  KDB::IMGUI::endMainMenuBar();
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
    g_GlfwErrorCallback = new std::function<void(int error, const char *description)>(
            [](int error, const char *description) {
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

  m_WindowHandle = glfwCreateWindow(ImGui_ImplVKGlfw_getWindowSettings().Width,
                                    ImGui_ImplVKGlfw_getWindowSettings().Height, m_Specification.Name.c_str(), NULL,
                                    NULL);

  if (ImGui_ImplVKGlfw_getWindowSettings().CenterWindow) {
    glfwSetWindowPos(m_WindowHandle,
                     monitorX + (videoMode->width - ImGui_ImplVKGlfw_getWindowSettings().Width) / 2,
                     monitorY + (videoMode->height - ImGui_ImplVKGlfw_getWindowSettings().Height) / 2);

    glfwSetWindowAttrib(m_WindowHandle, GLFW_RESIZABLE,
                        ImGui_ImplVKGlfw_getWindowSettings().WindowResizeable ? GLFW_TRUE : GLFW_FALSE);
  }

  glfwShowWindow(m_WindowHandle);

  // Setup Vulkan
  if (!glfwVulkanSupported()) {
    (*g_GlfwErrorCallback)(900, "GLFW: Vulkan Not Supported");
    return;
  } else {
  }

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
  HBUI::Themes::ThemeManager::applyTheme(HBUI::Themes::ImGuiTheme_SoDark_AccentBlue);

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
  // Set icon
  ApplicationTitleBarSettings &titleBarSpecs = ImGui_ImplVKGlfw_getApplicationSpecification().TitleBarSettings;


  if (titleBarSpecs.HasLogo) {
    std::string iconPathStr = titleBarSpecs.LogoPath.string();
    int channels;
    GLFWimage icon = {};
    unsigned char *data = stbi_load(iconPathStr.c_str(), &icon.width, &icon.height, &channels, 4);
    int img_size = icon.width * icon.height * channels;
    //TODO: Implement
    //        if(specification.TitleBarSettings.LogoRemoveBackground) {
    //            const ImVec4 removeColor = specification.TitleBarSettings.LogoBackgroundColor;
    //            for (int i = 0; i < img_size; i += channels) {
    //                if (data[i] == removeColor.x && data[i + 1] == removeColor.y && data[i + 2] == removeColor.z) {
    //                    // Set alpha to 0
    ////                    std::string colorString = "R: " + std::to_string(data[i]) + " G: " + std::to_string(data[i + 1]) + " B: " + std::to_string(data[i + 2]) + " A: " + std::to_string(data[i + 3]);
    ////                    std::string xy = "X: " + std::to_string(i % width) + " Y: " + std::to_string(i / width);
    ////                    std::cout << colorString << "  | at: " << xy << std::endl;
    //                    if (channels == 4) {
    //                        data[i + 3] = 0;
    //                    }
    //                }
    //            }
    //        }

    icon.pixels = data;
    titleBarSpecs.Logo = std::make_shared<Image>(icon.width, icon.height, ImageFormat::RGBA, icon.pixels);
    stbi_image_free(icon.pixels);
  }
  // titleBarSpecs.Logo = std::make_shared<Image>();

  //
  //    if(!titleBarSpecs.Logo->LoadImageFromPath(titleBarSpecs.LogoPath.string())){
  //      (*g_GlfwErrorCallback)(900, "GLFW: Failed to load logo");
  //      titleBarSpecs.Logo.reset();
  //    }else{
  //      titleBarSpecs.Logo->Resize(titleBarSpecs.LogoDrawSize.x, titleBarSpecs.LogoDrawSize.y);
  //    }
  //}
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
      ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, &g_MainWindowData,
                                             g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
      g_MainWindowData.FrameIndex = 0;

      // Clear allocated command buffers from here since entire pool is destroyed
      s_AllocatedCommandBuffers.clear();
      s_AllocatedCommandBuffers.resize(g_MainWindowData.ImageCount);

      m_Specification.WindowSettings.Width = width;
      m_Specification.WindowSettings.Height = height;

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

  auto titleBarSpecs = ImGui_ImplVKGlfw_getTitleBarSpecification();
  if (titleBarSpecs.HasLogo) {
    titleBarSpecs.Logo.reset();
  }

  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  //function pointers
  {
    delete g_GlfwErrorCallback;
    delete m_Specification.TitleBarSettings.MainMenuBarCallback;
  }

  if (m_Specification.TitleBarSettings.HasLogo) {
  }

  CleanupVulkanWindow();
  CleanupVulkan();

  glfwDestroyWindow(m_WindowHandle);
  glfwTerminate();
}

bool ImGui_ImplVKGlfw_shouldClose() {
  return glfwWindowShouldClose(m_WindowHandle);
}

void ImGui_ImplVKGlfw_setShouldClose(const bool value) {
  glfwSetWindowShouldClose(m_WindowHandle, value);
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

void ImGui_ImplVKGlfw_check_vk_result(VkResult err) {
  check_vk_result(err);
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

VkCommandBuffer ImGui_ImplVKGlfw_getCommandBuffer() {
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

void ImGui_ImplVKGlfw_submitResourceFree(std::function<void()> &&func) {
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

bool ImGui_ImplVKGlfw_isWindowMaximized() {
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

GLFWmonitor *ImGui_ImplVKGlfw_getCurrentMonitor() {
  return glfwGetWindowMonitor(m_WindowHandle);
}


ImVec2 ImGui_ImplVKGlfw_getCurrentMonitorContentScale() {
  float xscale, yscale;
  auto monitor = glfwGetPrimaryMonitor();
  if(monitor == nullptr)
    return ImVec2(1.0f, 1.0f);

  auto mode = glfwGetVideoMode(monitor);
  if(mode == nullptr)
    return ImVec2(1.0f, 1.0f);

  int widthMM, heightMM;
  glfwGetMonitorPhysicalSize(monitor, &widthMM, &heightMM);

  float dpiX = mode->width / (widthMM / 25.4);
  float dpiY = mode->height / (heightMM / 25.4);

  return ImVec2(dpiX, dpiY);
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

IMGUI_IMPL_API ImGuiContext* ImGui_ImplVKGlfw_getCurrentContext(){
  return ImGui::GetCurrentContext();
}