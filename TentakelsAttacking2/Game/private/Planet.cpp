//
// Purpur Tentakel
// 07.11.2022
//

#include "Planet.h"
#include "HRandom.h"
#include "AppContext.h"
#include "Player.h"

Planet::Planet(unsigned int ID, vec2pos position, std::shared_ptr<Player> player,
	bool isHomePlanet, int planetNumber)
	: SpaceObject(ID, position, player), m_isHomePlanet(isHomePlanet),
	m_planetNumber(planetNumber) {

	AppContext& appContext = AppContext::GetInstance();
	m_maxShips = appContext.constants.planet.maxShips;

	if (m_isHomePlanet) {
		m_production = appContext.constants.planet.homeworldProduction;
		m_ships = m_production * appContext.constants.planet.statringHumanShipsMultiplicator;
	}
	else {
		Random& random = Random::GetInstance();
		size_t r = static_cast<size_t>(random.random(
			appContext.constants.planet.maxProduction -
			appContext.constants.planet.minProduction
		));
		m_production = r + appContext.constants.planet.minProduction;
		m_ships = m_production * appContext.constants.planet.statringGlobalShipsMultiplicator;
	}
}

bool Planet::IsHomePlanet() const {
	return m_isHomePlanet;
}

void Planet::SetIsDestroyed(bool isDestroyed) {
	m_isDestroyed = isDestroyed;
}
bool Planet::IsDestroyed() const {
	return m_isDestroyed;
}
