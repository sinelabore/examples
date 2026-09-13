# The traffic light system, generated twice: once as C++ and once as Python.
#
# One model, one set of drawings, two targets. The model and the drawings live here
# because neither belongs to a language; each target's driver and Makefile live in its
# own directory, so its generated module cannot tread on the other's.
#
#   make            model drawings, then both targets
#   make svg        the drawings only
#   make cpp        the C++ target only    (cpp/testcase)
#   make python     the Python target only (python/system.py)
#   make run-cpp    build and run the C++ driver
#   make run-python generate and run the Python driver

JAVA=java
INPUT_SYSML_FILE=tl.sysml signalling.sysml

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

JFLAGS=-cp "$(CODEGEN_PATH)/*" codegen.Main

# One state chart per machine, the structure of the system, and one action drawn as an
# activity. Owned here rather than by either target: both would generate the same files,
# and whichever ran second would do the work again.
diagrams = tlcStateMachine.svg \
           tmcStateMachine.svg \
           TrafficLightSystem.svg \
           checkServiceCounter.svg

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

# -t names the part; the file is named after what is drawn in it.
tlcStateMachine.svg : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -t TrafficLight::BasicTrafficLightController -l svg -p sysml2text -o tlcStateMachine $(INPUT_SYSML_FILE)

tmcStateMachine.svg : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -t TrafficLight::TrafficManagementCenter -l svg -p sysml2text -o tmcStateMachine $(INPUT_SYSML_FILE)

# An action is drawn as its flow. 'action:' says which kind of element the name means,
# needed here because a part and an action could carry the same name; the name itself is
# the one the generator lists when -t names something the model does not contain.
checkServiceCounter.svg : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -t action:TrafficLightController::checkServiceCounter -l svg -p sysml2text -o checkServiceCounter $(INPUT_SYSML_FILE)

# The system has no state machine of its own, so -t on it draws what it is made of:
# the parts, their ports and what connects them. -pcd 1 keeps the detail on the three
# parts of the system and leaves the lamps inside them as plain boxes.
TrafficLightSystem.svg : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -t TrafficLight::TrafficLightSystem -l svg -p sysml2text -pcd 1 -o TrafficLightSystem $(INPUT_SYSML_FILE)

clean:
	$(MAKE) -C cpp clean
	$(MAKE) -C python clean
	rm -f $(diagrams)
