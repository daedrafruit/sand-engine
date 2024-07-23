#pragma once
#include <SDL.h>
#include <memory>
#include <optional>

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

    Entity(CellId p_id);

    Entity();

    void setId(CellId p_id, int p_currWorldUpdate);

    void setLastUpdated(int p_lastUpdated);

    void setRegister(char reg, int value);

    int getRegister(char reg) const;

    int getLastUpdated() const;

    CellId getId() const;

    bool isEmpty() const;

    virtual Color getColor() const;

    virtual std::unique_ptr<SwapOperation> update(const Entity* const* const grid, int x, int y);

		virtual ~Entity() = default;

private:
    CellId id;
    int lastUpdated;
    int ra;
};

class Sand : public Entity {
public:
	std::unique_ptr<SwapOperation> update(const Entity *const *const grid, int x, int y) override;
};



