#ifndef Y_TAB_H
#define Y_TAB_H
#include<stack>
using std::stack;
extern FILE* yyin;
extern int yyEOF;

// epsilon?
#define TNBound 191

extern YYSTYPE yyVal;
extern FILE* yyin;
// terminals
#define RETURN 128
#define CONTINUE 129
#define GOTO 130
#define WHILE 131
#define IF 132
#define DEFAULT 133
#define ELLIPSIS 134
#define DO 135
#define STRUCT 136
#define RIGHT_ASSIGN 137
#define IMAGINARY 138
#define ADD_ASSIGN 139
#define MUL_ASSIGN 140
#define DIV_ASSIGN 141
#define ELSE 142
#define OR_OP 143
#define VOLATILE 144
#define NE_OP 145
#define UNION 146
#define LEFT_ASSIGN 147
#define AUTO 148
#define GE_OP 149
#define BREAK 150
#define EQ_OP 151
#define SIZEOF 152
#define PTR_OP 153
#define AND_OP 154
#define STRING_LITERAL 155
#define DEC_OP 156
#define EXTERN 157
#define RESTRICT 158
#define SUB_ASSIGN 159
#define INC_OP 160
#define LEFT_OP 161
#define LE_OP 162
#define INT 163
#define RIGHT_OP 164
#define MOD_ASSIGN 165
#define REGISTER 166
#define CONSTANT 167
#define SHORT 168
#define AND_ASSIGN 169
#define XOR_ASSIGN 170
#define DOUBLE 171
#define OR_ASSIGN 172
#define TYPE_NAME 173
#define CASE 174
#define STATIC 175
#define SWITCH 176
#define UNSIGNED 177
#define TYPEDEF 178
#define INLINE 179
#define VOID 180
#define ENUM 181
#define CONST 182
#define CHAR 183
#define LONG 184
#define SIGNED 185
#define FLOAT 186
#define IDENTIFIER 187
#define BOOL 188
#define FOR 189
#define COMPLEX 190


class yyParser{

private:
    int token,now;
    stack<YYSTYPE> valStack;
    stack<int> status;
    stack<int> yySymbolStack;
	stack<tokenPlace> placeStack;
    int read();
    void shift(int target);
    void reduce(int producer);
    int parse();
	int yyMeetError();
public:
    void yyParse();
	//asmParser parser;
};



// nonterminals
#define declaration_list 191
#define external_declaration 192
#define translation_unit 193
#define jump_statement 194
#define iteration_statement 195
#define selection_statement 196
#define block_item 197
#define compound_statement 198
#define statement 199
#define initializer 200
#define type_name 201
#define identifier_list 202
#define constant_expression 203
#define assignment_expression 204
#define struct_or_union_specifier 205
#define struct_declarator_list 206
#define designation 207
#define initializer_list 208
#define parameter_declaration 209
#define parameter_list 210
#define assignment_operator 211
#define argument_expression_list 212
#define function_definition 213
#define start 214
#define enumerator 215
#define conditional_expression 216
#define exclusive_or_expression 217
#define enumerator_list 218
#define expression_statement 219
#define inclusive_or_expression 220
#define cast_expression 221
#define relational_expression 222
#define labeled_statement 223
#define primary_expression 224
#define struct_declaration 225
#define unary_expression 226
#define unary_operator 227
#define expression 228
#define equality_expression 229
#define logical_or_expression 230
#define postfix_expression 231
#define additive_expression 232
#define logical_and_expression 233
#define init_declarator 234
#define shift_expression 235
#define multiplicative_expression 236
#define declaration 237
#define block_item_list 238
#define type_qualifier 239
#define declaration_specifiers 240
#define designator_list 241
#define storage_class_specifier 242
#define designator 243
#define type_specifier 244
#define and_expression 245
#define struct_or_union 246
#define init_declarator_list 247
#define struct_declaration_list 248
#define enum_specifier 249
#define specifier_qualifier_list 250
#define struct_declarator 251
#define direct_abstract_declarator 252
#define function_specifier 253
#define abstract_declarator 254
#define declarator 255
#define direct_declarator 256
#define pointer 257
#define type_qualifier_list 258
#define parameter_type_list 259
#define acc 1000000
#define NLBound 260

#define NTerminalBase 191
#define isTerminal(x) (x<191)
#ifdef ISCPP
#endif
extern char const *I2S[];
extern int _GOTO[400][69];
extern int ACTION[400][191];
extern int proCnt[238];
extern int proGet[238];
extern char yytext[1024];
#endif
