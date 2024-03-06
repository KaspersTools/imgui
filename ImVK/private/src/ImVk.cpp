#include "../../../HBUI/include/HBUI/HBUIEnums.cpp"

#include "../include/ImVK/ImVk.h"

#include <cassert>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <functional>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <imgui_internal.h>
#include <map>
#include <optional>
#include <windows.h>

#define GLFW_INCLUDE_NONE  // GLFW including its own vulkan will break the build
#define GLFW_INCLUDE_VULKAN// GLFW including its own vulkan will break the build
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
//#include <vulkan/vulkan_beta.h>


#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../../misc/stb_image/stb_image.h"
#endif

#include <mutex>
#include <queue>
#include <thread>

struct ImVKDATA {
	ImVKDATA() : g_Allocator(nullptr), g_Instance(VK_NULL_HANDLE), g_PhysicalDevice(VK_NULL_HANDLE),
	             g_Device(VK_NULL_HANDLE), g_QueueFamily((uint32_t) -1), g_Queue(VK_NULL_HANDLE),
	             g_PipelineCache(VK_NULL_HANDLE), g_DescriptorPool(VK_NULL_HANDLE), g_MinImageCount(2),
	             g_SwapChainRebuild(false), g_wd(nullptr), s_CurrentFrameIndex(0) {}


	GLFWwindow *window                     = nullptr;
	VkAllocationCallbacks *g_Allocator     = NULL;
	VkInstance g_Instance                  = VK_NULL_HANDLE;
	VkPhysicalDevice g_PhysicalDevice      = VK_NULL_HANDLE;
	VkDevice g_Device                      = VK_NULL_HANDLE;
	uint32_t g_QueueFamily                 = (uint32_t) -1;
	VkQueue g_Queue                        = VK_NULL_HANDLE;
	VkPipelineCache g_PipelineCache        = VK_NULL_HANDLE;
	VkDescriptorPool g_DescriptorPool      = VK_NULL_HANDLE;
	int g_MinImageCount                    = 2;
	bool g_SwapChainRebuild                = false;
	VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;

	ImGui_ImplVulkanH_Window g_MainWindowData;
	ImGui_ImplVulkanH_Window *g_wd = &g_MainWindowData;

	std::vector<std::vector<VkCommandBuffer>> s_AllocatedCommandBuffers = {};
	std::vector<std::vector<std::function<void()>>> s_ResourceFreeQueue = {};
	uint32_t s_CurrentFrameIndex                                        = 0;

	std::mutex m_EventQueueMutex;
	std::queue<std::function<void()>> m_EventQueue;

	GLFWerrorfun g_GlfwErrorCallback = nullptr;
};


ImVKDATA *g_ImVKData = nullptr;

static void check_vk_result(VkResult err) {
	if (err == 0)
		return;
	fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
	if (err < 0)
		abort();
}

#define APP_USE_VULKAN_DEBUG_REPORT
#ifdef APP_USE_VULKAN_DEBUG_REPORT
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
#endif// APP_USE_VULKAN_DEBUG_REPORT


static bool IsExtensionAvailable(const ImVector<VkExtensionProperties> &properties, const char *extension) {
	for (const VkExtensionProperties &p: properties)
		if (strcmp(p.extensionName, extension) == 0)
			return true;
	return false;
}

static VkPhysicalDevice SetupVulkan_SelectPhysicalDevice() {
	uint32_t gpu_count;
	VkResult err = vkEnumeratePhysicalDevices(g_ImVKData->g_Instance, &gpu_count, nullptr);
	check_vk_result(err);
	IM_ASSERT(gpu_count > 0);

	ImVector<VkPhysicalDevice> gpus;
	gpus.resize(gpu_count);
	err = vkEnumeratePhysicalDevices(g_ImVKData->g_Instance, &gpu_count, gpus.Data);
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

static void SetupVulkan(ImVector<const char *> instance_extensions)
{
	VkResult err;

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
		if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME))
		{
			instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
			create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
		}
#endif

		// Enabling validation layers
#ifdef APP_USE_VULKAN_DEBUG_REPORT
		const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
		create_info.enabledLayerCount = 1;
		create_info.ppEnabledLayerNames = layers;
		instance_extensions.push_back("VK_EXT_debug_report");
#endif

		// Create Vulkan Instance
		create_info.enabledExtensionCount = (uint32_t)instance_extensions.Size;
		create_info.ppEnabledExtensionNames = instance_extensions.Data;
		err = vkCreateInstance(&create_info, g_ImVKData->g_Allocator, &g_ImVKData->g_Instance);
		check_vk_result(err);

		// Setup the debug report callback
#ifdef APP_USE_VULKAN_DEBUG_REPORT
		auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(g_ImVKData->g_Instance, "vkCreateDebugReportCallbackEXT");
		IM_ASSERT(vkCreateDebugReportCallbackEXT != nullptr);
		VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
		debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
		debug_report_ci.pfnCallback = debug_report;
		debug_report_ci.pUserData = nullptr;
		err = vkCreateDebugReportCallbackEXT(g_ImVKData->g_Instance, &debug_report_ci, g_ImVKData->g_Allocator, &g_ImVKData->g_DebugReport);
		check_vk_result(err);
#endif
	}

	// Select Physical Device (GPU)
	g_ImVKData->g_PhysicalDevice = SetupVulkan_SelectPhysicalDevice();

	// Select graphics queue family
	{
		uint32_t count;
		vkGetPhysicalDeviceQueueFamilyProperties(g_ImVKData->g_PhysicalDevice, &count, nullptr);
		VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * count);
		vkGetPhysicalDeviceQueueFamilyProperties(g_ImVKData->g_PhysicalDevice, &count, queues);
		for (uint32_t i = 0; i < count; i++)
			if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				g_ImVKData->g_QueueFamily = i;
				break;
			}
		free(queues);
		IM_ASSERT(g_ImVKData->g_QueueFamily != (uint32_t)-1);
	}

	// Create Logical Device (with 1 queue)
	{
		ImVector<const char*> device_extensions;
		device_extensions.push_back("VK_KHR_swapchain");

		// Enumerate physical device extension
		uint32_t properties_count;
		ImVector<VkExtensionProperties> properties;
		vkEnumerateDeviceExtensionProperties(g_ImVKData->g_PhysicalDevice, nullptr, &properties_count, nullptr);
		properties.resize(properties_count);
		vkEnumerateDeviceExtensionProperties(g_ImVKData->g_PhysicalDevice, nullptr, &properties_count, properties.Data);
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
		if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME))
			device_extensions.push_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
#endif

		const float queue_priority[] = { 1.0f };
		VkDeviceQueueCreateInfo queue_info[1] = {};
		queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_info[0].queueFamilyIndex = g_ImVKData->g_QueueFamily;
		queue_info[0].queueCount = 1;
		queue_info[0].pQueuePriorities = queue_priority;
		VkDeviceCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
		create_info.pQueueCreateInfos = queue_info;
		create_info.enabledExtensionCount = (uint32_t)device_extensions.Size;
		create_info.ppEnabledExtensionNames = device_extensions.Data;
		err = vkCreateDevice(g_ImVKData->g_PhysicalDevice, &create_info, g_ImVKData->g_Allocator, &g_ImVKData->g_Device);
		check_vk_result(err);
		vkGetDeviceQueue(g_ImVKData->g_Device, g_ImVKData->g_QueueFamily, 0, &g_ImVKData->g_Queue);
	}

	// Create Descriptor Pool
	// The example only requires a single combined image sampler descriptor for the font image and only uses one descriptor set (for that)
	// If you wish to load e.g. additional textures you may need to alter pools sizes.
	{
		VkDescriptorPoolSize pool_sizes[] =
		    {
		        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
		    };
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1;
		pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;
		err = vkCreateDescriptorPool(g_ImVKData->g_Device, &pool_info, g_ImVKData->g_Allocator, &g_ImVKData->g_DescriptorPool);
		check_vk_result(err);
	}
}

// All the ImGui_ImplVulkanH_XXX structures/functions are optional helpers used by the demo.
// Your real engine/app may not use them.
static void SetupVulkanWindow(ImGui_ImplVulkanH_Window *wd, VkSurfaceKHR surface, int width, int height) {
	wd->Surface = surface;

	// Check for WSI support
	VkBool32 res;
	vkGetPhysicalDeviceSurfaceSupportKHR(g_ImVKData->g_PhysicalDevice, g_ImVKData->g_QueueFamily, wd->Surface, &res);
	if (res != VK_TRUE) {
		fprintf(stderr, "Error no WSI support on physical device 0\n");
		exit(-1);
	}

	// Select Surface Format
	const VkFormat requestSurfaceImageFormat[]     = {VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM};
	const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
	wd->SurfaceFormat                              = ImGui_ImplVulkanH_SelectSurfaceFormat(g_ImVKData->g_PhysicalDevice, wd->Surface, requestSurfaceImageFormat, (size_t) IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

	// Select Present Mode
#ifdef APP_USE_UNLIMITED_FRAME_RATE
	VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR};
#else
	VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_FIFO_KHR};
#endif
	wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(g_ImVKData->g_PhysicalDevice, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
	//printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

	// Create SwapChain, RenderPass, Framebuffer, etc.
	IM_ASSERT(g_ImVKData->g_MinImageCount >= 2);
	ImGui_ImplVulkanH_CreateOrResizeWindow(g_ImVKData->g_Instance, g_ImVKData->g_PhysicalDevice, g_ImVKData->g_Device, wd, g_ImVKData->g_QueueFamily, g_ImVKData->g_Allocator, width, height, g_ImVKData->g_MinImageCount);
}

static void CleanupVulkan() {
	vkDestroyDescriptorPool(g_ImVKData->g_Device, g_ImVKData->g_DescriptorPool, g_ImVKData->g_Allocator);

#ifdef APP_USE_VULKAN_DEBUG_REPORT
	// Remove the debug report callback
	auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(g_ImVKData->g_Instance, "vkDestroyDebugReportCallbackEXT");
	vkDestroyDebugReportCallbackEXT(g_ImVKData->g_Instance, g_ImVKData->g_DebugReport, g_ImVKData->g_Allocator);
#endif

	vkDestroyDevice(g_ImVKData->g_Device, g_ImVKData->g_Allocator);
	vkDestroyInstance(g_ImVKData->g_Instance, g_ImVKData->g_Allocator);
}

static void CleanupVulkanWindow() {
	ImGui_ImplVulkanH_DestroyWindow(g_ImVKData->g_Instance, g_ImVKData->g_Device, &g_ImVKData->g_MainWindowData, g_ImVKData->g_Allocator);
}

static void FrameRender(ImGui_ImplVulkanH_Window *wd, ImDrawData *draw_data) {
	VkResult err;

	VkSemaphore image_acquired_semaphore  = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
	VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
	err                                   = vkAcquireNextImageKHR(g_ImVKData->g_Device, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
	if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
		g_ImVKData->g_SwapChainRebuild = true;
		return;
	}
	check_vk_result(err);

	ImGui_ImplVulkanH_Frame *fd = &wd->Frames[wd->FrameIndex];
	{
		err = vkWaitForFences(g_ImVKData->g_Device, 1, &fd->Fence, VK_TRUE, UINT64_MAX);// wait indefinitely instead of periodically checking
		check_vk_result(err);

		err = vkResetFences(g_ImVKData->g_Device, 1, &fd->Fence);
		check_vk_result(err);
	}
	{
		err = vkResetCommandPool(g_ImVKData->g_Device, fd->CommandPool, 0);
		check_vk_result(err);
		VkCommandBufferBeginInfo info = {};
		info.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
		check_vk_result(err);
	}
	{
		VkRenderPassBeginInfo info    = {};
		info.sType                    = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.renderPass               = wd->RenderPass;
		info.framebuffer              = fd->Framebuffer;
		info.renderArea.extent.width  = wd->Width;
		info.renderArea.extent.height = wd->Height;
		info.clearValueCount          = 1;
		info.pClearValues             = &wd->ClearValue;
		vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	// Record dear imgui primitives into command buffer
	ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

	// Submit command buffer
	vkCmdEndRenderPass(fd->CommandBuffer);
	{
		VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo info               = {};
		info.sType                      = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.waitSemaphoreCount         = 1;
		info.pWaitSemaphores            = &image_acquired_semaphore;
		info.pWaitDstStageMask          = &wait_stage;
		info.commandBufferCount         = 1;
		info.pCommandBuffers            = &fd->CommandBuffer;
		info.signalSemaphoreCount       = 1;
		info.pSignalSemaphores          = &render_complete_semaphore;

		err = vkEndCommandBuffer(fd->CommandBuffer);
		check_vk_result(err);
		err = vkQueueSubmit(g_ImVKData->g_Queue, 1, &info, fd->Fence);
		check_vk_result(err);
	}
}

static void FramePresent(ImGui_ImplVulkanH_Window *wd) {
	if (g_ImVKData->g_SwapChainRebuild)
		return;
	VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
	VkPresentInfoKHR info                 = {};
	info.sType                            = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	info.waitSemaphoreCount               = 1;
	info.pWaitSemaphores                  = &render_complete_semaphore;
	info.swapchainCount                   = 1;
	info.pSwapchains                      = &wd->Swapchain;
	info.pImageIndices                    = &wd->FrameIndex;
	VkResult err                          = vkQueuePresentKHR(g_ImVKData->g_Queue, &info);
	if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
		g_ImVKData->g_SwapChainRebuild = true;
		return;
	}
	check_vk_result(err);
	wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->SemaphoreCount;// Now we can use the next set of semaphores
}

namespace HBUI::Backend {
	//ImVK API0
	IMVK_IMPL_API bool initPlatformBackend(int width, int height, HBBackendWindowFlags backendWindowFlags, void *errorCallback) {
		g_ImVKData = new ImVKDATA();
		if (errorCallback != nullptr) {
			g_ImVKData->g_GlfwErrorCallback = (GLFWerrorfun) errorCallback;
		}
		// Setup GLFW window
		glfwSetErrorCallback([](int error, const char *description) {
			(g_ImVKData->g_GlfwErrorCallback)(error, description);
		});

		if (!glfwInit()) {
			(g_ImVKData->g_GlfwErrorCallback)(800, "GLFW: Failed to initialize GLFW");
			return false;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		GLFWmonitor *primaryMonitor  = glfwGetPrimaryMonitor();
		const GLFWvidmode *videoMode = glfwGetVideoMode(primaryMonitor);

		int monitorX, monitorY;
		glfwGetMonitorPos(primaryMonitor, &monitorX, &monitorY);

		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		//		context->io.width, context->io.height,
		//		NULL,
		g_ImVKData->window = glfwCreateWindow(width, height, "test", NULL, NULL);
		//		                                      context->io.title.c_str(),
		glfwSetWindowPos(g_ImVKData->window,
		                 monitorX + (videoMode->width - width) / 2,
		                 monitorY + (videoMode->height - height) / 2);
		glfwShowWindow(g_ImVKData->window);

		setBackendWindowFlags(backendWindowFlags);
		return true;
	}

	IMVK_IMPL_API bool initGraphicsBackend() {
		if (!glfwVulkanSupported()) {
			(g_ImVKData->g_GlfwErrorCallback)(900, "GLFW: Vulkan Not Supported");
			return false;
		}

		ImVector<const char *> extensions;
		uint32_t extensions_count    = 0;
		const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
		for (uint32_t i = 0; i < extensions_count; i++)
			extensions.push_back(glfw_extensions[i]);

		SetupVulkan(extensions);

		// Create Window Surface
		VkSurfaceKHR surface;
		VkResult err = glfwCreateWindowSurface(g_ImVKData->g_Instance, g_ImVKData->window, g_ImVKData->g_Allocator,
		                                       &surface);

		check_vk_result(err);

		// Create Framebuffers
		int w, h;
		glfwGetFramebufferSize(g_ImVKData->window, &w, &h);
		g_ImVKData->g_wd = &g_ImVKData->g_MainWindowData;
		SetupVulkanWindow(g_ImVKData->g_wd, surface, w, h);

		g_ImVKData->s_AllocatedCommandBuffers.resize(g_ImVKData->g_wd->ImageCount);
		g_ImVKData->s_ResourceFreeQueue.resize(g_ImVKData->g_wd->ImageCount);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		(void) io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport / Platform Windows
		io.ConfigViewportsNoAutoMerge   = false;
		io.FontAllowUserScaling         = true;
		io.ConfigViewportsNoTaskBarIcon = true;
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForVulkan(g_ImVKData->window, true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance                  = g_ImVKData->g_Instance;
		init_info.PhysicalDevice            = g_ImVKData->g_PhysicalDevice;
		init_info.Device                    = g_ImVKData->g_Device;
		init_info.QueueFamily               = g_ImVKData->g_QueueFamily;
		init_info.Queue                     = g_ImVKData->g_Queue;
		init_info.PipelineCache             = g_ImVKData->g_PipelineCache;
		init_info.DescriptorPool            = g_ImVKData->g_DescriptorPool;
		init_info.RenderPass                = g_ImVKData->g_wd->RenderPass;
		init_info.Subpass                   = 0;
		init_info.MinImageCount             = g_ImVKData->g_MinImageCount;
		init_info.ImageCount                = g_ImVKData->g_wd->ImageCount;
		init_info.MSAASamples               = VK_SAMPLE_COUNT_1_BIT;
		init_info.Allocator                 = g_ImVKData->g_Allocator;
		init_info.CheckVkResultFn           = check_vk_result;
		ImGui_ImplVulkan_Init(&init_info);

		{
			// Use any command queue
			VkCommandPool command_pool     = g_ImVKData->g_wd->Frames[g_ImVKData->g_wd->FrameIndex].CommandPool;
			VkCommandBuffer command_buffer = g_ImVKData->g_wd->Frames[g_ImVKData->g_wd->FrameIndex].CommandBuffer;

			err = vkResetCommandPool(g_ImVKData->g_Device, command_pool, 0);
			check_vk_result(err);
			VkCommandBufferBeginInfo begin_info = {};
			begin_info.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			err = vkBeginCommandBuffer(command_buffer, &begin_info);
			check_vk_result(err);

			VkSubmitInfo end_info       = {};
			end_info.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			end_info.commandBufferCount = 1;
			end_info.pCommandBuffers    = &command_buffer;
			err                         = vkEndCommandBuffer(command_buffer);
			check_vk_result(err);
			err = vkQueueSubmit(g_ImVKData->g_Queue, 1, &end_info, VK_NULL_HANDLE);
			check_vk_result(err);

			err = vkDeviceWaitIdle(g_ImVKData->g_Device);
			check_vk_result(err);
		}

		return true;
	}

	IMVK_IMPL_API void startRenderBackend() {
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();
		{
			std::scoped_lock<std::mutex> lock(g_ImVKData->m_EventQueueMutex);

			// Process custom event queue
			while (g_ImVKData->m_EventQueue.size() > 0) {
				auto &func = g_ImVKData->m_EventQueue.front();
				func();
				g_ImVKData->m_EventQueue.pop();
			}
		}
		// Resize swap chain?
		if (g_ImVKData->g_SwapChainRebuild) {
			int width, height;
			glfwGetFramebufferSize(g_ImVKData->window, &width, &height);
			if (width > 0 && height > 0) {
				ImGui_ImplVulkan_SetMinImageCount(g_ImVKData->g_MinImageCount);
				ImGui_ImplVulkanH_CreateOrResizeWindow(g_ImVKData->g_Instance, g_ImVKData->g_PhysicalDevice, g_ImVKData->g_Device,
				                                       &g_ImVKData->g_MainWindowData,
				                                       g_ImVKData->g_QueueFamily, g_ImVKData->g_Allocator, width, height,
				                                       g_ImVKData->g_MinImageCount);
				g_ImVKData->g_MainWindowData.FrameIndex = 0;

				// Clear allocated command buffers from here since entire pool is destroyed
				g_ImVKData->s_AllocatedCommandBuffers.clear();
				g_ImVKData->s_AllocatedCommandBuffers.resize(g_ImVKData->g_MainWindowData.ImageCount);

				g_ImVKData->g_SwapChainRebuild = false;
			}
		}

		// Start the Dear ImGui frame
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	IMVK_IMPL_API void endRenderBackend(const HBPlatformWindowData &window) {
		ImGuiIO &io                  = ImGui::GetIO();
		ImGui_ImplVulkanH_Window *wd = g_ImVKData->g_wd;

		ImGui::Render();
		ImDrawData *main_draw_data   = ImGui::GetDrawData();
		const bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);

		wd->ClearValue.color.float32[0] = window.clearColor.Value.x * window.clearColor.Value.w;
		wd->ClearValue.color.float32[1] = window.clearColor.Value.y * window.clearColor.Value.w;
		wd->ClearValue.color.float32[2] = window.clearColor.Value.z * window.clearColor.Value.w;
		wd->ClearValue.color.float32[3] = window.clearColor.Value.w;

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
	}

	IMVK_IMPL_API void setWindowShouldCloseBackend() {
		glfwSetWindowShouldClose(g_ImVKData->window, GLFW_TRUE);
	}

	IMVK_IMPL_API bool getWindowShouldCloseBackend() {
		return glfwWindowShouldClose(g_ImVKData->window);
	}

	IMVK_IMPL_API ImVec2 getWindowSize() {
		int width, height;
		glfwGetWindowSize(g_ImVKData->window, &width, &height);
		return ImVec2(width, height);
	}

	IMVK_IMPL_API ImVec2 getWindowPosition() {
		int x, y;
		glfwGetWindowPos(g_ImVKData->window, &x, &y);
		return ImVec2(x, y);
	}

	IMVK_IMPL_API ImVec2 getWindowFrameSize() {
		int width, height;
		glfwGetFramebufferSize(g_ImVKData->window, &width, &height);
		return {(float) width, (float) height};
	}

	IMVK_IMPL_API ImVec2 getMonitorSize() {
		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		if (monitor == nullptr) {
			return {0, 0};
		}

		const GLFWvidmode *mode = glfwGetVideoMode(monitor);
		if (mode == nullptr) {
			return {0, 0};
		}

		return {(float) mode->width, (float) mode->height};
	}

	IMVK_IMPL_API float getMonitorHeight() {
		return getMonitorSize().y;
	}

	IMVK_IMPL_API float getMonitorWidth() {
		return getMonitorSize().x;
	}

	IMVK_IMPL_API bool isMaximized() {
		ImVec2 size      = getWindowSize();
		ImVec2 frameSize = getWindowFrameSize();

		return (size.x + frameSize.x == getMonitorWidth() &&
		        size.y + frameSize.y == getMonitorHeight());
	}

	void shutdownBackend() {
		// Cleanup
		VkResult err = vkDeviceWaitIdle(g_ImVKData->g_Device);
		check_vk_result(err);

		// Free resources in queue
		for (auto &queue: g_ImVKData->s_ResourceFreeQueue) {
			for (auto &func: queue)
				func();
		}
		g_ImVKData->s_ResourceFreeQueue.clear();

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		CleanupVulkanWindow();
		CleanupVulkan();

		glfwDestroyWindow(g_ImVKData->window);
		glfwTerminate();

		//delete all left over pointeers
		delete g_ImVKData;
		g_ImVKData = nullptr;
	}

	void setDecorated(const bool isDecorated) {
		glfwSetWindowAttrib(g_ImVKData->window, GLFW_DECORATED, isDecorated);
	}
	void setTitleBar(const bool hasTitleBar) {
		//		glfwSetWindowAttrib(g_ImVKData->window, GLFW_TITLEBAR, hasTitleBar);
	}
	void setResize(const bool canResize) {
		glfwSetWindowAttrib(g_ImVKData->window, GLFW_RESIZABLE, canResize);
	}
	void setBackendWindowFlags(HBBackendWindowFlags backendWindowFlags) {
		if (backendWindowFlags & HBBackendWindowFlags_NoDecoration) {
			glfwSetWindowAttrib(g_ImVKData->window, GLFW_DECORATED, GLFW_FALSE);//CLOSE MINIMIZE BUTTONS ETX
		} else {
			glfwSetWindowAttrib(g_ImVKData->window, GLFW_DECORATED, GLFW_TRUE);
		}

		if (backendWindowFlags & HBBackendWindowFlags_NoTitleBar) {
						glfwSetWindowAttrib(g_ImVKData->window, GLFW_TITLEBAR, GLFW_FALSE);//No title bar, will leave the decoration
		} else {
						glfwSetWindowAttrib(g_ImVKData->window, GLFW_TITLEBAR, GLFW_TRUE);
		}

		if (backendWindowFlags & HBBackendWindowFlags_NoResize) {
			glfwSetWindowAttrib(g_ImVKData->window, GLFW_RESIZABLE, GLFW_FALSE);//No resizing the window
		} else {
			glfwSetWindowAttrib(g_ImVKData->window, GLFW_RESIZABLE, GLFW_TRUE);
		}

//		todo: implement this
//				if (backendWindowFlags & HBBackendWindowFlags_NoMove) {
//					glfwSetWindowAttrib(g_ImVKData->window, GLFW_FLOATING, GLFW_TRUE);
//				} else {
//					glfwSetWindowAttrib(g_ImVKData->window, GLFW_FLOATING, GLFW_FALSE);
//				}
	}
	float getWindowSizeDPIScaleFactor() {
#ifdef __APPLE__
		return 1.f;
#else
		ImVec2 scale = getWindowScaleFactor();
		return scale.x;
#endif
	}

	ImVec2 getWindowScaleFactor() {
		float xScale, y_scale;
		glfwGetWindowContentScale((GLFWwindow *) g_ImVKData->window, &xScale, &y_scale);
		return ImVec2(xScale, y_scale);
	}
}// namespace HBUI::Backend


/*
 ----------------------------------------------------------------------
 |	This function has to be out of namespace, because its objective c	|
 ----------------------------------------------------------------------
*/
#if __APPLE__
#import <AppKit/NSScreen.h>
#import <Foundation/Foundation.h>
#include <TargetConditionals.h>

float HBUI::Backend::getFontSizeIncreaseFactor() {
	float fontSizeIncreaseFactor = (float) NSScreen.mainScreen.backingScaleFactor;
	return fontSizeIncreaseFactor;
}

#else
float HBUI::Backend::getFontSizeIncreaseFactor() {
	return 1.f;
}
#endif