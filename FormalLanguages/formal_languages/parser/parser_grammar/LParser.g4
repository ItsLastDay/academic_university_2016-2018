parser grammar LParser;

options {tokenVocab=LLexer;}


program : statement EOF
 ;

statement
 : statement SEMICOLON statement 
 | simple_statement
 ;

simple_statement 
 : skip
 | assignment
 | write
 | read
 | while_stmt
 | if_stmt
 ;

compound_statement 
 : BEGIN statement SEMICOLON statement END
 | BEGIN simple_statement END
 ;
 

skip : SKIP_KW ;

assignment : identifier ASSIGN expression ;

write : WRITE OPEN_PAREN expression CLOSE_PAREN ;

read : READ OPEN_PAREN identifier CLOSE_PAREN ;

while_stmt : WHILE if_stmt_while_stmt_condition DO if_stmt_while_stmt_body ;

if_stmt : IF if_stmt_while_stmt_condition THEN if_stmt_while_stmt_body ELSE if_stmt_while_stmt_body ;

if_stmt_while_stmt_condition : OPEN_PAREN expression CLOSE_PAREN ;

if_stmt_while_stmt_body 
 : simple_statement
 | compound_statement
 ;


expression 
 : OPEN_PAREN expression CLOSE_PAREN            # parenExpr
 | expression (MUL | DIV | MOD) expression      # multDivModBinop
 | expression (ADD | SUB) expression            # plusMinusBinop
 | expression (GE | GEQ | LE | LEQ) expression  # greaterLessBinop
 | expression (EQ | NEQ) expression             # equalityBinop
 | expression AND expression                    # andBinop
 | expression OR expression                     # orBinop
 | identifier                                   # idExpr
 | value                                        # valueExpr
 ; 

value 
 : FLOATING 
 | TRUE
 | FALSE
 ; 

identifier : IDENTIFIER ; 

