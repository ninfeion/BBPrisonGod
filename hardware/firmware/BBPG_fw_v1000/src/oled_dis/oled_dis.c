#include "oled_dis.h"

static GPIO_PORT oledPowerControlPort;
static GPIO_PIN oledPowerControlPin;

static GPIO_PORT resetPort;
static GPIO_PIN resetPin;

static GPIO_PORT oledI2CPort;
static GPIO_PIN oledI2CSCL;
static GPIO_PIN oledI2CSDA;

//scl and sda has worry ,attention
#define SCL_HIGH    GPIO_SetActive(oledI2CPort, oledI2CSDA);
#define SCL_LOW     GPIO_SetInactive(oledI2CPort, oledI2CSDA);

#define SDA_HIGH    GPIO_SetActive(oledI2CPort, oledI2CSCL);
#define SDA_LOW     GPIO_SetInactive(oledI2CPort, oledI2CSCL);

#define RES_HIGH    GPIO_SetActive(resetPort, resetPin);
#define RES_LOW     GPIO_SetInactive(resetPort, resetPin);


 void  start()
{
  SCL_HIGH;;
  SDA_HIGH;
  SDA_LOW;
  SCL_LOW;
}


void  stop()
{
  SCL_LOW;
  SDA_LOW;
  SDA_HIGH;
  SCL_HIGH;
}


void write_w(uint8_t dat)
{
   uint8_t m,da;
   uint8_t j;
   da=dat;
   for(j=0;j<8;j++)
   {
     m=da;
     SCL_LOW;     
     m=m&0x80;
     if(m==0x80)
        {
      	  SDA_HIGH;
        }
     else
        {
      	  SDA_LOW;
        }     
     da=da<<1;
     SCL_HIGH;
    }
    SCL_LOW;
    SCL_HIGH;
}
      

void write_i(uint8_t ins)
{
   start();
   write_w(0x78);
   write_w(0x00);
   write_w(ins);
   stop();
}


void write_d(uint8_t dat)
{
   start();
   write_w(0x78);
   write_w(0x40);
   write_w(dat);
   stop();
}          


void delay(unsigned int t)
{
     while(t>0)
       {
       	  t--;
       }
}


void oledSSD1306Initialization(void)
{
    RES_LOW;
    delay(1000); // 316us
    RES_HIGH;
    delay(1000);
    
    write_i(0xAE);    /*display off*/       
      
    write_i(0x00);    /*set lower column address*/       
    write_i(0x12);    /*set higher column address*/
       
    write_i(0x00);    /*set display start line*/
       
    write_i(0xB0);    /*set page address*/
       
    write_i(0x81);    /*contract control*/
    write_i(0x4f);    /*128*/
       
    write_i(0xA0);    /*set segment remap*/
      
    write_i(0xA6);    /*normal / reverse*/
       
    write_i(0xA8);    /*multiplex ratio*/
    write_i(0x1F);    /*duty = 1/32*/
       
    write_i(0xC8);    /*Com scan direction*/
       
    write_i(0xD3);    /*set display offset*/
    write_i(0x00);
       
    write_i(0xD5);    /*set osc division*/
    write_i(0x80);    
       
    write_i(0xD9);    /*set pre-charge period*/
    write_i(0xf1);
       
    write_i(0xDA);    /*set COM pins*/
    write_i(0x12);
       
    write_i(0xdb);    /*set vcomh*/
    write_i(0x40);
       
    write_i(0x8d);    /*set charge pump enable*/
    write_i(0x14);
       
    write_i(0xAF);    /*display ON*/                           

}


void oledFill(uint8_t dat1,uint8_t dat2)
{
     uint8_t i,j;


	write_i(0x00);//Lower Column Address
	write_i(0x12);//Higher Column Address
    write_i(0xB0);    //set page address

    for(j=0;j<6;j++)
    {
        write_i(0xB0+j);    	//set page address
        write_i(0x00);//Lower Column Address
        write_i(0x12);//Higher Column Address
        for(i=0;i<96;i++)
        {
            write_d(dat1);
            write_d(dat2);
        }
    }

    delay(60000);
}


void oledPowerOn(void)
{
    GPIO_SetActive(oledPowerControlPort, oledPowerControlPin);
}

void oledPowerOff(void)
{
    GPIO_SetInactive(oledPowerControlPort, oledPowerControlPin);
}


void oledInit(GPIO_PORT oled_PowerControlPort,GPIO_PIN oled_PowerControlPin,
              GPIO_PORT oled_I2CPort, GPIO_PIN oled_I2CSCL, GPIO_PIN oled_I2CSDA,
              GPIO_PORT reset_Port, GPIO_PIN reset_Pin)
{
    oledPowerControlPort = oled_PowerControlPort;
    oledPowerControlPin = oled_PowerControlPin;
    
    resetPort = reset_Port;
    resetPin = reset_Pin;
    
    oledI2CPort = oled_I2CPort;
    oledI2CSCL = oled_I2CSCL;
    oledI2CSDA = oled_I2CSDA;
    
    GPIO_ConfigurePin(oled_PowerControlPort, oled_PowerControlPin, OUTPUT, PID_GPIO, false);
    
    GPIO_ConfigurePin(reset_Port, reset_Pin, OUTPUT, PID_GPIO, false);
    
	//GPIO_ConfigurePin(oled_I2CPort, oled_I2CSCL, INPUT, PID_I2C_SCL, false);
    //GPIO_ConfigurePin(oled_I2CPort, oled_I2CSDA, INPUT, PID_I2C_SDA, false);
    
    GPIO_ConfigurePin(oled_I2CPort, oled_I2CSCL, OUTPUT, PID_GPIO, false);
    GPIO_ConfigurePin(oled_I2CPort, oled_I2CSDA, OUTPUT, PID_GPIO, false);
    
    oledPowerOn();

    oledSSD1306Initialization();
}







