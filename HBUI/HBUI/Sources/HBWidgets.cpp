//
// Created by Kasper de Bruin on 10/02/2024.
//

// clang-format off
#include <HBUI/HBUI.h>
// clang-format on

static bool
    aabb(const ImVec2 &min1, const ImVec2 &max1, const ImVec2 &min2, const ImVec2 &max2) {
	return (min1.x < max2.x && max1.x > min2.x) && (min1.y < max2.y && max1.y > min2.y);
}

namespace HBUI {
	//-------------------------------------------------------------------------
	// [SECTION] Helpers
	//-------------------------------------------------------------------------
	HBUIBaseType_ typeToBaseType(HBUIType_ type){
		switch (type) {
			case HBUIType_::HBUIType_DockPanel:
			case HBUIType_::HBUIType_SideBar:
				return HBUIBaseType_Window;

		}
	}

}// namespace HBUI