#ifndef _MPU9250_FIFO_H_
#define _MPU9250_FIFO_H_

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
}QNodeType, *QueuePtrType;

typedef struct
{
    QueuePtrType front;
    QueuePtrType rear;
    
    uint16_t length;
}QueueType;

typedef enum
{
    ERROR,
    OVERFLOW,
    OK,
    //FALSE,
    //TRUE,
}FIFO_Status;

// data struct main body
static QueueType fifoIns;

FIFO_Status initQueue(void);
FIFO_Status destoryQueue(void);
FIFO_Status clearQueue(void);
FIFO_Status queueEmpty(void);
uint16_t    queueGetLength(void);

FIFO_Status queueGetHead(QElemType *data); // just get the head data, would not delete it
FIFO_Status queueAddAnElement(QElemType data);
FIFO_Status queuePopHead(QElemType *data);

FIFO_Status queueTraverse(void *targetFunc());

#endif


