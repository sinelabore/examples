CC=clang
JAVA=java
CFLAGS=-Wall -g -I. -I.. -Igenerated
JFLAGS= -Djava.awt.headless=true -cp "../../bin/*":"/Applications/astahcommunity/astahcommunity.app/Contents/Java/*"  codegen.Main

all:  vending_machine.c product_store_sm.c activity_change_algorithm.c

product_store_sm.c : ../vending_machine.asta
	$(JAVA) $(JFLAGS) -l cx -v -p ASTAH -o product_store_sm -t "machine:product_store_sm:sm"  ../vending_machine.asta

vending_machine.c : ../vending_machine.asta
	$(JAVA) $(JFLAGS) -l cx -v -p ASTAH -o vending_machine -t "machine:vending_machine:ui"  ../vending_machine.asta

activity_change_algorithm.c : ../vending_machine.asta
	$(JAVA) $(JFLAGS) -A -l cx -v -p ASTAH -o activity -t "machine:coin_store:change_algorithm"  ../vending_machine.asta


clean:
	rm -f *.h *.c
