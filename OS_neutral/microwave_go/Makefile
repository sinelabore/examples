JAVA=java

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

JFLAGS= -cp "$(CODEGEN_PATH)/*" codegen.Main

generated : oven.go oven.xml reactor.go
	go build -tags debug -o oven

oven.go : oven.xml
	$(JAVA) $(JFLAGS) -Trace -p ssc -l go -o oven  oven.xml

edit:
	$(JAVA) $(JFLAGS) -Trace -p ssc -E -l go -o oven  oven.xml

clean:
	go clean
	rm -rf oven
	touch oven.xml