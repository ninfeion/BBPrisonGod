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
    
	/* ��ȡ3D IMU */
	nowPos = (uint16_t)pow(sqrt(axis0) + sqrt(axis1) + sqrt(axis2), 0.5);
    
	/* �õ�����Ͳ��� */
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
	
    /* ����3D step״̬���� */
	lastPos = nowPos;
	
	/* �в���򲨹� */
	if(walkSta==true)
    {
		walkSta = false;
		ppDiff = newMax - newMin; /* �����벨�ȵĲ�ֵ */
		
        if(ppDiff > P_P_DIFF)
        {
			timeDiff = timestamp - lastTime;	/* ��ȡ����Ͳ��ȵ�ʱ��� */
			if(timeDiff < FAST_WALK_TIME_LIMIT_MS)
            {  
                /* ���岨��ʱ���С��200ms��ֱ��ȥ�� */
				return stepCount;
			}
			else if(timeDiff > SLOW_WALK_TIME_LIMIT_MS)
            { 
                /* ���岨��ʱ������10s����Ϊ��ֹ */
				walkOkSta = false;
				stepOK = 0;
				return stepCount;
			}
			stepOK++;
			if(stepOK>=STEP_OK)
            { 
                /* ��7��֮�����״̬ */
				walkOkSta = true;
			}
			lastTime = timestamp; /* ����ʱ�� */
		}
	}
	
	if(walkOkSta==true)
    { 
        /* ����10s����7�� */
		stepCount += stepOK;
		stepOK = 0;
	}
	return stepCount;
}
