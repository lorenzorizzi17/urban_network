#include"headers/config.hpp"
#include<fstream>
#include<iostream>

Config::Config() {}

void Config::load_from_file(std::string const& s)
{
	std::ifstream config_file;
	config_file.open(s);
	if (!config_file.is_open()) {
		std::cerr << "Error: could not open config file" << std::endl;
		exit(1);
	}
	while (config_file.good()) {
		std::string line;
		std::getline(config_file, line);
		if (line.empty()) { continue; };
		if ((line[0] == '\\') && (line[1] == '\\')) { continue; };
		std::string key = line.substr(0, line.find(':'));
		std::string value = line.substr(line.find(':') + 1);
		if (key == "LOAD_GRAPH") LOAD_GRAPH = (value=="true");
		if (key == "PARSING_MODE") PARSING_MODE = (value == "true");
		if (key == "PROCESS_STATS") PROCESS_STATS = (value == "true");
		if (key == "LOG_HISTO_FLOW") LOG_HISTO_FLOW = (value == "true");
		if (key == "LOG_HISTO_OCC") LOG_HISTO_OCC = (value == "true");
		if (key == "LOG_OCCUPATION_VS_TIME") LOG_OCCUPATION_VS_TIME = (value == "true");
		if (key == "LOG_OCCUPATION_VS_TIME_NODE") LOG_OCCUPATION_VS_TIME_NODE = std::stoi(value);
		if (key == "N_NODES") N_NODES = std::stoi(value);
		if (key == "ROAD_WEIGHT") ROAD_WEIGHT = std::stoi(value);
		if (key == "N_DIAGONAL_ROADS") N_DIAGONAL_ROADS = std::stoi(value);
		if (key == "N_REMOVED_ROADS") N_REMOVED_ROADS = std::stoi(value);
		if (key == "N_AGENTS_INITIAL") N_AGENTS_INITIAL = std::stoi(value);
		if (key == "MAX_SPAWNABLE") MAX_SPAWNABLE = std::stoi(value);
		if (key == "MIN_DIST_DIJKSTRA") MIN_DIST_DIJKSTRA = std::stoi(value);
		if (key == "MAX_CAP") MAX_CAP = std::stoi(value);
		if (key == "FLOW_RATE") FLOW_RATE = std::stoi(value);
		if (key == "TIME_TO_SLEEP") TIME_TO_SLEEP = std::stoi(value);
		if (key == "TIME_MAX_SIMULATION") TIME_MAX_SIMULATION = std::stoi(value);
		if (key == "LOG_HISTO_LIFESPAN") LOG_HISTO_LIFESPAN = (value == "true");
		if (key == "WRITE_DATA") WRITE_DATA = (value == "true");
		if (key == "FLUX_BIN") FLUX_BIN = std::stoi(value);
		if (key == "FLUX_MAX") FLUX_MAX = std::stoi(value);
		if (key == "FLUX_MIN") FLUX_MIN = std::stoi(value);
		if (key == "CONSTANT_AGENTS") CONSTANT_AGENTS = (value == "true");
		if (key == "PROBABILITY") PROBABILITY = std::stod(value);
		if (key == "AGENT_FOR_TICK") AGENT_FOR_TICK = std::stoi(value);
	}
}