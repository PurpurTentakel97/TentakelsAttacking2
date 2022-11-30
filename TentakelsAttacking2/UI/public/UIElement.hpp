//
// Purpur Tentakel
// 24.08.2022
//

#pragma once
#include "HSoundType.hpp"
#include "HAssetType.hpp"
#include "UIEvents.hpp"
#include "Allignment.h"
#include <raylib.h>
#include <cmath>
#include <numbers>

struct AppContext;

/**
 * provides the basic UIElement.
 * all elements in the ui should implement this.
 */
class UIElement {
protected:
	enum class MoveType { ///< contains the posible movment types of the element
		POINT_LINEAR,
		SPEED_LINEAR,
		POINT_ASYNPTOTIC,
		NONE,
	};
	MoveType m_moveType = MoveType::NONE; ///< contains the current movment type.
	Vector2 m_pos, m_size; ///< contains the realtive position (top left) and size on the screen
	Vector2 m_resolution; ///< contains the current resolution of the window
	
	Vector2 m_targetPosition = { 0.0f,0.0f }; ///< contains the target position the element is moving to
	Vector2 m_startingPosition = { 0.0f,0.0f }; ///< contains the atsrting position the element is moving from
	Vector2 m_relativeSpeed = { 0.0f, 0.0f }; ///< contains the speed the element is moving with
	
	Rectangle m_collider;  ///< contains the absolute position (top left) and size on the screen
	Alignment m_alignment; ///< contains the alignment of the element

	/**
	 * calculates the absolute position and size out of the relative position and size and the current resolution.
	 */
	virtual void UpdateCollider() {
		m_collider = {
			m_pos.x * m_resolution.x,
			m_pos.y * m_resolution.y,
			m_size.x * m_resolution.x,
			m_size.y * m_resolution.y
		};
	}
	/**
	 * calculates the relative position and size out of the absolute position and size and the current resolution.
	 */
	void UpdateColiderReverse() {
		m_pos = {
			m_collider.x / m_resolution.x,
			m_collider.y / m_resolution.y
		};
		m_size = {
			m_collider.width / m_resolution.x,
			m_collider.height / m_resolution.y
		};
	}

	/**
	 * moves the element to the set position in a linear way.
	 */
	void MovePointLinear() {
		if (m_moveType != MoveType::POINT_LINEAR) { return; }

		float time = GetFrameTime();
		Vector2 diff = {
			(m_targetPosition.x - m_startingPosition.x) * m_relativeSpeed.x * time,
			(m_targetPosition.y - m_startingPosition.y) * m_relativeSpeed.y * time
		};
		Move(diff);
	}
	/**
	 * moves the element to the set position in a asymptotic way.
	 */
	void MovePointAsynptotic() {
		if (m_moveType != MoveType::POINT_ASYNPTOTIC) { return; }

		float time = GetFrameTime();
		Vector2 diff = {
			(m_targetPosition.x - m_pos.x) * m_relativeSpeed.x * time,
			(m_targetPosition.y - m_pos.y) * m_relativeSpeed.y * time
		};
		Move(diff);
	}
	/**
	 * moves the element at a certain speed in a linea way.
	 */
	void MoveSpeedLinear() {
		if (m_moveType != MoveType::SPEED_LINEAR) { return; }

		float time = GetFrameTime();
		Vector2 diff = {
			m_relativeSpeed.x * time,
			m_relativeSpeed.y * time
		};
		Move(diff);
	}
	/**
	 * moves the element by the offset.
	 */
	void Move(Vector2 offset) {
		m_pos = {
			m_pos.x + offset.x,
			m_pos.y + offset.y
		};

		UpdateCollider();
		CheckStopMoving();
	}
	/**
	 * checks if the element has arrived at the target point.
	 * if so it calps the current position and stops the movement.
	 */
	void CheckStopMoving() {
		if (m_moveType == MoveType::SPEED_LINEAR 
			or m_moveType == MoveType::NONE) { return; }

		bool shouldStop = (m_targetPosition.x - m_pos.x) < 0.001f
			&& (m_targetPosition.x - m_pos.x) > -0.001f
			&& (m_targetPosition.y - m_pos.y) < 0.001f
			&& (m_targetPosition.y - m_pos.y) > -0.001f;
		if (shouldStop) {
			StopMoving();
		}
	}

public:
	/**
	 * ctor.
	 * calls the aligned collider.
	 */
	UIElement(Vector2 pos, Vector2 size, Alignment alignment, Vector2 resolution)
		: m_pos(pos), m_size(size), m_alignment(alignment), m_resolution(resolution) {
		
		m_collider = GetAlignedCollider(m_pos, m_size, alignment, resolution);
	}
	/**
	 * default virtual dtor.
	 */
	virtual ~UIElement() = default;

	/**
	 * returns if the element is currently moving
	 */
	[[nodiscard]] bool IsMoving() const {
		return m_moveType != MoveType::NONE;
	}
	/**
	 * returns the current alignment.
	 */
	[[nodiscard]] Alignment GetAlignment() const {
		return m_alignment;
	}
	/**
	 * sets a new relative position.
	 * calls to update the collider.
	 */
	virtual void SetPosition(Vector2 pos) {
		m_pos = pos;
		m_collider = GetAlignedCollider(m_pos, m_size, m_alignment, m_resolution);
	}
	/**
	 * returns the current relative position. 
	 */
	[[nodiscard]] Vector2 GetPosition() const {
		return m_pos;
	}
	/**
	 * sets a new relative size.
	 * calls to update the collider.
	 */
	virtual void SetSize(Vector2 size) {
		m_size = size;
		UpdateCollider();
	}
	/**
	 * returns the current relative size.
	 */
	[[nodiscard]] Vector2 GetSize() const {
		return m_size;
	}
	/**
	 * sets a new resolution.
	 */
	void SetResolution(Vector2 resolution) {
		m_resolution = resolution;
		UpdateCollider();
	}
	/**
	 *  returns the current resolution.
	 */
	[[nodiscard]] Vector2 GetResolution() const {
		return m_resolution;
	}
	/**
	 * sets a new absolute position and size.
	 * calles to update the relative position and size.
	 */
	virtual void SetCollider(Rectangle colider) {
		m_collider = colider;
		UpdateColiderReverse();
	}
	/**
	 * returns the current absolute position and size.
	 */
	[[nodiscard]] Rectangle GetCollider() const {
		return m_collider;
	}

	/**
	 * moves the element at a certain speed in a certain direction until it gets stoppt by StopMoving().
	 * result is a linear movment in one direction.
	 * the angle is clockwise.
	 */
	virtual void MoveBySpeed(float relativeSpeed, float angle) {
		m_moveType = MoveType::SPEED_LINEAR;
		m_targetPosition = { 0.0f,0.0f };
		m_startingPosition = { 0.0f,0.0f };

		float tau = std::numbers::pi_v<float> * 2.0f;

		float speedX = std::sin(angle * tau / 360.0f) * relativeSpeed;
		float speedY = -(std::cos(angle * tau / 360.0f) * relativeSpeed);

		m_relativeSpeed = { speedX, speedY };
	}
	/**
	 * moves the element to the provided position at the provided speed.
	 * result ia a linear movment in one direction.
	 */
	virtual void MoveToPositionLinear(Vector2 position, float relativeSpeed) {
		m_moveType = MoveType::POINT_LINEAR;
		m_startingPosition = m_pos;
		m_targetPosition = GetAllignedPosition(m_alignment, position, m_size);
		m_relativeSpeed = { relativeSpeed, relativeSpeed };
	}
	/**
	 * moves the element to the provided position at the provided speed.
	 * result is an asymptotic movment in one direction.
	 */
	virtual void MoveToPositionAsymptotic(Vector2 position, float relativeSpeed) {
		m_moveType = MoveType::POINT_ASYNPTOTIC;
		m_startingPosition = m_pos;
		m_targetPosition = GetAllignedPosition(m_alignment, position, m_size);
		m_relativeSpeed = { relativeSpeed, relativeSpeed };
	}
	/**
	 * stops all kinds off movments immideatly.
	 * resets all moving related values.
	 */
	virtual void StopMoving() {
		m_moveType = MoveType::NONE;
		m_startingPosition = { 0.0f,0.0f };
		m_targetPosition = { 0.0f,0.0f };
		m_relativeSpeed = { 0.0f,0.0f };
	}

	/**
	 * checks if there is a current movment.
	 * moves the element if so.
	 */
	virtual void CheckAndUpdate(Vector2 const&, AppContext const& ) {
		switch (m_moveType) {
			default:
			case MoveType::NONE:
				break;
			case MoveType::POINT_LINEAR:
				MovePointLinear();
				break;
			case MoveType::POINT_ASYNPTOTIC:
				MovePointAsynptotic();
				break;
			case MoveType::SPEED_LINEAR:
				MoveSpeedLinear();
				break;
		}
	}
	/**
	 * just virtual.
	 * to implement.
	 * should render the element.
	 */
	virtual void Render(AppContext const& appContext) = 0;
	/**
	 * sets new resolution.
	 * cals update collider.
	 */
	virtual void Resize(Vector2 resolution, AppContext const&) {
		m_resolution = resolution;
		UpdateCollider();
	};
};