CXX=g++
CXXFLAGS=$(INCLUDES) -pedantic-errors -Wall -Wextra -O0 -g3 
INCS=
INCLUDES=$(patsubst %, -I%, $(INCS))
TARGETS=ContactsParser base64
OBJS=$(patsubst %, %.o, $(TARGETS))
SHLIBS=

ContactsParser.a: $(OBJS)
	ar cr $@ $^ 

test: ContactsParser_test.cpp ContactsParser.a 
	g++ -g3 $^  $(INCLUDES) $(SHLIBS)

clean:
	-rm *.o *.out *.so

.PHONY: clean test 
