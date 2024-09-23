#ifndef CONFIG
#define CONFIG

#include<string>

struct Config {
	//general directives
	bool LOAD_GRAPH = true;
	bool PARSING_MODE = false;
	bool PROCESS_STATS = true;
	bool WRITE_DATA = true;

	//draw directives
	bool LOG_HISTO_FLOW = true;
	int FLUX_BIN = 10;
	int FLUX_MAX = 800;
	int FLUX_MIN = 600;
	bool LOG_HISTO_OCC = true;
	bool LOG_OCCUPATION_VS_TIME = true;
	bool LOG_HISTO_LIFESPAN = true;
	int LOG_OCCUPATION_VS_TIME_NODE = 355;

	//when building
	int N_NODES = 100;
	int ROAD_WEIGHT = 1;
	int N_DIAGONAL_ROADS = 10;
	int N_REMOVED_ROADS = 10;

	//dynamical parameter
	int N_AGENTS_INITIAL = 1400;
	double PROBABILITY = 1;
	int AGENT_FOR_TICK = 10;
	bool CONSTANT_AGENTS = true;
	int MAX_SPAWNABLE = 1500;
	int MIN_DIST_DIJKSTRA = 5;
	int MAX_CAP = 15;
	int FLOW_RATE = 3;
	int TIME_TO_SLEEP = 0;
	int TIME_MAX_SIMULATION = 20000;


	Config();
	void load_from_file(std::string const&);
};

#endif