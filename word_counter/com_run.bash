g++ -std=c++11 -mssse3 wc.h wc.cpp tests.cpp
if [ $? -eq 0 ]
then
	./a.out
fi