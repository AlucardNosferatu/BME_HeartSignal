#include <msp430.h> 
#include <msp430g2553.h>
#include <intrinsics.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <msp430.h>

#define LCD_5110_DIR            P2DIR
#define LCD_5110_OUT		    P2OUT
#define LED0 BIT0
#define LED1 BIT6

#define   LCD_RST    BIT4    //P2.4
#define   LCD_CE     BIT3   //P2.3
#define   LCD_DC     BIT2   //P2.2
#define   LCD_DIN    BIT1   //P2.1
#define   LCD_CLK    BIT0   //P2.0
#define   LCD_BL	 BIT5   //P2.5

void LCD_init(void);
void LCD_clear(void);
void LCD_write_byte(unsigned char dat, unsigned char dc);
void LCD_write_char(unsigned char c);
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s);
unsigned char electrocardio;  //�޸ı������� ��electrocardio ��� ԭ������ temp
void Clock_Initial();
void UART_Initial();
void ADC_Initial();
int count = 1;
volatile int heartrate = 0;
int electrocardio1 = 0;


unsigned char HR;
char HRstr[3];
const unsigned char font6x8[][6] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
    { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
    { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
    { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
    { 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
    { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};


/*-----------------------------------------------------------------------
LCD_init          : 5110LCD��ʼ��
-----------------------------------------------------------------------*/
void LCD_init(void)
  {
   LCD_5110_DIR |= LCD_RST + LCD_CE + LCD_DC + LCD_DIN + LCD_CLK + LCD_BL;
   LCD_5110_OUT &= ~LCD_BL;

   LCD_5110_OUT &= ~LCD_RST;// ����һ����LCD��λ�ĵ͵�ƽ����,LCD_RST = 0;
   _delay_cycles(10);
   LCD_5110_OUT |= LCD_RST; // LCD_RST = 1;

   /*
   LCD_5110_OUT &= ~ LCD_CE;// enable LCD ,LCD_CE = 0;
   _delay_cycles(10);
   LCD_5110_OUT |= LCD_CE;// disable LCD,LCD_CE = 1;
   _delay_cycles(10);
   */

   LCD_write_byte(0x21, 0);	// ʹ����չ��������LCDģʽ
   LCD_write_byte(0xc8, 0);	// ���ù�����ѹ
   LCD_write_byte(0x06, 0);	// �¶�ϵ��
   LCD_write_byte(0x12, 0);	// ƫ�õ�ѹ
   LCD_write_byte(0x20, 0);	// ʹ�û������

   LCD_clear();	        // ����

   LCD_write_byte(0x0c, 0);	// �趨��ʾģʽ��������ʾ
   //LCD_write_byte(0x0d, 0);	// �趨��ʾģʽ����ת��ʾ

  }

/*-----------------------------------------------------------------------
LCD_clear         : LCD��������
-----------------------------------------------------------------------*/
void LCD_clear(void)
  {
    unsigned int i;

    LCD_write_byte(0x0c, 0);
    LCD_write_byte(0x80, 0);

    for (i=0; i<504; i++)
      LCD_write_byte(0, 1);
  }

/*-----------------------------------------------------------------------
LCD_set_XY        : ����LCD���꺯��

���������X       ��0��83
       Y       ��0��5
-----------------------------------------------------------------------*/
void LCD_set_XY(unsigned char X, unsigned char Y)
  {
    LCD_write_byte(0x40 | Y, 0);		// column
    LCD_write_byte(0x80 | X, 0);          	// row
  }

/*-----------------------------------------------------------------------
LCD_write_char    : ��ʾӢ���ַ�

���������c       ����ʾ���ַ���
-----------------------------------------------------------------------*/
void LCD_write_char(unsigned char c)
  {
    unsigned char line;

   c -= 32;

    for (line=0; line<6; line++)
      LCD_write_byte(font6x8[c][line], 1);
  }


/*-----------------------------------------------------------------------
LCD_write_english_String  : Ӣ���ַ�����ʾ����

���������*s      ��Ӣ���ַ���ָ�룻
          X��Y    : ��ʾ�ַ�����λ��,x 0-83 ,y 0-5
-----------------------------------------------------------------------*/
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s)
  {
    LCD_set_XY(X,Y);
    while (*s)
      {
	 LCD_write_char(*s);
	 s++;
      }
  }


/*-----------------------------------------------------------------------
LCD_write_byte    : ʹ��SPI�ӿ�д���ݵ�LCD

���������data    ��д������ݣ�
          command ��д����/����ѡ��

-----------------------------------------------------------------------*/
void LCD_write_byte(unsigned char dat, unsigned char command)
  {
    unsigned char i;
	LCD_5110_OUT &= ~LCD_CE;	// enable LCD, chip selection

    if (command == 0)
    	LCD_5110_OUT &= ~LCD_DC;// LCD_DC = 0;��������
    else
    	LCD_5110_OUT |= LCD_DC;// LCD_DC = 1;��������
		for(i=0;i<8;i++)
		{
			if(dat&0x80)
			   LCD_5110_OUT |= LCD_DIN; //DIN = 1;
			else
			   LCD_5110_OUT &= ~LCD_DIN;//DIN = 0;

			LCD_5110_OUT &= ~LCD_CLK;//CLK = 0;
			dat = dat << 1;
			LCD_5110_OUT |= LCD_CLK;//CLK = 1;
		}
	 LCD_5110_OUT |= LCD_CE; // disable LCD, chip selection
  }








#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{

  electrocardio=(unsigned char)(ADC10MEM>>2);   // �޸ı������ƣ� ��LSB����λ��ȥ
  while(UCA0STAT&UCBUSY);

   if(electrocardio >170 )    //if �����ô�ж��������ϵ�����
{
   if(electrocardio > electrocardio1)
    {
      heartrate = 1000/count;
      count = 0;
}
}
  count++;

  electrocardio1 = electrocardio;

 //ʹ��������ʵ�ʱ��LED����
  P1OUT = 0;
if(count == 1)
{
  // UCA0TXBUF = heartrate;
   P1OUT = BIT0;

}

//HR = heartrate;
/*if(heartrate>50)
{
	if(heartrate<130){
		HR = heartrate;
   sprintf(HRstr,"%d",HR);
   LCD_write_english_string(20,2,HRstr);
	}
}*/
     HR = heartrate;
   sprintf(HRstr,"%d",HR);
   LCD_write_english_string(20,2,HRstr);

 UCA0TXBUF=electrocardio;    // �޸ı�����

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
	ADC10AE0 = BIT3;
  ADC10CTL1 = ADC10DIV_3 + INCH_3 + SHS_2 + CONSEQ_2;  // TA trig., rpt, A10 �޸���channel ʹ���ĵ��źŴ�P1.3����
  ADC10CTL0 = SREF_1 + ADC10SHT_3  + ADC10IE + REFON + ADC10ON +REF2_5V;//�޸�REF Ϊ2.5V ��Ϊ�Ŵ���Ϊ400+ ʹ���ĵ��źŵķ�ֵ��Ϊ 2+V
 // ADC10CTL0 = SREF_3 + SREF_4 +ADC10SHT_3 + ADC10IE + ADC10ON;  //�� ��Դ�����ӵ�Ƭ����P1.4 ��Դ�����ӵ�Ƭ�ڵ�P1.3  ���ı�VREF��ֵ
  _delay_cycles(1000);
  ADC10CTL0 |= ENC;
  TACTL |= TASSEL_2 + MC_1;                  // SMCLK, cont-mode
  CCR0=1000;                                //SampleRate=TACLK/(2*CCR0)  ����CCRO��ֵ���޸�ȡ����Ϊ500HZ
  TACCTL0 |= OUTMOD_4;                       // Toggle on EQU1 (TAR = 0)
}



void main(void)
{ WDTCTL = WDTPW + WDTHOLD;
   BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
   DCOCTL = CALDCO_1MHZ;

   UART_Initial();
     ADC_Initial();
     P1DIR|=LED0;

   LCD_init();                           //��ʼ��Һ��
   LCD_clear();

   LCD_write_english_string(18,5,"BME@SUSTech");
   LCD_5110_OUT |=LCD_BL;
   LCD_write_english_string(0,0,"Heart Rate:");




  // DCOCTL=0;
   /*UART_Initial();
   ADC_Initial();
   P1DIR|=LED0;
  //IE2 |= UCA0RXIE;    */                      // Enable USCI_A0 RX interrupt
  _BIS_SR(LPM0_bits+GIE);

}

