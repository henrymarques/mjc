# compilador 
CPP=g++
ARGS=-std=c++17 -Wall

# dependências
all:
	$(CPP) src/*.cpp $(ARGS) -o mj_compiler
