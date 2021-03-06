#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct Array {
  int capacity;  // How many elements can this array hold?
  int count;  // How many states does the array currently hold?
  char **elements;  // The string elements contained in the array
} Array;


/************************************
 *
 *   CREATE, DESTROY, RESIZE FUNCTIONS
 *
 ************************************/

void set_rest_null(Array *arr)
{
  for(int i=arr->count; i<arr->capacity; i++)
  {
    arr->elements[i] = NULL;
  }
}

/*****
 * Allocate memory for a new array
 *****/
Array *create_array (int capacity) {
  // Allocate memory for the Array struct
  struct Array *array;
  array = malloc(sizeof(struct Array));
  // Set initial values for capacity and count
  array->capacity = capacity;
  array->count = 0;
  // Allocate memory for elements
  array->elements = malloc(capacity*sizeof(char*));
  set_rest_null(array);
  return array;
}


/*****
 * Free memory for an array and all of its stored elements
 *****/
void destroy_array(Array *arr) {

  // Free all elements
  for(int i=0; i<arr->count; i++)
  {
    free(arr->elements[i]);
  }
  free(arr->elements);
  // Free array
  free(arr);
}

/*****
 * Create a new elements array with double capacity and copy elements
 * from old to new
 *****/
void resize_array(Array *arr) {

  // Create a new element storage with double capacity
  char **new_storage = malloc(arr->capacity*2*sizeof(char*));
  // Copy elements into the new storage
  char **p = arr->elements;
  for(int i=0; i<arr->count; i++)
  {
    new_storage[i] = *p++;
  }
  // Free the old elements array (but NOT the strings they point to)
  free(arr->elements);
  // Update the elements and capacity to new values
  arr->elements = new_storage;
  set_rest_null(arr);
  arr->capacity *= 2;
}



/************************************
 *
 *   ARRAY FUNCTIONS
 *
 ************************************/

/*****
 * Return the element in the array at the given index.
 *
 * Throw an error if the index is out of range.
 *****/
char *arr_read(Array *arr, int index) {

  // Throw an error if the index is greater than the current count
  if(index > arr->count)
  {
    fprintf(stderr, "invalid index\n");
    exit(2);
  }
  // Otherwise, return the element at the given index
  else
  {
    return arr->elements[index];
  }
}

int get_length(char *string)
{
  int length = 0;
  for(int i=0; string[i]!='\0'; i++)
  {
    length++;
  }
  return ++length;
}

char* copy_of_string(char* string)
{
  int length = get_length(string);
  char *p = malloc(length);
  for(int i=0; i<length; i++)
  {
    p[i] = string[i];
  }
  return p;
}

/*****
 * Insert an element to the array at the given index
 *****/
void arr_insert(Array *arr, char *element, int index) {

  // Throw an error if the index is greater than the current count
  if(index > arr->count)
  {
    fprintf(stderr, "index is greater than current count\n");
    exit(3);
  }
  else
  {
  // Resize the array if the number of elements is over capacity
    if(arr->count == arr->capacity)
    {
      resize_array(arr);
    }
  // Move every element after the insert index to the right one position
    for(int i=arr->count-1; i>index-1; i--)
    {
      arr->elements[i+1] = arr->elements[i];
    }
  // Copy the element and add it to the array
    char *p = copy_of_string(element);
    arr->elements[index] = p;
  // Increment count by 1
    arr->count++;
  }
}

/*****
 * Append an element to the end of the array
 *****/
void arr_append(Array *arr, char *element) {

  // Resize the array if the number of elements is over capacity
  // or throw an error if resize isn't implemented yet.
  if(arr->count == arr->capacity)
  {
    resize_array(arr);
  }
  // Copy the element and add it to the end of the array
  char *p = copy_of_string(element);
  arr->elements[arr->count] = p;
  // Increment count by 1
  arr->count++;
}

int is_same(char *arr_string, char *string)
{
  int length1 = get_length(arr_string);
  int length2 = get_length(string);
  if(length1!=length2)
  {
    return 0;
  }
  for(int i=0; i<length1; i++)
  {
    if(arr_string[i]!=string[i])
    {
      return 0;
    }
  }
  return 1;
}

/*****
 * Remove the first occurence of the given element from the array,
 * then shift every element after that occurence to the left one slot.
 *
 * Throw an error if the value is not found.
 *****/
void arr_remove(Array *arr, char *element) {

  // Search for the first occurence of the element and remove it.
  // Don't forget to free its memory!
  int i = 0;
  while(!is_same(arr->elements[i], element))
  {
    i++;
  }
  free(arr->elements[i]);
  // Shift over every element after the removed element to the left one position
  for(int j=i+1; j<arr->count; j++)
  {
    arr->elements[j-1] = arr->elements[j];
  }
  arr->elements[arr->count-1] = NULL;
  // Decrement count by 1
  arr->count--;
}


/*****
 * Utility function to print an array.
 *****/
void arr_print(Array *arr) {
  printf("[");
  for (int i = 0 ; i < arr->count ; i++) {
    printf("%s", arr->elements[i]);
    if (i != arr->count - 1) {
      printf(",");
    }
  }
  printf("]\n");
}


#ifndef TESTING
int main(void)
{

  Array *arr = create_array(1);

  arr_insert(arr, "STRING1", 0);
  arr_append(arr, "STRING4");
  arr_insert(arr, "STRING2", 0);
  arr_insert(arr, "STRING3", 1);
  arr_print(arr);
  arr_remove(arr, "STRING3");
  arr_print(arr);

  destroy_array(arr);

  return 0;
}
#endif
