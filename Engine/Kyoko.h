#pragma once

class Kyoko {
public:

	static void Init();

	static const bool ProcessMessage();

	static void FirstUpdateInLoop();

	static void PreDraw();

	static void PostDraw();

	static void Finalize();
};