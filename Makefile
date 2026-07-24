CC = g++

CFLAGS = -Wall -g $(shell pkg-config --cflags realsense2)
LDFLAGS = $(shell pkg-config --libs realsense2)

TARGET = stan-tare-calib 
SRC = stan-tare-calib.cpp

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)
