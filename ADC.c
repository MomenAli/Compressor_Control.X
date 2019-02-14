///////////////////////////////////////
//
//  EAT240 ADC Library Functions 
//
//  Written by Dr Mike Knowles
//  October 2017
//
//  Target Device - PIC18F2455 (ECIO)
//
///////////////////////////////////////

#include<xc.h>
#define _XTAL_FREQ 48000000

///////////////////////////////////////
// configureAnalogueInput
//
// this function will set up pin A0
// to read analogue values
///////////////////////////////////////
/*
 bit 5-2 CHS3:CHS0: Analog Channel Select bits
0000 = Channel 0 (AN0)
0001 = Channel 1 (AN1)
0010 = Channel 2 (AN2)
 */

void configurePotAnalogueInput()
{
    //Set TRIS for input
    TRISAbits.TRISA0=1;
    //Configure analog pins, voltage reference and digital I/O (ADCON1)
    //Value is 0000 1110
    ADCON1=14;
    //Select A/D input channel (ADCON0)
    //Value is 0000 0000
    ADCON0=0;
    //Select A/D acquisition time (ADCON2)
    //Select A/D conversion clock (ADCON2)
    ADCON2=0b00110110;
    //Turn on A/D module (ADCON0)
    ADCON0bits.ADON=1;
    __delay_ms(10);
    
}

void configureInletAnalogueInput()
{
    //Set TRIS for input
    TRISAbits.TRISA1=1;
    //Configure analog pins, voltage reference and digital I/O (ADCON1)
    //Value is 0000 1110
    ADCON1=14;
    //Select A/D input channel (ADCON0)
    //Value is 0000 0100
    ADCON0=4;
    //Select A/D acquisition time (ADCON2)
    //Select A/D conversion clock (ADCON2)
    ADCON2=0b00110110;
    //Turn on A/D module (ADCON0)
    ADCON0bits.ADON=1;
    __delay_ms(10);
    
}


void configureCylinderAnalogueInput()
{
    //Set TRIS for input
    TRISAbits.TRISA2=1;
    //Configure analog pins, voltage reference and digital I/O (ADCON1)
    //Value is 0000 1110
    ADCON1=14;
    //Select A/D input channel (ADCON0)
    //Value is 0000 1000
    ADCON0=8;
    //Select A/D acquisition time (ADCON2)
    //Select A/D conversion clock (ADCON2)
    ADCON2=0b00110110;
    //Turn on A/D module (ADCON0)
    ADCON0bits.ADON=1;
    __delay_ms(10);
    
}

///////////////////////////////////////
// readADC()
//
// this function will read an analogue
// value and return it as an int
///////////////////////////////////////
int readADC()
{
    int result;

    //Begin ADC Read
    ADCON0bits.GO_DONE=1;
    while(ADCON0bits.GO_DONE!=0);
    result=ADRESH+1;

    return result;
}

