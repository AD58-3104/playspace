#include <string>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <yaml-cpp/emitterstyle.h>
#include <yaml-cpp/eventhandler.h>
int main(){
	std::string s("controller.yaml");
	YAML::Node config 	= YAML::LoadFile(s);
//	int controller			= config["robot"]["controller"].as<int>();
//	int baudrate			= config["robot"]["baudrate"].as<int>();
//	std::string device 	= config["robot"]["device"].as<std::string>();
//	std::string robot   = config["model"].as<std::string>();
//	std::cout << robot << device << baudrate << std::endl;
}
