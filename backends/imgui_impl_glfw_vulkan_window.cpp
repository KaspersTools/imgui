//
// Created by Kasper de Bruin on 19/01/2024.
//

#include <imgui_impl_glfw_vulkan_window.h>


extern VkAllocationCallbacks* g_Allocator;
extern VkInstance g_Instance;
extern VkPhysicalDevice g_PhysicalDevice;
extern VkDevice g_Device;
extern uint32_t g_QueueFamily;
extern VkQueue g_Queue;
extern VkDebugReportCallbackEXT g_DebugReport;
extern VkPipelineCache g_PipelineCache;
extern VkDescriptorPool g_DescriptorPool;
extern ImGui_ImplVulkanH_Window g_MainWindowData;
extern int g_MinImageCount;
extern bool g_SwapChainRebuild;

VkResult g_err;
GLFWwindow* g_GlfwWindow;
ImGui_ImplVulkanH_Window *g_Wd;

void generateWindow(ImGuiConfigFlags imGuiConfigFlags) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        exit(-1);

    // Create window with Vulkan context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    g_GlfwWindow = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+Vulkan example", nullptr, nullptr);
    if (!glfwVulkanSupported()) {
        printf("GLFW: Vulkan Not Supported\n");
        exit(-1);
    }

    ImVector<const char *> extensions;
    uint32_t extensions_count = 0;
    const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
    for (uint32_t i = 0; i < extensions_count; i++)
        extensions.push_back(glfw_extensions[i]);
    SetupVulkan(extensions);

    // Create Window Surface
    VkSurfaceKHR surface;
    glfwCreateWindowSurface(g_Instance, g_GlfwWindow, g_Allocator, &surface);
    check_vk_result(g_err);

    // Create Framebuffers
    int w, h;
    glfwGetFramebufferSize(g_GlfwWindow, &w, &h);
    g_Wd = &g_MainWindowData;
    SetupVulkanWindow(g_Wd, surface, w, h);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= imGuiConfigFlags;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForVulkan(g_GlfwWindow, true);
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
    init_info.ImageCount = g_Wd->ImageCount;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = g_Allocator;
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info, g_Wd->RenderPass);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
}

void startRender(){
    glfwPollEvents();

    // Resize swap chain?
    if (g_SwapChainRebuild) {
        int width, height;
        glfwGetFramebufferSize(g_GlfwWindow, &width, &height);
        if (width > 0 && height > 0) {
            ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
            ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, &g_MainWindowData,
                                                   g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
            g_MainWindowData.FrameIndex = 0;
            g_SwapChainRebuild = false;
        }
    }

    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void endRender(){
    ImGuiIO &io = ImGui::GetIO();
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    // Rendering
    ImGui::Render();
    ImDrawData *main_draw_data = ImGui::GetDrawData();
    const bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);
    g_Wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
    g_Wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
    g_Wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
    g_Wd->ClearValue.color.float32[3] = clear_color.w;
    if (!main_is_minimized)
        FrameRender(g_Wd, main_draw_data);

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    // Present Main Platform Window
    if (!main_is_minimized)
        FramePresent(g_Wd);
}

void cleanupWindow(){
    // Cleanup
    g_err = vkDeviceWaitIdle(g_Device);
    check_vk_result(g_err);
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    CleanupVulkanWindow();
    CleanupVulkan();

    glfwDestroyWindow(g_GlfwWindow);
    glfwTerminate();
}