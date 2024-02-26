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
	HBUI_API HBContext *
	    initialize(const std::string &title, int width, int height, MainWindowFlags flags) {
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
		//from hello_imgui
		HBIO& io = getIO();

		io.dpiWindowSizeFactor = getWindowSizeDpiScaleFactor();
		float fontSizeIncreaseFactor = 1.f;

#ifdef __APPLE__
		// Crisp fonts on macOS:
		// cf https://github.com/ocornut/imgui/issues/5301
		// Issue with macOS is that it pretends screen has 2x fewer pixels than it actually does.
		// This simplifies application development in most cases, but in our case we happen to render fonts at 1x scale
		// while screen renders at 2x scale.
		fontSizeIncreaseFactor = getFontSizeIncreaseFactor();
#endif
		io.fontRenderingScale = 1.0f / fontSizeIncreaseFactor;

		ImGui::GetIO().FontGlobalScale = io.fontRenderingScale;
		ImGui::GetIO().DisplayFramebufferScale = getWindowScaleFactor();


		g_HBUICTX->fontLoader = new HBUI::Fonts::FontLoader(true, true);

//		ImGui::GetIO().Fonts->ConfigData();

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
		auto vp          = ImGui::GetMainViewport();

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

		ImGui::Text("text %s", ICON_FA_CC_STRIPE);
		ImGui::Text("text %s", ICON_FA_CC_VISA);
		ImGui::Text("text %s", ICON_FA_CENTERCODE);
		ImGui::Text("text %s", ICON_FA_CENTOS);
		ImGui::Text("text %s", ICON_FA_CHROME);
		ImGui::Text("text %s", ICON_FA_CHROMECAST);
		ImGui::Text("text %s", ICON_FA_CLOUDFLARE);
		ImGui::Text("text %s", ICON_FA_CLOUDSCALE);
		ImGui::Text("text %s", ICON_FA_CLOUDSMITH);
		ImGui::Text("text %s", ICON_FA_CLOUDVERSIFY);
		ImGui::Text("text %s", ICON_FA_CMPLID);


		ImGui::Text("Icon: ICON_FA_42_GROUP %s",ICON_FA_42_GROUP);
		ImGui::Text("Icon: ICON_FA_500PX %s",ICON_FA_500PX);
		ImGui::Text("Icon: ICON_FA_ACCESSIBLE_ICON %s",ICON_FA_ACCESSIBLE_ICON);
		ImGui::Text("Icon: ICON_FA_ACCUSOFT %s",ICON_FA_ACCUSOFT);
		ImGui::Text("Icon: ICON_FA_ADN %s",ICON_FA_ADN);
		ImGui::Text("Icon: ICON_FA_ADVERSAL %s",ICON_FA_ADVERSAL);
		ImGui::Text("Icon: ICON_FA_AFFILIATETHEME %s",ICON_FA_AFFILIATETHEME);
		ImGui::Text("Icon: ICON_FA_AIRBNB %s",ICON_FA_AIRBNB);
		ImGui::Text("Icon: ICON_FA_ALGOLIA %s",ICON_FA_ALGOLIA);
		ImGui::Text("Icon: ICON_FA_ALIPAY %s",ICON_FA_ALIPAY);
		ImGui::Text("Icon: ICON_FA_AMAZON %s",ICON_FA_AMAZON);
		ImGui::Text("Icon: ICON_FA_AMAZON_PAY %s",ICON_FA_AMAZON_PAY);
		ImGui::Text("Icon: ICON_FA_AMILIA %s",ICON_FA_AMILIA);
		ImGui::Text("Icon: ICON_FA_ANDROID %s",ICON_FA_ANDROID);
		ImGui::Text("Icon: ICON_FA_ANGELLIST %s",ICON_FA_ANGELLIST);
		ImGui::Text("Icon: ICON_FA_ANGRYCREATIVE %s",ICON_FA_ANGRYCREATIVE);
		ImGui::Text("Icon: ICON_FA_ANGULAR %s",ICON_FA_ANGULAR);
		ImGui::Text("Icon: ICON_FA_APP_STORE %s",ICON_FA_APP_STORE);
		ImGui::Text("Icon: ICON_FA_APP_STORE_IOS %s",ICON_FA_APP_STORE_IOS);
		ImGui::Text("Icon: ICON_FA_APPER %s",ICON_FA_APPER);
		ImGui::Text("Icon: ICON_FA_APPLE %s",ICON_FA_APPLE);
		ImGui::Text("Icon: ICON_FA_APPLE_PAY %s",ICON_FA_APPLE_PAY);
		ImGui::Text("Icon: ICON_FA_ARTSTATION %s",ICON_FA_ARTSTATION);
		ImGui::Text("Icon: ICON_FA_ASYMMETRIK %s",ICON_FA_ASYMMETRIK);
		ImGui::Text("Icon: ICON_FA_ATLASSIAN %s",ICON_FA_ATLASSIAN);
		ImGui::Text("Icon: ICON_FA_AUDIBLE %s",ICON_FA_AUDIBLE);
		ImGui::Text("Icon: ICON_FA_AUTOPREFIXER %s",ICON_FA_AUTOPREFIXER);
		ImGui::Text("Icon: ICON_FA_AVIANEX %s",ICON_FA_AVIANEX);
		ImGui::Text("Icon: ICON_FA_AVIATO %s",ICON_FA_AVIATO);
		ImGui::Text("Icon: ICON_FA_AWS %s",ICON_FA_AWS);
		ImGui::Text("Icon: ICON_FA_BEHANCE %s",ICON_FA_BEHANCE);
		ImGui::Text("Icon: ICON_FA_BILIBILI %s",ICON_FA_BILIBILI);
		ImGui::Text("Icon: ICON_FA_BIMOBJECT %s",ICON_FA_BIMOBJECT);
		ImGui::Text("Icon: ICON_FA_BITBUCKET %s",ICON_FA_BITBUCKET);
		ImGui::Text("Icon: ICON_FA_BITCOIN %s",ICON_FA_BITCOIN);
		ImGui::Text("Icon: ICON_FA_BITY %s",ICON_FA_BITY);
		ImGui::Text("Icon: ICON_FA_BLACK_TIE %s",ICON_FA_BLACK_TIE);
		ImGui::Text("Icon: ICON_FA_BLACKBERRY %s",ICON_FA_BLACKBERRY);
		ImGui::Text("Icon: ICON_FA_BLOGGER %s",ICON_FA_BLOGGER);
		ImGui::Text("Icon: ICON_FA_BLOGGER_B %s",ICON_FA_BLOGGER_B);
		ImGui::Text("Icon: ICON_FA_BLUETOOTH %s",ICON_FA_BLUETOOTH);
		ImGui::Text("Icon: ICON_FA_BLUETOOTH_B %s",ICON_FA_BLUETOOTH_B);
		ImGui::Text("Icon: ICON_FA_BOOTSTRAP %s",ICON_FA_BOOTSTRAP);
		ImGui::Text("Icon: ICON_FA_BOTS %s",ICON_FA_BOTS);
		ImGui::Text("Icon: ICON_FA_BRAVE %s",ICON_FA_BRAVE);
		ImGui::Text("Icon: ICON_FA_BRAVE_REVERSE %s",ICON_FA_BRAVE_REVERSE);
		ImGui::Text("Icon: ICON_FA_BTC %s",ICON_FA_BTC);
		ImGui::Text("Icon: ICON_FA_BUFFER %s",ICON_FA_BUFFER);
		ImGui::Text("Icon: ICON_FA_BUROMOBELEXPERTE %s",ICON_FA_BUROMOBELEXPERTE);
		ImGui::Text("Icon: ICON_FA_BUY_N_LARGE %s",ICON_FA_BUY_N_LARGE);
		ImGui::Text("Icon: ICON_FA_BUYSELLADS %s",ICON_FA_BUYSELLADS);
		ImGui::Text("Icon: ICON_FA_CANADIAN_MAPLE_LEAF %s",ICON_FA_CANADIAN_MAPLE_LEAF);
		ImGui::Text("Icon: ICON_FA_CC_AMAZON_PAY %s",ICON_FA_CC_AMAZON_PAY);
		ImGui::Text("Icon: ICON_FA_CC_AMEX %s",ICON_FA_CC_AMEX);
		ImGui::Text("Icon: ICON_FA_CC_APPLE_PAY %s",ICON_FA_CC_APPLE_PAY);
		ImGui::Text("Icon: ICON_FA_CC_DINERS_CLUB %s",ICON_FA_CC_DINERS_CLUB);
		ImGui::Text("Icon: ICON_FA_CC_DISCOVER %s",ICON_FA_CC_DISCOVER);
		ImGui::Text("Icon: ICON_FA_CC_JCB %s",ICON_FA_CC_JCB);
		ImGui::Text("Icon: ICON_FA_CC_MASTERCARD %s",ICON_FA_CC_MASTERCARD);
		ImGui::Text("Icon: ICON_FA_CC_PAYPAL %s",ICON_FA_CC_PAYPAL);
		ImGui::Text("Icon: ICON_FA_CC_STRIPE %s",ICON_FA_CC_STRIPE);
		ImGui::Text("Icon: ICON_FA_CC_VISA %s",ICON_FA_CC_VISA);
		ImGui::Text("Icon: ICON_FA_CENTERCODE %s",ICON_FA_CENTERCODE);
		ImGui::Text("Icon: ICON_FA_CENTOS %s",ICON_FA_CENTOS);
		ImGui::Text("Icon: ICON_FA_CHROME %s",ICON_FA_CHROME);
		ImGui::Text("Icon: ICON_FA_CHROMECAST %s",ICON_FA_CHROMECAST);
		ImGui::Text("Icon: ICON_FA_CLOUDFLARE %s",ICON_FA_CLOUDFLARE);
		ImGui::Text("Icon: ICON_FA_CLOUDSCALE %s",ICON_FA_CLOUDSCALE);
		ImGui::Text("Icon: ICON_FA_CLOUDSMITH %s",ICON_FA_CLOUDSMITH);
		ImGui::Text("Icon: ICON_FA_CLOUDVERSIFY %s",ICON_FA_CLOUDVERSIFY);
		ImGui::Text("Icon: ICON_FA_CMPLID %s",ICON_FA_CMPLID);
		ImGui::Text("Icon: ICON_FA_CODEPEN %s",ICON_FA_CODEPEN);
		ImGui::Text("Icon: ICON_FA_CODIEPIE %s",ICON_FA_CODIEPIE);
		ImGui::Text("Icon: ICON_FA_CONFLUENCE %s",ICON_FA_CONFLUENCE);
		ImGui::Text("Icon: ICON_FA_CONNECTDEVELOP %s",ICON_FA_CONNECTDEVELOP);
		ImGui::Text("Icon: ICON_FA_CONTAO %s",ICON_FA_CONTAO);
		ImGui::Text("Icon: ICON_FA_COTTON_BUREAU %s",ICON_FA_COTTON_BUREAU);
		ImGui::Text("Icon: ICON_FA_CPANEL %s",ICON_FA_CPANEL);
		ImGui::Text("Icon: ICON_FA_CREATIVE_COMMONS %s",ICON_FA_CREATIVE_COMMONS);
		ImGui::Text("Icon: ICON_FA_CREATIVE_COMMONS_BY %s",ICON_FA_CREATIVE_COMMONS_BY);
		ImGui::Text("Icon: ICON_FA_CREATIVE_COMMONS_NC %s",ICON_FA_CREATIVE_COMMONS_NC);
		ImGui::Text("Icon: ICON_FA_CREATIVE_COMMONS_NC_EU %s",ICON_FA_CREATIVE_COMMONS_NC_EU);
		ImGui::Text("Icon: ICON_FA_CREATIVE_COMMONS_NC_JP %s",ICON_FA_CREATIVE_COMMONS_NC_JP);
		ImGui::Text("Icon: ICON_FA_CREATIVE_COMMONS_ND %s",ICON_FA_CREATIVE_COMMONS_ND);
		ImGui::Text("Icon: ICON_FA_CREATIVE_COMMONS_PD %s",ICON_FA_CREATIVE_COMMONS_PD);
		ImGui::Text("Icon: ICON_FA_CREATIVE_COMMONS_PD_ALT %s",ICON_FA_CREATIVE_COMMONS_PD_ALT);
		ImGui::Text("Icon: ICON_FA_CREATIVE_COMMONS_REMIX %s",ICON_FA_CREATIVE_COMMONS_REMIX);
		ImGui::Text("Icon: ICON_FA_CREATIVE_COMMONS_SA %s",ICON_FA_CREATIVE_COMMONS_SA);
		ImGui::Text("Icon: ICON_FA_CREATIVE_COMMONS_SAMPLING %s",ICON_FA_CREATIVE_COMMONS_SAMPLING);
		ImGui::Text("Icon: ICON_FA_CREATIVE_COMMONS_SAMPLING_PLUS %s",ICON_FA_CREATIVE_COMMONS_SAMPLING_PLUS);
		ImGui::Text("Icon: ICON_FA_CREATIVE_COMMONS_SHARE %s",ICON_FA_CREATIVE_COMMONS_SHARE);
		ImGui::Text("Icon: ICON_FA_CREATIVE_COMMONS_ZERO %s",ICON_FA_CREATIVE_COMMONS_ZERO);
		ImGui::Text("Icon: ICON_FA_CRITICAL_ROLE %s",ICON_FA_CRITICAL_ROLE);
		ImGui::Text("Icon: ICON_FA_CSS3 %s",ICON_FA_CSS3);
		ImGui::Text("Icon: ICON_FA_CSS3_ALT %s",ICON_FA_CSS3_ALT);
		ImGui::Text("Icon: ICON_FA_CUTTLEFISH %s",ICON_FA_CUTTLEFISH);
		ImGui::Text("Icon: ICON_FA_D_AND_D %s",ICON_FA_D_AND_D);
		ImGui::Text("Icon: ICON_FA_D_AND_D_BEYOND %s",ICON_FA_D_AND_D_BEYOND);
		ImGui::Text("Icon: ICON_FA_DAILYMOTION %s",ICON_FA_DAILYMOTION);
		ImGui::Text("Icon: ICON_FA_DASHCUBE %s",ICON_FA_DASHCUBE);
		ImGui::Text("Icon: ICON_FA_DEBIAN %s",ICON_FA_DEBIAN);
		ImGui::Text("Icon: ICON_FA_DEEZER %s",ICON_FA_DEEZER);
		ImGui::Text("Icon: ICON_FA_DELICIOUS %s",ICON_FA_DELICIOUS);
		ImGui::Text("Icon: ICON_FA_DEPLOYDOG %s",ICON_FA_DEPLOYDOG);
		ImGui::Text("Icon: ICON_FA_DESKPRO %s",ICON_FA_DESKPRO);
		ImGui::Text("Icon: ICON_FA_DEV %s",ICON_FA_DEV);
		ImGui::Text("Icon: ICON_FA_DEVIANTART %s",ICON_FA_DEVIANTART);
		ImGui::Text("Icon: ICON_FA_DHL %s",ICON_FA_DHL);
		ImGui::Text("Icon: ICON_FA_DIASPORA %s",ICON_FA_DIASPORA);
		ImGui::Text("Icon: ICON_FA_DIGG %s",ICON_FA_DIGG);
		ImGui::Text("Icon: ICON_FA_DIGITAL_OCEAN %s",ICON_FA_DIGITAL_OCEAN);
		ImGui::Text("Icon: ICON_FA_DISCORD %s",ICON_FA_DISCORD);
		ImGui::Text("Icon: ICON_FA_DISCOURSE %s",ICON_FA_DISCOURSE);
		ImGui::Text("Icon: ICON_FA_DOCHUB %s",ICON_FA_DOCHUB);
		ImGui::Text("Icon: ICON_FA_DOCKER %s",ICON_FA_DOCKER);
		ImGui::Text("Icon: ICON_FA_DRAFT2DIGITAL %s",ICON_FA_DRAFT2DIGITAL);
		ImGui::Text("Icon: ICON_FA_DRIBBBLE %s",ICON_FA_DRIBBBLE);
		ImGui::Text("Icon: ICON_FA_DROPBOX %s",ICON_FA_DROPBOX);
		ImGui::Text("Icon: ICON_FA_DRUPAL %s",ICON_FA_DRUPAL);
		ImGui::Text("Icon: ICON_FA_DYALOG %s",ICON_FA_DYALOG);
		ImGui::Text("Icon: ICON_FA_EARLYBIRDS %s",ICON_FA_EARLYBIRDS);
		ImGui::Text("Icon: ICON_FA_EBAY %s",ICON_FA_EBAY);
		ImGui::Text("Icon: ICON_FA_EDGE %s",ICON_FA_EDGE);
		ImGui::Text("Icon: ICON_FA_EDGE_LEGACY %s",ICON_FA_EDGE_LEGACY);
		ImGui::Text("Icon: ICON_FA_ELEMENTOR %s",ICON_FA_ELEMENTOR);
		ImGui::Text("Icon: ICON_FA_ELLO %s",ICON_FA_ELLO);
		ImGui::Text("Icon: ICON_FA_EMBER %s",ICON_FA_EMBER);
		ImGui::Text("Icon: ICON_FA_EMPIRE %s",ICON_FA_EMPIRE);
		ImGui::Text("Icon: ICON_FA_ENVIRA %s",ICON_FA_ENVIRA);
		ImGui::Text("Icon: ICON_FA_ERLANG %s",ICON_FA_ERLANG);
		ImGui::Text("Icon: ICON_FA_ETHEREUM %s",ICON_FA_ETHEREUM);
		ImGui::Text("Icon: ICON_FA_ETSY %s",ICON_FA_ETSY);
		ImGui::Text("Icon: ICON_FA_EVERNOTE %s",ICON_FA_EVERNOTE);
		ImGui::Text("Icon: ICON_FA_EXPEDITEDSSL %s",ICON_FA_EXPEDITEDSSL);
		ImGui::Text("Icon: ICON_FA_FACEBOOK %s",ICON_FA_FACEBOOK);
		ImGui::Text("Icon: ICON_FA_FACEBOOK_F %s",ICON_FA_FACEBOOK_F);
		ImGui::Text("Icon: ICON_FA_FACEBOOK_MESSENGER %s",ICON_FA_FACEBOOK_MESSENGER);
		ImGui::Text("Icon: ICON_FA_FANTASY_FLIGHT_GAMES %s",ICON_FA_FANTASY_FLIGHT_GAMES);
		ImGui::Text("Icon: ICON_FA_FEDEX %s",ICON_FA_FEDEX);
		ImGui::Text("Icon: ICON_FA_FEDORA %s",ICON_FA_FEDORA);
		ImGui::Text("Icon: ICON_FA_FIGMA %s",ICON_FA_FIGMA);
		ImGui::Text("Icon: ICON_FA_FIREFOX %s",ICON_FA_FIREFOX);
		ImGui::Text("Icon: ICON_FA_FIREFOX_BROWSER %s",ICON_FA_FIREFOX_BROWSER);
		ImGui::Text("Icon: ICON_FA_FIRST_ORDER %s",ICON_FA_FIRST_ORDER);
		ImGui::Text("Icon: ICON_FA_FIRST_ORDER_ALT %s",ICON_FA_FIRST_ORDER_ALT);
		ImGui::Text("Icon: ICON_FA_FIRSTDRAFT %s",ICON_FA_FIRSTDRAFT);
		ImGui::Text("Icon: ICON_FA_FLICKR %s",ICON_FA_FLICKR);
		ImGui::Text("Icon: ICON_FA_FLIPBOARD %s",ICON_FA_FLIPBOARD);
		ImGui::Text("Icon: ICON_FA_FLY %s",ICON_FA_FLY);
		ImGui::Text("Icon: ICON_FA_FONT_AWESOME %s",ICON_FA_FONT_AWESOME);
		ImGui::Text("Icon: ICON_FA_FONTICONS %s",ICON_FA_FONTICONS);
		ImGui::Text("Icon: ICON_FA_FONTICONS_FI %s",ICON_FA_FONTICONS_FI);
		ImGui::Text("Icon: ICON_FA_FORT_AWESOME %s",ICON_FA_FORT_AWESOME);
		ImGui::Text("Icon: ICON_FA_FORT_AWESOME_ALT %s",ICON_FA_FORT_AWESOME_ALT);
		ImGui::Text("Icon: ICON_FA_FORUMBEE %s",ICON_FA_FORUMBEE);
		ImGui::Text("Icon: ICON_FA_FOURSQUARE %s",ICON_FA_FOURSQUARE);
		ImGui::Text("Icon: ICON_FA_FREE_CODE_CAMP %s",ICON_FA_FREE_CODE_CAMP);
		ImGui::Text("Icon: ICON_FA_FREEBSD %s",ICON_FA_FREEBSD);
		ImGui::Text("Icon: ICON_FA_BANDCAMP %s",ICON_FA_BANDCAMP);
		ImGui::Text("Icon: ICON_FA_BATTLE_NET %s",ICON_FA_BATTLE_NET);
		ImGui::End();

		getCurrentContext()->fontLoader->renderDebugWindow();
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