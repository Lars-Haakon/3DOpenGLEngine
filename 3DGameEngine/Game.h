#ifndef GAME_H
#define GAME_H

class Game
{
public:
	virtual void input() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
};

#endif