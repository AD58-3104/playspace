		// auto getSimulatorPort = []() -> int32_t //これ恐らくもういらないはず。
		// {
		// 	int32_t color = 0;
		// 	int32_t ID = static_cast<int32_t>(getenv("ROBOCUP_ROBOT_ID")[0] - '0');
		// 	if (std::string("blue") == std::string(getenv("ROBOCUP_TEAM_COLOR")))
		// 	{
		// 		color = 20;
		// 	}
		// 	return 10000 + color + ID;
		// };