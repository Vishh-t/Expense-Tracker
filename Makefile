CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGET = expense_tracker

SRCS = main.cpp \
       helpers.cpp \
       auth.cpp \
       expense_tracker.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q *.o $(TARGET).exe 2>nul || true
