#include "nm_step_count.h"

/*****************************************************************
** input: 3 axis or angle
** output: step count
** user read:
	3 axis is filter value.
******************************************************************/
uint32_t nmStepCount(uint32_t timestamp, int16_t axis0, int16_t axis1, int16_t axis2)
{
	uint16_t nowPos = 0;
	uint16_t ppDiff = 0;
	uint16_t timeDiff = 0;
    
	/* 获取3D IMU */
	nowPos = (uint16_t)pow(sqrt(axis0) + sqrt(axis1) + sqrt(axis2), 0.5);
    
	/* 得到波峰和波谷 */
	if((pSta==RISING_EDGE) && (nowPos<=lastPos))
    {
		pSta = FALLING_EDGE;
		newMax = lastPos;
		walkSta = true;
	}
	else if((pSta==FALLING_EDGE) && (nowPos>lastPos))
    {
		pSta = FALLING_EDGE;
		newMin = lastPos;
		walkSta = true;
	}
	else
    {
		walkSta = false;
	}
	
    /* 更新3D step状态数据 */
	lastPos = nowPos;
	
	/* 有波峰或波谷 */
	if(walkSta==true)
    {
		walkSta = false;
		ppDiff = newMax - newMin; /* 波峰与波谷的差值 */
		
        if(ppDiff > P_P_DIFF)
        {
			timeDiff = timestamp - lastTime;	/* 获取波峰和波谷的时间差 */
			if(timeDiff < FAST_WALK_TIME_LIMIT_MS)
            {  
                /* 波峰波谷时间差小于200ms的直接去掉 */
				return stepCount;
			}
			else if(timeDiff > SLOW_WALK_TIME_LIMIT_MS)
            { 
                /* 波峰波谷时间差大于10s的视为静止 */
				walkOkSta = false;
				stepOK = 0;
				return stepCount;
			}
			stepOK++;
			if(stepOK>=STEP_OK)
            { 
                /* 走7步之后更新状态 */
				walkOkSta = true;
			}
			lastTime = timestamp; /* 更新时间 */
		}
	}
	
	if(walkOkSta==true)
    { 
        /* 满足10s内走7步 */
		stepCount += stepOK;
		stepOK = 0;
	}
	return stepCount;
}
