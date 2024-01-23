#include "Kyoko.h"
#include <Windows.h>
#include <memory>
#include "SceneSystem/SceneManager/SceneManager.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {

	Kyoko::Init();

	std::unique_ptr<SceneManager> sceneManager = std::make_unique<SceneManager>();
	sceneManager->Run();

	sceneManager.reset();

	Kyoko::Finalize();

	return 0;
}