EXE_DIR := ./bin
INC_DIR := ./include
SRC_DIR := ./src

OBJECTS := ./src/pool_main.c ./src/pool_threads.c ./src/pool_que.c

INCLUDES := ./include/pool_que.h ./include/pool_threads.h ./include/pool_common.h

$(EXE_DIR)/server:$(OBJECTS) $(INCLUDES)
	gcc -g -o $@ $(OBJECTS) -I$(INC_DIR) -lpthread
