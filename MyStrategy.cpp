#include "MyStrategy.h"

#define PI 3.14159265358979323846
#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdlib>

using namespace model;
using namespace std;


void MyStrategy::move(const Hockeyist& self, const World& world, const Game& game, Move& move) {

 
	Player Me = world.getMyPlayer();
	Player rival = world.getOpponentPlayer();   // RIVAL - "Противник"
	int defence=1;
	int attack=0;
	bool flag = false;

	// Определение владельца шайбы
	if (world.getPuck().getOwnerHockeyistId() == self.getId()){ 
		flag = true;
		if (self.getTeammateIndex() == 1){
			attack = 1;
			defence = 0;
		}
		if (self.getTeammateIndex() == 0){ 
			attack = 0;
			defence = 1;
		}
	 }


	// Тактика Нападающего
	if (self.getTeammateIndex() == attack){
		if (flag == true){
			move.setTurn(self.getAngleTo(world.getWidth(),world.getHeight() / 2));
			move.setAction(STRIKE);
		}
		else{
			move.setTurn(self.getAngleTo(world.getPuck()));
			move.setSpeedUp(0.7);
			move.setAction(TAKE_PUCK);
		}
	}

	// Тактика Защитника
	if (self.getTeammateIndex() == defence){
		
		
		if (flag == true){
			move.setTurn(self.getAngleTo(Me.getNetRight()+90, (Me.getNetTop()+Me.getNetBottom())/2));
			double Dist = self.getDistanceTo(Me.getNetRight() + 90, (Me.getNetTop() + Me.getNetBottom()) / 2)/100;
			if (Dist > 1.0){ Dist = 1.0; }
			move.setSpeedUp(Dist);
		}
		else{
			move.setTurn(self.getAngleTo(world.getPuck()));
			move.setAction(TAKE_PUCK);
		}
		
	}
	


	

}

MyStrategy::MyStrategy() { }
