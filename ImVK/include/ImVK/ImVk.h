//
// Created by Kasper de Bruin on 11/02/2024.
//
#pragma once

#ifndef IMVK_IMPL_API
#define IMVK_IMPL_API
#endif


#ifdef _WIN32
#define NOMINMAX
#endif

#include <imgui.h>

namespace HBUI {
	namespace Backend {
		IMVK_IMPL_API bool initPlatformBackend(int width, int height, HBBackendWindowFlags backendWindowFlags, void *errorCallback = nullptr);
		IMVK_IMPL_API bool initGraphicsBackend();
		IMVK_IMPL_API void startRenderBackend();
		IMVK_IMPL_API void endRenderBackend(const HBPlatformWindowData& window);
		IMVK_IMPL_API void shutdownBackend();
		IMVK_IMPL_API bool isMaximizedBackend();
		IMVK_IMPL_API void setWindowShouldCloseBackend();
		IMVK_IMPL_API bool getWindowShouldCloseBackend();
		IMVK_IMPL_API ImVec2 getWindowSize();
		IMVK_IMPL_API ImVec2 getWindowPosition();
		IMVK_IMPL_API ImVec2 getMonitorSize();
		IMVK_IMPL_API ImVec2 getWindowFrameSize();
		IMVK_IMPL_API float getMonitorHeight();
		IMVK_IMPL_API float getMonitorWidth();
		IMVK_IMPL_API void shutdownBackend();
		IMVK_IMPL_API void setBackendWindowFlags(HBBackendWindowFlags flags);
		IMVK_IMPL_API float getWindowSizeDPIScaleFactor();
		IMVK_IMPL_API float getFontSizeIncreaseFactor();
		IMVK_IMPL_API ImVec2 getWindowScaleFactor();
		}// namespace Backend
}// namespace HBUI