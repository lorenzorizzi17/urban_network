#define N_NODES 100
#define N_AGENTS 4000 //a lungo andare, converge
#define MIN_DIST_DIJKSTRA 5
#define MAX_CAP 15  //CAPIENZA MAX = 5400
#define FLOW_RATE 3 //dipende dalla fisica, 
//coda piena come se raddoppiassi peso distanza
//300 m per strada con 10 m/s, 30 s a percorrerla. Max capacità per 15: 3 auto ogni dieci secondi con peso strada 3 rinormalizzata
#define TIME_TO_SLEEP 0 //in ms
#define ROAD_WEIGHT 1
constexpr double LINEAR_FACTOR = 2*double(ROAD_WEIGHT) / double(MAX_CAP);
#define TIME_MAX_SIMULATION 10000
#define N_DIAGONAL_ROADS 10
#define N_REMOVED_ROADS 10
#define WAITING_TIME 5
#define LOAD_GRAPH true
#define COLOR_DUAL true

//N_AGENTS = 200 ok, 400 anche. 800 equilibrio   -- flow a 3, max  8
//N_AGENTS = 800 se flow a 8 e max a 15

#include"./headers/simulation.hpp"

int main()
{
    Simulation s(N_NODES);
    s.run(TIME_MAX_SIMULATION);
}


//dinamica asincrona: inserimento che faccia superare capienza max. si esatto, sforo capacità. 
//generazione soppressa se strada intasata?
//fine tuning parametri?