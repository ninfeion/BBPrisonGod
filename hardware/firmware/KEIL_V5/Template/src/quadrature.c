//------------------------------------------------------------------------------
// (c) Copyright 2012, Dialog Semiconductor BV
// All Rights Reserved
//------------------------------------------------------------------------------
// Dialog SEMICONDUCTOR CONFIDENTIAL
//------------------------------------------------------------------------------
// This code includes Confidential, Proprietary Information and is a Trade 
// Secret of Dialog Semiconductor BV. All use, disclosure, and/or reproduction 
// is prohibited unless authorized in writing.
//------------------------------------------------------------------------------
// Description: quadrature source file

#include "global_io.h"
#include "gpio.h"
#include <core_cm0.h>

void init_quadrature(short ch1,short ch2,short ch3){
	SetBits16(CLK_PER_REG,QUAD_ENABLE,1);

	SetBits16(QDEC_CTRL_REG,QD_IRQ_THRES,20);
	SetBits16(QDEC_CTRL_REG,QD_IRQ_MASK,1);
	SetWord16(QDEC_CLOCKDIV_REG,1000);

	SetBits16(QDEC_CTRL2_REG,CHX_PORT_SEL,ch1);
	SetBits16(QDEC_CTRL2_REG,CHY_PORT_SEL,ch2);
	SetBits16(QDEC_CTRL2_REG,CHZ_PORT_SEL,ch3);

	NVIC_EnableIRQ(WKUP_QUADEC_IRQn);   

}

void close_quadrature(void){
	NVIC_DisableIRQ(WKUP_QUADEC_IRQn);   
	SetBits16(QDEC_CTRL2_REG,CHX_PORT_SEL,QD_PIN_NONE);
	SetBits16(QDEC_CTRL2_REG,CHY_PORT_SEL,QD_PIN_NONE);
	SetBits16(QDEC_CTRL2_REG,CHZ_PORT_SEL,QD_PIN_NONE);
	SetBits16(CLK_PER_REG,QUAD_ENABLE,0);
}

