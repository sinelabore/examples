# The microwave oven, generated twice: once as C++ and once as Python.
#
# One model, one set of drawings, two targets. The model and the drawings live here
# because neither belongs to a language; each target's driver and Makefile live in its
# own directory, so its generated module cannot tread on the other's.
#
#   make            model drawings, then both targets
#   make svg        the drawings only
#   make cpp        the C++ target only   (cpp/oven)
#   make python     the Python target only (python/system.py)
#   make run-cpp    build and run the C++ driver
#   make run-python build and run the Python driver

JAVA=java
INPUT_SYSML_FILE=oven.sysml

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

JFLAGS=-cp "$(CODEGEN_PATH)/*" codegen.Main

# One state chart per machine in the model, and the structure of the oven. Owned here
# rather than by either target: both would generate the same files, and whichever ran
# second would do the work again.
diagrams = MicrowaveOfen.svg \
           ControllerStateMachine.svg \
           TimerDisplayStateMachine.svg \
           DoorContactStateMachine.svg

.PHONY: all svg cpp python run-cpp run-python clean

all : svg cpp python

svg : $(diagrams)

cpp :
	$(MAKE) -C cpp

python :
	$(MAKE) -C python

run-cpp :
	$(MAKE) -C cpp run

run-python :
	$(MAKE) -C python run

# -t names the part; the file is named after what is drawn in it. Oven has no state
# machine of its own, so -t on it draws what it is made of: the parts, their ports and
# what connects them.
MicrowaveOfen.svg : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -t MicrowaveOfen::Oven -pcd 2 -pc none -l svg -p sysml2text -o MicrowaveOfen $(INPUT_SYSML_FILE)

# A part that does have one is drawn as that machine. 'codegen.Main -t <wrong name>'
# lists what this model offers, which is where these come from.
ControllerStateMachine.svg : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -t MicrowaveOfen::Controller -l svg -p sysml2text -o ControllerStateMachine $(INPUT_SYSML_FILE)

TimerDisplayStateMachine.svg : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -t MicrowaveOfen::TimerDisplay -l svg -p sysml2text -o TimerDisplayStateMachine $(INPUT_SYSML_FILE)

DoorContactStateMachine.svg : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -t MicrowaveOfen::DoorContact -l svg -p sysml2text -o DoorContactStateMachine $(INPUT_SYSML_FILE)

clean:
	$(MAKE) -C cpp clean
	$(MAKE) -C python clean
	rm -f $(diagrams)
