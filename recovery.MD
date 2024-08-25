# Aircraft Remover Problem

## Overview

This repository provides the implementation of the **Aircraft Remover Problem**, a complex optimization challenge aimed at minimizing disruptions in airline schedules caused by unexpected events such as storms, airport closures, and unscheduled aircraft maintenance. The solution involves exact mathematical models and a hybrid mathematical heuristic designed to handle large instances efficiently.

### Abstract

The problem of aircraft recovery arises when unexpected disruptions dismantle the original flight schedule. This work initially presents an exact mathematical model to recover an airline's schedule. Due to the NP-Hard nature of the problem, the exact model is computationally infeasible for large instances, leading to the development of a two-part heuristic: a multi-product network flow model to obtain a new schedule with minimal flight cancellations and delays, and an integer linear programming model to minimize aircraft exchanges from the original schedule.

## Key Features

- **Technologies Used:** C++, Gurobi Optimizer
- **Problem Type:** Mixed-Integer Linear Programming (MILP)
- **Solution Approach:** Hybrid mathematical heuristic combining fleet assignment and aircraft rotation models.

## Problem Description

The Aircraft Remover Problem deals with reassigning aircraft and rescheduling flights in response to disruptions such as delays, cancellations, and unscheduled maintenance. The objective is to minimize the impact on passengers and operational costs for airlines while ensuring a return to the original schedule as soon as possible.

### Publications

- **Thesis:** *Study of the Aircraft Recovery Problem* - [Escola Politécnica da Universidade de São Paulo, 2019].
- **Paper:** *Exact Model and Mathematical Heuristics for the Aircraft Recovery Problem* - [Link to paper, if available].

## Solution Approach

The solution is divided into two main components:

1. **Fleet Assignment:** This step defines the delay or cancellation of each flight while respecting fleet size and airport capacity. It uses a mixed-integer linear programming model.
2. **Aircraft Rotation:** This step assigns specific aircraft to each flight, aiming to minimize aircraft exchanges while respecting operational constraints.

### Mathematical Model

The problem is formulated using a space-time network where nodes represent events (takeoffs, landings, maintenance) and arcs represent flights or ground activities. The objective is to minimize the total cost, considering both delays and cancellations.

**Objective Function:**

\[ \min \sum_{fc \in FC} \sum_{k \in K_{fc}} x_{fck} \cdot \text{cost}_{dly\_swp_{fck}} + \sum_{f \in F} y_f \cdot \text{cost}_{cancel_f} \]

### Heuristics

The proposed heuristic decouples the problem into two parts:
- **Fleet Assignment:** Determines which flights to cancel or delay.
- **Aircraft Rotation:** Assigns specific aircraft to flights to minimize disruptions.

### Results

The heuristic was tested on instances with up to 470 flights, achieving results within 0.5% of the optimal solution in less than one minute. Larger instances were solved using the hybrid heuristic
