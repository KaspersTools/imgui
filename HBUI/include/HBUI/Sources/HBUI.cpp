//
// Created by Kasper de Bruin on 06/02/2024.
//todo: cleanup with sections

#include "../headers/Backend.h"

#include <UIItems/HBUIItemBase.h>
#include <Animation/Animation.h>
#include <Animation/Animations.h>
#include <fonts/FontLoader.h>
#include <fa-brands/fontAwesome_codes.hpp>
#include <HBUI/HBUI.h>



#ifndef g_HBUICTX
HBContext *g_HBUICTX = NULL;
#endif


//-------------------------------------
// [SECTION] HBContext
//-------------------------------------

HBContext::~HBContext() {
  delete widgetManager;
  delete animManager;
}

void HBContext::initialize(){
  initialized = true;

  widgetManager = new HBWidgetManager();
  animManager = new HBUI::Animation::HBAnimManager();
}
void HBContext::startFrame() {
  IM_ASSERT(initialized && "HBContext::startFrame() called before HBContext::initialize()");

  animManager->startFrame();
  widgetManager->startFrame();
}
void HBContext::endFrame() {
  IM_ASSERT(initialized && "HBContext::endFrame() called before HBContext::initialize()");

  animManager->endFrame();
  widgetManager->endFrame();
}
bool HBContext::hasAnimation(const ImGuiID &id) {
  IM_ASSERT(initialized && "HBContext::hasAnimation() called before HBContext::initialize()");


  return animManager->hasAnimation(id);
}
bool HBContext::hasAnimation(const std::string &id) {
  IM_ASSERT(initialized && "HBContext::hasAnimation() called before HBContext::initialize()");

  return animManager->hasAnimation(ImGui::GetID(id.c_str()));
}
ImVec2 HBContext::addAnimation(const std::string &id, HBAnimProps<ImVec2> props) {
  IM_ASSERT(initialized && "HBContext::addAnimation() called before HBContext::initialize()");

  animManager->addAnimation(id, props);
}

//-------------------------------------
// [SECTION] HBUI
//-------------------------------------
namespace HBUI {
  HBUI_API HBContext *
  initialize(const std::string &title, int width, int height, MainWindowFlags flags) {
    if (g_HBUICTX == NULL) {
      g_HBUICTX = new HBContext();
    }
    g_HBUICTX->initialize();

    auto io = HBIO{
        .title = title,
        .width = width,
        .height = height,
        .mainWindowFlags = flags};
    g_HBUICTX->io = io;


    if (!initPlatformBackend(g_HBUICTX)) {
      std::cerr << "Failed to initialize platform backend" << std::endl;
      return nullptr;
    }

		if(!initGraphicsBackend(g_HBUICTX)){
			std::cerr << "Failed to initialize graphics backend" << std::endl;
			return nullptr;
		}

		ImGuiIO& imIo = ImGui::GetIO();
		imIo.FontAllowUserScaling = true; // activate zoom feature with ctrl + mousewheel


		afterBackendInitialized();
		return g_HBUICTX;
  }

	HBUI_API void afterBackendInitialized(){
	    g_HBUICTX->fontLoader = new HBUI::Fonts::FontLoader(true);


		  HBTime::init();
	}

  HBUI_API void
  setCurrentContext(HBContext *ctx) {
    g_HBUICTX = ctx;
  }

  HBUI_API HBContext *
  getCurrentContext() {
    return g_HBUICTX;
  }

  HBUI_API void
  clearContext() {//todo: implement
  }

  HBUI_API HBStyle &
  getStyle() {
    return getCurrentContext()->style;
  }

  HBUI_API HBIO &
  getIO() {
    return getCurrentContext()->io;
  }

  HBUI_API ImVec2
  getCursorViewportPos() {
    auto windowPos = ImVec2(0, 0);
    auto vp        = ImGui::GetMainViewport();

    return ImVec2(windowPos.x + vp->Pos.x,
                  windowPos.y + vp->Pos.y);
  }

  HBUI_API ImVec2
  getContentRegionAvail() {
    ImVec2 windowPos = ImGui::GetCursorScreenPos();
    auto   vp        = ImGui::GetMainViewport();

    return ImVec2(vp->Size.x - windowPos.x,
                  vp->Size.y - windowPos.y);
  }

  HBUI_API ImVec2
  getWindowSize() {
    return ImGui::GetIO().DisplaySize;
  }

  HBUI_API ImVec2
  getViewportPos() {
    return ImGui::GetMainViewport()->Pos;
  }

  HBUI_API ImVec2
  getViewportSize() {
    return ImGui::GetMainViewport()->Size;
  }

  HBUI_API void update(float deltatime) {

  }

  /*********************************************
    * Rendering
    * *********************************************/

  void fullScreenemptyWindow() {
  }

  HBUI_API void
  startFrame() {
    HBTime::startFrame();
    g_HBUICTX->startFrame();

    startRenderBackend();

		ImGui::Begin("test");
		ImGui::Text("This is a text with icons %s ...",  fontAwesome::java);
		ImGui::End();
  }

  HBUI_API void
  endFrame() {
    g_HBUICTX->endFrame();
    endRenderBackend();
    HBTime::endFrame();
  }

  HBUI_API bool
  wantToClose() {
    return getWindowShouldCloseBackend();
  }

  HBUI_API void
  shutdown() {
    shutdownBackend();
    delete g_HBUICTX;
    g_HBUICTX = NULL;
  }


  /*********************************************
    * Rendering
    * *********************************************/
  HBUI_API bool
  mouseOverRect(const ImVec2 &start, const ImVec2 &end) {
    ImVec2 mousePos = ImGui::GetMousePos();
    return mousePos.x > start.x && mousePos.x < end.x && mousePos.y > start.y && mousePos.y < end.y;
  }

  HBUI_API bool
  mouseOverCircle(const ImVec2 &center, float radius) {
    ImVec2 mousePos = ImGui::GetMousePos();
    return (mousePos.x - center.x) * (mousePos.x - center.x) + (mousePos.y - center.y) * (mousePos.y - center.y) <
           radius * radius;
  }

  /*********************************************
      * Flags
      * *********************************************/
  HBUI_API void toggleFlag(int flag) {
    g_HBUICTX->io.mainWindowFlags ^= flag;
    setBackendWindowFlags(*g_HBUICTX);
  }

  HBUI_API bool isFlagSet(int *flags, int flag) {
    return (*flags & flag) == flag;
  }

  HBUI_API ImGuiID getId(const char *str) {
//    ImGuiID seed = HBUI::getDrawData().IDStack.back();
//    ImGuiID id = ImHashStr(str, str_end ? (str_end - str) : 0, seed);
//    ImGuiContext& g = *Ctx;
    ImGuiID id = 10;
    return id;
  }

}// namespace HBUI