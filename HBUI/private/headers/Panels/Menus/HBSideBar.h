//
// Created by Kasper de Bruin on 21/02/2024.
//

#ifndef IMGUI_HBSIDEBAR_H
#define IMGUI_HBSIDEBAR_H

namespace HBUI {
	class HBSideBar : public RectWidget {
public:
		HBSideBar()
		    : RectWidget() {

			if (m_flags & HBSideBarFlags_Horizontal) {
				HBUI::setLayout(HBLayoutType_Horizontal);
			} else {
				HBUI::setLayout(HBLayoutType_Vertical);
			}
		}

		~HBSideBar() {
			HBUI::setLayout(HBLayoutType_Vertical);
		}

		void render() override {
			RectWidget::render();
		}


private:
		HBLayoutType_ m_BackupLayoutType = HBLayoutType_Vertical;
	};
}// namespace HBUI


#endif//IMGUI_HBSIDEBAR_H
