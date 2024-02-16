//
// Created by Kasper de Bruin on 23/01/2024.
//

#pragma once

#include <HBUI/HBUI.h>
#include <HBUI/UIWindow.h>

namespace HummingBirdCore::UI {
		class WindowManager {
		public:
				WindowManager() = default;

				~WindowManager() = default;

				void addWindow(const std::string &name, const int addition, std::shared_ptr<UIWindow> uiWindow);

				void render();

				static WindowManager *getInstance();

				static void setInstance(WindowManager *instance);

		private:
				// UIWindows
				std::map<std::string, std::shared_ptr<UIWindow>> m_windows;
		};
}// namespace HummingBirdCore::UI