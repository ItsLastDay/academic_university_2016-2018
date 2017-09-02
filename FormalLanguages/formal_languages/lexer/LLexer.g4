lexer grammar LLexer;

@lexer::members
{ 
    self.nested_multiline_comment_count = 0 
}

/// This grammar is influenced by
/// https://github.com/antlr/grammars-v4/blob/master/python3/Python3.g4
/// https://github.com/antlr/antlr4/blob/master/doc/lexer-rules.md
/// https://github.com/antlr/antlr4/blob/master/doc/grammars.md


IF : 'if';
THEN : 'then';
ELSE : 'else';
WHILE : 'while';
DO : 'do';
READ : 'read';
WRITE : 'write';
BEGIN : 'begin';
END : 'end';
TRUE : 'true';
FALSE : 'false';


IDENTIFIER : ID_START ID_CONT*
 ;

FLOATING
 : '-'? NUMBER ('.' NUMBER_ZERO EXPONENT?)?
 | '-'? NUMBER EXPONENT?
 | '-'? '.' NUMBER_ZERO EXPONENT?
 ;


OPEN_PAREN : '(';
CLOSE_PAREN : ')';
COLON : ';';
ASSIGN : ':=';
ADD : '+';
SUB : '-';
POW : '**';
MUL : '*';
DIV : '/';
MOD : '%';
EQ : '==';
NEQ : '!=';
GE : '>';
GEQ : '>=';
LE : '<';
LEQ : '<=';
AND : '&&';
OR : '||';


COMMENT : COMMENT_  -> channel(HIDDEN)
 ;

MULTILINE_COMMENT : '(*' { self.nested_multiline_comment_count = 1 } -> more, pushMode(INSIDE_COMMENT)
 ;


mode INSIDE_COMMENT;

CLOSE : '*)' { self.nested_multiline_comment_count == 1 }? { self.nested_multiline_comment_count -= 1 } 
 -> popMode, channel(HIDDEN) 
 ;
CLOSE_INTERIM : '*)' { self.nested_multiline_comment_count > 1 }? { self.nested_multiline_comment_count -= 1 } -> more
 ;
OPEN_NEW : '(*' { self.nested_multiline_comment_count += 1 } -> more
 ;
TEXT : . -> more;

mode DEFAULT_MODE;


SKIP_ : (SPACE | LINEBREAK) -> skip 
 ;


fragment
NUMBER 
 : '0'
 | DIGIT_ONE DIGIT*
 ;


fragment 
EXPONENT : EXP SIGN? NUMBER
 ;


fragment
NUMBER_ZERO 
 : NUMBER
 | '0' DIGIT+
 ;


fragment
SIGN : [-+];


fragment
EXP : 'e';


fragment
DIGIT : [0-9];


fragment
DIGIT_ONE : [1-9];


fragment
SPACE : [ \t\f] 
 ;

fragment
LINEBREAK : '\r' | '\n' | '\r\n' 
 ;

/// This cannot link with the above definition LINEBREAK:
/// https://github.com/antlr/antlr4/issues/70
fragment 
COMMENT_ : '//' ~[\r\n]*
 ;


fragment 
ID_START 
 : '_'
 | [a-z]
 | [A-Z]
 ;


fragment 
ID_CONT
 : ID_START
 | [0-9]
 ;

