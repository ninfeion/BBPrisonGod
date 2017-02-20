#ifndef _NM_STEP_COUNT_H_
#define _NM_STEP_COUNT_H_

#include "stdint.h"
#include "math.h"

#define P_P_DIFF	            1000 /* ����-���ȵĲ�ֵ����3D��ֵ */
#define RISING_EDGE             1 /* ������״̬ */
#define FALLING_EDGE            0 /* �½���״̬ */
#define FAST_WALK_TIME_LIMIT_MS	200 	/* ms */
#define SLOW_WALK_TIME_LIMIT_MS	10000 /* 10s ��û����һ�� �����Ʋ� */
#define STEP_OK                 7	/* 7������ */
	
uint16_t lastPos = 0;	/* ������ */
uint16_t newMax = 0, newMin = 0; /* ����-���� */

bool walkSta = false; /* ���һ�η�ֵ״̬ */
bool walkOkSta = false; /* ����10s������7�� ��Ч����״̬ */
bool pSta = RISING_EDGE; /* 3D���ݲ���״̬ */

long lastTime = 0;		/* ��һ�� walkSta ��ʱ�� */
uint8_t stepOK = 0; /* ��ʼ�Ʋ�����  �˳����� */
static uint32_t stepCount = 0; /* ����ֵ */

uint32_t nmStepCount(uint32_t timestamp, int16_t axis0, int16_t axis1, int16_t axis2);
    
#endif


