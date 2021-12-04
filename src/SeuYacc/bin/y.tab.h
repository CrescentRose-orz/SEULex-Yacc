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


class yyParser{

private:
    int token,now;
    stack<YYSTYPE> valStack;
    stack<int> status;
    stack<int> yySymbolStack;
    int read();
    void shift(int target);
    void reduce(int producer);
    int parse();
public:
    void yyParse();
	asmParser parser;
};



// nonterminals
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
#ifdef ISCPP
#endif
extern char const *I2S[];
extern int _GOTO[400][69];
extern int ACTION[400][191];
extern int proCnt[238];
extern int proGet[238];
extern char yytext[1024];
#endif
