#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct moving_average
{
    long sum;
    int pos;
    int * buffer;
    int length;
    bool is_filled;
} moving_average_t;

int movingAvg(moving_average_t* av_obj, int new_element)
{
  //Subtract the oldest number from the prev sum, add the new number
  av_obj->sum = av_obj->sum - av_obj->buffer[av_obj->pos] + new_element;
  //Assign the nextNum to the position in the array
  av_obj->buffer[av_obj->pos] = new_element;
  //Increment position internaly
  av_obj->pos++;
  if (av_obj->pos >= av_obj->length){
    av_obj->pos = 0;
    av_obj->is_filled = true;
  }
  printf("is_filled %d", av_obj->is_filled);
  //return the average
  return av_obj->sum / (av_obj->is_filled ? av_obj->length:av_obj->pos);
}

moving_average_t* allocate_moving_average(int len)
{
    moving_average_t* av_obj = (moving_average_t*)malloc(sizeof(moving_average_t));
    av_obj->sum       = 0;
    av_obj->pos       = 0;
    av_obj->length    = len;
    av_obj->is_filled = false;
    av_obj->buffer = (int*)malloc(len * sizeof(int));
    return av_obj;
}

void free_moving_average(moving_average_t * av_obj)
{
    free(av_obj->buffer);
    av_obj->buffer = NULL;
    free(av_obj);
}