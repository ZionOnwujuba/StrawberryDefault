/* SlidePot.cpp
 * Students put your names here
 * Modified: put the date here
 * 12-bit ADC input on ADC1 channel 5, PB18
 */
#include <ti/devices/msp/msp.h>
#include "../inc/Clock.h"
#include "../inc/SlidePot.h"
#define ADCVREF_VDDA 0x000
#define ADCVREF_INT  0x200


void SlidePot::Init(void){
// write code to initialize ADC1 channel 5, PB18
// Your measurement will be connected to PB18
// 12-bit mode, 0 to 3.3V, right justified
// software trigger, no averaging
  
  ADC1->ULLMEM.GPRCM.RSTCTL = 0xB1000003; // 1) reset
  ADC1->ULLMEM.GPRCM.PWREN = 0x26000001;  // 2) activate
  Clock_Delay(24);                        // 3) wait
  ADC1->ULLMEM.GPRCM.CLKCFG = 0xA9000000; // 4) ULPCLK
  ADC1->ULLMEM.CLKFREQ = 7;               // 5) 40-48 MHz
  ADC1->ULLMEM.CTL0 = 0x03010000;         // 6) divide by 8
  ADC1->ULLMEM.CTL1 = 0x00000000;         // 7) mode
  ADC1->ULLMEM.CTL2 = 0x00000000;         // 8) MEMRES
  ADC1->ULLMEM.MEMCTL[0] = 5;             // 9) channel 5 is PB18
  ADC1->ULLMEM.SCOMP0 = 0;                // 10) 8 sample clocks
  ADC1->ULLMEM.CPU_INT.IMASK = 0;         // 11) no interrupt
}


uint32_t SlidePot::In(void){
  // write code to sample ADC1 channel 5, PB18 once
  // return digital result (0 to 4095)
  ADC1->ULLMEM.CTL0 |= 0x00000001;             // 1) enable conversions
  ADC1->ULLMEM.CTL1 |= 0x00000100;             // 2) start ADC
  uint32_t volatile delay=ADC1->ULLMEM.STATUS; // 3) time to let ADC start
  while((ADC1->ULLMEM.STATUS&0x01)==0x01){}    // 4) wait for completion
  return ADC1->ULLMEM.MEMRES[0];               // 5) 12-bit result
}

void SlidePot::InitADC0(void){
// write code to initialize ADC1 channel 5, PB18
// Your measurement will be connected to PB18
// 12-bit mode, 0 to 3.3V, right justified
// software trigger, no averaging
  
  ADC0->ULLMEM.GPRCM.RSTCTL = 0xB1000003; // 1) reset
  ADC0->ULLMEM.GPRCM.PWREN = 0x26000001;  // 2) activate
  Clock_Delay(24);                        // 3) wait
  ADC0->ULLMEM.GPRCM.CLKCFG = 0xA9000000; // 4) ULPCLK
  ADC0->ULLMEM.CLKFREQ = 7;               // 5) 40-48 MHz
  ADC0->ULLMEM.CTL0 = 0x03010000;         // 6) divide by 8
  ADC0->ULLMEM.CTL1 = 0x00000000;         // 7) mode
  ADC0->ULLMEM.CTL2 = 0x00000000;         // 8) MEMRES
  ADC0->ULLMEM.MEMCTL[0] = 5;             // 9) channel 5 is PB24
  ADC0->ULLMEM.SCOMP0 = 0;                // 10) 8 sample clocks
  ADC0->ULLMEM.CPU_INT.IMASK = 0;         // 11) no interrupt
}


uint32_t SlidePot::InADC0(void){
  // write code to sample ADC1 channel 5, PB18 once
  // return digital result (0 to 4095)
  ADC0->ULLMEM.CTL0 |= 0x00000001;             // 1) enable conversions
  ADC0->ULLMEM.CTL1 |= 0x00000100;             // 2) start ADC
  uint32_t volatile delay=ADC0->ULLMEM.STATUS; // 3) time to let ADC start
  while((ADC0->ULLMEM.STATUS&0x01)==0x01){}    // 4) wait for completion
  return ADC0->ULLMEM.MEMRES[0];               // 5) 12-bit result
}


// constructor, invoked on creation of class
// m and b are linear calibration coefficents
SlidePot::SlidePot(uint32_t m, uint32_t b){
   // write this, runs on creation
   this->slope = m;
   this->offset = b;

}

void SlidePot::Save(uint32_t n){
    // write this
    this->data = n;
    this->flag = 1;
}
uint32_t SlidePot::Convert(uint32_t n){
  return ((this->slope*(n))-(this->offset))/4096; // replace this with solution
}
// do not use this function in final lab solution
// it is added just to show you how SLOW floating point in on a Cortex M0+
float SlidePot::FloatConvert(uint32_t input){
  return 0.00048828125*input -0.0001812345;
}

void SlidePot::Sync(void){
  // write this
  while(!(this->flag)){};
  
}


uint32_t SlidePot::Distance(void){  // return distance value (0 to 2000), 0.001cm
  this->flag = 0;
   return Convert(this->data); // replace this with solution
}

