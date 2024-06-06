typedef struct list_node
{
  int val; 
  struct list_node* next; 
  struct list_node* prev; 
} list_node;

typedef struct DblLinkedList 
{
  int size;
  list_node *head;
  list_node *tail;
} DblLinkedList;

DblLinkedList* initDblLinkedList();

void pushBack(DblLinkedList *list, int value);

void deletElem(list_node *lst);