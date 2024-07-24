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
protected:
	Entity(CellId p_id);

	Entity();

public:

	void setId(CellId p_id, int p_currWorldUpdate);

	void setLastUpdated(int p_lastUpdated);

	void setRegister(char reg, int value);

	int getRegister(char reg) const;

	int getLastUpdated() const;

	CellId getId() const;

	bool isEmpty() const;

	virtual Color getColor() const = 0;

	virtual std::unique_ptr<SwapOperation> update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y);

	virtual ~Entity() = default;

private:
    CellId id;
    int lastUpdated;
    int ra;
};

class Air : public Entity {
public:
	Color getColor() const override {
		return {0, 0, 0};
	}
};

class Stone : public Entity {
public:
	Color getColor() const override {
		return {200, 200, 200};
	}
};

class Sand : public Entity {
public:

	std::unique_ptr<SwapOperation> update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) override;

	Color getColor() const override {
		return {245, 200, 70};
	}
};



