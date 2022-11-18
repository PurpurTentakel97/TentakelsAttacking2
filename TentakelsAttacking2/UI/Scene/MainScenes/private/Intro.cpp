//
// Purpur Tentakel
// 12.09.2022
//

#include "Intro.h"
#include "AppContext.h"
#include "UIManager.h"
#include "ClassicButton.h"
#include "Text.h"
#include "Title.h"
#include "SceneType.h"
#include "HRandom.h"
#include "HInput.h"


#define BTN_SPEED 0.2f

void Intro::Initialize(AppContext& appContext, Vector2 resolution) {
	m_title = std::make_shared<Title>(
		GetElementPosition(0.5f, 0.1f),
		GetElementSize(0.9f, 0.3f),
		Alignment::TOP_MID,
		resolution,
		true,
		appContext
		);
	m_elements.push_back(m_title);

	m_btn = std::make_shared<ClassicButton>(
		1,
		GetElementPosition(0.5f, 1.1f),
		GetElementSize(0.3f, 0.2f),
		Alignment::MID_MID,
		resolution,
		"Start Game",
		SoundType::ACCEPTED
		);
	m_btn->SetEnabled(false);
	m_elements.push_back(m_btn);

	auto skipText = std::make_shared<Text>(
		GetElementPosition(0.99f, 0.98f),
		GetElementSize(0.11f, 0.03f),
		Alignment::BOTTOM_RIGHT,
		resolution,
		Alignment::BOTTOM_RIGHT,
		0.03f,
		"skip with [ESC]"
		);
	m_elements.push_back(skipText);


	std::function<void()> gameStart = [this]() {
		this->StartGame();
	};
	m_btn->SetOnClick(gameStart);
}

void Intro::MoveBtn() {
	auto btnPos = m_btn->GetPosition();
	if (btnPos.y - (BTN_SPEED * GetFrameTime()) < m_maxBtnPosition) {
		BtnMoveFinish();
		return;
	}
	btnPos.y -= BTN_SPEED;
	m_btn->Move(btnPos);
}
void Intro::BtnMoveFinish(){
	auto btnPos = m_btn->GetPosition();
	m_btn->SetPosition({ btnPos.x, m_maxBtnPosition});
	m_btnMovmendFinish = true;

	AppContext& appContext = AppContext::GetInstance();
	auto event = PlaySoundEvent(SoundType::ACCEPTED);
	appContext.eventManager.InvokeEvent(event);
}

void Intro::StartGame() {
	auto event = SwitchSceneEvent(m_nextScene);
	AppContext::GetInstance().eventManager.InvokeEvent(event);
}

Intro::Intro(Vector2 resolution)
	:Scene(Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f), Alignment::DEFAULT, resolution) {

	AppContext& appContext = AppContext::GetInstance();
	Initialize(appContext, resolution);
}

void Intro::CheckAndUpdate(Vector2 const& mousePosition, AppContext const& appContext) {
	m_title->CheckAndUpdate(mousePosition, appContext);

	if (m_title->IsTitleFinished()) {
		m_btn->SetEnabled(true);
		return;
	}

	bool skipBtn =
		IsBackInputPressed()
		and m_title->HasFinishedTitle()
		and !m_btnMovmendFinish;
	if (skipBtn) {
		BtnMoveFinish();
		return;
	}

	if (m_title->HasFinishedTitle() and !m_btnMovmendFinish) {
		MoveBtn();
	}
	m_btn->CheckAndUpdate(mousePosition, appContext);
}
void Intro::Render(AppContext const& appContext) {
	for (auto& e : m_elements) {
		e->Render(appContext);
	}
}
void Intro::Resize(Vector2 resolution, AppContext const& appContext) {
	for (auto& e : m_elements) {
		e->Resize(resolution, appContext);
	}
}

void Intro::SetActive(bool active, AppContext const& appContext) {
	Scene::SetActive(active, appContext);
	auto event = SelectFocusElementEvent(m_btn.get());
	appContext.eventManager.InvokeEvent(event);
}
