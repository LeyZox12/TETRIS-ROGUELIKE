CXXFLAGS = -std=c++17 -g -I"../../../SFML-3.0.0/include" -Iinclude -I"../../class"
POINTENGINE = "../../class/PointEngine"

final : main.o Card.o UIutils.o PointEngine.o Point.o Rectangle.o PhysicConstraint.o
	g++ $(CXXFLAGS) Card.o UIutils.o PointEngine.o Point.o Rectangle.o PhysicConstraint.o main.o -o Tetris -L"../../../SFML-3.0.0/lib" -lsfml-graphics-d -lsfml-window-d -lsfml-system-d

main.o : main.cpp
	g++ $(CXXFLAGS) -c main.cpp 

Card.o : src/Card.cpp
	g++ $(CXXFLAGS) -c src/Card.cpp -Iinclude/Card.h

UIutils.o : ../../class/UIutils.cpp
	g++ $(CXXFLAGS) -c "../../class/UIutils.cpp" 

PointEngine.o : ../../class/PointEngine/src/PointEngine.cpp
	g++ $(CXXFLAGS) -c  $(POINTENGINE)/src/PointEngine.cpp -I"../../class/PointEngine/src/PointEngine.h"

Point.o : ../../class/PointEngine/src/Point.cpp
	g++ $(CXXFLAGS) -c $(POINTENGINE)/src/Point.cpp

Rectangle.o : ../../class/PointEngine/src/Rectangle.cpp
	g++ $(CXXFLAGS) -c $(POINTENGINE)/src/Rectangle.cpp

PhysicConstraint.o : ../../class/PointEngine/src/PhysicConstraint.cpp
	g++ $(CXXFLAGS) -c $(POINTENGINE)/src/PhysicConstraint.cpp

clean : 
	del main.o Card.o UIutils.o PointEngine.o PhysicConstraint.o Rectangle.o cards.o Point.o
