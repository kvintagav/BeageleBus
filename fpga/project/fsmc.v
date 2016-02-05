`include "define.v"

module fsmc(
input wire CLK,
input wire CLE,
input wire ALE,
input wire NRE,
input wire NCE,
input wire NWE,

output NWAIT,
output RESET_FSMC,

inout   [`FSMC_WIDTH-1:0] BIDIR,
input wire[`FSMC_WIDTH-1:0] IN_DATA,
output[`FSMC_WIDTH-1:0] OUT_DATA,

output [15 : 0] 				COLUMN_ADDR,
output [15 : 0] 					ROW_ADDR,
output READ_WRITE
//input wire STOP_BUFER
);

wire [7 : 0] in_bus;
wire [`FSMC_WIDTH-1:0]bus_data_com;
wire [`FSMC_WIDTH-1 : 0 ] DIn;
wire [`FSMC_WIDTH-1 : 0 ] wire_out;
assign DIn = BIDIR[`FSMC_WIDTH-1:0];
assign in_bus = BIDIR[7:0];


reg 								 reset_fsmc=1'b0;
reg							    read=1'b0;
reg [15 : 0] 					 column_addr=16'b0;
reg [15 : 0] 					 row_addr;
reg [7:0] 						 command;
reg [`FSMC_WIDTH-1:0]		 in_byte;

reg [`DATA_READ_RAZR-1:0]	 read_byte_cnt=16'b0;
reg [`FSMC_WIDTH-1:0]		 bus_out_bufer;	

reg [7:0]					status=8'b0000_0000;
reg [`FSMC_WIDTH-1:0]		 reg_for_write_to_memory=16'b0;	


`define STATE_IDLE  			0
`define STATE_WAIT_ROW  		1
`define STATE_WAIT_COLUMN  		2
`define STATE_READ_MEMORY   	3
`define STATE_WAIT_READ_MEMORY 	4
`define STATE_WRITE_MEMORY  	5
`define STATE_WAIT_WRITE_MEMORY 6

`define STATE_READ_ID   10
`define STATE_READ_STATUS   11

reg [7:0] avtomat=8'b0000_0000;

assign OUT_DATA = (avtomat==`STATE_WRITE_MEMORY) ? reg_for_write_to_memory: 16'b0000_0000_0000_0000;



assign COLUMN_ADDR = (avtomat==`STATE_READ_MEMORY) ? read_byte_cnt :column_addr;
assign BIDIR = (read==1'b1) ? wire_out : 16'bzzzz_zzzz_zzzz_zzzz;

assign RESET_FSMC = reset_fsmc;
assign NWAIT = 1'b0;


assign READ_WRITE= (avtomat==`STATE_WRITE_MEMORY) ? 1'b0 : 1'b1;



assign wire_out =(command==`CMD_READ_2) ? IN_DATA : bus_out_bufer;


initial
begin
	bus_out_bufer	=16'b0000_0000_0000_0000;
	read           =1'b0;
	command			=8'b0000_0000;
	column_addr		=16'b0000_0000_0000_0000;
	row_addr		=16'b0000_0000_0000_0000;
	in_byte			=8'b0000_0000;
	
end




always@ (negedge NRE or negedge ALE)
begin
	if (ALE==0)
	begin
		 read_byte_cnt<=column_addr;
	end
	else
	begin
		case(command)
			`CMD_READ_2:
			begin
			
				read_byte_cnt<=read_byte_cnt+1'b1;					
			end		
			`CMD_READ_STATUS:
			begin
				bus_out_bufer[15:8]<=8'b0000_0000;
				bus_out_bufer[7:0]<=status;
			end
			`CMD_READ_ID:
			begin
				bus_out_bufer[15:8]<=8'b0000_0000;
				bus_out_bufer[7:0]<=8'b0000_0000;
				
			end
			default:begin
			end
		endcase	
	end
end


always @(posedge NCE or negedge NRE)
begin
	if (NCE)
	begin
		read <= 1'b0;
	end
	else
	begin
		read <= 1'b1;

	end
end	

/*Latches 8 bit address on rising edge of WE#
Latches data on rising edge of WE#
*/

//assign COLUMN_ADDR = column_addr;
assign ROW_ADDR = row_addr;


always @ (posedge NWE or posedge NCE )
begin
	if (NCE)
	begin
		reset_fsmc<=1'b0;
		avtomat<=`STATE_IDLE;
	
	end
	else if (NWE)
	begin

		if ( ALE && ~NCE && ~CLE && NRE) //receive address
		begin	
			case (avtomat)
				`STATE_WAIT_COLUMN:
				begin
					if (command==`CMD_READ_1)
						column_addr<=DIn+1'b1 ;				
					else	
						column_addr<=DIn;				
					avtomat<=`STATE_WAIT_ROW;
				end
				`STATE_WAIT_ROW:
				begin
					row_addr<=DIn;	
					if (command==`CMD_WRITE_1)
					
						avtomat<=`STATE_WRITE_MEMORY;
					else
						avtomat<=`STATE_IDLE;
				end
				default:
				begin
				end
			endcase
			
		end
		if ( ~ALE  && ~NCE && ~CLE && NRE) //write byte
		begin

			if (command==`CMD_WRITE_1)
			begin
				reg_for_write_to_memory<=DIn;
				column_addr<=column_addr+1'b1;
			end
			
		end	
		if ( ~ALE && ~NCE && CLE && NRE)  //receive command
		begin	
			command <= in_bus[7:0];
			case(in_bus[7:0])
				
				`CMD_READ_1:
				begin
					column_addr		<=16'b0000_0000_0000_0000;
					row_addr		<=16'b0000_0000_0000_0000;
					avtomat<=`STATE_WAIT_ROW;
				end
				`CMD_READ_2:
				begin
				
					avtomat<=`STATE_READ_MEMORY;
				end	
				`CMD_WRITE_1:
				begin
					column_addr		<=16'b0000_0000_0000_0000;
					row_addr		<=16'b0000_0000_0000_0000;
					avtomat<=`STATE_WAIT_ROW;
				end
				`CMD_WRITE_2:
				begin
					
					avtomat<=`STATE_IDLE;
				end
				`CMD_RESET:reset_fsmc<=1'b1;
				`CMD_READ_STATUS:avtomat<=`STATE_READ_STATUS;
				
				default:begin
				end
			endcase	
		end	
	end
	else
	begin
	
	end
end

endmodule
