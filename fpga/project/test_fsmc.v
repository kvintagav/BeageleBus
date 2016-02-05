module test_fsmc;

reg reset,clk,CLE,ALE,NRE,NCE,NWE;
input wire NWAIT;
inout   [`FSMC_WIDTH-1:0] BIDIR;
wire [`FSMC_WIDTH-1:0]  in_data;
reg [`FSMC_WIDTH-1:0]  out_data;
reg [`FSMC_WIDTH-1:0]  status;
reg read_write; // 1 - write ; 0 - read
assign BIDIR = (read_write==1) ? out_data : 16'bzzzz_zzzz_zzzz_zzzz;
project_fsmc project_fsmc(clk,CLE,ALE,NRE,NCE,NWE,NWAIT,BIDIR);

assign in_data = BIDIR[`FSMC_WIDTH-1:0] ;
reg [`FSMC_WIDTH-1:0]  i;
always
	#2 clk =~clk;

initial
begin
	clk = 0;
	reset=0;
	CLE=0;
	NCE=1;
	NWE=1;
	ALE=0;
	NRE=1;
	
	out_data=16'b0000_0000_0000_0000;
	//write page
	read_write=0;
	//#20	write=0;
	#10 reset=1;
	#10 reset =0;
	#20;
	CLE=1;
	NCE=0;
	read_write=1;
	out_data=16'b0000_0000_1000_0000;
	#5;
	NWE=0;
	#5
	NWE=1;
	#5
	CLE=0;
	ALE=1;
	#5
	out_data=16'b0000_0000_0000_0000; //column
	#5;
	NWE=0;
	#5
	NWE=1;
	#5;
	out_data=16'b0000_0000_0000_0000; //row
	NWE=0;
	#5
	NWE=1;
	#5;
	ALE=0;
	read_write=0;
	#10;
	read_write=1;
	#5;
	for (i=0; i < 16; i=i+1)
	begin
		
		
		NWE=0;
		#5;
		NWE=1;
		#5;
		out_data=out_data+1;
	end 
	read_write=0;
	#10;
	CLE=1;
	read_write=1;
	out_data=16'b0000_0000_0001_0000;
	#5;
	NWE=0;
	#5;
	NWE=1;
	#5;
	CLE=0;
	read_write=0;
	#20;
	read_write=1;
	CLE=1;
	NWE=0;
	out_data=16'b0000_0000_0111_0000;
	#5;
	NWE=1;
	#5;
	read_write=0;
	#5;
	CLE=0;
	NRE=0;
	#5;
	NRE=1;
	#5;
	NCE=1;
	#5
	//read_page
	read_write=1;
	CLE=1;
	NCE=0;
	NWE=0;
	ALE=0;
	NRE=1;
	out_data[7:0]=16'b0000_0000_0000_0000;
	#2;
	CLE=0;
	#5;
	NWE=1;
	#5;
	NWE=0;
	ALE=1;
	out_data=16'b0000_0000_0000_0000; //column
	#5;
	NWE=1;
	#5;
	NWE=0;
	out_data=16'b0000_0000_0000_0000; //row
	#5;
	NWE=1;
	#3;
	ALE=0;
	#2;
	CLE=1;
	NWE=0;
	out_data=16'b0000_0000_0011_0000;
	#5;
	NWE=1;
	#2;
	CLE=0;
	#20;
	read_write=0;
	for (i=0; i < 16; i=i+1)
	begin
		NRE=0;
		#5;
		NRE=1;
		#5;
	end 
	#20;
	NCE=1;
end

initial
begin
	#800 $finish;
end

initial
begin
$dumpfile("out.vcd");
$dumpvars(0,test_fsmc);
end

endmodule
