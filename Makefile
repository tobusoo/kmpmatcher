APP_NAME = kmpmatcher

CFLAGS = -Wall -Werror -I src
DEPSFLAGS = -MMD
CC = gcc

OBJ_DIR = obj
SRC_DIR = src

APP_SOURCES = $(wildcard $(SRC_DIR)/*.c)
APP_OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(APP_SOURCES))

DEPS = $(APP_OBJECTS:.o=.d)

.PHONY: all clean logs
all: $(APP_NAME) logs

-include $(DEPS)

# LOGS
logs:
	mkdir -p logs

# BUILD
$(APP_NAME): $(APP_OBJECTS) 
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(DEPSFLAGS) -c -o $@ $< 

# CLEAN
clean:
	$(RM) -r $(APP_NAME) $(OBJ_DIR)/$(SRC_DIR)/*.[od] logs