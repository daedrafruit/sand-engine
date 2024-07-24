#pragma once
#include <SDL.h>
#include <memory>
#include <vector>

struct SwapOperation {
	int x1, y1;
	int x2, y2;
};

struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
};

enum class CellId {
    Air,
    Stone,
    Sand,
    Water,
    Fire,
    Smoke
};

class Entity {
public:

	Entity(int lastUpdated);
	virtual ~Entity() = default;

	void setLastUpdated(int p_lastUpdated);
	void setRegister(char reg, int value);

	int getRegister(char reg) const;
	int getLastUpdated() const;
	virtual CellId getId() const = 0;
	virtual Color getColor() const = 0;

	virtual std::unique_ptr<SwapOperation> update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y);

private:
    int lastUpdated;
    int ra;
};

// *********************************************************************
// Air
// *********************************************************************

class Air : public Entity {
public:
	using Entity::Entity;

	Color getColor() const override {
		return {0, 0, 0};
	}

	CellId getId() const override {
		return CellId::Air;
	}
};

// *********************************************************************
// Stone
// *********************************************************************

class Stone : public Entity {
public:
	using Entity::Entity;

	Color getColor() const override {
		return {200, 200, 200};
	}

	CellId getId() const override {
		return CellId::Stone;
	}
};

// *********************************************************************
// Sand
// *********************************************************************

class Sand : public Entity {
public:
	using Entity::Entity;

	std::unique_ptr<SwapOperation> update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) override;

	Color getColor() const override {
		return {245, 200, 70};
	}

	CellId getId() const override {
		return CellId::Sand;
	}
};

// *********************************************************************
// Water
// *********************************************************************

class Water : public Entity {
public:
	using Entity::Entity;

	std::unique_ptr<SwapOperation> update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) override;

	Color getColor() const override {
		return {0, 0, 255};
	}

	CellId getId() const override {
		return CellId::Water;
	}
};

// *********************************************************************
// Fire
// *********************************************************************

class Fire : public Entity {
public:
	using Entity::Entity;

	std::unique_ptr<SwapOperation> update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) override;

	Color getColor() const override {
		return {255, 0, 0};
	}

	CellId getId() const override {
		return CellId::Fire;
	}
};

// *********************************************************************
// Smoke
// *********************************************************************

class Smoke : public Entity {
public:
	using Entity::Entity;

	std::unique_ptr<SwapOperation> update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) override;

	Color getColor() const override {
		return {70, 70, 70};
	}

	CellId getId() const override {
		return CellId::Smoke;
	}
};
