# Simulated-Annealing-Cell-Placer
# Description
This repository contains the implementation of a Simulated Annealing (SA) algorithm for optimizing cell placement. The project aims to minimize the total wire length in a grid layout, showcasing the effectiveness of SA in solving complex optimization problems. It includes a C++ algorithm implementation and Python data visualization scripts.

# Features
C++ Implementation: Core SA algorithm for cell placement optimization.

Python Visualization: Scripts to plot graphs and create GIF animations of the cell placement process.

Detailed Analysis: In-depth analysis of the algorithm's performance with varying cooling rates and the impact on optimization.

# Getting Started
  # Prerequisites
  
  C++ Compiler (GCC recommended)
  
  Python 3.x
  
  Python Libraries: Matplotlib, PIL (Python Imaging Library)
    
  # Installation
  1. Clone the repository to your local machine:
    git clone https://github.com/ShahdElmahallawy/Simulated-Annealing-Cell-Placer

  3. Navigate to the cloned directory
     
     cd simulated-annealing-cell-placement
     
# Usage
1. Run the C++ Program
   
   Compile the C++ code:
     `g++ -o main main.cpp`
   
   Run the executable:
    `./main`
   
3. Visualize Results with Python
   
  Ensure Python and required libraries are installed.

  Run the Python scripts to generate plots and animations.
  
  `python CoolingRatevsTWL.py`
  
  `python CoolingRatevsTWL.py`
  
  `python TempvsTWL.py`

# Performance
The code has passed through all the test cases. In the largest test case, "t3.txt," it was almost 4 seconds. 

# Repository Structure

- main.cpp: Main C++ file containing the SA algorithm.

- tests/: Directory that has all the tests provided in the project. 

- graph_scripts/: Directory containing Python two scripts for generating plots (one for Cooling Rate VS. TWL) and the other for temperature VS. TWL.)

- log/: Directory containing input files and output logs.

- gif_script_and_log/: Directory containing Python script for generating GIF and containing input files and output logs for the grid.

- gifs/: Directory where generated animations are saved.

- graphs/: Directory where generated plots are saved.

# Output
The Program outputs the cells before and after placements and its binary representation. 
It also displays the wire length before and after executing the algorithm.
The TWL and the temperature are saved to a text file for visualization in the Python code. As well as the grid after each temperature to use the results for the GIF.
All the graphs and GIFs are saved after generating them. 


