#include "MyStrategy.h"

#define PI 3.14159265358979323846
#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdlib>

using namespace model;
using namespace std;


void MyStrategy::move(const Hockeyist& self, const World& world, const Game& game, Move& move) {

    //move.setSpeedUp(-1.0);
    //move.setTurn(PI);
    //move.setAction(STRIKE); ///
	/*double num;
 	if (world.getPuck().getOwnerHockeyistId() == self.getId()){ move.setSpeedUp(0.7); }
	if (self.getId() == 0){
		if (self.getX() > 200, self.getX() < 100, self.getY() < 360, self.getY() > 560){
			move.setTurn(self.getAngleTo(105, 400));
			move.setSpeedUp(0.6);
		}
		else
		{
			move.setTurn(self.getAngleTo(world.getPuck()));
			move.setAction(TAKE_PUCK);
		}

	}*/
	int defence=0;
	int attack=1;
	if (world.getPuck().getOwnerHockeyistId() == self.getId()){ 
		if (self.getTeammateIndex() == 1){
			attack = 1;
			defence = 0;
		}
		if (self.getTeammateIndex() == 0){ 
			attack = 0;
			defence = 1;
		}
		
	 }

	


	if (world.getPuck().getOwnerHockeyistId() == self.getId()){ move.setAction(SWING); }
	else{
		move.setAction(TAKE_PUCK);
	};

}

MyStrategy::MyStrategy() { }
