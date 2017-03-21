CXX = g++
TARGET = i2c_sensor
CXXFLAGS = -Wall -O2 -pthread -std=c++0x -pipe
SRCS = i2c_logger.cpp dev_hdc1000.cpp dev_ms5607.cpp dev_mpl115a2.cpp sys_governor.cpp sys_display.cpp sys_log.cpp sys_postgres.cpp
OBJS := $(SRCS:.cpp=.o)
LIBS = -lwiringPi -lpqxx -lpq
$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBS)

clean:
	rm -f $(TARGET) $(OBJS)
