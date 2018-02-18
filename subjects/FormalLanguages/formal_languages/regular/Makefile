DOT_FILES=$(wildcard *.dot)
DOTPNG=$(DOT_FILES:.dot=.png)

%.png: %.dot
	dot -Tpng $< > $@

.PHONY: dot
dot: $(DOTPNG)
