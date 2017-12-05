NAME = moisture_guess
GCC = gcc
CFLAGS = -Wall -I./include
LFLAGS = 

SRC = src/main.c src/cJSON.c
OBJ = $(addsuffix .o, $(basename $(SRC)))

all: $(NAME)

$(NAME): $(OBJ)
	$(GCC) $^ -o $(NAME) $(CFLAGS) $(LFLAGS)

src/%.o: %.c
	$(GCC) $< -c $(CFLAGS) $(LFLAGS)

clean:
	rm -f $(OBJ)
	rm -f $(NAME)

