all: bitcounter
	
bitcounter: double_linked_list.h
	gcc main.c double_linked_list.c -o bitcounter -lpthread

.PHONY: bitcounter