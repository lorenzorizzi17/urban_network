## Agent-based OD model to simulate the vehicular traffic in a urban network

![Recording 2024-09-23 at 16 09 16](https://github.com/user-attachments/assets/d2372c9b-89a6-4715-81ed-6e4b350fc863)

This project tries to implement an agent-based Origin-Destination (OD) model designed to simulate vehicular traffic within an urban network. The final goal of this dynamic model is to study the conditions that need to be met for a congestion to spread across the whole network (_total gridlock_) and to characterize the properties of the various phases of the systems. In fact, it might be possible to characterize the transition between a _free flow_ state to a congested one as a physical phase transition (see, for example, ref. [1](https://www.pnas.org/doi/pdf/10.1073/pnas.1801545116), [2](https://www.nature.com/articles/s42005-023-01144-w))



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

