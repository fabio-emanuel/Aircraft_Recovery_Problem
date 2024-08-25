Aircraft Remover Problem
Overview

This repository provides the implementation of the Aircraft Remover Problem, a complex optimization challenge aimed at minimizing disruptions in airline schedules caused by unexpected events such as storms, airport closures, and unscheduled aircraft maintenance. The solution involves exact mathematical models and a hybrid mathematical heuristic designed to handle large instances efficiently.
Abstract

The problem of aircraft recovery arises when unexpected disruptions dismantle the original flight schedule. This work initially presents an exact mathematical model to recover an airline's schedule. Due to the NP-Hard nature of the problem, the exact model is computationally infeasible for large instances, leading to the development of a two-part heuristic: a multi-product network flow model to obtain a new schedule with minimal flight cancellations and delays, and an integer linear programming model to minimize aircraft exchanges from the original schedule.
Key Features

    Technologies Used: C++, Gurobi Optimizer
    Problem Type: Mixed-Integer Linear Programming (MILP)
    Solution Approach: Hybrid mathematical heuristic combining fleet assignment and aircraft rotation models.

Problem Description

The Aircraft Remover Problem deals with reassigning aircraft and rescheduling flights in response to disruptions such as delays, cancellations, and unscheduled maintenance. The objective is to minimize the impact on passengers and operational costs for airlines while ensuring a return to the original schedule as soon as possible.
Publications

    Thesis: Study of the Aircraft Recovery Problem - [Escola Politécnica da Universidade de São Paulo, 2019].
    Paper: Exact Model and Mathematical Heuristics for the Aircraft Recovery Problem - [Link to paper, if available].

Solution Approach

The solution is divided into two main components:

    Fleet Assignment: This step defines the delay or cancellation of each flight while respecting fleet size and airport capacity. It uses a mixed-integer linear programming model.
    Aircraft Rotation: This step assigns specific aircraft to each flight, aiming to minimize aircraft exchanges while respecting operational constraints.

Mathematical Model

The problem is formulated using a space-time network where nodes represent events (takeoffs, landings, maintenance) and arcs represent flights or ground activities. The objective is to minimize the total cost, considering both delays and cancellations.

Objective Function:

min⁡∑fc∈FC∑k∈Kfcxfck⋅costdly_swpfck+∑f∈Fyf⋅costcancelfmin∑fc∈FC​∑k∈Kfc​​xfck​⋅costdly_swpfck​​+∑f∈F​yf​⋅costcancelf​​
Heuristics

The proposed heuristic decouples the problem into two parts:

    Fleet Assignment: Determines which flights to cancel or delay.
    Aircraft Rotation: Assigns specific aircraft to flights to minimize disruptions.

Results

The heuristic was tested on instances with up to 470 flights, achieving results within 0.5% of the optimal solution in less than one minute. Larger instances were solved using the hybrid heuristic, producing high-quality solutions within practical time limits.
Getting Started
Prerequisites

    Gurobi Optimizer: Ensure that Gurobi is installed and properly licensed. Installation Guide
    C++ Compiler: Compatible C++ compiler (e.g., GCC, Clang).

Installation

Clone the repository and navigate to the project directory:

bash

git clone https://github.com/fabio-emanuel/aircraft-remover-problem.git
cd aircraft-remover-problem

Running the Code

To compile and run the code:

bash

g++ -o aircraft_remover main.cpp -lgurobi_c++ -lgurobiXX
./aircraft_remover

Input Data

    Input data should be in the format specified in the data/ directory, including flight schedules, aircraft types, and disruption details.

Output

    The program outputs a rescheduled flight plan, including details on delays, cancellations, and aircraft assignments. Results are stored in the results/ directory.

Examples

    Example 1: Single disruption event. [Link to example]
    Example 2: Multiple disruptions with airport capacity reduction. [Link to example]

Contributing

Contributions are welcome. Please open an issue or submit a pull request with your suggestions.
License

This project is licensed under the MIT License - see the LICENSE file for details.
Acknowledgments

    Institution: Escola Politécnica da Universidade de São Paulo
    Special Thanks: To Gurobi Optimization Inc. for providing the solver.

Contact

    Author: Fabio Emanuel
    Email: [Your Email]
    LinkedIn: [Your LinkedIn Profile]

Compila-se todos os CPPs em um projeto.

Configura-se o arquivo de inputs: rodadas

Estrutura rodadas:
	0,1 => roda o modelo exato ou nao
	Gap => gap aceito
	Tempo MAximos em segundos
	min_opt ; minute_cost ; cancel_cost
	[ diretorio das rodadas ; nome ; discretização ; 1 ; 1; 1 ] => Lista de Instâncias
	# => marcador de finalização
