SOURCES := $(wildcard *.cpp)
HEADERS := $(wildcard *.h)
OBJECTS := $(SOURCES:.c=.o)

default: emulator


emulator: $(OBJECTS) $(SOURCES) $(HEADERS)
	clang++ $(OBJECTS) -std=c++11 -o $@

clean:
	rm emulator