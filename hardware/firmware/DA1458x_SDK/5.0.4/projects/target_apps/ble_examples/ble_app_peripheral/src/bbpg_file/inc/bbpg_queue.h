#ifndef _BBPG_QUEUE_H_
#define _BBPG_QUEUE_H_

#include "stdint.h"

#include "ke_mem.h"
// API TRANS INDEX
#define api_malloc(n)   ke_malloc(n, KE_MEM_NON_RETENTION)
#define api_free(n)     ke_free(n)
typedef int16_t QElemType;
    
typedef struct QNode
{
    QElemType data;
    struct QNode *next;
}QNodeType, *QNodePtrType;

typedef struct
{
    QNodePtrType front;
    QNodePtrType rear;
    
    uint16_t length;
}QueueType;

static QueueType motionDetectFIFO;
//static QueueType mpuFIFO;

typedef enum
{
    ERROR,
    OVERFLOW,
    OK,
}FIFO_Status;

FIFO_Status initQueue(QueueType *queue);
FIFO_Status destoryQueue(QueueType *queue);
FIFO_Status clearQueue(QueueType *queue);
FIFO_Status queueEmpty(QueueType *queue);
uint16_t    queueGetLength(QueueType *queue);

FIFO_Status queueGetHead(QueueType *queue, QElemType *data); // just get the head data, would not delete it
FIFO_Status queueAddAnElement(QueueType *queue, QElemType data);
FIFO_Status queuePopHead(QueueType *queue, QElemType *data);

FIFO_Status queueGetMultipleElements(QueueType *queue, uint16_t firstNum, uint16_t lastNum, QElemType *data);

FIFO_Status queueTraverse(QueueType *queue, void *targetFunc());

#endif

