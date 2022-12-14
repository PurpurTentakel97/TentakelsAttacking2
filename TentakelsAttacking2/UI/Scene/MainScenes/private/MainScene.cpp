//
// Purpur Tentakel
// 21.11.2022
//

#include "MainScene.h"
#include "GalaxyAndSlider.h"
#include "AppContext.h"
#include "ClassicButton.h"
#include "GenerelEvents.hpp"
#include "HPlayerCollection.h"
#include "Title.h"
#include "Text.h"


void MainScene::Initialize() {

	AppContext& appContext = AppContext::GetInstance();

	// Title
	auto title = std::make_shared<Title>(
		GetElementPosition(0.31f, 0.0f),
		GetElementSize(0.55f, 0.2f),
		Alignment::TOP_MID,
		m_resolution,
		false,
		AppContext::GetInstance()
		);
	m_elements.push_back(title);

	// Btn
	auto settingsBtn = std::make_shared<ClassicButton>(
		5,
		GetElementPosition(0.95f, 0.02f),
		GetElementSize(0.05f, 0.05f),
		Alignment::TOP_RIGHT,
		m_resolution,
		"settings",
		SoundType::CLICKED_RELEASE_STD
		);
	m_elements.push_back(settingsBtn);


	auto galaxyBtn = std::make_shared<ClassicButton>(
		1,
		GetElementPosition(0.7f, 0.02f),
		GetElementSize(0.1f, 0.05f),
		Alignment::TOP_RIGHT,
		m_resolution,
		"galaxy",
		SoundType::CLICKED_RELEASE_STD
		);
	galaxyBtn->SetOnClick([this]() {
		this->Switch(MainSceneType::GALAXY);
		});
	m_elements.push_back(galaxyBtn);

	auto planetTableBtn = std::make_shared<ClassicButton>(
		2,
		GetElementPosition(0.8f, 0.02f),
		GetElementSize(0.1f, 0.05f),
		Alignment::TOP_RIGHT,
		m_resolution,
		"planet table",
		SoundType::CLICKED_RELEASE_STD
		);
	planetTableBtn->SetOnClick([this]() {
		this->Switch(MainSceneType::PLANET_TABLE);
		});
	m_elements.push_back(planetTableBtn);

	auto fleetTableBtn = std::make_shared<ClassicButton>(
		3,
		GetElementPosition(0.9f, 0.02f),
		GetElementSize(0.1f, 0.05f),
		Alignment::TOP_RIGHT,
		m_resolution,
		"fleet table",
		SoundType::CLICKED_RELEASE_STD
		);
	fleetTableBtn->SetOnClick([this]() {
		this->Switch(MainSceneType::FLEET_TABLE);
		});
	m_elements.push_back(fleetTableBtn);

	m_nextBtn = std::make_shared<ClassicButton>(
		4,
		GetElementPosition(0.99f, 0.95f),
		GetElementSize(0.1f, 0.05f),
		Alignment::BOTTOM_RIGHT,
		m_resolution,
		"next Player",
		SoundType::ACCEPTED
		);
	m_nextBtn->SetOnClick([]() {
		AppContext::GetInstance().eventManager.InvokeEvent(TriggerNextTermEvent());
		});
	m_elements.push_back(m_nextBtn);

	// text
	auto currentPlayerLabel = std::make_shared<Text>(
		GetElementPosition(0.7f, 0.08f),
		GetElementSize(0.1f, 0.02f),
		Alignment::TOP_RIGHT,
		m_resolution,
		Alignment::TOP_RIGHT,
		0.02f,
		"current player:"
		);
	// currentPlayerLabel->RenderRectangle(true);
	m_elements.push_back(currentPlayerLabel);

	m_currentPlayerName = std::make_shared<Text>(
		GetElementPosition(0.71f, 0.08f),
		GetElementSize(0.2f, 0.02f),
		Alignment::TOP_LEFT,
		m_resolution,
		Alignment::TOP_LEFT,
		0.02f,
		"" 
		);
	// m_currentPlayerName->RenderRectangle(true);
	m_elements.push_back(m_currentPlayerName);


	auto currentRoundLabel = std::make_shared<Text>(
		GetElementPosition(0.7f, 0.1f),
		GetElementSize(0.1f, 0.02f),
		Alignment::TOP_RIGHT,
		m_resolution,
		Alignment::TOP_RIGHT,
		0.02f,
		"current round:"
		);
	// currentRoundLabel->RenderRectangle(true);
	m_elements.push_back(currentRoundLabel);

	m_currentRound = std::make_shared<Text>(
		GetElementPosition(0.71f, 0.1f),
		GetElementSize(0.2f, 0.02f),
		Alignment::TOP_LEFT,
		m_resolution,
		Alignment::TOP_LEFT,
		0.02f,
		std::to_string(appContext.constants.global.currentRound)
		);
	// m_currentRound->RenderRectangle(true);
	m_elements.push_back(m_currentRound);


	auto currentTargetRoundLabel = std::make_shared<Text>(
		GetElementPosition(0.7f, 0.12f),
		GetElementSize(0.1f, 0.02f),
		Alignment::TOP_RIGHT,
		m_resolution,
		Alignment::TOP_RIGHT,
		0.02f,
		"target round:"
		);
	// currentTargetRoundLabel->RenderRectangle(true);
	m_elements.push_back(currentTargetRoundLabel);

	m_currentTargetRound = std::make_shared<Text>(
		GetElementPosition(0.71f, 0.12f),
		GetElementSize(0.2f, 0.02f),
		Alignment::TOP_LEFT,
		m_resolution,
		Alignment::TOP_LEFT,
		0.02f,
		std::to_string(appContext.constants.global.currentTargetRound)
		);
	// m_currentTargetRound->RenderRectangle(true);
	m_elements.push_back(m_currentTargetRound);


	auto nextPlayerNameLabel = std::make_shared<Text>(
		GetElementPosition(0.9f, 0.98f),
		GetElementSize(0.1f, 0.02f),
		Alignment::BOTTOM_RIGHT,
		m_resolution,
		Alignment::BOTTOM_RIGHT,
		0.02f,
		"next player:"
		);
	// nextPlayerNameLabel->RenderRectangle(true);
	m_elements.push_back(nextPlayerNameLabel);

	m_nextPlayerName = std::make_shared<Text>(
		GetElementPosition(0.91f, 0.98f),
		GetElementSize(0.25f, 0.02f),
		Alignment::BOTTOM_LEFT,
		m_resolution,
		Alignment::BOTTOM_LEFT,
		0.02f,
		""
		);
	// m_nextPlayerName->RenderRectangle(true);
	m_elements.push_back(m_nextPlayerName);
}
void MainScene::InitialzeGalaxy() {
	AppContext& appContext = AppContext::GetInstance();
	if (m_galaxy) {
		m_galaxy->SetActive(false, appContext);
		m_elements.erase(std::remove(m_elements.begin(), m_elements.end(), m_galaxy), m_elements.end());
		m_galaxy = nullptr;
	}

	m_galaxy = std::make_shared<GalaxyScene>(
		GetElementPosition(0.01f, 0.99f),
		GetElementSize(0.85f, 0.85f),
		Alignment::BOTTOM_LEFT,
		m_resolution
		);
	m_elements.push_back(m_galaxy);
}

void MainScene::NextTerm() {
	AppContext const& appContext = AppContext::GetInstance();
	Switch(MainSceneType::CLEAR);
	SetPlayerText();
	InitialzeGalaxy();

	auto event = ShowMessagePopUpEvent(
		"start term?",
		"next plyer: " + m_currentPlayer.name + "\naccept to start your trun",
		[this]() {
			this->Switch(MainSceneType::GALAXY);
		}
	);
	appContext.eventManager.InvokeEvent(event);
}
void MainScene::NextRound() {
	AppContext& appContext = AppContext::GetInstance();

	SetPlayerText();
	InitialzeGalaxy();

	m_currentRound->SetText(std::to_string(appContext.constants.global.currentRound));
	m_currentTargetRound->SetText(std::to_string(appContext.constants.global.currentTargetRound));

	auto event = ShowMessagePopUpEvent(
		"start round",
		"next round is starting \n just for debug to know"
	);
	appContext.eventManager.InvokeEvent(event);

	Switch(MainSceneType::GALAXY);
}

void MainScene::SetPlayerText() {

	m_currentPlayerName->SetText(m_currentPlayer.name);
	m_currentPlayerName->SetColor(m_currentPlayer.color);

	m_nextPlayerName->SetText(m_nextPlayer.name);
	m_nextPlayerName->SetColor(m_nextPlayer.color);
}

void MainScene::Switch(MainSceneType sceneType) {
	AppContext& appContext = AppContext::GetInstance();

	m_galaxy->SetActive(sceneType == MainSceneType::GALAXY, appContext);
	// m_planetTable->SetActive(sceneType == MainSceneType::PLANET_TABLE, appContext);
	// m_fleetTable->SetActive(sceneType == MainSceneType::FLEET_TABLE, appContext);
}

MainScene::MainScene(Vector2 resolution)
	: Scene({ 0.0f,0.0f }, { 1.0f,1.0f }, Alignment::DEFAULT, resolution) {

	AppContext& appContext = AppContext::GetInstance();
	appContext.eventManager.AddListener(this);
	appContext.eventManager.InvokeEvent(StartGameEvent());

	Initialize();
	InitialzeGalaxy();
	SetPlayerText();
	Switch(MainSceneType::GALAXY);
}
MainScene::~MainScene() {
	AppContext::GetInstance().eventManager.RemoveListener(this);
}

void MainScene::OnEvent(Event const& event) {
	AppContext const& appContext = AppContext::GetInstance();
	
	// player
	if (auto const* playerEvent = dynamic_cast<UpdateCurrentPlayerIDEvent const*>(&event)) {
		m_currentPlayer = appContext.playerCollection.GetPlayerByID(playerEvent->GetID());
		return;
	}
	if (auto const* playerEvent = dynamic_cast<UpdateNextPlayerIDEvent const*>(&event)) {
		m_nextPlayer = appContext.playerCollection.GetPlayerByID(playerEvent->GetID());
		return;
	}

	// terms and rounds
	if (auto const* playerEvent = dynamic_cast<ShowNextTermEvent const*>(&event)) {
		NextTerm();
		return;
	}
	if (auto const* playerEvent = dynamic_cast<ShowNextRoundEvent const*>(&event)) {
		NextRound();
		return;
	}
}
