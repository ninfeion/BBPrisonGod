#include "bbpg_motion_detect.h"

float invSqrt(float number) 
{
    volatile long i;
    volatile float x, y;
    volatile const float f = 1.5F;

    x = number * 0.5F;
    y = number;
    i = * (( long * ) &y);
    i = 0x5f375a86 - ( i >> 1 );
    y = * (( float * ) &i);
    y = y * ( f - ( x * y * y ) );
    return y;
}

#include "math.h"

#define AREA_PEAK_VALUE_DETECT_STEP 7
#define EFFECTIVE_CSVM_THRESHOLD    0.2*9.8
#define EFFECTIVE_PEAK_TIME_DIFF    200 //ms
#define PAR_AD_TRANS_G              2048.0

static uint16_t GLOBAL_STEPS_COUNT;

void motionEvenAnalysis(uint32_t timestamp, int16_t xAccel, int16_t yAccel, int16_t zAccel)
{
    static int16_t areaPeakValueDetectTemp[AREA_PEAK_VALUE_DETECT_STEP];
//    static int16_t previousPeakSVM;
    static int16_t previousValleySVM;
    
    static uint32_t previousPeakTimestamp = 0;
//    static uint32_t previousValleyTimestamp = 0;
    
    uint8_t middleValueIndex;
    int16_t newSVMAccel;
    
    newSVMAccel = pow(sqrt(xAccel)+sqrt(yAccel)+sqrt(zAccel), 0.5);
    
    if(queueGetLength(&motionDetectFIFO) <AREA_PEAK_VALUE_DETECT_STEP)  // fifo have not full
    {
        queueAddAnElement(&motionDetectFIFO, newSVMAccel);
    }
    else
    {        
        /*
        ** process that area peak value detect
        */
        queueAddAnElement(&motionDetectFIFO, newSVMAccel);
        queueGetMultipleElements(&motionDetectFIFO, 1, AREA_PEAK_VALUE_DETECT_STEP, areaPeakValueDetectTemp);
        
        middleValueIndex = AREA_PEAK_VALUE_DETECT_STEP/2 +1;
        
        if ((areaPeakValueDetectTemp[middleValueIndex]  
             - areaPeakValueDetectTemp[middleValueIndex -1] >0) && // u=a(t)-a(t-1)>0, t:middle value 
            (areaPeakValueDetectTemp[middleValueIndex +1]  
             - areaPeakValueDetectTemp[middleValueIndex] <0)    && // v=a(t+1)-a(t)<0
            (areaPeakValueDetectTemp[middleValueIndex -1]
             >areaPeakValueDetectTemp[middleValueIndex -2]) && // a(t-1)>a(t-2)
            (areaPeakValueDetectTemp[middleValueIndex -2]
             >areaPeakValueDetectTemp[middleValueIndex -3]) && // a(t-2)>a(t-3)  
            (areaPeakValueDetectTemp[middleValueIndex +1]
             >areaPeakValueDetectTemp[middleValueIndex +2]) && // a(t+1)>a(t+2)
            (areaPeakValueDetectTemp[middleValueIndex +2]
             >areaPeakValueDetectTemp[middleValueIndex +3]))   // a(t+2)>a(t+3)     
        {
            // find the peak value is location at areaPeakValueDetectTemp[middleValueIndex] 
            if((areaPeakValueDetectTemp[middleValueIndex] - previousValleySVM) > EFFECTIVE_CSVM_THRESHOLD*PAR_AD_TRANS_G)
            {
                if(timestamp - previousPeakTimestamp > EFFECTIVE_PEAK_TIME_DIFF)
                {
                    GLOBAL_STEPS_COUNT ++;
                    
                    previousPeakTimestamp = timestamp;
//                    previousPeakSVM = areaPeakValueDetectTemp[middleValueIndex];
                }
            }// calculate CSVM = new_peak_SVM - previous_valley_SVM,if CSVM > threshold, we find the peak.
        }
        else
        if ((areaPeakValueDetectTemp[middleValueIndex]  
             - areaPeakValueDetectTemp[middleValueIndex -1] <0) && // u=a(t)-a(t-1)<0, t:middle value 
            (areaPeakValueDetectTemp[middleValueIndex +1]  
             - areaPeakValueDetectTemp[middleValueIndex] >0)    && // v=a(t+1)-a(t)>0
            (areaPeakValueDetectTemp[middleValueIndex -1]
             <areaPeakValueDetectTemp[middleValueIndex -2]) && // a(t-1)<a(t-2)
            (areaPeakValueDetectTemp[middleValueIndex -2]
             <areaPeakValueDetectTemp[middleValueIndex -3]) && // a(t-2)<a(t-3)  
            (areaPeakValueDetectTemp[middleValueIndex +1]
             <areaPeakValueDetectTemp[middleValueIndex +2]) && // a(t+1)<a(t+2)
            (areaPeakValueDetectTemp[middleValueIndex +2]
             <areaPeakValueDetectTemp[middleValueIndex +3]))   // a(t+2)<a(t+3)     
        {
            // find the valley value is location at areaPeakValueDetectTemp[middleValueIndex] 
            previousValleySVM = areaPeakValueDetectTemp[middleValueIndex];
//            previousValleyTimestamp = timestamp; 
        }   
    }
}

