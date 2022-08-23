#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "inc/hw_memmap.h"

void switch_interrupt(void);
void TOGGLED(void);

int main()
{
  
  //Priority setting
  
  IntPrioritySet(INT_TIMER0A,0x00);
  IntPrioritySet(INT_GPIOF,0xE0);
  
   //Clock Portf
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
  GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4 , GPIO_STRENGTH_2MA , GPIO_PIN_TYPE_STD_WPU);
  
  //Timer0
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)){};
  TimerDisable(TIMER0_BASE, TIMER_A);
  TimerIntDisable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
  TimerConfigure(TIMER0_BASE, (TIMER_CFG_16_BIT | TIMER_TAMR_TAMR_PERIOD));
  TimerLoadSet(TIMER0_BASE, TIMER_A, (16000*250-1));
  TimerIntRegister(TIMER0_BASE,TIMER_A,TOGGLED);
  TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
  TimerEnable(TIMER0_BASE, TIMER_A);
  
  
  //interrupt portf
  GPIOIntDisable(GPIO_PORTF_BASE,GPIO_INT_PIN_4);
  GPIOIntRegister (GPIO_PORTF_BASE, switch_interrupt);
  GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_FALLING_EDGE);
  GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);


  while(1)
  {
   
  }
}


void switch_interrupt(void)
{
  GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_4);
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,GPIO_PIN_2);
  while((GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4) & 0x10) == 0){};
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,0);
  
}
void TOGGLED(void){
  GPIO_PORTF_DATA_R ^= 0x02;
  TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
  
  }