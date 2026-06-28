%{
    #include "pic.cc"
    extern "C" void yyerror(const char *s);
    extern int yylex(void);
	
    vector<ASTNode*>* program_statement_list = NULL; // List of all statements in the program
%}

%union{
    ASTNode* nodeptr;
    string* strptr;
    vector<ASTNode*>* nodeptrlistptr;
}

%token LET INT_VAL FLT_VAL TRUE_VAL FALSE_VAL STR_VAL STR_ID PRINT LT GT LTEQ GTEQ EQ NEQ AND OR NOT ADD_ASSIGN MINUS_ASSIGN MULT_ASSIGN DIV_ASSIGN POW_ASSIGN IF ELSE WHILE DO RETURN FN

%nonassoc IF_WO_ELSE
%nonassoc ELSE

%right '?' ':'
%left OR
%left AND
%left EQ NEQ
%left LT GT LTEQ GTEQ
%left '+' '-' 														// Left associative operators '+' and '-'
%left '*' '/'   													// Left associative operators '*' and '/'
%right '^' 														    // Right associative operator '^'
%right NOT 														    // Right associative unary not operator
%right Uminus 														// Right associative unary minus operator

%type <strptr> INT_VAL FLT_VAL STR_ID STR_VAL
%type <nodeptr> identifier expression object formal_function_parameter_list call_function_parameter_list function_definition function_call stmt compound_stmt
%type <nodeptrlistptr> stmt_list

%start program 														// Starting rule for the grammar
%%

/* GRAMMAR */

program
	:	stmt_list			                            { program_statement_list = $1; }  // A program consists of a list of statements
    |   %empty                                          { program_statement_list = new vector<ASTNode*>; } // Empty program
;

stmt_list
	:	stmt_list stmt									{ $1->push_back($2); $$ = $1; }
	|	stmt											{ $$ = new vector<ASTNode*>; $$->push_back($1); }
;

compound_stmt
    :	'{' stmt_list '}'							{ $$ = new ASTNode(COMPOUND_STATEMENT, NULL, $2); } // A compound statement is a list of statements enclosed in curly braces
;

stmt
	: LET identifier '=' object ';'                 { $$ = new ASTNode(DECLARATION_ASSIGNMENT, NULL, $2,  $4); }  
    | identifier '=' object ';'	     	    		{ $$ = new ASTNode(ASSIGNMENT, NULL, $1, $3); }
    | ';'                                           { $$ = new ASTNode(EMPTY, NULL); }

    | identifier ADD_ASSIGN expression ';'		    { $$ = new ASTNode(ASSIGNMENT, NULL, $1, new ASTNode(ARITHMETIC_OPERATOR, new string("PLUS"), $1, $3)); }
    | identifier MINUS_ASSIGN expression ';'		{ $$ = new ASTNode(ASSIGNMENT, NULL, $1, new ASTNode(ARITHMETIC_OPERATOR, new string("MINUS"), $1, $3)); }
    | identifier MULT_ASSIGN expression ';'		    { $$ = new ASTNode(ASSIGNMENT, NULL, $1, new ASTNode(ARITHMETIC_OPERATOR, new string("MULTIPLY"), $1, $3));}
    | identifier DIV_ASSIGN expression ';'		    { $$ = new ASTNode(ASSIGNMENT, NULL, $1, new ASTNode(ARITHMETIC_OPERATOR, new string("DIVIDE"), $1, $3));}
    | identifier POW_ASSIGN expression ';'			{ $$ = new ASTNode(ASSIGNMENT, NULL, $1, new ASTNode(ARITHMETIC_OPERATOR, new string("POWER"), $1, $3));}

    | PRINT '(' expression ')' ';'				    { $$ = new ASTNode(PRINT_STATEMENT, NULL, $3); }

    | compound_stmt 								{ $$ = $1; } // A compound statement is a list of statements enclosed in curly braces

    | IF '(' expression ')' stmt %prec IF_WO_ELSE	{ $$ = new ASTNode(IF_STATEMENT, NULL, new ASTNode(IF_CONDITION, NULL, $3), new ASTNode(IF_THEN_STATEMENT, NULL, $5)); }
    | IF '(' expression ')' stmt ELSE stmt 		    { $$ = new ASTNode(IF_STATEMENT, NULL, new ASTNode(IF_CONDITION, NULL, $3), new ASTNode(IF_THEN_STATEMENT, NULL, $5), new ASTNode(ELSE_STATEMENT, NULL, $7)); }
/* Like C++, I will be following standard rule for dangling else, ie else joins with the innermost if */

    | WHILE '(' expression ')' stmt				    { $$ = new ASTNode(WHILE_STATEMENT, NULL, new ASTNode(WHILE_CONDITION, NULL, $3), new ASTNode(WHILE_BODY, NULL, $5)); }
    | DO stmt WHILE '(' expression ')' ';'		    { $$ = new ASTNode(DO_WHILE_STATEMENT, NULL, new ASTNode(DO_WHILE_BODY, NULL, $2), new ASTNode(DO_WHILE_CONDITION, NULL, $5)); }

    | RETURN object ';'							    { $$ = new ASTNode(RETURN_STATEMENT, NULL, $2); }
    | object ';'								    { $$ = new ASTNode(RETURN_STATEMENT, NULL, $1); }
    | RETURN ';'                                    { $$ = new ASTNode(RETURN_STATEMENT, NULL); }
;

function_definition
    : FN '(' ')' compound_stmt		                            { $$ = new ASTNode(FUNCTION_DEFINITION, NULL, new ASTNode(FORMAL_FUNC_PARAM_LIST, NULL), $4); }
    | FN '(' formal_function_parameter_list ')' compound_stmt	{ $$ = new ASTNode(FUNCTION_DEFINITION, NULL, $3, $5); }
;

formal_function_parameter_list
    : formal_function_parameter_list ',' identifier	{ $1->addChild($3); $$ = $1; }
    | identifier									{ $$ = new ASTNode(FORMAL_FUNC_PARAM_LIST, NULL, $1); }
;

function_call
    : identifier '(' ')'							    { $$ = new ASTNode(FUNCTION_CALL, NULL, $1, new ASTNode(CALL_FUNC_PARAM_LIST, NULL)); }
    | identifier '(' call_function_parameter_list ')'	{ $$ = new ASTNode(FUNCTION_CALL, NULL, $1, $3); }
;

call_function_parameter_list
    : call_function_parameter_list ',' object	    { $1->addChild($3); $$ = $1; }
    | object									    { $$ = new ASTNode(CALL_FUNC_PARAM_LIST, NULL, $1); }
;

object
    : expression            			            { $$ = $1; }
    | function_definition                           { $$ = $1; }
;

expression
    : TRUE_VAL					   	                { $$ = new ASTNode(BOOLEAN, new string("true")); }
    | FALSE_VAL							            { $$ = new ASTNode(BOOLEAN, new string("false")); }
    | expression AND expression		                { $$ = new ASTNode(BOOLEAN_OPERATOR, new string("AND"), $1, $3); }
    | expression OR expression		                { $$ = new ASTNode(BOOLEAN_OPERATOR, new string("OR"), $1, $3); }
    | NOT expression %prec NOT			            { $$ = new ASTNode(BOOLEAN_OPERATOR, new string("NOT"), $2); }
/* These 3 are supposed to be exclusive operators for booleans */
    | expression LT expression			            { $$ = new ASTNode(COMPARISION_OPERATOR, new string("LT"), $1, $3); }
    | expression GT expression			            { $$ = new ASTNode(COMPARISION_OPERATOR, new string("GT"), $1, $3); }
    | expression EQ expression			            { $$ = new ASTNode(COMPARISION_OPERATOR, new string("EQ"), $1, $3); }
    | expression NEQ expression			            { $$ = new ASTNode(COMPARISION_OPERATOR, new string("NEQ"), $1, $3); }
    | expression LTEQ expression		            { $$ = new ASTNode(COMPARISION_OPERATOR, new string("LTEQ"), $1, $3); }
    | expression GTEQ expression		            { $$ = new ASTNode(COMPARISION_OPERATOR, new string("GTEQ"), $1, $3); }
/* These 6 are supposed to only compare 2 numbers (ints or floats) */
    | INT_VAL									    { $$ = new ASTNode(INT, $1);}
	| FLT_VAL									    { $$ = new ASTNode(FLOAT, $1); }
	| expression '+' expression		                { $$ = new ASTNode(ARITHMETIC_OPERATOR, new string("PLUS"), $1, $3); }	
    | expression '-' expression		                { $$ = new ASTNode(ARITHMETIC_OPERATOR, new string("MINUS"), $1, $3); }
    | expression '*' expression		                { $$ = new ASTNode(ARITHMETIC_OPERATOR, new string("MULTIPLY"), $1, $3); }
    | expression '/' expression		                { $$ = new ASTNode(ARITHMETIC_OPERATOR, new string("DIVIDE"), $1, $3); }
    | expression '^' expression		                { $$ = new ASTNode(ARITHMETIC_OPERATOR, new string("POWER"), $1, $3); }
	| '-' expression %prec Uminus		            { $$ = new ASTNode(ARITHMETIC_OPERATOR, new string ("UMINUS"), $2 ); }
/* The above 6 have 4 exclusive operators for numbers with + also valid between 2 strings and * between a string and a int */
    | expression '?' expression ':' expression	    { $$ = new ASTNode(TERNARY_OPERATOR, NULL, $1, $3, $5); }
/* This takes a boolean at first position and a numerical expression in 2nd and 3rd position */
    | '(' expression ')'					        { $$ = $2; }
    | identifier                                    { $$ = $1; }
    | function_call                                 { $$ = $1; }
/* These 2 can hold either boolean, string or numbers */
    | STR_VAL                                       { $$ = new ASTNode(STRING, $1); }
;

identifier 
    : STR_ID                                        { $$ = new ASTNode(IDENTIFIER, $1); }
/* Can hold any object, including functions and expressions */
;

%%

/* ADDITIONAL C CODE */

int main() { 
    yyparse();

    Scope* global_scope = new Scope();
    Object* obj = NULL;
    for (auto stmt : *program_statement_list) {
        executeAST(stmt, global_scope, obj);
    }

  return 0;
}