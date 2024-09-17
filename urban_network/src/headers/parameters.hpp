//parameters and setting for the simulation
#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

//general directives
inline bool LOAD_GRAPH = true;
inline bool PARSING_MODE = false;
inline bool ENABLE_GRAPHICS = true;
inline bool PROCESS_STATS = true;

//draw directives
inline bool LOG_HISTO_FLOW = true;
inline bool LOG_HISTO_OCC = true;
inline bool LOG_OCCUPATION_VS_TIME = true;
inline int LOG_OCCUPATION_VS_TIME_NODE = 355;

//when building
inline int N_NODES = 100;
inline int ROAD_WEIGHT = 1;
inline int N_DIAGONAL_ROADS = 10;
inline int N_REMOVED_ROADS = 10;

//dynamical parameter
inline int N_AGENTS = 900;
inline int MAX_SPAWNABLE = 1500;
inline int MIN_DIST_DIJKSTRA = 5;
inline int MAX_CAP = 15;
inline int FLOW_RATE = 3;
inline int TIME_TO_SLEEP = 0;
inline int TIME_MAX_SIMULATION = 20000;

#endif
