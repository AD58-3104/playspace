
#include <iostream>
#include "InfoShare.h"

using namespace CitBrains;

std::ostream& operator<<(std::ostream& os, const Pos2D& pos2d)
{
	return (os << '(' << pos2d.x << ',' << pos2d.y << ',' << pos2d.th << ')');
}

std::ostream& operator<<(std::ostream& os, const Pos2DCf& pos2dcf)
{
	struct tm *time = localtime(&pos2dcf.time);
	return (os << pos2dcf.pos << ',' << pos2dcf.cf << ',' << time->tm_hour << ',' << time->tm_min << ',' << time->tm_sec);
}

int main(int argc, char *argv[])
{
	if (argc < 2){
		std::cerr << "InfoShareTest <IP address>" << std::endl;
		exit(1);
	}

	int i;
	std::string ipaddress(argv[1]);
	InfoShare info_share(1, 1, COLOR_MAGENTA, ipaddress);
	Pos2DCf ball_cf(1,2,0.3,50, time(0));
	Pos2DCf self_pos_cf(4,5,0.6,7,time(0));
	std::vector<Pos2D> our_robot;
	our_robot.push_back(self_pos_cf.pos);

	std::vector<Pos2D> enemy_robot;
	enemy_robot.push_back(Pos2D(-8,-9,-1.1));

	std::vector<Pos2D> black_pole;
	black_pole.push_back(Pos2D(12,13,1.4));

	std::string message = "HOGE";
	RobotStatus state;
	state.MotorVoltage = 1200;

	int fps = 10;

	while(true){
		ball_cf.time = time(0);
		self_pos_cf.time = time(0);
		info_share.SendCommonInfo(ball_cf, self_pos_cf, our_robot, enemy_robot, black_pole, fps, message, state);
		boost::this_thread::sleep(boost::posix_time::milliseconds(500));
		std::cout << ball_cf << std::endl;
		std::cout << self_pos_cf << std::endl;
		std::cout << "our_robot" << std::endl;
		for(i = 0; i < our_robot.size(); i ++) std::cout << our_robot[i];
		std::cout << std::endl;
		std::cout << "enemy_robot" << std::endl;
		for(i = 0; i < enemy_robot.size(); i ++) std::cout << enemy_robot[i];
		std::cout << std::endl;
		std::cout << "black_pole" << std::endl;
		for(i = 0; i < black_pole.size(); i ++) std::cout << black_pole[i];
		std::cout << std::endl;
		std::cout << std::endl;
	}

	return 0;
}

