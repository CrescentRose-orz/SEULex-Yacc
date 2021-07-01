#ifndef Y_TAB_H
#define Y_TAB_H


#define epsilon -1


#define TNBound 191
#define INT 128
#define EQ_OP 129
#define SHORT 130
#define EXTERN 131
#define RIGHT_OP 132
#define XOR_ASSIGN 133
#define AND_ASSIGN 134
#define FLOAT 135
#define WHILE 136
#define RIGHT_ASSIGN 137
#define DIV_ASSIGN 138
#define GOTO 139
#define CONTINUE 140
#define AUTO 141
#define AND_OP 142
#define MOD_ASSIGN 143
#define RESTRICT 144
#define TYPE_NAME 145
#define INC_OP 146
#define STATIC 147
#define RETURN 148
#define UNSIGNED 149
#define STRING_LITERAL 150
#define SIGNED 151
#define SWITCH 152
#define OR_OP 153
#define CASE 154
#define SIZEOF 155
#define ELSE 156
#define IDENTIFIER 157
#define IMAGINARY 158
#define INLINE 159
#define REGISTER 160
#define OR_ASSIGN 161
#define CONSTANT 162
#define DO 163
#define TYPEDEF 164
#define VOID 165
#define UNION 166
#define LEFT_OP 167
#define LE_OP 168
#define GE_OP 169
#define PTR_OP 170
#define DEFAULT 171
#define DEC_OP 172
#define NE_OP 173
#define ADD_ASSIGN 174
#define MUL_ASSIGN 175
#define LONG 176
#define DOUBLE 177
#define VOLATILE 178
#define ELLIPSIS 179
#define BOOL 180
#define COMPLEX 181
#define STRUCT 182
#define CHAR 183
#define ENUM 184
#define SUB_ASSIGN 185
#define IF 186
#define FOR 187
#define BREAK 188
#define CONST 189
#define LEFT_ASSIGN 190


#define function_definition 191
#define translation_unit 192
#define jump_statement 193
#define selection_statement 194
#define expression_statement 195
#define compound_statement 196
#define labeled_statement 197
#define designator 198
#define designator_list 199
#define designation 200
#define initializer_list 201
#define direct_abstract_declarator 202
#define identifier_list 203
#define parameter_declaration 204
#define parameter_list 205
#define parameter_type_list 206
#define type_qualifier_list 207
#define declaration_list 208
#define pointer 209
#define direct_declarator 210
#define declarator 211
#define block_item 212
#define enumerator 213
#define block_item_list 214
#define enumerator_list 215
#define exclusive_or_expression 216
#define declaration_specifiers 217
#define relational_expression 218
#define specifier_qualifier_list 219
#define multiplicative_expression 220
#define type_specifier 221
#define iteration_statement 222
#define struct_declarator 223
#define constant_expression 224
#define logical_and_expression 225
#define struct_declarator_list 226
#define primary_expression 227
#define additive_expression 228
#define argument_expression_list 229
#define shift_expression 230
#define and_expression 231
#define equality_expression 232
#define declaration 233
#define unary_operator 234
#define cast_expression 235
#define start 236
#define unary_expression 237
#define external_declaration 238
#define conditional_expression 239
#define struct_or_union_specifier 240
#define initializer 241
#define inclusive_or_expression 242
#define assignment_expression 243
#define function_specifier 244
#define expression 245
#define assignment_operator 246
#define init_declarator_list 247
#define abstract_declarator 248
#define struct_declaration_list 249
#define statement 250
#define type_qualifier 251
#define enum_specifier 252
#define init_declarator 253
#define struct_or_union 254
#define logical_or_expression 255
#define storage_class_specifier 256
#define type_name 257
#define postfix_expression 258
#define struct_declaration 259
#define acc 159

#define NTerminalBase 191
#define isTerminal(x) (x<191)
char *I2S[260]={
"\\0",
"\\1",
"\\2",
"\\3",
"\\4",
"\\5",
"\\6",
"\\7",
"\\8",
"\\9",
"\\10",
"\\11",
"\\12",
"\\13",
"\\14",
"\\15",
"\\16",
"\\17",
"\\18",
"\\19",
"\\20",
"\\21",
"\\22",
"\\23",
"\\24",
"\\25",
"\\26",
"\\27",
"\\28",
"\\29",
"",
"",
" ",
"!",
"\\",
"#",
"$",
"%",
"&",
"'",
"(",
")",
"*",
"+",
",",
"-",
".",
"/",
"0",
"1",
"2",
"3",
"4",
"5",
"6",
"7",
"8",
"9",
":",
";",
"<",
"=",
">",
"?",
"@",
"A",
"B",
"C",
"D",
"E",
"F",
"G",
"H",
"I",
"J",
"K",
"L",
"M",
"N",
"O",
"P",
"Q",
"R",
"S",
"T",
"U",
"V",
"W",
"X",
"Y",
"Z",
"[",
"\\",
"]",
"^",
"_",
"`",
"a",
"b",
"c",
"d",
"e",
"f",
"g",
"h",
"i",
"j",
"k",
"l",
"m",
"n",
"o",
"p",
"q",
"r",
"s",
"t",
"u",
"v",
"w",
"x",
"y",
"z",
"{",
"|",
"}",
"~",
"",
"INT",
"EQ_OP",
"SHORT",
"EXTERN",
"RIGHT_OP",
"XOR_ASSIGN",
"AND_ASSIGN",
"FLOAT",
"WHILE",
"RIGHT_ASSIGN",
"DIV_ASSIGN",
"GOTO",
"CONTINUE",
"AUTO",
"AND_OP",
"MOD_ASSIGN",
"RESTRICT",
"TYPE_NAME",
"INC_OP",
"STATIC",
"RETURN",
"UNSIGNED",
"STRING_LITERAL",
"SIGNED",
"SWITCH",
"OR_OP",
"CASE",
"SIZEOF",
"ELSE",
"IDENTIFIER",
"IMAGINARY",
"INLINE",
"REGISTER",
"OR_ASSIGN",
"CONSTANT",
"DO",
"TYPEDEF",
"VOID",
"UNION",
"LEFT_OP",
"LE_OP",
"GE_OP",
"PTR_OP",
"DEFAULT",
"DEC_OP",
"NE_OP",
"ADD_ASSIGN",
"MUL_ASSIGN",
"LONG",
"DOUBLE",
"VOLATILE",
"ELLIPSIS",
"BOOL",
"COMPLEX",
"STRUCT",
"CHAR",
"ENUM",
"SUB_ASSIGN",
"IF",
"FOR",
"BREAK",
"CONST",
"LEFT_ASSIGN",
"function_definition",
"translation_unit",
"jump_statement",
"selection_statement",
"expression_statement",
"compound_statement",
"labeled_statement",
"designator",
"designator_list",
"designation",
"initializer_list",
"direct_abstract_declarator",
"identifier_list",
"parameter_declaration",
"parameter_list",
"parameter_type_list",
"type_qualifier_list",
"declaration_list",
"pointer",
"direct_declarator",
"declarator",
"block_item",
"enumerator",
"block_item_list",
"enumerator_list",
"exclusive_or_expression",
"declaration_specifiers",
"relational_expression",
"specifier_qualifier_list",
"multiplicative_expression",
"type_specifier",
"iteration_statement",
"struct_declarator",
"constant_expression",
"logical_and_expression",
"struct_declarator_list",
"primary_expression",
"additive_expression",
"argument_expression_list",
"shift_expression",
"and_expression",
"equality_expression",
"declaration",
"unary_operator",
"cast_expression",
"start",
"unary_expression",
"external_declaration",
"conditional_expression",
"struct_or_union_specifier",
"initializer",
"inclusive_or_expression",
"assignment_expression",
"function_specifier",
"expression",
"assignment_operator",
"init_declarator_list",
"abstract_declarator",
"struct_declaration_list",
"statement",
"type_qualifier",
"enum_specifier",
"init_declarator",
"struct_or_union",
"logical_or_expression",
"storage_class_specifier",
"type_name",
"postfix_expression",
"struct_declaration"};
int proCnt[238]={
1,1,1,1,3,1,4,3,4,3,3,2,2,6,7,1,3,1,2,2,2,2,4,1,1,1,1,1,1,1,4,1,3,3,3,1,3,3,1,3,3,1,3,3,3,3,1,3,3,1,3,1,3,1,3,1,3,1,3,1,5,1,3,1,1,1,1,1,1,1,1,1,1,1,1,3,1,2,3,1,2,1,2,1,2,1,2,1,3,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,4,2,1,1,1,2,3,2,1,2,1,1,3,1,2,3,4,5,5,6,2,1,3,1,3,1,1,1,1,2,1,1,3,5,4,4,6,6,5,4,3,4,4,3,1,2,2,3,1,2,1,3,1,3,2,2,1,1,3,1,2,1,1,2,3,2,3,3,4,3,4,2,3,3,4,1,3,4,1,2,3,4,2,1,2,3,2,1,1,1,1,1,1,3,4,3,2,3,1,2,1,1,1,2,5,7,5,5,7,6,7,6,7,3,2,2,2,3,1,2,1,1,4,3,1,2};
int proGet[238]={
236,227,227,227,227,258,258,258,258,258,258,258,258,258,258,229,229,237,237,237,237,237,237,234,234,234,234,234,234,235,235,220,220,220,220,228,228,228,230,230,230,218,218,218,218,218,232,232,232,231,231,216,216,242,242,225,225,255,255,239,239,243,243,246,246,246,246,246,246,246,246,246,246,246,245,245,224,233,233,217,217,217,217,217,217,217,217,247,247,253,253,256,256,256,256,256,221,221,221,221,221,221,221,221,221,221,221,221,221,221,221,240,240,240,254,254,249,249,259,219,219,219,219,226,226,223,223,223,252,252,252,252,252,215,215,213,213,251,251,251,244,211,211,210,210,210,210,210,210,210,210,210,210,210,210,210,209,209,209,209,207,207,206,206,205,205,204,204,204,203,203,257,257,248,248,248,202,202,202,202,202,202,202,202,202,202,202,241,241,241,201,201,201,201,200,199,199,198,198,250,250,250,250,250,250,197,197,197,196,196,214,214,212,212,195,195,194,194,194,222,222,222,222,222,222,193,193,193,193,193,192,192,238,238,191,191,208,208};
#endif
