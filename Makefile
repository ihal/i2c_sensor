# Makefile for i2c_sensor.cpp
i2c_sensor: i2c_sensor.cpp
	g++ -Wall -O2 -o a.out i2c_sensor.cpp -lwiringPi -pthread -std=c++0x 

