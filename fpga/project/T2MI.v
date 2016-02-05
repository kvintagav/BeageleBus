`include "define.v"

//=======================================================
//  This code is generated by Terasic System Builder
//=======================================================

module T2MI(


	

	//////////// CLOCK //////////
	OSC_50_B3B,
	OSC_50_B4A,
	OSC_50_B5B,
	OSC_50_B8A,

	//////////// LED //////////
	LED,

	//////////// KEY //////////
	KEY,
	RESET_n,

	//////////// SW //////////
	SW,

	

	//////////// HSMC, HSMC connect to HTG - HSMC to PIO Adaptor //////////
	GPIO0_0,
	GPIO0_1,
	GPIO0_10,
	GPIO0_11,
	GPIO0_12,
	GPIO0_13,
	GPIO0_14,
	GPIO0_15,
	GPIO0_16,
	GPIO0_17,
	GPIO0_18,
	GPIO0_19,
	GPIO0_2,
	GPIO0_20,
	GPIO0_21,
	GPIO0_22,
	GPIO0_23,
	GPIO0_24,
	GPIO0_25,
	GPIO0_26,
	GPIO0_27,
	GPIO0_28,
	GPIO0_29,
	GPIO0_3,
	GPIO0_30,
	GPIO0_31,
	GPIO0_32,
	GPIO0_33,
	GPIO0_34,
	GPIO0_35,
	GPIO0_4,
	GPIO0_5,
	GPIO0_6,
	GPIO0_7,
	GPIO0_8,
	GPIO0_9,
	GPIO1_0,
	GPIO1_1,
	GPIO1_10,
	GPIO1_11,
	GPIO1_12,
	GPIO1_13,
	GPIO1_14,
	GPIO1_15,
	GPIO1_16,
	GPIO1_17,
	GPIO1_18,
	GPIO1_19,
	GPIO1_2,
	GPIO1_20,
	GPIO1_21,
	GPIO1_22,
	GPIO1_23,
	GPIO1_24,
	GPIO1_25,
	GPIO1_26,
	GPIO1_27,
	GPIO1_28,
	GPIO1_29,
	GPIO1_3,
	GPIO1_30,
	GPIO1_31,
	GPIO1_32,
	GPIO1_33,
	GPIO1_34,
	GPIO1_35,
	GPIO1_4,
	GPIO1_5,
	GPIO1_6,
	GPIO1_7,
	GPIO1_8,
	GPIO1_9,
	
	cle,
	ale,
	nre,
	nce,
	nwe,

	nwait,
	 bidir,
	address,
	
);

//=======================================================
//  PARAMETER declarations
//=======================================================


//=======================================================
//  PORT declarations
//=======================================================

//////////// CLOCK //////////
input 		          		OSC_50_B3B;
input 		          		OSC_50_B4A;
input 		          		OSC_50_B5B;
input 		          		OSC_50_B8A;

//////////// LED //////////
output		     [3:0]		LED;

//////////// KEY //////////
input 		     [3:0]		KEY;
input 		          		RESET_n;

//////////// SW //////////
input 		     [3:0]		SW;


//////////// HSMC, HSMC connect to HTG - HSMC to PIO Adaptor //////////
inout 		          		GPIO0_0;
inout 		          		GPIO0_1;
inout 		          		GPIO0_10;
inout 		          		GPIO0_11;
inout 		          		GPIO0_12;
inout 		          		GPIO0_13;
inout 		          		GPIO0_14;
inout 		          		GPIO0_15;
inout 		          		GPIO0_16;
inout 		          		GPIO0_17;
inout 		          		GPIO0_18;
inout 		          		GPIO0_19;
inout 		          		GPIO0_2;
inout 		          		GPIO0_20;
inout 		          		GPIO0_21;
inout 		          		GPIO0_22;
inout 		          		GPIO0_23;
inout 		          		GPIO0_24;
inout 		          		GPIO0_25;
inout 		          		GPIO0_26;
inout 		          		GPIO0_27;
inout 		          		GPIO0_28;
inout 		          		GPIO0_29;
inout 		          		GPIO0_3;
inout 		          		GPIO0_30;
inout 		          		GPIO0_31;
inout 		          		GPIO0_32;
inout 		          		GPIO0_33;
inout 		          		GPIO0_34;
inout 		          		GPIO0_35;
inout 		          		GPIO0_4;
inout 		          		GPIO0_5;
inout 		          		GPIO0_6;
inout 		          		GPIO0_7;
inout 		          		GPIO0_8;
inout 		          		GPIO0_9;
inout 		          		GPIO1_0;
inout 		          		GPIO1_1;
inout 		          		GPIO1_10;
inout 		          		GPIO1_11;
inout 		          		GPIO1_12;
inout 		          		GPIO1_13;
inout 		          		GPIO1_14;
inout 		          		GPIO1_15;
inout 		          		GPIO1_16;
inout 		          		GPIO1_17;
inout 		          		GPIO1_18;
inout 		          		GPIO1_19;
inout 		          		GPIO1_2;
inout 		          		GPIO1_20;
inout 		          		GPIO1_21;
inout 		          		GPIO1_22;
inout 		          		GPIO1_23;
inout 		          		GPIO1_24;
inout 		          		GPIO1_25;
inout 		          		GPIO1_26;
inout 		          		GPIO1_27;
inout 		          		GPIO1_28;
inout 		          		GPIO1_29;
inout 		          		GPIO1_3;
inout 		          		GPIO1_30;
inout 		          		GPIO1_31;
inout 		          		GPIO1_32;
inout 		          		GPIO1_33;
inout 		          		GPIO1_34;
inout 		          		GPIO1_35;
inout 		          		GPIO1_4;
inout 		          		GPIO1_5;
inout 		          		GPIO1_6;
inout 		          		GPIO1_7;
inout 		          		GPIO1_8;
inout 		          		GPIO1_9;

input  cle;
input  ale;
input  nre;
input  nce;
input  nwe;

output nwait;
inout   [`FSMC_WIDTH-1:0] bidir;
input 	[19:0] address;
wire read_write;

wire [`FSMC_WIDTH-1:0]row_addr;
wire [`FSMC_WIDTH-1:0]column_addr;
wire [`FSMC_WIDTH-1:0]data_fsmc_to_memory;
wire [`FSMC_WIDTH-1:0]data_memory_to_fsmc;
wire clk;

wire reset;

 pll pll_inst(
.refclk(OSC_50_B3B),   //  refclk.clk
.rst(RESET_n),      //   reset.reset
.outclk_0(clk)  // outclk0.clk
	);

	
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
.RESET(RESET_n),
.READ_WRITE(read_write),
.IN_DATA(data_fsmc_to_memory),
.OUT_DATA(data_memory_to_fsmc),
.COLUMN_ADDR(column_addr), 
.ROW_ADDR(row_addr));



//=======================================================
//  REG/WIRE declarations
//=======================================================




//=======================================================
//  Structural coding
//=======================================================



endmodule
