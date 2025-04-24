/*
 * Switch.cpp
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){
    // write this

  IOMUX->SECCFG.PINCM[PA8INDEX] = 0x00040081; //Jump button
    IOMUX->SECCFG.PINCM[PB4INDEX] = 0x00040081; // hit button
  IOMUX->SECCFG.PINCM[PB20INDEX] = 0x00040081; // block
  IOMUX->SECCFG.PINCM[PA28INDEX] = 0x00040081;  // special move

      IOMUX->SECCFG.PINCM[PB13INDEX] = 0x00040081; //Jump button
    IOMUX->SECCFG.PINCM[PB12INDEX] = 0x00040081; // hit button
  IOMUX->SECCFG.PINCM[PA12INDEX] = 0x00040081; // block
  IOMUX->SECCFG.PINCM[PB19INDEX] = 0x00040081;  // special move

  GPIOA->POLARITY15_0 = (1<<(8<<1)) +  (1<<(13<<1)) + (1<<(10<<1));        // PA8 rising
  GPIOA->POLARITY31_16 = (1<<((28-16)<<1)); // PA28 rising
  GPIOB->POLARITY15_0 = (1<<(4<<1))+(1<<(13<<1)) + (1<<(12<<1));       // PB1 rising
   GPIOB->POLARITY31_16 = (1<<((19-16)<<1)); // PA28 rising


  GPIOA->CPU_INT.ICLR = (1<<8)|(1<<28);  // clear bit 8,28
  GPIOB->CPU_INT.ICLR = (1<<4)|(1<<13)|(1<<19)|(1<<12); // clear bit 1,4

  GPIOA->CPU_INT.IMASK = (1<<8)|(1<<28);  // arm PA8 and PA28
  GPIOB->CPU_INT.IMASK = (1<<4)|(1<<13)|(1<<19)|(1<<12); // arm PB1 and PB4


  NVIC->IP[0] = (NVIC->IP[0]&(~0x0000FF00))|2<<14; // set priority (bits 15,14) IRQ 1
  NVIC->ISER[0] = 1 << 1; // Group1 interrupt
}
// return current state of switches
uint32_t Block_P2(void){
    // write this
  uint32_t data = GPIOA->DIN31_0;
  data = (data>>12)&(1);
  return data; // return 0; //replace this your code
}

uint32_t Block_P1(void){
    // write this
  uint32_t data = GPIOB->DIN31_0;
  data = (data>>20)&(1);
  return data; // return 0; //replace this your code
}
