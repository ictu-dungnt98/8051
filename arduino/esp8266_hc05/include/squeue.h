#ifndef _SQUEUE_H_
#define _SQUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * Copyright (c) 2021 <Hunonic>
 * Author		: dungnt98
 * Email		: nguyentrongdung0498@gmail.com
 * Create on	: 2021/12/15
 *
 * brief: Static Queue Library
 */

#include <stdint.h>

#define MAX_QUEUE_SIZE  (4096)  

typedef struct {
    uint32_t sizeof_item;
    uint32_t num_items;
    uint32_t head;
    uint32_t tail;
    uint8_t buffer[MAX_QUEUE_SIZE];
} sq_t;

uint8_t sq_init(sq_t *queue, uint32_t sizeof_item, uint32_t num_items);
uint8_t sq_push(sq_t *queue, void *item);
uint8_t sq_pop(sq_t *queue, void *item);
uint8_t sq_is_empty(sq_t *queue);
uint8_t sq_is_full(sq_t *queue);
void sq_free(sq_t *queue);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif