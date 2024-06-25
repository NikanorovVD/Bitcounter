TARGET = bitcounter
CC = gcc
CFLAGS = -c
LDFLAGS = -lpthread -Werror -g

all: $(TARGET)
	
$(TARGET): main.o double_linked_list.o
	$(CC) main.o double_linked_list.o -o  $(TARGET) $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) main.c

double_linked_list.o: double_linked_list.c
	$(CC) $(CFLAGS) double_linked_list.c

clean:
	rm -rf $(TARGET) *.o

.PHONY: $(TARGET)