`include "define.v"

module memory(
	input wire CLK,
	input wire RESET,
	input wire READ_WRITE, //1- READ ; 0 -WRITE
	input wire [`FSMC_WIDTH-1:0] IN_DATA,
	output [`FSMC_WIDTH-1:0] OUT_DATA,
	input [15 : 0] 				COLUMN_ADDR,
	input [15 : 0] 					ROW_ADDR
);

reg [`FSMC_WIDTH-1:0] bufer[`LENTH_BUFER-1:0];
reg [`FSMC_WIDTH-1:0] out=16'b0000_0000_0000_0000;

wire [11:0]read_addr;
assign read_addr=COLUMN_ADDR[11:0];
always @(posedge CLK )
begin
   
		
        if (READ_WRITE==1)
		    out<=bufer[COLUMN_ADDR];
		  
        else
			bufer[read_addr]<=IN_DATA;
		  
       
	
end

assign OUT_DATA=out;
endmodule 

