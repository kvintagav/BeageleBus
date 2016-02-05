`include "define.v"

module project_fsmc(
input wire clk,
input wire cle,
input wire ale,
input wire nre,
input wire nce,
input wire nwe,

output nwait,
inout   [`FSMC_WIDTH-1:0] bidir

);


wire read_write;
wire reset;
wire [`FSMC_WIDTH-1:0]row_addr;
wire [`FSMC_WIDTH-1:0]column_addr;
wire [`FSMC_WIDTH-1:0]data_fsmc_to_memory;
wire [`FSMC_WIDTH-1:0]data_memory_to_fsmc;

fsmc fsmc_inst(
.CLK(clk),
.CLE(cle),
.ALE(ale),
.NRE(nre),
.NCE(nce),
.NWE(nwe),
.NWAIT(nwait),
.RESET_FSMC(reset),
.BIDIR(bidir),
.IN_DATA(data_memory_to_fsmc),
.OUT_DATA(data_fsmc_to_memory),
.COLUMN_ADDR (column_addr),
.ROW_ADDR(row_addr),
.READ_WRITE(read_write));

memory memory_inst(
.CLK(clk),
.RESET(reset),
.READ_WRITE(read_write),
.IN_DATA(data_fsmc_to_memory),
.OUT_DATA(data_memory_to_fsmc),
.COLUMN_ADDR(column_addr), 
.ROW_ADDR(row_addr));

endmodule
