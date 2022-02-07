#include <string>
#include <iostream>
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/emitterstyle.h"
#include "yaml-cpp/eventhandler.h"
int main(){
	YAML::Node config 	= YAML::LoadFile("controller.yaml");
	int controller			= config["robot"]["controller"].as<int>();
	int baudrate			= config["robot"]["baudrate"].as<int>();
	std::string device 	= config["robot"]["device"].as<std::string>();
	std::string robot   = config["model"].as<std::string>();
	std::cout << robot << device << baudrate << std::endl;
}
