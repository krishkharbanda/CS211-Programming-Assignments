input [3:0] numeral_bit;

output e;

wire not_numeral_bit_3;
wire not_numeral_bit_2;
wire not_numeral_bit_1;
wire not_numeral_bit_0;

wire minterm_00;
wire minterm_02;
wire minterm_06;
wire minterm_08;

assign not_numeral_bit_3 = ~ numeral_bit[3];
assign not_numeral_bit_2 = ~ numeral_bit[2];
assign not_numeral_bit_1 = ~ numeral_bit[1];
assign not_numeral_bit_0 = ~ numeral_bit[0];

assign minterm_00 = numeral_bit[0] | numeral_bit[1];
assign minterm_00 = minterm_00 | numeral_bit[2];
assign minterm_00 = minterm_00 | numeral_bit[3];
assign minterm_00 = ~ minterm_00;

assign minterm_02 = not_numeral_bit_1 | numeral_bit[0];
assign minterm_02 = minterm_02 | numeral_bit[2];
assign minterm_02 = minterm_02 | numeral_bit[3];
assign minterm_02 = ~ minterm_02;

assign minterm_06 = not_numeral_bit_1 | numeral_bit[0];
assign minterm_06 = minterm_06 | not_numeral_bit_2;
assign minterm_06 = minterm_06 | numeral_bit[3];
assign minterm_06 = ~ minterm_06;

assign minterm_08 = numeral_bit[0] | numeral_bit[1];
assign minterm_08 = minterm_08 | numeral_bit[2];
assign minterm_08 = minterm_08 | not_numeral_bit_3;
assign minterm_08 = ~ minterm_08;

assign e = minterm_00 | minterm_02;
assign e = e | minterm_06;
assign e = e | minterm_08;