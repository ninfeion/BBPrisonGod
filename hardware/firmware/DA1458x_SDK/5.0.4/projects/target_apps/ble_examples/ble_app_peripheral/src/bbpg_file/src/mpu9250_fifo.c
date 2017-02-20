#include "mpu9250_fifo.h"

FIFO_Status initQueue(void)
{
    fifoIns.front = (QueuePtrType)api_malloc(sizeof(QNodeType));
    if(!fifoIns.front)
    {
        return OVERFLOW;
    }
    else
    {
        fifoIns.rear  = fifoIns.front;
    }
    
    fifoIns.length = 0;
    fifoIns.front->next = NULL;
    return OK;
}

FIFO_Status destoryQueue(void)
{
    while(fifoIns.front)
    {
        fifoIns.rear = fifoIns.front->next; // rear be used as a temp
        api_free(fifoIns.front);
        fifoIns.front = fifoIns.rear;
    }
    return OK;
}

FIFO_Status queueAddAnElement(QElemType data)
{
    QueuePtrType tempPtr;
    
    tempPtr = (QueuePtrType)api_malloc(sizeof(QNodeType));
    if(!tempPtr)
    {
        return OVERFLOW;
    }
    
    tempPtr->data = data;
    tempPtr->next = NULL;
    
    fifoIns.rear->next = tempPtr;
    fifoIns.rear = tempPtr;
    
    fifoIns.length ++;
    return OK;
}

FIFO_Status queuePopHead(QElemType *data)
{
    QueuePtrType tempPtr;
    
    if(fifoIns.front == fifoIns.rear)
    {
        return ERROR;
    }
    
    tempPtr = fifoIns.front->next;
    *data = tempPtr->data;
    
    fifoIns.front->next = tempPtr->next;
    if(fifoIns.rear == tempPtr)
    {
        fifoIns.rear = fifoIns.front;
    }
    
    api_free(tempPtr);
    
    fifoIns.length --;
    return OK;
}


uint16_t queueGetLength(void)
{
    return fifoIns.length;
}

