//
// Created by Kasper de Bruin on 06/02/2024.
//todo: cleanup with sections

#include "../headers/Backend.h"

#include <Animation/Animation.h>
#include <Animation/Animations.h>
#include <HBUI/HBUI.h>
#include <UIItems/HBUIItemBase.h>
#include <fonts/FontLoader.h>


#ifndef g_HBUICTX
HBContext *g_HBUICTX = NULL;
#endif


void renderWindowOuterBorders(ImGuiWindow *window) {

	IM_ASSERT(window != nullptr && "renderWindowOuterBorders() called with nullptr window");

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
		if (border_n == ImGuiDir_Left) {
			return ImRect(rect.Min.x - thickness, rect.Min.y + perp_padding, rect.Min.x + thickness,
			              rect.Max.y - perp_padding);
		}
		if (border_n == ImGuiDir_Right) {
			return ImRect(rect.Max.x - thickness, rect.Min.y + perp_padding, rect.Max.x + thickness,
			              rect.Max.y - perp_padding);
		}
		if (border_n == ImGuiDir_Up) {
			return ImRect(rect.Min.x + perp_padding, rect.Min.y - thickness, rect.Max.x - perp_padding,
			              rect.Min.y + thickness);
		}
		if (border_n == ImGuiDir_Down) {
			return ImRect(rect.Min.x + perp_padding, rect.Max.y - thickness, rect.Max.x - perp_padding,
			              rect.Max.y + thickness);
		}
		IM_ASSERT(0);
		return ImRect();
	};

	ImGuiContext &g   = *GImGui;
	float rounding    = window->WindowRounding;
	float border_size = 1.0f;// window->WindowBorderSize;
	if (border_size > 0.0f && !(window->Flags & ImGuiWindowFlags_NoBackground)) {
		window->DrawList->AddRect(window->Pos, {window->Pos.x + window->Size.x, window->Pos.y + window->Size.y},
		                          ImGui::GetColorU32(ImGuiCol_Border), rounding, 0, border_size);
	}

	int border_held = window->ResizeBorderHeld;
	if (border_held != -1) {
		const ImGuiResizeBorderDef &def = resize_border_def[border_held];
		ImRect border_r                 = GetResizeBorderRect(window, border_held, rounding, 0.0f);
		ImVec2 p1                       = ImLerp(border_r.Min, border_r.Max, def.SegmentN1);
		const float offsetX             = def.InnerDir.x * rounding;
		const float offsetY             = def.InnerDir.y * rounding;
		p1.x += 0.5f + offsetX;
		p1.y += 0.5f + offsetY;

		ImVec2 p2 = ImLerp(border_r.Min, border_r.Max, def.SegmentN2);
		p2.x += 0.5f + offsetX;
		p2.y += 0.5f + offsetY;

		window->DrawList->PathArcTo(p1, rounding, def.OuterAngle - IM_PI * 0.25f, def.OuterAngle);
		window->DrawList->PathArcTo(p2, rounding, def.OuterAngle, def.OuterAngle + IM_PI * 0.25f);
		window->DrawList->PathStroke(ImGui::GetColorU32(ImGuiCol_SeparatorActive), 0,
		                             ImMax(2.0f, border_size));// Thicker than usual
	}
	if (g.Style.FrameBorderSize > 0 && !(window->Flags & ImGuiWindowFlags_NoTitleBar) && !window->DockIsActive) {
		float y = window->Pos.y + window->TitleBarHeight() - 1;
		window->DrawList->AddLine(ImVec2(window->Pos.x + border_size, y),
		                          ImVec2(window->Pos.x + window->Size.x - border_size, y),
		                          ImGui::GetColorU32(ImGuiCol_Border), g.Style.FrameBorderSize);
	}
}

//-------------------------------------
// [SECTION] HBContext
//-------------------------------------

HBContext::~HBContext() {
	delete widgetManager;
	delete animManager;
}

void HBContext::initialize() {
	initialized = true;

	widgetManager = new HBWidgetManager();
	animManager   = new HBUI::Animation::HBAnimManager();
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
	HBUI_API HBContext *initialize(const std::string &title, int width, int height, MainWindowFlags flags) {
		if (g_HBUICTX == NULL) {
			g_HBUICTX = new HBContext();
		}
		g_HBUICTX->initialize();

		auto io = HBIO{
		    .title           = title,
		    .width           = width,
		    .height          = height,
		    .mainWindowFlags = flags};
		g_HBUICTX->io = io;


		if (!initPlatformBackend(g_HBUICTX)) {
			std::cerr << "Failed to initialize platform backend" << std::endl;
			return nullptr;
		}

		if (!initGraphicsBackend()) {
			std::cerr << "Failed to initialize graphics backend" << std::endl;
			return nullptr;
		}

		ImGuiIO &imIo             = ImGui::GetIO();
		imIo.FontAllowUserScaling = true;// activate zoom feature with ctrl + mousewheel


		afterBackendInitialized();
		return g_HBUICTX;
	}

	HBUI_API void afterBackendInitialized() {
		g_HBUICTX->fontLoader = new HBUI::Fonts::FontLoader(true);
		HBTime::init();
	}

	HBUI_API void setCurrentContext(HBContext *ctx) {
		g_HBUICTX = ctx;
	}

	HBUI_API HBContext *getCurrentContext() {
		return g_HBUICTX;
	}

	HBUI_API void clearContext() {//todo: implement
	}

	HBUI_API HBStyle &getStyle() {
		return getCurrentContext()->style;
	}

	HBUI_API HBIO &getIO() {
		return getCurrentContext()->io;
	}

	HBUI_API ImGuiWindow *getMainImGuiWindow() {
		HBContext *ctx = getCurrentContext();
		IM_ASSERT(ctx != nullptr && "HBUI::getMainImGuiWindow() but context is null");

		return ctx->mainWindow;
	}

	HBUI_API ImVec2 getWindowSize() {
		return ImGui::GetIO().DisplaySize;
	}

	HBUI_API ImVec2 getViewportPos() {
		return ImGui::GetMainViewport()->Pos;
	}

	HBUI_API ImVec2 getViewportSize() {
		return ImGui::GetMainViewport()->Size;
	}

	HBUI_API void update(float deltatime) {
	}

	HBUI_API void startFrame() {
		HBTime::startFrame();
		startRenderBackend();

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking |
		                                ImGuiWindowFlags_NoTitleBar |
		                                ImGuiWindowFlags_NoCollapse |
		                                ImGuiWindowFlags_NoResize |
		                                ImGuiWindowFlags_NoMove |
		                                ImGuiWindowFlags_NoBringToFrontOnFocus |
		                                ImGuiWindowFlags_NoNavFocus;

		ImGuiViewport *viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		const bool isMaximized = false;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, isMaximized ? ImVec2(6.0f, 6.0f) : ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{0.0f, 0.0f, 0.0f, 0.0f});

		ImGui::Begin("MainBorderlessWindow", nullptr, window_flags);

		ImGui::PopStyleColor();// MenuBarBg
		ImGui::PopStyleVar(2);
		ImGui::PopStyleVar(2);
		g_HBUICTX->mainWindow = ImGui::GetCurrentWindow();
		{
			ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(50, 50, 50, 255));
			if (!isMaximized)
				renderWindowOuterBorders(g_HBUICTX->mainWindow);
			ImGui::PopStyleColor();// ImGuiCol_Border
		}
		ImGui::End();

		g_HBUICTX->startFrame();


		{
			ImGui::Begin("test");

			//			ImGui::Button("\uf473");
			//			ImGui::Button(ICON_FA_CAMPGROUND);

			//			ImGui::SmallButton(ICON_FA_UBUNTU);
			//		ImGui::SmallButton(ICON_FA_CAMPGROUND);

			ImGui::Button("B");
			const std::string iconGit = HBUI::wchar32ToUtf8(NF_ICON_github);
			ImGui::Button(iconGit.c_str());
			ImGui::SmallButton(iconGit.c_str());
			//ImGui::Text("Icon: ICON_FA_BATTLE_NET %s", ICON_FA_BATTLE_NET);
			ImGui::End();
		}
	}

	HBUI_API void endFrame() {
		g_HBUICTX->endFrame();
		endRenderBackend();
		HBTime::endFrame();
	}

	HBUI_API bool wantToClose() {
		return getWindowShouldCloseBackend();
	}

	HBUI_API void shutdown() {
		shutdownBackend();
		delete g_HBUICTX;
		g_HBUICTX = NULL;
	}

	HBUI_API ImFont *getBigFont() {
		return g_HBUICTX->fontLoader->getBigFont();
	}

	HBUI_API void toggleFlag(int flag) {
		g_HBUICTX->io.mainWindowFlags ^= flag;
		setBackendWindowFlags(*g_HBUICTX);
	}

	HBUI_API bool isFlagSet(int *flags, int flag) {
		return (*flags & flag) == flag;
	}

	//-------------------------------------
	// [SECTION] Fonts
	//-------------------------------------
	HBUI_API float getWindowSizeDpiScaleFactor() {
		return getWindowSizeDpiScaleFactorBackend();
	}

	HBUI_API float getFontSizeIncreaseFactor() {
		return getFontSizeIncreaseFactorBackend();
	}

	HBUI_API ImVec2 getWindowScaleFactor() {
		return getWindowScaleFactorBackend();
	}
}// namespace HBUI