#include <stdio.h>
#define MAXSIZE 1000

void array_print(char *s, int heap[], int no) {
  printf("%s ", s);
  int i;
  for (i = 0; i < no; i++) printf("%d ", heap[i]);
  printf("\n");
}

void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

void quick_sort(int list[], int low, int high, int num) {
  int pivot, pivot_index, low_index, high_index;
  printf("low %d, high %d, ", low,high);
  if (low < high) {
    pivot_index = (high+low)/2;
    pivot = list[pivot_index];
    low_index = low;
    high_index = high;
    printf("pivot %d, pivot_index %d\n",pivot, pivot_index);

    while (low_index < high_index) {
      //printf("%d",high_index);
      while (list[low_index] <= pivot && low_index < pivot_index) {
        low_index++;  // from low,  find an element >  pivot
      }
      while (list[high_index] > pivot && high_index > pivot_index) {
        high_index--;  // from high, find an element <= pivot
      }
      if (low_index < high_index) {  // two elements is in wrong order
      //printf("%d\n",list[low_index]);
        swap(&list[low_index], &list[high_index]);  // swap two elements
        if (low_index == pivot_index) {
          pivot_index = high_index;
          low_index++;
        }
        if (high_index == pivot_index) {
          pivot_index = low_index;
          high_index--;
        }
      }
    }                                     // all elements are checked
    //swap(&list[low], &list[high_index]);  // put pivot in final position
    printf("high_index %d, low_index %d, ", pivot,high, low,high_index,low_index);
    array_print("swap", list, num);

    quick_sort(list, low, high_index - 1, num);   // sort low  part
    quick_sort(list, high_index + 1, high, num);  // sort high part
  }
}

void main() {
  int array[MAXSIZE];
  int i, num;

  printf("enter the number of elements n = ");
  scanf("%d", &num);
  printf("enter %d elements one by one\n", num);
  for (i = 0; i < num; i++) {
    scanf("%d", &array[i]);
  }
  array_print("the original array is", array, num);
  quick_sort(array, 0, num - 1, num);
  array_print("the  sorted  array is", array, num);
}
