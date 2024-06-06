#pragma once
#include <SDL.h>

struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
};

class Entity {
public:
	Entity(int p_id);
	void setId(int p_id);
	bool isEmpty();
	bool getLastUpdated();
	void setLastUpdated(bool p_lastUpdated);

	int getId();

    virtual Color getColor() const;

private:
	int id;
	bool lastUpdated;
};

class Sand : Entity {
	
};
