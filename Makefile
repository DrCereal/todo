DEPS  = main.cc entry.cc file.cc

all:
    g++ $(DEPS) -o todo
