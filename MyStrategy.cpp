#include "MyStrategy.h"
#include <fstream>

#define PI 3.14159265358979323846
#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdlib>

using namespace model;
using namespace std;


double CalPSpeed(double X, double Y){
	if (X < 0){ X = X*(-1); }
	if (Y < 0){ Y = Y*(-1); }
	return sqrt(X*X + Y*Y);
}


double FriendIndex(const Hockeyist& self){
	if (self.getTeammateIndex() == 1){ return 0; }

	else
	{
		return 1;
	}
}


bool HockFrontOfMyGoal(const Hockeyist& self, const Player& Me){
	if ((self.getX() > (Me.getNetRight() + 60))
		& (self.getX() < (Me.getNetRight() + 120))
		& (self.getY() > (Me.getNetTop() + 40))
		& (self.getY() < (Me.getNetBottom() - 40))){
		return true;
	}
	else{
		return false;
	}
}

double SpeedGradualDecrease(const Hockeyist& self, double X, double Y){
	double Speed = self.getDistanceTo(X,Y ) / 300;
	if (Speed > 1.0){ Speed = 1.0; }
	return Speed;
}


double GetPuckMoveAngle(){
	
}



void MyStrategy::move(const Hockeyist& self, const World& world, const Game& game, Move& move) {

 
	Player Me = world.getMyPlayer();
	Player rival = world.getOpponentPlayer();   // RIVAL - "Противник"
	int defence=1;
	int attack=0;
	bool MyHocHavePuck = false;
	bool RivalHocHavePuck = false;
	double FriendId = FriendIndex(self);
	// Определение владельца шайбы
	if (world.getPuck().getOwnerPlayerId() == Me.getId()){
		MyHocHavePuck = true;
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
		double H_0;
		double H_1;
		fin >> H_0;
		fin >> H_1;
		fin.close();
		double F_Dist;
		if (world.getPuck().getOwnerPlayerId() == -1){ RivalHocHavePuck = false; }
		else{ RivalHocHavePuck = true; }
		if (self.getTeammateIndex() == 0){ F_Dist = H_1; }
		else { F_Dist = H_0; }


		//Определение атакующего и защитника по положению относительно собственных ворот
		if (self.getDistanceTo(Me.getNetRight() + 90, (Me.getNetTop() + Me.getNetBottom()) / 2) < F_Dist){
			attack = FriendId;
			defence = self.getTeammateIndex();
		}
		else{
			attack = self.getTeammateIndex();
			defence = FriendId;
		}


	}
	
	
	


	// Запись в файл расстояний до ворот
	if (self.getTeammateIndex() == 0){
		std::ofstream fout("DIST.txt");
		fout << self.getDistanceTo(Me.getNetRight() + 90, (Me.getNetTop() + Me.getNetBottom()) / 2);
		fout << std::endl;
		fout.close();
	}
	if (self.getTeammateIndex() == 1){
		std::ofstream fout("DIST.txt", std::ios::app);
		fout << self.getDistanceTo(Me.getNetRight() + 90, (Me.getNetTop() + Me.getNetBottom()) / 2);
		fout << std::endl;
		fout.close();
	}





	double Y_move = (world.getHeight() - rival.getNetBottom()) / 2 +150  ;
	double X_move = rival.getNetFront() - (world.getHeight() - rival.getNetBottom());
	double a = Me.getNetBottom();

	// Тактика Нападающего
	if (self.getTeammateIndex() == attack){
		if (MyHocHavePuck == true){
			if (self.getY() > 460){
				Y_move += rival.getNetBottom()-180;
				a = Me.getNetTop();
			}
			
			if ((self.getDistanceTo(X_move, Y_move)>70) & (self.getX()<1050)){
				move.setTurn(self.getAngleTo(X_move, Y_move));
				move.setSpeedUp(SpeedGradualDecrease(self,X_move ,Y_move ));
			
			}
			else{
				int ang;
				move.setTurn(self.getAngleTo(rival.getNetFront(),a));
				if (self.getAngleTo(rival.getNetFront(), a) > 0){ ang = 1; }
				else{ ang = -1; }
				if (ang*self.getAngleTo(rival.getNetFront(), a)< PI / 180){ move.setAction(STRIKE); }
				
			}
			
		}
		else{
			move.setTurn(self.getAngleTo(world.getPuck()));
			move.setSpeedUp(1.0);
			move.setAction(TAKE_PUCK);
		}
	}






	double PuckSpeedX = world.getPuck.getSpeedX() ; 
	double PuckSpeedY = world.getPuck.getSpeedY();
	double PuckSpeed = CalPSpeed(PuckSpeedX, PuckSpeedY);  // CalPSpeed = calculate puck speed  

	// Тактика Защитника	
	if (self.getTeammateIndex() == defence){

		if (HockFrontOfMyGoal(self, Me) == true){
			move.setTurn(self.getAngleTo(world.getPuck()));
			if ((MyHocHavePuck == false)
				&(RivalHocHavePuck == false)
				&(PuckSpeed > 3.0)){
				move.setTurn(self.getAngleTo(world.getPuck()));
				move.setAction(SWING);
				if (self.getDistanceTo(world.getPuck()) < game.getStickLength()){ move.setAction(STRIKE); }
			}

		}
		else{
			move.setTurn(self.getAngleTo(Me.getNetRight() + 90, world.getHeight() / 2));
			move.setSpeedUp(SpeedGradualDecrease(self, Me.getNetRight() + 90, (Me.getNetTop() + Me.getNetBottom()) / 2));
		}


		if ((MyHocHavePuck == false)
			&(RivalHocHavePuck == false)
			&(PuckSpeed < 2.0)
			&(self.getDistanceTo(world.getPuck()) < 100)){
			move.setTurn(self.getAngleTo(world.getPuck()));
			move.setAction(TAKE_PUCK);
		}




		/*if (MyHocHavePuck == true){
			if (HockFrontOfMyGoal(self,Me)==true){
				move.setTurn(self.getAngleTo(world.getPuck()));
			}
			else{
				move.setTurn(self.getAngleTo(Me.getNetRight() + 90, world.getHeight() / 2));
				move.setSpeedUp(SpeedGradualDecrease(self, Me.getNetRight() + 90, (Me.getNetTop() + Me.getNetBottom()) / 2));
			}
		}
		else if (RivalHocHavePuck == true){
			
		}
		else {

		}
		*/

			
			
			
			/*if (HockFrontOfMyGoal(self,Me)==true){
				move.setTurn(self.getAngleTo(world.getPuck()));
				move.setAction(TAKE_PUCK);
			}
			else{
				move.setTurn(self.getAngleTo(Me.getNetRight() + 90, world.getHeight() / 2));
				
				move.setSpeedUp(SpeedGradualDecrease(self, Me.getNetRight() + 90, (Me.getNetTop() + Me.getNetBottom()) / 2));
			}*/
		

	}
	
	

	

}

MyStrategy::MyStrategy() { }
