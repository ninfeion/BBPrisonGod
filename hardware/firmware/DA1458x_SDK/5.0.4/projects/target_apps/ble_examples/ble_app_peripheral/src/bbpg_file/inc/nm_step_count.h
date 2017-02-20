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
	
uint16_t lastPos = 0;	/* 旧数据 */
uint16_t newMax = 0, newMin = 0; /* 波峰-波谷 */

bool walkSta = false; /* 获得一次峰值状态 */
bool walkOkSta = false; /* 连续10s内走了7步 有效行走状态 */
bool pSta = RISING_EDGE; /* 3D数据波形状态 */

long lastTime = 0;		/* 上一次 walkSta 的时间 */
uint8_t stepOK = 0; /* 初始计步门限  滤除干扰 */
static uint32_t stepCount = 0; /* 步数值 */

uint32_t nmStepCount(uint32_t timestamp, int16_t axis0, int16_t axis1, int16_t axis2);
    
#endif


