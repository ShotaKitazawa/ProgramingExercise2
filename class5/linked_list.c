#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  float data;
  struct node* next;
} node;

void print_list(node* head);
node* insert_list(node* head);
node* delete_list(node* head);

void main(){
  node* head = NULL;
  node* new = NULL;

  head = (node*)malloc(sizeof(node));
  head->data = 1.0f;
  head->next = NULL;

  // add new nodes to the end of list
  int i;
  for (i = 0; i < 19; i++) {       // 19 nodes
    new = (node*)malloc(sizeof(node)); // allocate memory
    new->next = head;
    head = new;
    new->data = (i + 2) * 1.0f;        // assign data in new node
  }

  node* pointer = head;

  print_list(head);
  head = insert_list(head);
  print_list(head);
  head = delete_list(head);
  print_list(head);

}

void print_list(node* head){
  node* pointer = head;
  while (pointer != NULL){
    printf("%f\n", pointer->data);
    pointer = pointer->next;
  }
}

node* insert_list(node* head){
  int n;
  float f;
  printf("position to insert: ");
  scanf("%d",&n);
  printf("data: ");
  scanf("%f",&f);
  node* pointer = head;
  if (n == 0){
    node* new = (node*)malloc(sizeof(node));
    new->data = f;
    new->next = pointer;
    head = new;
  }
  else if (n > 0){
    int i;
    for (i = 0; i < n-1; i++){
      pointer = pointer->next;
    }
    node* new = (node*)malloc(sizeof(node));
    new->data = f;
    new->next = pointer->next;
    pointer->next = new;
  }
  else {
    printf("error: invalid input\n");
  }
  return head;
}

node* delete_list(node* head){
  int n;
  printf("position to delete: ");
  scanf("%d",&n);
  node* pointer = head;
  if (n == 0){
    head = pointer->next;
  }
  else if (n > 0){
    int i;
    for (i = 0; i < n-1; i++){
      pointer = pointer->next;
    }
    pointer->next = pointer->next->next;
  }
  else{
    printf("error: invalid input\n");
  }
  return head;
}
