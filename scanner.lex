%{
#include <stdio.h>
#include <stdlib.h>
#include "semantics.hpp"
#include "parser.tab.hpp"
#include "hw3_output.hpp"
%}

%option yylineno
%option noyywrap

whitespace ([\t\n\r ])

%%

void	{yylval = new Node("", "VOID"); return VOID;}
int		{yylval = new Node("", "INT"); return INT;}
byte 	{yylval = new Node("", "BYTE"); return BYTE;}
b 		return B;
bool 	{yylval = new Node("", "BOOL"); return BOOL;}
enum	{yylval = new Node("","ENUM"); return ENUM;}
and 	return AND;
or 		return OR;
not 	return NOT;
true	{yylval = new Node("", "BOOL"); return TRUE;}
false	{yylval = new Node("", "BOOL"); return FALSE;}
return 	return RETURN;
if 		return IF;
else 	return ELSE;
while 	return WHILE;
break 	return BREAK;
continue return CONTINUE;
; 		return SC;
, 	return COMMA;
\( 	return LPAREN;
\) 	return RPAREN;
\{ 	return LBRACE;
\} 	return RBRACE;
= 	return ASSIGN;
\=\=|\!\=       {yylval = new BoolOp(yytext); return EQOP;}
\<|\>|\<\=|\>\=	{yylval = new BoolOp(yytext); return CMPOP;}
\+      {yylval = new BinaryOp(yytext); return ADDSUBOP;}
\-		{yylval = new BinaryOp(yytext); return ADDSUBOP;}	
\*		{yylval = new BinaryOp(yytext); return MULDIVOP;}
\/		{yylval = new BinaryOp(yytext); return MULDIVOP;}
[a-zA-Z][a-zA-Z0-9]* 	{yylval = new Node(yytext, ""); return ID;}
0|[1-9][0-9]*		{yylval = new Num(yytext); return NUM;}
\"([^\n\r\"\\]|\\[rnt"\\])+\" 	{yylval = new Node("","STRING"); return STRING;}
{whitespace}     ;
<<EOF>>                     return END_OF_FILE;
\/\/[^\r\n]*[\r|\n|\r\n]?		;
.       { output::errorLex(yylineno); exit(0); }

%%