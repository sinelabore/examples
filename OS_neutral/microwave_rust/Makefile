JAVA=java

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

JFLAGS= -cp "$(CODEGEN_PATH)/*" codegen.Main

oven : src/oven.rs src/main.rs src/cooking_timer.rs src/magnetron.rs
	cargo build

src/oven.rs : oven.xml codegen.cfg
	$(JAVA) $(JFLAGS) -p ssc -l rust -o src/oven oven.xml

run : oven
	cargo run

edit:
	$(JAVA) $(JFLAGS) -p ssc -E -l rust -o src/oven oven.xml

clean:
	cargo clean
	touch oven.xml
