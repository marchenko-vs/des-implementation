CC := gcc
CFLAGS := -Wall -Werror -std=c17
SRC_DIR := src
INC_DIR := inc
OBJ_DIR := out

app.exe : $(OBJ_DIR) $(OBJ_DIR)/main.o $(OBJ_DIR)/des.o
	$(CC) $(OBJ_DIR)/*.o -o app.exe

$(OBJ_DIR) :
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/main.o : $(SRC_DIR)/main.c $(INC_DIR)/des.h
	$(CC) $(CFLAGS) -I $(INC_DIR) $(SRC_DIR)/main.c -c -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/des.o : $(SRC_DIR)/des.c $(INC_DIR)/des.h
	$(CC) $(CFLAGS) -I $(INC_DIR) $(SRC_DIR)/des.c -c -o $(OBJ_DIR)/des.o

.PHONY : clean

clean :
	rm -rf $(OBJ_DIR) app.exe *.bin
