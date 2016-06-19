.PHONY: all clean

PROGS=feature

all:	$(PROGS)

$(PROGS): %: %.C
	c++ $< -o $@

clean:
	rm -f $(PROGS)
