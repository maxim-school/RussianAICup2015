#include "MyStrategy.h"
#include <fstream>

#define PI 3.14159265358979323846
#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdlib>

using namespace model;
using namespace std;


double FriendIndex(const Hockeyist& self){
	if (self.getTeammateIndex() == 1){ return 0; }

	else
	{
		return 1;
	}
}
void MyStrategy::move(const Hockeyist& self, const World& world, const Game& game, Move& move) {

 
	Player Me = world.getMyPlayer();
	Player rival = world.getOpponentPlayer();   // RIVAL - "Противник"
	int defence=1;
	int attack=0;
	bool flag = false;
	double FriendId = FriendIndex(self);
	// Определение владельца шайбы
	if (self.getTeammateIndex() == 0){ 
		std::ofstream fout("DIST.txt");
		fout << self.getDistanceTo(Me.getNetRight() + 90, (Me.getNetTop() + Me.getNetBottom()) / 2);
		fout << std::endl;
		fout.close();
	}
	if (self.getTeammateIndex() == 0){
		std::ofstream fout("DIST.txt", std::ios::app);
		fout << self.getDistanceTo(Me.getNetRight() + 90, (Me.getNetTop() + Me.getNetBottom()) / 2);
		fout << std::endl;
		fout.close();
	}

	if (world.getPuck().getOwnerPlayerId() == Me.getId()){
		flag = true;
		if (world.getPuck().getOwnerHockeyistId() == self.getId()){
			attack = self.getTeammateIndex();
			defence = FriendId;
		}
		else {
			attack = FriendId;
			defence = self.getTeammateIndex();
		}

	}else{
		std::ifstream fin("DIST.txt", std::ios::app);
		int H_0;
		int H_1;
		fin >> H_0;
		fin >> H_1;
		fin.close();
		if (self.getTeammateIndex() == 0){
			if (self.getDistanceTo(Me.getNetRight() + 90, (Me.getNetTop() + Me.getNetBottom()) / 2) < H_1){
				attack = FriendId;
				defence = self.getTeammateIndex();
			}
			else{
				attack = self.getTeammateIndex();
				defence = FriendId;
			}
		}
		else{
			if (self.getDistanceTo(Me.getNetRight() + 90, (Me.getNetTop() + Me.getNetBottom()) / 2) < H_0){
				attack = self.getTeammateIndex();
				defence = FriendId;
			}
			else{
				attack = FriendId;
				defence = self.getTeammateIndex();в
			}
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
			double Speed = self.getDistanceTo(Me.getNetRight() + 90, (Me.getNetTop() + Me.getNetBottom()) / 2)/200;
			if (Speed > 1.0){ Speed = 1.0; }
			move.setSpeedUp(Speed);
		}
		else{
			move.setTurn(self.getAngleTo(world.getPuck()));
			move.setAction(TAKE_PUCK);
		}
		
	}
	


	

}

MyStrategy::MyStrategy() { }
