NAME = moisture_guess
GCC = g++
CFLAGS = -I./include -Wall
LFLAGS = -lpthread -lpistache

SRC = src/cJSON.c src/server.cpp src/watering_guess.cpp src/weather_fetcher.cpp src/pigproject_main.cpp
OBJ = $(addsuffix .o, $(basename $(SRC)))

all: $(NAME)

$(NAME): $(OBJ)
	$(GCC) $^ -o $(NAME) $(CFLAGS) $(LFLAGS)

%.o: %.cpp
	$(GCC) -c $< -o $@ $(CFLAGS) $(LFLAGS)

%.o: %.c
	gcc -c $< -o $@ $(CFLAGS) $(LFLAGS)

clean:
	rm -f $(OBJ)
	rm -f $(NAME)

