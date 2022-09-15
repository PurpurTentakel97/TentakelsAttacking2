//
// Purpur Tentakel
// 24.08.2022
//

#include "UIManager.h"
#include "TestScene.h"

Vector2 UIManager::GetResolution() const {
	Vector2 newResolution;
	int display = GetCurrentMonitor();
	newResolution.x = static_cast<float>(GetMonitorWidth(display));
	newResolution.y = static_cast<float>(GetMonitorHeight(display));
	return newResolution;
}
Focus& UIManager::GetFocus() {
	return m_focus;
}
void UIManager::ToggleFullScreen() {
	ToggleFullscreen();
}
void UIManager::CheckAndUpdateResolution() {
	Vector2 newResolution = GetResolution();

	bool isNewResolution = (m_resolution.x != newResolution.x)
		or (m_resolution.y != newResolution.y);

	if (isNewResolution) {
		m_resolution = newResolution;
		SetWindowSize(static_cast<int>(m_resolution.x), static_cast<int>(m_resolution.y));
		m_sceneManager.Resize(newResolution, m_appContext);
	}
}

void UIManager::CheckAndUpdate() {
	bool quit = 
		IsKeyDown(KeyboardKey::KEY_LEFT_ALT)
		and IsKeyPressed(KeyboardKey::KEY_F4);
	if (quit) {
		CloseWindow();
	}

	if (IsKeyPressed(KEY_F11)) {
		ToggleFullScreen();
	}

	Vector2 mousePosition = GetMousePosition();
	m_focus.CheckAndUpdate();
	m_sceneManager.CheckAndUpdate(mousePosition, m_appContext);
}
void UIManager::Render() {
	BeginDrawing();
	ClearBackground(BLACK);
	m_sceneManager.Render(m_appContext);
	m_focus.Render();
	EndDrawing();
}

void UIManager::UILoop() {
	while (!WindowShouldClose()) {
		CheckAndUpdateResolution();
		CheckAndUpdate();
		Render();
	}
	CloseWindow();

}

UIManager::UIManager()
	: m_appContext(AppContext::GetInstance()), m_resolution({ 0.0f,0.0f }), m_sceneManager(this) {
	SetTargetFPS(60);
	SetExitKey(KeyboardKey::KEY_NULL);

	m_resolution = GetResolution();
	SetWindowSize(static_cast<int>(m_resolution.x), static_cast<int>(m_resolution.y));
}

void UIManager::StartUI() {
	m_sceneManager.SwitchScene(SceneType::LOGO, m_appContext);
	UILoop();
}

