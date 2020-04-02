#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include "sec_grt_elt.h"

// the structure to be used by the program
struct node {
  int val;
  struct node* down;
};

typedef struct node node;

node* create_node (int ele);
void display (node* l);
void test (int* numbers, int length, node** arr, int arr_len);
void compare (node** arr, int index, int arr_len);
void shift (node** arr, int arr_len);

double log2n(double n)        // function to calculate log to the base 2 of n
{
    return log(n)/log(2.00);
}

node* create_node (int ele)     // function to create a node of desired value
{
  node* temp = (node*)malloc(sizeof(node));
  if (temp) {
    temp->down = NULL;
    temp->val = ele;
  }
  return temp;
}

void display (node* l)      // function to display a particular linked list
{
  node* temp = l;
  while (temp != NULL)
  {
    printf ("%d ", temp->val);
    temp = temp->down;
  }
  printf ("\n");
}

// my function to free the nodes that will not be requireed in the next match onwards
void myfree (node* arr)
{
  node* temp = arr;
  node* temp2 = NULL;
  while (temp != NULL) {
    temp2 = temp;
    temp = temp->down;
    free (temp2);
  }
}

void max (int length, int arr_len, node** arr)      // function to get the largest element in the last linked list...
{                                                   // ...excluding the first node
  node *temp;
  temp = arr[arr_len-1]->down;
  int max = temp->val;
  while (temp!=NULL) {
    if (temp->val > max) {max = temp->val;}
    temp = temp->down;
  }
  printf ("\n%d\n", max);
}

void find_second_greatest(int *numbers, int length)       // function to get second largest element
{
  length = (double)length;
  int arr_len = ceil(log2n(length))+1;         // length of the structure pointer array to point to the linked lists
  node* arr[arr_len];
  for (int i=0; i<arr_len; i++){
    arr[i] = NULL;
  }
  test (numbers, length, arr, arr_len);   // main function of the program containing the logic
  // display (arr[arr_len-1]);
  max (length, arr_len, arr);   // calling this function to display the last number in the output, the second largest elt
  myfree (arr[arr_len-1]);    // freeing the last linked list to avoid memory leak
}

// the function that iterates through the input array until last structure pointer is not null
void test (int* numbers, int length, node** arr, int arr_len)
{
    // int flag = 0;
    // if ( (length) & (length-1) != 0 ) {
    //   ++time;
    //   length = length-1;
    //   flag = 1;
    // }
    while (arr[arr_len-1] == NULL) {// running the loop till the last node in the structure pointer array is not null i.e. input array is exhausted
      for (int i=0; i<length; ++i) {      // runs loop till there are no elements in the input array
        node* val1 = create_node (numbers[i]);
        if (arr[0] == NULL) {   // checks if first pointer points to a linked list or not
          arr[0] = val1;
          if ( i == length-1 ) {
            shift (arr, arr_len);  // if it is the last element in the input array, shift to cover all null spaces in between
          }
          continue;
        }
        node* val2 = val1;
        val1 = arr[0];
        printf ("%d x %d = ", val1->val, val2->val);    // the match is played in the following condtions
        if (val1->val > val2->val) {
          // printf ("%d\n", val1->val);
          arr[0] = val1;
          val1->down = val2;
        }
        else {
          // printf ("%d\n", val2->val);
          arr[0] = val2;
          val2->down = val1;
        }
        display (arr[0]);  // the winner is displayed and loser next
        compare (arr, 0, arr_len);
          node* temp = arr[0];  // shifting the winner to the next round if it is null
          arr[1] = temp;
          arr[0] = NULL;
      }
      shift (arr, arr_len);   // shift if after the array is exhausted, any null spaces remain in between
  }
}

void disp_match (node* opp1, node* opp2)   // displays the match being played in the current loop
{
  node *temp = opp1;
  while (temp!=NULL) {
    printf ("%d ", temp->val);
    temp = temp->down;
  }
  printf ("x ");
  node *temp2 = opp2;
  while (temp2!=NULL) {
    printf ("%d ", temp2->val);
    temp2 = temp2->down;
  }
  printf ("= ");
}

// main function to compare opponents and play the required number of matches
void compare (node** arr, int index, int arr_len)
{
  while (arr[index+1] != NULL && (index+1) != arr_len) { //loop runs till there is some opponent in the next node
    node* first = arr[index];
    node* second = arr[index+1];
    disp_match (second, first);   // displays the match being played in the following code
    if (second->val > first->val) {
      node* losers = second->down;
      second->down = first;
      myfree(first->down);    // freeing the losers opponents
      first->down = losers;   // attaching the last played opponent to the all the losers of the winner
      arr[index+1] = second;
      arr[index] = NULL;
    }
    else {
      node* losers = first->down;
      first->down = second;
      myfree(second->down);
      second->down = losers;
      arr[index+1] = first;
      arr[index] = NULL;
    }
    node* temp3 = arr[index+1];
    while (temp3!=NULL) {         // printing the winner of the match
      printf ("%d ", temp3->val);
      temp3 = temp3->down;
    }
    printf ("\n");
    if (arr[index+2] == NULL) {   // winner moves to the next round if there is no opponent to play
      node* temp = arr[index+1];
      arr[index+2] = temp;
      arr[index+1] = NULL;  // setting previous to null
      break;
    }
    ++index;  // go to the next index to check if any opponent remains
  }
}

/*int isnull (node** arr, int arr_len) {
  for (int i=0; i<arr_len; i++) {
    if (arr[i] != NULL) { ++time;return 0;}
  }
  return 1;
}*/

void shift (node** arr, int arr_len) {  // function to remove null gaps in the structure pointer array
  for (int i=0; i<arr_len; i++) {
    if (arr[i] != NULL) {   // if a linked list is present
      while (arr[i+1] == NULL) {   // till the next linked list is present
        node* temp = arr[i];
        arr[i+1] = temp;   // move the player to the next structure pointer
        arr[i] = NULL;  // set previous to null
        ++i;   // go to the next index
      }
      compare (arr, i, arr_len); // play a match between them
    }
  }
}
