## Installation  (tested under Ubuntu 16.04)
In order to build grammar from `.g4` file, you need to install **ANTLR4** (an excerpt from the [site](http://www.antlr.org/)):
```
$ cd /usr/local/lib
$ wget http://www.antlr.org/download/antlr-4.6-complete.jar
$ export CLASSPATH=".:/usr/local/lib/antlr-4.6-complete.jar:$CLASSPATH"
$ alias antlr4='java -jar /usr/local/lib/antlr-4.6-complete.jar'
$ alias grun='java org.antlr.v4.gui.TestRig'
```

Then, to be able to run the grammar, install **Python3**.  
Also, install python requirements using this command:
```
pip3 install -r requirements.txt
```

## Building the grammar
This command produces `LLexer.py`:
```
antlr4 -Dlanguage=Python3 LLexer.g4
```

## Running the lexer
There is a script *main.py* that takes one filename as an argument. It reads the file and outputs all lexemes.
Example usage:
```
python3 main.py ./tests/positive/sample_prog.l
```
There is an optional key `--filter`: if it is present, comment lexemes are not shown.

Example output:
```
READ("read", 2, 0, 3)
IDENTIFIER("x", 2, 5, 5)
COLON(";", 2, 6, 6)

IF("if", 3, 0, 1)
IDENTIFIER("y", 3, 3, 3)
ADD("+", 3, 5, 5)
FLOATING("1", 3, 7, 7)
EQ("==", 3, 9, 10)
IDENTIFIER("x", 3, 12, 12)

THEN("then", 4, 0, 3)
WRITE("write", 4, 5, 9)
IDENTIFIER("y", 4, 11, 11)

ELSE("else", 5, 0, 3)
WRITE("write", 5, 5, 9)
IDENTIFIER("x", 5, 11, 11)
```

## Tests
All tests are manual.
There are two types of tests:
 - *positive* tests are used to check that all valid programs are lexed correctly. This includes some corner-cases as well as all possible keyword\operator usage.
 - *negative* tests are used to check that invalid programs are either not lexed or lexed arbitrarily. For example, a string `123var` cannot be an identifier, however, it can be lexed as `FLOATING, IDENTIFIER` which is ok.
