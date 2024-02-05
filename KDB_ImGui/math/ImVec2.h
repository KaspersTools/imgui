//
// Created by Kasper de Bruin on 05/02/2024.
//

#pragma once

//define IM_VEC2_CLASS_EXTRA
//create extension to ImVec2
//::HalfSize() -> ImVec2(x/2, y/2)
#define IM_VEC2_CLASS_EXTRA                                \
  ImVec2 HalfSize() const { return ImVec2(x / 2, y / 2); } \
  ImVec2 DoubleSize() const { return ImVec2(x * 2, y * 2); }
