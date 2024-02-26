//
// Created by Kasper de Bruin on 13/02/2024.
//

#ifndef HBUI_BACKEND_H
#define HBUI_BACKEND_H

#include <imgui.h>

struct HBContext;

//-----------------------------------------------------------------------------
// [SECTION] ImVK
//-----------------------------------------------------------------------------
//Forward declaration
#ifndef IMVK_IMPL_API
#define IMVK_IMPL_API
#endif

IMVK_IMPL_API bool
initPlatformBackend(HBContext *context, void *errorCallback = nullptr);

IMVK_IMPL_API bool
initGraphicsBackend();

IMVK_IMPL_API void
startRenderBackend();

IMVK_IMPL_API void
endRenderBackend();

IMVK_IMPL_API void
shutdownBackend();

IMVK_IMPL_API bool
isMaximized();

IMVK_IMPL_API void
setWindowShouldCloseBackend();

IMVK_IMPL_API bool
getWindowShouldCloseBackend();

IMVK_IMPL_API ImVec2
getWindowSize();

IMVK_IMPL_API ImVec2
getMonitorSize();

IMVK_IMPL_API ImVec2
getWindowFrameSize();

IMVK_IMPL_API float
getMonitorHeight();

IMVK_IMPL_API float
getMonitorWidth();

IMVK_IMPL_API void
shutdownBackend();

IMVK_IMPL_API void
setBackendWindowFlags(const HBContext &ctx);

IMVK_IMPL_API ImVec2
getWindowScaleFactor();

IMVK_IMPL_API float
getWindowSizeDpiScaleFactor();

IMVK_IMPL_API float
getFontSizeIncreaseFactor();

#endif//HBUI_BACKEND_H
