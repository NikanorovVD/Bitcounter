#include <stdio.h>
#include <malloc.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "double_linked_list.h"

int count_ones(int val){
    int res = 0;
    while (val)
    {
        res++;
        val &= val-1;
    }
    return res;
}

int count_zeros(int val){
    return sizeof(val)*8 - count_ones(val);
}


int* generate_random_array(int size){
  int* array = (int*)malloc(size*sizeof(int));
  srand(time(NULL));
  for(int i = 0; i < size; i++){
    array[i] = rand();
  }
  return array;
}

void print_result(int thread_num, int counted, int sum){
  printf("============thread%d============\n", thread_num);
  printf("counted: %d\n", counted);
  printf("%s bits: %d\n", (thread_num == 1) ? "ones":"zeros", sum);
  printf("===============================\n");
}


typedef struct thread_args{
  list_node* lst;
  int counted;
  int sum;
  bool from_end;
  bool count_ones;

  int* count_all; //the number of elements counted by both streams in total
  int len;
} thread_args;


pthread_mutex_t m;

void* count_bits_in_DbLinkList(void* args){
  thread_args* th_args = (thread_args*) args;
  list_node* p = th_args->lst;
  int count_all;
  do {
    pthread_mutex_lock( &m );
      *(th_args->count_all) = *(th_args->count_all) + 1;
      count_all = *(th_args->count_all);
    pthread_mutex_unlock( &m );

    if(count_all > th_args->len) break;
    th_args->sum += (th_args->count_ones) ? count_ones(*((int*)p->val)) : count_zeros(*((int*)p->val));
    th_args->counted ++;
    list_node* next = (th_args->from_end) ? p->prev : p->next;
    deletElem(p);
    p = next;
   
  } while (p != NULL);
}


int main(int argc, char **argv){

  int array_size;

  if(argc == 1){
    printf("No array size input argument\nUsing 10000 as default\n");
    array_size = 10000;
  }

  else if(argc == 2){
      array_size = atoi(argv[1]);
      if(array_size <= 0){
        printf("Array size must be a positive number\n");
        return 1;
      }
  }

  else {
    printf("Must be one input argument\n");
    return 1;
  }

  int* values = generate_random_array(array_size);
  DblLinkedList* list = fromArray(values,array_size,sizeof(int) );


  pthread_t thread1, thread2;
  thread_args args1, args2;

  args1.lst = list->head;
  args2.lst = list->tail;
  args1.counted = args2.counted = 0;
  args1.sum = args2.sum = 0;

  args1.from_end = false;
  args2.from_end = true;

  args1.count_ones = true;
  args2.count_ones = false;

  static int count_all = 0;
  args1.count_all = args2.count_all = &count_all;
  args1.len = args2.len = array_size;

  pthread_mutex_init(&m, NULL);
  pthread_create(&thread1, NULL, count_bits_in_DbLinkList, &args1);
  pthread_create(&thread2, NULL, count_bits_in_DbLinkList, &args2);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  print_result(1, args1.counted, args1.sum);
  printf("\n");
  print_result(2, args2.counted, args2.sum);

  free(list);
  free(values);
  pthread_mutex_destroy(&m);
  
  return 0;
}