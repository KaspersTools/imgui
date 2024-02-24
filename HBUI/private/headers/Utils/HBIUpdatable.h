//
// Created by Kasper de Bruin on 22/02/2024.
//

#ifndef IMGUI_HBIUPDATABLE_H
#define IMGUI_HBIUPDATABLE_H

class HBIUpdateable {
  virtual void startFrame() = 0;

  virtual void endFrame() = 0;
};


#endif//IMGUI_HBIUPDATABLE_H
