#include "MyStrategy.h"

#define PI 3.14159265358979323846
#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdlib>

using namespace model;
using namespace std;

void MyStrategy::move(const Hockeyist& self, const World& world, const Game& game, Move& move) {
	if (world.getPuck().getOwnerHockeyistId() == self.getId()){ move.setAction(SWING); }
	else{
		move.setAction(TAKE_PUCK);
	};
}

MyStrategy::MyStrategy() { }
