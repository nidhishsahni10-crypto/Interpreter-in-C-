%{
    #include "pic.hh"
    #include "parse.tab.h"
    using namespace std;
%}

/* DEFINITIONS */

/* yywrap handles the end-of-input logic, if noyywrap is used, program exits on end-of-file (EOF) */
%option noyywrap

digit                       [0-9]
fraction                    ([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)
ws                          [ \t\n]*
letter                      [a-zA-Z]
letter_                     [a-zA-Z_]

/* RULES */

%%

let                          { return LET; }
fn                           { return FN; }
return                       { return RETURN; }

print                        { return PRINT; }

true                         { return TRUE_VAL; }
false                        { return FALSE_VAL; }

if                           { return IF; }
else                         { return ELSE; }
while                        { return WHILE; }
do                           { return DO; }

;                            { return ';'; }
,                            { return ','; }
:                            { return ':'; }
\?                            { return '?'; }

\+                           { return '+'; }
\-                           { return '-'; }
\*                           { return '*'; }
\/                           { return '/'; }
\^                           { return '^'; }  /* I am using ^ for power, if it is inconsistent with your testcases, do tell */

=                            { return '='; }
\+=                          { return ADD_ASSIGN; }
\-=                          { return MINUS_ASSIGN; }
\*=                          { return MULT_ASSIGN; }
\/=                          { return DIV_ASSIGN; }
\^=                          { return POW_ASSIGN; }

\<                           { return LT; }
\>                           { return GT; }
==                           { return EQ; }
\<=                          { return LTEQ; }
\>=                          { return GTEQ; }
!=                           { return NEQ; }

&&                           { return AND; }
\|\|                         { return OR; }
!                            { return NOT; }

\(                           { return '('; }
\)                           { return ')'; }
\{                           { return '{'; }
\}                           { return '}'; }
\[                           { return '['; }
\]                           { return ']'; }

{digit}+                    { yylval.strptr = new string(yytext); return INT_VAL; }
{fraction}                  { yylval.strptr = new string(yytext); return FLT_VAL; }

{letter_}({letter_}|{digit})* { yylval.strptr = new string(yytext); return STR_ID; }
\"([^\n\"]*)\"                { yylval.strptr = new string(yytext + 1, strlen(yytext) - 2); return STR_VAL; }

{ws}                         ; /* Ignore spaces */
"//"[^\n]*\n                 ; /* Ignore comments */

.                            { fprintf(stderr, "unknown_token: %d\n", yytext[0]); exit(1); }

%%