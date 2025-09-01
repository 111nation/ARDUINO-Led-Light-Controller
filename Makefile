CFLAGS=-std=c++14 -Wall
RM = del
CXX = g++
LIB = ./Lib/debug.hpp ./Lib/display.hpp ./Lib/serial.hpp ./Lib/PaintLib/font.h ./Lib/PaintLib/paint.h
OBJ = main.o serial.o ./Lib/PaintLib/font.o ./Lib/PaintLib/paint.o # Exclude debug.o
BIN = LED

all: $(BIN) 
	@./$(BIN)

$(BIN): $(OBJ)
	@$(CXX) $(OBJ) -o $(BIN) -lgdi32 -luser32

paint.o: $(LIB) ./Lib/PaintLib/paint.cpp
	@$(CXX) $(CFLAGS) -c ./Lib/PaintLib/paint.cpp

font.o: $(LIB) ./Lib/PaintLib/font.cpp
	@$(CXX) $(CFLAGS) -c ./Lib/PaintLib/font.cpp

serial.o: $(LIB) serial.cpp
	@$(CXX) $(CFLAGS) -c serial.cpp

main.o: $(LIB) main.cpp
	@$(CXX) $(CFLAGS) -c main.cpp

clean: 
	@$(DEL) *.o
	@$(DEL) ./$(BIN)
