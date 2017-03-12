#include "bbpg_queue.h"

FIFO_Status initQueue(QueueType *queue)
{
    queue->front = (QNodePtrType)api_malloc(sizeof(QNodeType));
    if(!queue->front)
    {
        return OVERFLOW;
    }
    else
    {
        queue->rear  = queue->front;
    }
    
    queue->length = 0;
    queue->front->next = NULL;
    return OK;
}

FIFO_Status destoryQueue(QueueType *queue)
{
    while(queue->front)
    {
        queue->rear = queue->front->next; // rear be used as a temp
        api_free(queue->front);
        queue->front = queue->rear;
    }
    return OK;
}

FIFO_Status queueAddAnElement(QueueType *queue, QElemType data)
{
    QNodePtrType tempPtr;
    
    tempPtr = (QNodePtrType)api_malloc(sizeof(QNodeType));
    if(!tempPtr)
    {
        return OVERFLOW;
    }
    
    tempPtr->data = data;
    tempPtr->next = NULL;
    
    queue->rear->next = tempPtr;
    queue->rear = tempPtr;
    
    queue->length ++;
    return OK;
}

FIFO_Status queuePopHead(QueueType *queue, QElemType *data)
{
    QNodePtrType tempPtr;
    
    if(queue->front == queue->rear)
    {
        return ERROR;
    }
    
    tempPtr = queue->front->next;
    *data = tempPtr->data;
    
    queue->front->next = tempPtr->next;
    if(queue->rear == tempPtr)
    {
        queue->rear = queue->front;
    }
    
    api_free(tempPtr);
    
    queue->length --;
    return OK;
}

uint16_t queueGetLength(QueueType *queue)
{
    return queue->length;
}

FIFO_Status queueGetMultipleElements(QueueType *queue, uint16_t firstNum, uint16_t lastNum, QElemType *data)
{
    uint16_t searchCount;
    QNodePtrType tempPtr;
    
    if(firstNum < lastNum)
    {
        return ERROR;
    }
    
    tempPtr = queue->front;
    
    for(searchCount=1; searchCount <= queue->length; searchCount++)
    {
        if ((searchCount>=firstNum)||(searchCount<=lastNum))
        {
            *(data+ (searchCount-firstNum)) = tempPtr->data;
        }
        
        tempPtr = tempPtr->next;
    }
    
    return OK;
}


