#!/bin/bash
iverilog -o build fsmc.v define.v test_fsmc.v project_fsmc.v memory.v
vvp project
gtkwave out.vcd

STRING="Finish build"
echo $STRING
