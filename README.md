TLM2.0 based NoC Simulator
1. Introduction
TLM_NOC is a Network-on-Chip (NoC) simulator developed using SystemC 2.3.2 and licensed under the Apache License.
It enables configurable sizing of the Processing Element (PE) array, with the requirement that the row size matches
 the column size. Each PE in TLM_NOC is equipped with its own local SRAM, with configurable memory size. Additionally,
 the simulator features on-chip SRAM, organized as slices around the grid. PE units are arranged in a row-major order,
 denoted by coordinates (x, y), where x represents the row and y represents the column. Moreover, the system includes
a host processor node positioned at the top-left corner with coordinates (0, 0).

2. Configuration

The "tlm_noc.h" file provides various configuration options, including parameters like row and column sizes, NoC bus width,
 router pipeline depth, system clock cycle, PE internal SRAM size, and L2 cache size, among others. These parameters enable
customization and optimization of the TLM_NOC simulator to meet specific design requirements and constraints. For detailed configuration options, please refer to the "tlm_noc.h" file.


4. Simulation
   
3.1. Clone the Git Project
git clone https://github.com/mike-hls/systemc-tlm-noc.git

3.2. Change to the working directory
cd systemc-tlm-noc

3.3. Run Simulation
Execute the 'make' command to begin simulation.

4. Simulation Results
Upon completion of the simulation, intermediate files will be generated in the 'build' directory, and simulation results will
be displayed on the screen. Additionally, a waveform file named 'trace.vcd' containing TLM payload routing information will be
 generated in the 'wave' directory.

Note: Ensure proper installation of SystemC 2.3.2 before running the simulation.

simulaiton log:
![image](https://github.com/mike-hls/systemc-tlm-noc/assets/168926118/dfc11d28-c6a1-4e02-a368-514e62f1117a)


simulation waveform:
![image](https://github.com/mike-hls/systemc-tlm-noc/assets/168926118/06ab644c-4b3e-4b4f-a301-e526addd64eb)




