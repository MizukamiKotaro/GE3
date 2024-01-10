#pragma once

#include "Externals/imgui/imgui.h"
#include "Externals/imgui/imgui_impl_dx12.h"
#include "Externals/imgui/imgui_impl_win32.h"

class ImGuiManager
{
public:

	static void Initialize();

	static void Finalize();

	static void Begin();

	static void End();

	static void Draw();

};

