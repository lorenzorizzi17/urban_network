## Agent-based OD model to simulate the vehicular traffic in a urban network

![Recording 2024-09-23 at 16 09 16](https://github.com/user-attachments/assets/d2372c9b-89a6-4715-81ed-6e4b350fc863)

This project tries to implement an agent-based Origin-Destination (OD) model designed to simulate vehicular traffic within an urban network. The final goal of this dynamic model is to study the conditions that need to be met for a congestion to spread across the whole network (_total gridlock_) and to characterize the properties of the various phases of the systems. In fact, it might be possible to characterize the transition between a _free flow_ state to a congested one as a physical phase transition (see, for example, ref. [1](https://www.pnas.org/doi/pdf/10.1073/pnas.1801545116), [2](https://www.nature.com/articles/s42005-023-01144-w))

## The physical model

### Topology of the network
The urban grid on which vehicles move (from now on called _agents_) is represented as a network where each road is associated with a vertex, and two vertices are connected by an edge if there is a way to travel from one road to the other by crossing a single intersection. Note that this specific representation, where each road is mapped to a vertex (the _dual representation_), is not the most intuitive one (road -> edge, _direct representation_), but it is the most effective when dealing with the dynamics of agents moving through the grid. Hence, we will only use the direct representation of the graph when it comes to displaying the network itself.

The network's structure was chosen to represent a homogeneous bidimensional squared lattice with some embedded randomness (e.g., diagonal roads or deleted edges). Have a look at ([1](graph/graph.png), [2](graph/graph_dual.png)) to see the actual network used in the simulations, in both the direct and dual form. The user can choose whether he wants to generate a new graph from the ensemble when launching the program or to load an existing one stored in a `.dot` file

### Dynamical rules
A critical parameter of the model is $N$, the total number of agents living in the network. When an agent is created, it chooses randomly an origin node (O) to spawn in and a destination node (D) such that the distance O-D is at least $k=5$ (tunable). To get from the origin to the destination, the agent computes the shortest path using Dijkstra's algorithm on the (weighted) graph. The weight associated with each road is not constant but changes with the occupancy of the node according to a monotonic non-linear function: if a road is particularly crowded, then its crossing time is expected to be large when compared to a less populated road, and an agent may prefer avoiding that road to escape the traffic jam. Once the agent reaches the destination node D, it despawns.

The dynamical evolution of the system is modulated by two fundamental parameters: $c, \Phi$. At each time step, agents try to move following their route (determined during creation) by jumping from one vertex to another. In particular:

- Each vertex has a maximum flow rate $\Phi$, meaning that at each iteration only $\Phi$ agents can leave the current node.
- Each vertex has a maximum capacity $c$, meaning that it cannot host more than $c$ agents. If a vehicle wants to enter a node that is already saturated, it must wait until its occupancy drops below $c$.


## Results

Phase transition:

Let the simulation run for $T=10,000$ time steps. If the traffic load $N$ is below a certain threshold, the system evolves freely and reaches a stationary distribution where traffic flows steadily. Above that threshold, the system evolves into a gridlock state, where mobility is severely congested and agents can no longer move.

![P(Gridlock)_page-0001](https://github.com/user-attachments/assets/2837fef7-2334-44a0-8dc0-3c1c77412695)


The graph above shows the occurrence of gridlocks when the system is allowed to evolve for T=10, ⁣000T=10,000 time steps. This behavior is a strong indicator of a potential phase transition around a specific value of the traffic load. However, since we are not in the thermodynamic limit, the transition appears smooth rather than abrupt.


(TODO) Flux vs load (MFE)

(TODO) Relaxation time


## Requirements
- [SFML library](https://www.sfml-dev.org/)
- [Boost library](https://www.boost.org/)

Be careful! __Boost__ is a header-only library, so it doesn't really need to be linked and built. The existing CMake script assumes that there exists a folder in your home directory `~\boost\` that contains all the Boost's header files. If this is not the case, just edit the file `src/CMakeLists.txt` identifying the line:

`include_directories("~/boost")`

and replacing it with: 

`include_directories("/path/to/boost")`



## Building the program (CMake)

To correctly compile and build the program, one can take advantage of the CMake software. Just enter the terminal and, in the root directory, type:

`$ cmake -S ./src -B ./build `

(N.B. add `-DCMAKE_BUILD_TYPE=Release` if you want to run the program in release mode with O3 optimization).

Once the CMake configuration has finished, build the code:

`$ cmake --build build`

This will automatically create in `/build` directory an executable file, `main.out`:

`$ build/main.out`

