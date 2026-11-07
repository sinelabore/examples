JAVA = java

# CODEGEN_PATH is the folder with codegen.jar. It is needed only when the model
# changed: src/fsm/blinky.rs is checked in, so cargo builds without the generator.
JFLAGS = -cp "$(CODEGEN_PATH)/*" codegen.Main

MODEL = model/blinky.xml
MACHINE = src/fsm/blinky.rs
NEED_CODEGEN = @test -n "$(CODEGEN_PATH)" || { echo "Set CODEGEN_PATH to the folder with codegen.jar, e.g. make CODEGEN_PATH=~/sinelabore/bin"; exit 1; }

.PHONY: all test qemu flash edit clean

# host build of the library with the generated machine
all : $(MACHINE)
	cargo build

# regenerate the machine when the model or codegen.cfg is newer
$(MACHINE) : $(MODEL) codegen.cfg
	$(NEED_CODEGEN)
	$(JAVA) $(JFLAGS) -p ssc -l rust -o src/fsm/blinky $(MODEL)

test : $(MACHINE)
	cargo test

# run in the emulator (qemu-system-arm, target thumbv7m-none-eabi)
qemu : $(MACHINE)
	cd qemu && cargo run

# build and flash the NUCLEO-L432KC (probe-rs, target thumbv7em-none-eabihf)
flash : $(MACHINE)
	cd nucleo-l432kc && cargo run --release

# open the model in the editor; its generate button writes src/fsm/blinky.rs
edit :
	$(NEED_CODEGEN)
	$(JAVA) $(JFLAGS) -p ssc -E -l rust -o src/fsm/blinky $(MODEL)

clean :
	cargo clean
	cd qemu && cargo clean
	cd nucleo-l432kc && cargo clean
