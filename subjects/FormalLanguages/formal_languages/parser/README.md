## Installation  (tested under Ubuntu 16.04)
In order to build grammar from `.g4` file, you need to install **ANTLR4** (an excerpt from the [site](http://www.antlr.org/)):
```
$ cd /usr/local/lib
$ wget http://www.antlr.org/download/antlr-4.7-complete.jar
$ export CLASSPATH=".:/usr/local/lib/antlr-4.7-complete.jar:$CLASSPATH"
$ alias antlr4='java -jar /usr/local/lib/antlr-4.7-complete.jar'
$ alias grun='java org.antlr.v4.gui.TestRig'
```

Then, to be able to run the grammar, install **Python3**.  
Also, install python requirements using this command:
```
pip3 install -r requirements.txt
```

## Building the grammar
This command produces `LLexer.py`, `LParser.py` and others `LParserListener.py` (as well as some other files):
```
cd ./parser_grammar
antlr4 -Dlanguage=Python3 LLexer.g4 LParser.g4

```

## Running the lexer
There is a script *main.py* that takes one filename as an argument. It reads the file and parses it.
Example usage:
```
python3 main.py ./tests/positive/sample_prog.l
```


## Tests
All tests are manual.
There are two types of tests:
 - *positive* tests are used to check that all valid programs are parsed correctly; 
 - *negative* tests are used to check that invalid programs are not parser. 
