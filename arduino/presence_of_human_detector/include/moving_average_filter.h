#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct moving_average
{
    long sum;
    int pos;
    int * buffer;
    int length;
    bool is_filled;
} moving_average_t;

int movingAvg(moving_average_t *av_obj, int new_element);
moving_average_t* allocate_moving_average(int len);
void free_moving_average(moving_average_t * av_obj);