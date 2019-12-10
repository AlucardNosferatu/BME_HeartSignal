/*
 * Lesson2_Lab3_TempSensor_AD10_UART_10Hz.c
 *
 *  Created on: 2014-5-27
 *      Author: changquan
 */
#include <msp430G2553.h>
unsigned char voltage;
void Clock_Initial();
void UART_Initial();
void ADC_Initial();

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  DCOCTL=0;
  Clock_Initial();
  UART_Initial();
  ADC_Initial();

  //P1DIR|=LED0;
  //IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
  _BIS_SR(LPM0_bits+GIE);
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
 voltage=(unsigned char)(ADC10MEM>>2);
  while(UCA0STAT&UCBUSY);
  UCA0TXBUF=voltage;
}

void Clock_Initial()
{
	BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
	DCOCTL = CALDCO_1MHZ;
}

void UART_Initial()
{
  P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 104;                            // 1MHz 9600
  UCA0BR1 = 0;                              // 1MHz 9600
  UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}

void ADC_Initial()
{
	 ADC10AE0 |=BIT3;
  ADC10CTL1 = ADC10DIV_3 + INCH_3 + SHS_2 + CONSEQ_2;  // TA trig., rpt, A10
  ADC10CTL0 = SREF_1 + ADC10SHT_3  + ADC10IE + REF2_5V+REFON + ADC10ON;//REF1_5V
  //ADC10AE0 |=0x01;
  _delay_cycles(1000);
  ADC10CTL0 |= ENC;
  TACTL |= TASSEL_2 + MC_1;                  // SMCLK, cont-mode
  CCR0=5000;                                //SampleRate=TACLK/(2*CCR0)
  TACCTL0 |= OUTMOD_4;                       // Toggle on EQU1 (TAR = 0)
}




