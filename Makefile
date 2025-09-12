INC_DIR := ../PrscHQC/include
SRC_DIR := ../PrscHQC/src

CC := clang
CFLAGS := -O3 -std=c11 -Wall -Wextra -Wno-unused-parameter -I$(INC_DIR)

OBJS := $(SRC_DIR)/channel.c \
        $(SRC_DIR)/crypto.c  \
        $(SRC_DIR)/packet.c

SRC := experiments/badpk.c

all: badpk

badpk: $(SRC)
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@

clean:
	rm -f badpk
