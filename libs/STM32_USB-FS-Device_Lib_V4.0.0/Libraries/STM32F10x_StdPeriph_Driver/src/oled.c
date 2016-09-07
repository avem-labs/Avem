#include "oled.h"
#include "bit.h"
#include "stm32f10x.h"


void delay_oled(volatile unsigned int nus) {
    for(nus *= 4; nus; nus--);
}


void oled_portInit() {
    RCC->APB2ENR |= 1<<3;       //GPIOB enable
    RCC->APB2ENR |= 1;          //AFIO enable
    AFIO->MAPR &= 0xF8FFFFFF;   //Clean SWJ_CFG[2:0]
    AFIO->MAPR |= 0x04000000;   //Set SWJ_CFG[2:0] = 100


    GPIOB->CRL &= 0x00000FFF;
    GPIOB->CRL |= 0x33333FFF;

    SDIN_OLED = 0;
    SCLK_OLED = 0;
    DC_OLED = 0;
    RST_OLED = 0;
    CS_OLED = 0;

}

void oled_sendData(unsigned char dat) {
    CS_OLED = 0;
    DC_OLED = 0;

    unsigned char i;
    for(i = 0; i < 8; i++) {
        SDIN_OLED = (dat&0x80)>>7;
        dat <<= 1;
        delay_oled(10);
        SCLK_OLED = 1;
        delay_oled(10);
        SCLK_OLED = 0;
    }
    delay_oled(10);
    CS_OLED = 1;
}

void oled_sendCmd(unsigned char cmd) {
    CS_OLED = 0;
    DC_OLED = 1;

    unsigned char i;
    for(i = 0; i < 8; i++) {
        SDIN_OLED = (cmd&0x80)>>7;
        cmd <<= 1;
        delay_oled(10);
        SCLK_OLED = 1;
        delay_oled(10);
        SCLK_OLED = 0;
    }
    delay_oled(10);
    CS_OLED = 1;
}

void oled_init() {

    oled_portInit();

    RST_OLED = 1;
    delay_oled(10);
    RST_OLED = 0;
    delay_oled(10);
    RST_OLED = 1;

    //Command Lock
    oled_sendCmd (0xFD);
    oled_sendData (0x12);

    //Command Lock
    oled_sendCmd (0xFD);
    oled_sendData (0xB1);

    //Set Sleep Mode On
    oled_sendCmd (0xAE);

    //Set Display Clock Divide Ratio/Oscillator Frequency
    oled_sendCmd (0xB3);
    oled_sendData (0xF1);

    //Set Multiplex Ratio
    oled_sendCmd (0xCA);
    oled_sendData (0x7F);

    //Set Display Offset
    oled_sendCmd (0xA2);
    oled_sendData (0x00);

    //Set Display Start Line
    oled_sendCmd (0xA1);
    oled_sendData (0x00);

    //Set Re-Map & Color Depth
    oled_sendCmd (0xA0);
    oled_sendData (0x74);

    //Set GPIO
    oled_sendCmd (0xB5);
    oled_sendData (0x00);


    //Function Selection
    oled_sendCmd (0xAB);
    oled_sendData (0x01);

    //Set Segment Low Voltage
    oled_sendCmd (0xB4);
    oled_sendData (0xA0);
    oled_sendData (0xB5);
    oled_sendData (0x55);

    //Set Contrast Current
    oled_sendCmd (0xC1);
    oled_sendData (0xC8);
    oled_sendData (0x80);
    oled_sendData (0xC8);

    //Set Master Current Control
    oled_sendCmd (0xC7);
    oled_sendData (0x0F);

    //Gamma Look up Table
    oled_sendCmd (0xB8);
    oled_sendData (0x02);
    oled_sendData (0x03);
    oled_sendData (0x04);
    oled_sendData (0x05);
    oled_sendData (0x06);
    oled_sendData (0x07);
    oled_sendData (0x08);
    oled_sendData (0x09);
    oled_sendData (0x0A);
    oled_sendData (0x0B);
    oled_sendData (0x0C);
    oled_sendData (0x0D);
    oled_sendData (0x0E);
    oled_sendData (0x0F);
    oled_sendData (0x10);
    oled_sendData (0x11);
    oled_sendData (0x12);
    oled_sendData (0x13);
    oled_sendData (0x15);
    oled_sendData (0x17);
    oled_sendData (0x19);
    oled_sendData (0x1B);
    oled_sendData (0x1D);
    oled_sendData (0x1F);
    oled_sendData (0x21);
    oled_sendData (0x23);
    oled_sendData (0x25);
    oled_sendData (0x27);
    oled_sendData (0x2A);
    oled_sendData (0x2D);
    oled_sendData (0x30);
    oled_sendData (0x33);
    oled_sendData (0x36);
    oled_sendData (0x39);
    oled_sendData (0x3C);
    oled_sendData (0x3F);
    oled_sendData (0x42);
    oled_sendData (0x45);
    oled_sendData (0x48);
    oled_sendData (0x4C);
    oled_sendData (0x50);
    oled_sendData (0x54);
    oled_sendData (0x58);
    oled_sendData (0x5C);
    oled_sendData (0x60);
    oled_sendData (0x64);
    oled_sendData (0x68);
    oled_sendData (0x6C);
    oled_sendData (0x70);
    oled_sendData (0x74);
    oled_sendData (0x78);
    oled_sendData (0x7D);
    oled_sendData (0x82);
    oled_sendData (0x87);
    oled_sendData (0x8C);
    oled_sendData (0x91);
    oled_sendData (0x96);
    oled_sendData (0x9B);
    oled_sendData (0xA0);
    oled_sendData (0xA5);
    oled_sendData (0xAA);
    oled_sendData (0xAF);
    oled_sendData (0xB4);
    oled_sendData (0x00);

    //Set Phase Length
    oled_sendCmd (0xB1);
    oled_sendData (0x32);


    //Enhance Driving Scheme Capability
    oled_sendCmd (0xB2);
    oled_sendData (0xA4);
    oled_sendData (0x00);
    oled_sendData (0x00);

    //Set Pre-Charge Voltage
    oled_sendCmd (0xBB);
    oled_sendData (0x17);

    //Set Second Pre-Charge Period
    oled_sendCmd (0xB6);
    oled_sendData (0x01);

    //Set VCOMH Voltage
    oled_sendCmd (0xBE);
    oled_sendData (0x05);

    //Set Display Mode
    oled_sendCmd (0xA6);

    //Clear Screen
    fill_ram (0x03, 0x00);

    //Set Sleep Mode Off
    oled_sendCmd (0xAF);

}

void fill_ram (unsigned char h, unsigned char l) {
              unsigned char i, c;
              oled_sendCmd (0x15);
              oled_sendData (0x00);
              oled_sendData (0x7F);

              oled_sendCmd (0x75);
              oled_sendData (0x00);
              oled_sendData (0x7F);

              oled_sendCmd (0x5C);

              for (i = 0; i < 128; i++) {
                      for (c = 0; c < 128; c++) {
                                oled_sendData (h);
                                oled_sendData (l);
                      }
              }

}
