//
// Created by Kasper de Bruin on 06/02/2024.
//

#ifndef HUMMINGBIRD_HBUI_H
#define HUMMINGBIRD_HBUI_H
#include <iostream>
#include <chrono>
#include <map>
#include <imgui.h>

namespace HBUI {
	[[maybe_unused]]void initialize();

    [[maybe_unused]]ImVec2 getMainScreenSize();
    [[maybe_unused]]ImVec2 getMainScreenPos();
}// namespace HBUI
#endif//HUMMINGBIRD_HBUI_H
