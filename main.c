/*
 * File:   main.c
 * Author: engmo
 *
 * Created on November 7, 2018, 1:00 PM
 */


#include <xc.h>
#include "ADC.h"

#define _XTAL_FREQ 48000000

#define s_D LATB0     //output of the 7 segment in D 
#define s_C LATB1     //output of the 7 segment in C 
#define s_DP LATB2     //output of the 7 segment in DP 
#define s_B LATB3     //output of the 7 segment in B 
#define s_G LATB4     //output of the 7 segment in G
#define s_E LATB5     //output of the 7 segment in E 
#define s_F LATB6     //output of the 7 segment in F 
#define s_A LATB7     //output of the 7 segment in A  

#define READY LATAbits.LATA5     //output of the ready signal to plc 
#define FAULT LATAbits.LATA4     //output of the fault signal to plc  
#define COMPLETE LATAbits.LATA3     //output of the complete signal to plc  


float atmosphericPressure = 101.325;
float thresholdPressure , inletPressure , cylinderPressure , readValue ;
int count;

void sevenSegmentDisp(int num);

/*
 transfer function
 * 
 * Vout = VS*(0.0012858*P+0.04)
 * 
 * p = ((vout/vs)-.04)/0.0012858 
 */

void main(void) {
    
    //First Configure Digital Ports 
    // 7 segment
    TRISBbits.TRISB0=0;  //Set as output pin (D)
    TRISBbits.TRISB1=0;  //Set as output pin (C)
    TRISBbits.TRISB2=0;  //Set as output pin (DP)
    TRISBbits.TRISB3=0;  //Set as output pin (B)
    TRISBbits.TRISB4=0;  //Set as output pin (G)
    TRISBbits.TRISB5=0;  //Set as output pin (E)
    TRISBbits.TRISB6=0;  //Set as output pin (F)
    TRISBbits.TRISB7=0;  //Set as output pin (A)
    
    // plc signal
    TRISAbits.TRISA5=0;  //Set as output pin (READY)
    TRISAbits.TRISA4=0;  //Set as output pin (FAULT)
    TRISAbits.TRISA3=0;  //Set as output pin (COMPLETE)
    
    
    // initilize the virables
    READY = 1;
    FAULT = 0;
    COMPLETE = 1;
    __delay_ms(1000);
    READY = 0;
    FAULT = 1;
    COMPLETE = 0;
    readValue = 0;
    thresholdPressure = 0;
    inletPressure = 0;
    cylinderPressure = 0;
    count = 6;
    sevenSegmentDisp(-1);
    
    while(1){
        // read the value from the pot
        configurePotAnalogueInput();
        __delay_ms(5);
        readValue = readADC();
        /* 
         * p = ((vout/vs)-.04)/0.0012858) 
         */
        thresholdPressure = ((readValue/256)-.04)/0.0012858;
       
        
        // read the value from the inlet pressure
        configureInletAnalogueInput();
        __delay_ms(5);
        readValue = readADC();
        inletPressure = ((readValue/256)-.04)/0.0012858;
        
        // if the inlet pressure > pot value send READY signal to the plc 
        if( inletPressure > thresholdPressure ){
            READY  = 1;
        }else{
            READY = 0;
            count = 6;
            sevenSegmentDisp(-1);
        }
        // red the value from the cylinder pressure
        configureCylinderAnalogueInput();
            __delay_ms(5);
            readValue = readADC();
            cylinderPressure = ((readValue/256)-.04)/0.0012858;
            // if the cylinder value is bigger than the pot count down
          
            if(cylinderPressure < inletPressure && cylinderPressure > atmosphericPressure){
                FAULT = 0;
            }else{
                FAULT = 1;
            }
        if(READY){
             if( cylinderPressure > thresholdPressure ){
                count = count -1;
                sevenSegmentDisp(count);
            }else{
                COMPLETE = 0;
                count = 6;
            }
            if(count == 0){
                COMPLETE = 1;
            }
            // delay for the rest of the second
            __delay_ms(955);
        }
    }
    
    return;
}


void sevenSegmentDisp(int num){
        switch(num){
            case 5:
                //A,F,G,C,D is on and B,E,DP is off
                s_A = 1; s_F = 1; s_G = 1; s_C = 1; s_D = 1; s_B = 0; s_E = 0; s_DP = 0;
                break;
            case 4:
                //F,G,B,C is on and A,E,d,DP is off
                s_A = 0; s_F = 1; s_G = 1; s_C = 1; s_D = 0; s_B = 1; s_E = 0; s_DP = 0;
                break;     
            case 3:
                //A,B,C,G,D is on 
                s_A = 1; s_F = 0; s_G = 1; s_C = 1; s_D = 1; s_B = 1; s_E = 0; s_DP = 0;
                break;
            case 2:
                //A,B,G,E,D is on
                s_A = 1; s_F = 0; s_G = 1; s_C = 0; s_D = 1; s_B = 1; s_E = 1; s_DP = 0;
                break;
            case 1:
                //B,C is on
                s_A = 0; s_F = 0; s_G = 0; s_C = 1; s_D = 0; s_B = 1; s_E = 0; s_DP = 0;
                break;
            case 0:
                //G,DP is off
                s_A = 1; s_F = 1; s_G = 0; s_C = 1; s_D = 1; s_B = 1; s_E = 1; s_DP = 0;
                break; 
            default:
                //all is of
                s_A = 0; s_F = 0; s_G = 0; s_C = 0; s_D = 0; s_B = 0; s_E = 0; s_DP = 0;
        }
    }