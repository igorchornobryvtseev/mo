# a: main.cpp
# 	g++ -o a.out main.cpp

a: main.cpp MoPipeMode.cpp Database.cpp Mo.cpp
	g++ -o a.out main.cpp MoPipeMode.cpp Database.cpp Mo.cpp
