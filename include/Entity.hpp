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


	void setId(int p_id, int p_currWorldUpdate);
	bool isEmpty();
	int getLastUpdated() const;
	void setLastUpdated(int p_currWorldUpdate);

	int getId() const;

  virtual Color getColor() const;

	

private:
	int id;
	int lastUpdated;
};

class Sand : Entity {
	
};
