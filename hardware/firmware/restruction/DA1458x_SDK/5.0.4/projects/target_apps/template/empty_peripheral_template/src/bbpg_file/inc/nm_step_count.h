#ifndef _NM_STEP_COUNT_H_
#define _NM_STEP_COUNT_H_

#include "stdint.h"
#include "math.h"

#define P_P_DIFF	            1000 /* 波峰-波谷的差值，即3D阈值 */
#define RISING_EDGE             1 /* 上升沿状态 */
#define FALLING_EDGE            0 /* 下降沿状态 */
#define FAST_WALK_TIME_LIMIT_MS	200 	/* ms */
#define SLOW_WALK_TIME_LIMIT_MS	10000 /* 10s 内没有走一步 结束计步 */
#define STEP_OK                 7	/* 7步法则 */
	
uint32_t nmStepCount(uint32_t timestamp, int16_t axis0, int16_t axis1, int16_t axis2);

uint32_t getStepCountVal(void);
    
#endif


