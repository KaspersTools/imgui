//
// Created by Kasper de Bruin on 11/02/2024.
//
#pragma once

#ifndef IMVK_IMPL_API
#define IMVK_IMPL_API
#endif
#include <imgui.h>

struct HBContext;
namespace HBUI::Backend {
	IMVK_IMPL_API bool initPlatformBackend(HBContext *context, void *errorCallback = nullptr);

	IMVK_IMPL_API bool initGraphicsBackend();

	IMVK_IMPL_API void startRenderBackend();

	IMVK_IMPL_API void endRenderBackend();

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

	IMVK_IMPL_API void setBackendWindowFlags(const HBContext &ctx);

	IMVK_IMPL_API float getWindowSizeDPIScaleFactor();
	IMVK_IMPL_API float getFontSizeIncreaseFactor();
	IMVK_IMPL_API ImVec2 getWindowScaleFactor();
}// namespace HBUI::Backend