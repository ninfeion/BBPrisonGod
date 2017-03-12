#ifndef _BBPG_MOTION_DETECT_H_
#define _BBPG_MOTION_DETECT_H_

#include "bbpg_queue.h"

void motionEvenAnalysis(uint32_t timestamp, int16_t xAccel, int16_t yAccel, int16_t zAccel);

#endif

