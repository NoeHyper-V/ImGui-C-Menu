
#ifndef IMGUI_SETTINGS_H
#define IMGUI_SETTINGS_H
#include "imgui.h"

namespace c {

	inline ImVec4 accent = ImVec4(1, 204, 205, 255);



	namespace bg
	{

		inline ImVec4 background = ImColor(0, 0, 0, 255);
		inline ImVec2 size = ImVec2(960, 610);
		inline float rounding = 9.f;

	}

	namespace child
	{

		inline ImVec4 background = ImColor(17, 17, 19, 145);
		inline ImVec4 outline = ImColor(0.0f, 208.0f, 208.0f, 0.5f);
		inline float rounding = 4.0f;

	}

	namespace checkbox
	{
		inline ImVec4 background = ImColor(34, 34, 37, 255);
		inline ImVec4 circle_inactive = ImColor(46, 48, 55, 255);
		inline float rounding = 5.f;
	}

	namespace slider
	{
		inline ImVec4 background = ImColor(34, 34, 37, 255);
		inline ImVec4 circle_inactive = ImColor(46, 48, 55, 255);
		inline float rounding = 2.f;
	}

	namespace tabs
	{
		inline ImVec4 background = ImVec4(47.0f / 255.0f, 47.0f / 255.0f, 47.0f / 255.0f, 0.84f);


		inline ImVec4 outline = ImColor(22, 24, 26, 255);
		inline float rounding = 6.f;
	}

	namespace combo
	{
		inline ImVec4 background = ImVec4(47.0f / 255.0f, 47.0f / 255.0f, 47.0f / 255.0f, 1.0f);
		inline ImVec4 outline = ImColor(22, 24, 26, 255);
		inline ImVec4 icon = ImColor(255, 227, 122, 255);

		inline float rounding = 2.f;
	}

	namespace picker
	{
		inline ImVec4 background = ImColor(16, 17, 19, 255);
		inline ImVec4 outline = ImColor(22, 24, 26, 255);

		inline float rounding = 2.f;
	}

	namespace button
	{
		inline ImVec4 background_active = ImColor(26, 27, 29, 255);
		inline ImVec4 background_inactive = ImColor(47.0f / 255.0f, 47.0f / 255.0f, 47.0f / 255.0f, 0.84f);

		inline ImVec4 outline = ImColor(22, 24, 26, 255);

		inline float rounding = 5.f;
	}

	namespace keybind
	{
		inline ImVec4 background_active = ImColor(26, 27, 29, 255);
		inline ImVec4 background_inactive = ImVec4(47.0f / 255.0f, 47.0f / 255.0f, 47.0f / 255.0f, 0.84f);

		inline ImVec4 outline = ImColor(22, 24, 26, 255);

		inline float rounding = 2.f;
	}

	namespace input
	{
		inline ImVec4 background_active = ImColor(26, 27, 29, 255);
		inline ImVec4 background_inactive = ImColor(16, 17, 19, 255);

		inline ImVec4 outline = ImColor(22, 24, 26, 255);
		inline ImVec4 icon = ImColor(26, 27, 31, 255);

		inline float rounding = 2.f;
	}

	namespace scroll
	{
		inline float rounding = 30.f;
	}

	namespace text
	{
		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline ImVec4 text_hov = ImColor(86, 88, 95, 255);
		inline ImVec4 text = ImColor(101, 102, 105, 255);

		inline float glow_size = 8.f;
	}
}
#endif // IMGUI_SETTINGS_H
