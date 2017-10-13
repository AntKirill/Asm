g++ -std=c++11 -mssse3 -O2 world_count.h world_count.cpp main.cpp
if [ $? -eq 0 ]
then
	./a.out
fi