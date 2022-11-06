//
// PurpurTentakel
// 08.08.22
//

#pragma once
#include "Vec2.hpp"
#include <string>
#include <memory> // for std::weak_ptr

class Galaxy;
class Player;

class SpaceObject {
protected:
	int m_id;
	int m_ships;
	Vec2<double> m_position;

public:
	std::weak_ptr<Player> player;
	SpaceObject(int id,double x, double y,int ships, std::weak_ptr<Player> player);

	virtual void PreUpdate(Galaxy const& galaxy) = 0; // bevor main update -> z.B. move
	virtual void Update(Galaxy const& galaxy) = 0; // main update
	virtual void PostUpdate(Galaxy const& galaxy) = 0; // after main update
	[[nodiscard]] virtual std::string ToString() const = 0; // debug Print

	[[nodiscard]] Vec2<double> GetPosition() const;
	[[nodiscard]] int GetId() const;

	friend void operator+=(SpaceObject& lhs, int ships);
	friend void operator-=(SpaceObject& lhs, int ships);
	friend bool operator==(SpaceObject const& lhs, SpaceObject const& rhs);
};

