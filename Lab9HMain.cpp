// Lab9HMain.cpp
// Runs on MSPM0G3507
// Lab 9 ECE319H
// Your name
// Last Modified: 12/26/2024

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/SlidePot.h"
#include "../inc/DAC5.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "images/images.h"
#include "Sprite.h"
extern "C" void __disable_irq(void);
extern "C" void __enable_irq(void);
extern "C" void TIMG12_IRQHandler(void);
extern "C" void TIMG6_IRQHandler(void);
extern "C" void GROUP1_IRQHandler(void);
// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void){ // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}


uint32_t M=1;
bool move = false;
int x = 0;
int y = 109;
int vy = 0;
int vy2 = 0;
int block1Last = 0;
int block2Last = 0;
bool switch1On = false;
bool switch2On = false;
int prevResult = 0;
int startY = 109;
bool testStats = false;
int timeRemaining = 200;
int timeX = 9;
int timeIndex = 0;
Sprite s1(0, startY, 33, 30, 0, 8, StrawberryBMPDefault);
Sprite s2(50, startY, 33, 30, 0, 10, BananaBMPDefault);
Sprite health1(0, 40, 16, 10, 16, 10, SmallEnemy20pointA);
int prevS1Health = s1.health;
int prevS2Health = s2.health;
uint32_t Random32(void){
  M = 1664525*M+1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32()>>16)%n;
}

SlidePot Sensor(1939,22); // copy calibration from Lab 7
SlidePot Sensor2(1939,22); // copy calibration from Lab 7
void intToChar(int num){
  int length = 0;
  int numCpy = num;
  while(numCpy != 0){
    numCpy /= 10;
    length++;
  }
  int* charArr = new int[length];
 for(int i = length-1; i > -1; i--){
    charArr[i] = (num % 10);
    num /= 10;
  }
  for(int i = 0; i < length; i++){
    ST7735_OutChar(charArr[i]+48);
  }
  delete []charArr;
}
// games  engine runs at 30Hz
void TIMG12_IRQHandler(void){uint32_t pos,msg, now1, now2;
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
// game engine goes here
    // 1) sample slide pot
    //Sensor.Save(Sensor.In());
    int charArr[4] = {0, 0, 0, 0};
    int Data = Sensor.In();
    int Data2 = Sensor2.InADC0();
    
    for(int i = 0; i < 100; i++){
    Data += Sensor.In();
     Data2 += Sensor2.InADC0();
  }

  int result = Sensor.Convert((Data/100));
  int result2 = Sensor2.Convert((Data2/100));


  int32_t convertedResult = ((result/(475))-2);
  int32_t convertedResult2 = ((result2/(475))-2);
if(testStats){
  ST7735_SetCursor(0, 0);
  char *pt = "Velocity: ";
  ST7735_OutString(pt);
  if(convertedResult < 0){
    ST7735_OutChar('-');
    int negativeResult = -(convertedResult);
    ST7735_OutChar(negativeResult+48);
  } else {
    ST7735_OutChar('+');
  ST7735_OutChar(convertedResult+48);
  }
  ST7735_SetCursor(0, 1);
  char *pt2 = "ADC Position: ";
  ST7735_OutString(pt2);

intToChar(result);
ST7735_SetCursor(0, 2);
ST7735_OutString("S1 Health: ");
intToChar(s1.health*10);
ST7735_SetCursor(0, 3);
ST7735_OutString("S2 Health: ");
intToChar(s2.health*10);
}



 

  if(s1.y < (109 - (s1.h/2) - 5)){
    vy = 1;
  } else if (s1.y == startY + 1 && switch1On){
    s1.y = startY;
    vy = -1;
    switch1On = false;
  }

    if(s2.y < (109 - (s2.h/2) - 5)){
    vy2 = 1;
  } else if (s2.y == startY + 1 && switch2On){
    s2.y = startY;
    vy2 = -1;
    switch2On = false;
  }
  //Health
  if(prevS1Health != s1.health){
ST7735_FillRect(0,27,43,16,ST7735_BLUE);
 ST7735_FillRect(5,30,(40*(s1.health))/10,10,ST7735_WHITE);
 prevS1Health = s1.health;
  }
   
if(prevS2Health != s2.health){
    ST7735_FillRect(80,27,43,16,ST7735_MAGENTA);
 ST7735_FillRect(83+(40*(10-s2.health))/10,30,(40*(s2.health))/10,10,ST7735_WHITE);
//health1.Place((health1.w*(s2.health))/10, health1.h);
prevS2Health = s2.health;

}

  //Blocking
now1 = Block_P1();
now2 = Block_P2();


s1.Block(StrawberryBMPDefault,now1 == 1);
s2.Block(BananaBMPDefault,now2 == 1);



  //Move 
   s1.Move(convertedResult, vy, switch1On, StrawberryBMPDefault, s2);
  s2.Move(convertedResult2, vy2, switch2On, BananaBMPDefault, s1);


//Time
timeIndex++;
if(timeRemaining == 0){
  timeIndex = 0;
}
if(timeIndex == 30){
  timeIndex = 0;
  timeRemaining--;
  if(timeRemaining == 9){
    timeX++;
  }
  ST7735_FillRect(47,27,25,20,ST7735_BLACK);
  ST7735_SetCursor(timeX,3);
  ST7735_OutUDec(timeRemaining);


}

  
 
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
  }
}

void GROUP1_IRQHandler(void){
  if(GPIOA->CPU_INT.RIS&(1<<8)){ // PA8
    GPIOA->CPU_INT.ICLR = 1<<8;
    switch1On = true;
    vy = -1;
  }
    if(GPIOB->CPU_INT.RIS&(1<<4)){ // PB4
    GPIOB->CPU_INT.ICLR = 1<<4;
    s1.Hit(s2, PlayerShip3);
  }

  if(GPIOA->CPU_INT.RIS&(1<<28)){ // PA28
    GPIOA->CPU_INT.ICLR = 1<<28;

  }

  //Player 2
  if(GPIOB->CPU_INT.RIS&(1<<13)){ // PB13
    GPIOB->CPU_INT.ICLR = 1<<13;
    switch2On = true;
    vy2 = -1;
  }
    if(GPIOA->CPU_INT.RIS&(1<<11)){ // PA11
    GPIOA->CPU_INT.ICLR = 1<<11;
    s2.Hit(s1, SmallEnemy10pointA);
  }

  if(GPIOB->CPU_INT.RIS&(1<<19)){ // PB19
    GPIOA->CPU_INT.ICLR = 1<<19;

  }

}


 
uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}

typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Language_Portuguese[]="Portugu\x88s";
const char Language_French[]="Fran\x87" "ais";
const char *Phrases[3][4]={
  {Hello_English,Hello_Spanish,Hello_Portuguese,Hello_French},
  {Goodbye_English,Goodbye_Spanish,Goodbye_Portuguese,Goodbye_French},
  {Language_English,Language_Spanish,Language_Portuguese,Language_French}
};


// use main1 to observe special characters
int main1(void){ // main1
    char l;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(0x0000);            // set screen to black
  for(int myPhrase=0; myPhrase<= 2; myPhrase++){
    for(int myL=0; myL<= 3; myL++){
         ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
      ST7735_OutChar(' ');
         ST7735_OutString((char *)Phrases[myPhrase][myL]);
      ST7735_OutChar(13);
    }
  }
  Clock_Delay1ms(3000);
  ST7735_FillScreen(0x0000);       // set screen to black
  l = 128;
  while(1){
    Clock_Delay1ms(2000);
    for(int j=0; j < 3; j++){
      for(int i=0;i<16;i++){
        ST7735_SetCursor(7*j+0,i);
        ST7735_OutUDec(l);
        ST7735_OutChar(' ');
        ST7735_OutChar(' ');
        ST7735_SetCursor(7*j+4,i);
        ST7735_OutChar(l);
        l++;
      }
    }
  }
}

// use main2 to observe graphics
int main2(void){ // main2
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_DrawBitmap(22, 159, PlayerShip0, 18,8); // player ship bottom
  ST7735_DrawBitmap(53, 151, Bunker0, 18,5);
  ST7735_DrawBitmap(42, 159, PlayerShip1, 18,8); // player ship bottom
  ST7735_DrawBitmap(62, 159, PlayerShip2, 18,8); // player ship bottom
  ST7735_DrawBitmap(82, 159, PlayerShip3, 18,8); // player ship bottom
  ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
  ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
  ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
  ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16,10);
  ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);

  for(uint32_t t=500;t>0;t=t-5){
    SmallFont_OutVertical(t,104,6); // top left
    Clock_Delay1ms(50);              // delay 50 msec
  }
  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString((char *)"GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString((char *)"Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString((char *)"Earthling!");
  ST7735_SetCursor(2, 4);
  ST7735_OutUDec(1234);
  while(1){
  }
}

// use main3 to test switches and LEDs
int main3(void){ // main3
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  while(1){
    // write code to test switches and LEDs

  }
}
// use main4 to test sound outputs
int main4(void){ uint32_t last=0,now;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(ADC0,6,0); // ADC1 channel 6 is PB20, TExaS scope
  __enable_irq();
  while(1){
    now = Switch_In(); // one of your buttons
    if((last == 0)&&(now == 1)){
      Sound_Shoot(); // call one of your sounds
    }
    if((last == 0)&&(now == 2)){
      Sound_Killed(); // call one of your sounds
    }
    if((last == 0)&&(now == 4)){
      Sound_Explosion(); // call one of your sounds
    }
    if((last == 0)&&(now == 8)){
      Sound_Fastinvader1(); // call one of your sounds
    }
    // modify this to test all your sounds
  }
}
// ALL ST7735 OUTPUT MUST OCCUR IN MAIN
int main5(void){ // final main
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  Sensor.Init(); // PB18 = ADC1 channel 5, slidepot
  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
    // initialize interrupts on TimerG12 at 30 Hz
  
  // initialize all data structures
  __enable_irq();

  while(1){
    // wait for semaphore
       // clear semaphore
       // update ST7735R
    // check for end game or level switch
  }
}



int main(void){ // main6
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  Sensor.Init(); //PB18 = ADC1 channel 5, slidepot
Sensor2.InitADC0();
    // initialize interrupts on TimerG12 at 30 Hz
    TimerG12_IntArm(2666666, 2);

Switch_Init();

  __enable_irq();
  ST7735_SetCursor(timeX,3);
  ST7735_OutUDec(timeRemaining);

  ST7735_FillRect(0,27,46,16,ST7735_BLUE);
  ST7735_FillRect(3,30,(40*(s1.health))/10,10,ST7735_WHITE);

  ST7735_FillRect(80,27,46,16,ST7735_MAGENTA);
  ST7735_FillRect(83,30+(40-(40*(s2.health))/10),(40*(s1.health))/10,10,ST7735_WHITE);

  ST7735_SetCursor(0,2);
  ST7735_OutString("Strawberry");

   ST7735_SetCursor(14,2);
  ST7735_OutString("Banana");

  while(1){
       // complete this
    // wait for semaphore using a call to a method in Sensor


    GPIOB->DOUTTGL31_0 = RED; // toggle PB26 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = RED; // toggle PB26 (minimally intrusive debugging)
    // toggle red LED2 on Port B, PB26
     // convert Data to Position
     int result = Sensor.Distance();
     ST7735_SetCursor(0,0);

   // move cursor to top
   // display distance in top row OutFix


    GPIOB->DOUTTGL31_0 = RED; // toggle PB26 (minimally intrusive debugging)
  }
}