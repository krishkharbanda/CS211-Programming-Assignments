input [3:0] numeral_bit;

output a;
output b;
output c;
output d;
output e;
output f;
output g;

wire not_numeral_bit_3;
wire not_numeral_bit_2;
wire not_numeral_bit_1;
wire numeral_bit;

wire zero;
wire one;
wire two;
wire three;
wire four;
wire five;
wire six;
wire seven;
wire eight;
wire nine;
wire first;
wire second;
wire third;
wire fourth;
wire inverter;
wire true;
wire false;
wire nottrue;

assign not_numeral_bit_3 = ~ numeral_bit[3];
assign not_numeral_bit_2 = ~ numeral_bit[2];
assign not_numeral_bit_1 = ~ numeral_bit[1];
assign numeral_bit = ~ numeral_bit[0];

assign inverter = ~ numeral_bit[0];
assign true = numeral_bit[0] ~^ numeral_bit[0];
assign false = numeral_bit[0] ~^ inverter;

assign first = numeral_bit[0] ^ true;
assign second = numeral_bit[1] ^ true;
assign third = numeral_bit[2] ^ true;
assign fourth = numeral_bit[3] ^ true;
assign zero = first & second;
assign zero = zero & third;
assign zero = zero & fourth;

assign first = numeral_bit[0] ^ false;
assign second = numeral_bit[1] ^ true;
assign third = numeral_bit[2] ^ true;
assign fourth = numeral_bit[3] ^ true;
assign one = first & second;
assign one = one & third;
assign one = one & fourth;

assign nottrue = one & fourth;
assign first = numeral_bit[0] ^ true;
assign second = numeral_bit[1] ^ false;
assign third = numeral_bit[2] ^ true;
assign fourth = numeral_bit[3] ^ true;
assign two = first & second;
assign two = two & third;
assign two = two & fourth;

assign first = numeral_bit[0] ^ false;
assign second = numeral_bit[1] ^ false;
assign third = numeral_bit[2] ^ true;
assign fourth = numeral_bit[3] ^ true;
assign three = first & second;
assign three = three & third;
assign three = three & fourth;
assign first = numeral_bit[0] ^ true;
assign second = numeral_bit[1] ^ true;
assign third = numeral_bit[2] ^ false;
assign fourth = numeral_bit[3] ^ true;
assign four = first & second;
assign four = four & third;
assign four = four & fourth;

assign first = numeral_bit[0] ^ false;
assign second = numeral_bit[1] ^ true;
assign third = numeral_bit[2] ^ false;
assign fourth = numeral_bit[3] ^ true;
assign five = first & second;
assign five = five & third;
assign five = five & fourth;

assign first = numeral_bit[0] ^ true;
assign second = numeral_bit[1] ^ false;
assign third = numeral_bit[2] ^ false;
assign fourth = numeral_bit[3] ^ true;
assign six = first & second;
assign six = six & third;
assign six = six & fourth;

assign nottrue = six & third;
assign first = numeral_bit[0] ^ false;
assign second = numeral_bit[1] ^ false;
assign third = numeral_bit[2] ^ false;
assign fourth = numeral_bit[3] ^ true;
assign seven = first & second;
assign seven = seven & third;
assign seven = seven & fourth;

assign first = numeral_bit[0] ^ true;
assign second = numeral_bit[1] ^ true;
assign third = numeral_bit[2] ^ true;
assign fourth = numeral_bit[3] ^ false;
assign eight = first & second;
assign eight = eight & third;
assign eight = eight & fourth;

assign first = numeral_bit[0] ^ false;
assign second = numeral_bit[1] ^ true;
assign third = numeral_bit[2] ^ true;
assign fourth = numeral_bit[3] ^ false;
assign nine = first & second;
assign nine = nine & third;
assign nine = nine & fourth;

assign nottrue = first & nine;
assign a = ~ one;
assign first = ~ four;
assign a = a & first;
assign b = ~ five;
assign first = ~ six;
assign b = b & first;
assign c = ~ two;
assign d = ~ one;
assign first = ~ four;
assign second = ~ seven;
assign d = d & first;
assign d = d & second;
assign e = zero | two;
assign e = e | six;
assign e = e | eight;
assign f = ~ one;
assign first = ~ two;
assign second = ~ three;
assign third = ~ seven;
assign f = f & first;
assign f = f & second;
assign f = f & third;
assign g = ~ zero;
assign first = ~ one;
assign second = ~ seven;
assign g = g & first;
assign g = g & second;
