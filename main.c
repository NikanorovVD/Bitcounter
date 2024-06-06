#include <stdio.h>
#include <malloc.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "double_linked_list.h"

int count_ones(int val)
{
  int res = 0;
  while (val)
  {
    res++;
    val &= val - 1;
  }
  return res;
}

int count_zeros(int val)
{
  return sizeof(val) * 8 - count_ones(val);
}


DblLinkedList* generate_random_DblLinkedList(int size)
{
  DblLinkedList* list = initDblLinkedList();
  srand(time(NULL));
  for(int i = 0; i < size; i++)
  {
    pushBack(list, rand());
  }
  return list;
}

void print_result(int thread_num, int counted, int sum)
{
  printf("============thread%d============\n", thread_num);
  printf("counted: %d\n", counted);
  printf("%s bits: %d\n", (thread_num == 1) ? "ones" : "zeros", sum);
  printf("===============================\n");
}


typedef struct thread_args
{
  list_node* lst;
  int counted;
  int sum;
  bool from_end;
  bool count_ones;
  bool* end;
} thread_args;


pthread_mutex_t m;

void* count_bits_in_DbLinkList(void* args)
{
  thread_args* th_args = (thread_args*) args;
  list_node* p = th_args->lst;
  do 
  {
    pthread_mutex_lock(&m);
    if(*(th_args->end)) break; 
    int val = p->val;
    list_node* next = th_args->from_end ? p->prev : p->next;
    if(!next) *(th_args->end) = true;
    deletElem(p);
    pthread_mutex_unlock(&m);

    th_args->sum += (th_args->count_ones) ? count_ones(val) : count_zeros(val);
    th_args->counted ++;
    p = next;
  } while (p != NULL);
}


int main(int argc, char **argv)
{
  int list_size;

  if(argc == 1)
  {
    printf("No list size input argument\nUsing 10000 as default\n");
    list_size = 10000;
  }

  else if(argc == 2)
  {
    list_size = atoi(argv[1]);
    if(list_size <= 0)
    {
      printf("List size must be a positive number\n");
      return 1;
    }
  }

  else 
  {
    printf("Must be one input argument\n");
    return 1;
  }

  DblLinkedList* list = generate_random_DblLinkedList(list_size);

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

  static bool end;
  args1.end = args2.end = &end;

  pthread_mutex_init(&m, NULL);
  pthread_create(&thread1, NULL, count_bits_in_DbLinkList, &args1);
  pthread_create(&thread2, NULL, count_bits_in_DbLinkList, &args2);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  print_result(1, args1.counted, args1.sum);
  printf("\n");
  print_result(2, args2.counted, args2.sum);

  free(list);
  pthread_mutex_destroy(&m);
  
  return 0;
}