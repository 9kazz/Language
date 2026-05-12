.PHONY: all clean

all:
	cd Frontend && $(MAKE) && ./program
	cd Backend && $(MAKE) && ./program
	cd Virtual_machine && $(MAKE) run

reverse:
	cd Frontend && $(MAKE) && ./program
	cd Reversend && $(MAKE) && ./program

clean:
	$(MAKE) -C Frontend clean
	$(MAKE) -C Backend clean
	$(MAKE) -C Virtual_machine clean