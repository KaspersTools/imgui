//
// Created by Kasper de Bruin on 22/02/2024.
//

#ifndef IMGUI_HBNEWLINE_H
#define IMGUI_HBNEWLINE_H
namespace HBUI {
	class HBNewLine : public IWidget<float> {

private:
		virtual void render() override {
			IWidgetBase::render();
		};

		virtual float calculateTotalWidth() override{

		};

		virtual float calculateTotalHeight() override{

		};

		virtual void addWidth(float toAdd) override{

		};

		virtual void addHeight(float toAdd) override{

		};

public:
		HBNewLine(float size, HBWidgetResizeType_ resizeType) : IWidget(
		                                                            ImGui::GetID("HBNewLine"),//const std::string &id,
		                                                            HBDrawFlags_NonDrawable,  //const HBDrawLocation drawLocationFlag,
		                                                            false,                    //bool withBackground,
		                                                            HBUIType_NewLine,         //HBUIType uiType,
		                                                            "",                       //const std::string &label,
		                                                            false,                    //bool isVisible,
		                                                            {},                       //const ImVec2 &position,
		                                                            size,                     //const SizeType &size,
		                                                            resizeType,               //HBWidgetResizeType_ resizeTypeXAxis,
		                                                            resizeType,               //HBWidgetResizeType_ resizeTypeYAxis,
		                                                            HBLayoutType_None         //HBLayoutType layoutType
		                                                        ) {
		}
	};
}// namespace HBUI
#endif//IMGUI_HBNEWLINE_H
