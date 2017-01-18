//********************************************************************
//*                       ������ 64 �������                          *
//*                      (C)  �.������, 2016                         *
//*                   ������ 1.0 �� 22.09.2016                       *
//*                   ������ 1.1 �� 01.11.2016  (PB0)                *
//*                   ������ 2.0 �� 03.11.2016  (����� � EEPROM)     *
//*                   ������ 2.1 �� 10.12.2017  (��� �������)        *
//*                   AVR Studio V4.19 Build 730                     *
//********************************************************************
//������ ���������� ������������ ����� ��������� ��� �� ATtiny2313
//������������ ����� �� ���-������, ����� ������� ����������
//********************************************************************
//������� �� �� ���������� ����������: 8 ���
//Low Fuse:  DE
//High Fuse: DB
//PD0 - ������ SCL � ���������� EEPROM (2K) AT24C16
//PD1 - ������ SDA � ���������� EEPROM (2K) AT24C16
//PD2 - ������� ������ (��������� �� ����� ������)
//PD6 - ��������� ������ ���������� ������ �������
//(��� ������ ��������� - ����� ����������������� �������� �������)
//PB0 - ������������� � ������������ ������� ������� �������
//��������� ������ ��� ��� ����������� � ������� ������ A ������� 1,
//����������� � ������ ���������� �� ���������. ��� ������������
//������ ������� "�������" ������������ ����� B ����� �� �������.
//������ 0 ������������ ��� ��������� ���������� ������ �������.


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#define nm 64 //���������� ������������ �������
#define m_len 32 //���������� ����������� ����� � ������ �������
unsigned char buf [m_len]; //����� ��� ����� �������

#define CLK_HZ 8000000ul //�������� ������� ��
#define T1_PS 8    //����������� ������������ ������� 1
#define T1A_1 CLK_HZ/(2 * T1_PS) //����� ����� ������ A ������� 1 ��� ����������� ������� 1 ��
const unsigned int T1_G  = T1A_1/392; //����� ����� ������� 1 ��� ����������� ������� ���� ����
const unsigned int T1_G_ = T1A_1/415; //����� ����� ������� 1 ��� ����������� ������� ���� ����-���� 
const unsigned int T1_A  = T1A_1/440; //����� ����� ������� 1 ��� ����������� ������� ���� �� 
const unsigned int T1_A_ = T1A_1/466; //����� ����� ������� 1 ��� ����������� ������� ���� ��-���� 
const unsigned int T1_B  = T1A_1/494; //����� ����� ������� 1 ��� ����������� ������� ���� �� 
const unsigned int T1_C  = T1A_1/523; //����� ����� ������� 1 ��� ����������� ������� ���� �� 
const unsigned int T1_C_ = T1A_1/554; //����� ����� ������� 1 ��� ����������� ������� ���� ��-���� 
const unsigned int T1_D  = T1A_1/587; //����� ����� ������� 1 ��� ����������� ������� ���� �� 
const unsigned int T1_D_ = T1A_1/622; //����� ����� ������� 1 ��� ����������� ������� ���� ��-����
const unsigned int T1_E  = T1A_1/659; //����� ����� ������� 1 ��� ����������� ������� ���� �� 
const unsigned int T1_F  = T1A_1/698; //����� ����� ������� 1 ��� ����������� ������� ���� �� 
const unsigned int T1_F_ = T1A_1/740; //����� ����� ������� 1 ��� ����������� ������� ���� ��-���� 

//#define T0_PS 1024 //����������� ������������ ������� 0
#define vibr_strobe 120 //������������ ������ "�������", ��� 
#define vibr_period 50  //������ ���������� ������� "�������", ��
//����� ����� ������ B ������� 1 ��� ������������ ������ "�������"
const unsigned int T1B_0 = (CLK_HZ/1000000ul)*vibr_strobe/T1_PS;
//����� ����� ������ B ������� 1 ��� ���������� ������ "�������"
const unsigned int T1B_1 = (CLK_HZ/1000000ul)*(vibr_period*1000ul-vibr_strobe)/T1_PS;
#define temp0 400  //������������ ����� ������� ��� ��������� �����, ��
#define temp1 250  //������������ ����� ������� ��� ������� �����, ��

volatile unsigned char flag_t; //���� ����� ��������������� ����: 1 - �������, 0 - ���������
volatile unsigned char flag_o; //���� ������ ����: 1 - �� ������ ����� (2*f)
volatile unsigned char flag_v; //���� �������: 1 - � �������, 0 - ��� �������
volatile unsigned char m; //����� ������� 0..63
volatile unsigned char n; //����� ���� 0..31 (����������� ������� ��� �������� ��8 � ����. �����)
volatile unsigned int n1; //��������� ����������� ���� ���� ��� �������� ��������� ������� 1
volatile unsigned char pause = 0;    //������� ��������� �����
volatile unsigned char strobe = 0;   //������� �������� ������ "�������"


//=== ��������� ������� ��������

unsigned char check_jmp (void)
//��������, �������� �� ��������� ���������� ������ �������
{
return (!(PIND & (1 << PD6)));
}


unsigned char check_pressed (void)
//��������, ������ �� ������ ������
{
return (!(PIND & (1 << PD2)));
}


unsigned char safe_check_pressed (void)
//�������� ������� ������ ������ � �������������
{
unsigned char cycle = 5; //����� ������� ������� ������
while ((check_pressed()) && cycle)
  {
  _delay_ms(10); //��������
  cycle--;
  }
return (cycle==0);
}


//=== ���� ������ � EEPROM �� ���� I2C
//��������� ������ SDA �� ��������� - ���� �� � ������� �������

//������������� ������ ���� I2C ��� ���������� ������
#define SCL_PORT     PORTD
#define SCL_PORTD    DDRD
#define SDA_PORT     PORTD
#define SDA_PORTD    DDRD
#define SCL_PORT_INP PIND
#define SDA_PORT_INP PIND
#define SCL	0
#define SDA	1
#define USI_DELAY 5  //�������� ��� ������ �� ���� I2C, ���


void set_scl (void)
//��������� ���� SCL � ��������� �������� ������
{
SCL_PORTD |= (1 << SCL); //����� SCL
SCL_PORT  |= (1 << SCL);
}


void clr_scl (void)
//��������� ���� SDA � ��������� ������� ������
{
SCL_PORTD |= (1 << SCL); //����� SCL
SCL_PORT  &= ~(1 << SCL);
}


void set_sda (void)
//��������� ���� SDA � ��������� �������� ������
{
SDA_PORTD |= (1 << SDA); //����� SDA
SDA_PORT  |= (1 << SDA);
}


void clr_sda (void)
//��������� ���� SDA � ��������� ������� ������
{
SDA_PORTD |= (1 << SDA); //����� SDA
SDA_PORT  &= ~(1 << SDA);
}


void in_sda (void)
//������� ������� SDA � ��������� ����� ��
{
SDA_PORTD &= ~(1 << SDA); //���� SDA
SDA_PORT  |= (1 << SDA);  //������������� �������� ��� SDA
}


unsigned char get_sda (void)
//������ ��������� ������� SDA (��������������, ��� SDA � ������ �����)
{
//in_sda ();
unsigned char b = (SDA_PORT_INP & (1 << SDA)) >> SDA;  //0 ��� 1
return (b);
}


void mem_delay (void)
//������������ �������� ��� ������� � ������ �� ���� I2C
{
_delay_us (USI_DELAY);
}


void mem_start (void)
//������������ ��������� �����
{
set_sda ();
mem_delay ();
set_scl ();
mem_delay ();
clr_sda ();
mem_delay ();
clr_scl ();
mem_delay ();
in_sda ();
}


void mem_stop (void)
//������������ ��������� ����
{
clr_sda ();
mem_delay ();
set_scl ();
mem_delay ();
set_sda ();
mem_delay ();
in_sda ();
}


void mem_ack (void)
//unsigned char mem_ack (void) //0 - ��� �������������, ����� - ����
//������������� �� ������
{
mem_delay ();
set_sda ();
mem_delay ();
in_sda ();
set_scl ();
unsigned char c = 1; //������� ����� ������ ACK
unsigned char i = 4; //������� ������ ������ ACK
while ((i > 0) && (c > 0)) //���� ������ ACK
  {
  i--;
  mem_delay ();
  c = get_sda (); //c = 0 - �������� ������������� ACK, c = 1 - �� ��������
  }
clr_scl ();
mem_delay ();
mem_delay ();
//return((c==0));
}


void mem_wb (unsigned char wb)
//������ ����� � ���� ������ � �������
{
for (unsigned char i=0; i<8; i++)
  {
  if ((wb & (0x80 >> i)) == 0)
     clr_sda ();
  else
     set_sda ();
  mem_delay ();  
  set_scl ();
  mem_delay ();
  clr_scl ();
  mem_delay ();
  }
in_sda ();
}


unsigned char mem_rb (void)
//������ ����� �� ���� ������ � �������
{
unsigned char data = 0;
set_sda ();
in_sda ();
for (unsigned char i=0; i<8; i++)
  {
  set_scl ();
  mem_delay ();  
  mem_delay ();  
  data = (data << 1) | get_sda ();
  clr_scl ();
  mem_delay ();  
  mem_delay ();
  }
return (data);
}


/*
void mem_write (unsigned int adr, unsigned char wb)
//������ ����� � ������ - �� ����������� � ����� �� ������������
{
mem_start ();
unsigned char adrL = adr & 0xFF;
unsigned char adrH = (((adr >> 8) << 1) & (0x0E)) | 0xA0;
mem_wb (adrH);
mem_ack ();
mem_wb (adrL);
mem_ack ();
mem_wb (wb);
mem_ack ();
mem_stop ();
_delay_ms (1); ?
}
*/


unsigned char mem_read (unsigned int adr)
//������ ����� �� ������
{
mem_start ();
unsigned char adrL = adr & 0xFF;
unsigned char adrH = (((adr >> 8) << 1) & (0x0E)) | 0xA0;
mem_wb (adrH);
mem_ack ();
mem_wb (adrL);
mem_ack ();
mem_delay ();
mem_stop ();
mem_delay ();
mem_start ();
adrH = (((adr >> 8) << 1) & (0x0E)) | 0xA0 | 0x01;
mem_wb (adrH);
mem_ack ();
mem_delay ();
unsigned char b = mem_rb ();
mem_ack ();
mem_stop ();
return (b);
}


//=== ��������� ����� �������

void mem_read_buf (unsigned int adr, unsigned char buf[])
//������ m_len ������ �� ������ � �����
{
for (unsigned char i=0; i<m_len; i++)
  buf[i] = mem_read (adr + i);
}


unsigned char random_m (void)
//��������� ���������� ������ �������
{
return (TCNT0 & 0x3F); //��������� ����� �� �������� ������� 0
}


unsigned int melody_addr (unsigned char m)
//���������� ������ ������ 0..(2K-1) �� ������ ������� m=0..63
{
return ((m & 0x3F) << 5); //m * 32
}


void parse_code (unsigned char code)
//������ ���� ���� code �� ������� ������� � ����������� ����������:
//flag_t - ���� �����, flag_o - ���� ������, flag_v - ���� �������, n - ����� ����
{
flag_t = (code & 0x80);
flag_o = (code & 0x40);
flag_v = (code & 0x20);
n = code & 0x1F;
}


unsigned int tone_code (unsigned char flag_o, unsigned char n)
//����������� ���� ���� ���� ��� �������� � ������� ���������� A ������� 1
//������� �� ����� ������� 1 (� ������ ������������) - 1 ���
//������� ���������:
//flag_o - ���� ������� ������
//n - ����� ����
//���� ������������ �������� 0, �� ����������� �����
{
volatile unsigned long k = 0; //���������
if ((n & 0x07) != 0x07)
   //�� �����
   switch (n)
   {
	 case 0: //����
	   k = T1_G;
	   break;
	 case 2: //����#
	   k = T1_G_; 
	   break;
	 case 4: //��
	   k = T1_A; 
	   break;
	 case 6: //��#
	   k = T1_A_; 
	   break;
	 case 8: //��
	   k = T1_B; 
	   break;
	 case 9: //�� - ���� ������������
	   k = T1_B; 
	   break;
	 case 11: //��
	   k = T1_C; 
	   break;
	 case 13: //��#
	   k = T1_C_; 
	   break;
	 case 16: //��
	   k = T1_D; 
	   break;
	 case 19: //��#
	   k = T1_D_; 
	   break;
	 case 22: //��
	   k = T1_E; 
	   break;
	 case 25: //��
	   k = T1_F; 
	   break;
	 case 28: //��#
	   k = T1_F_; 
	   break;
     default: //�������������� ����
	   k = 0;
   } //switch
   if ((flag_o) && (k > 0)) 
      k = (k >> 1); //�� ������ �����: ������������ ��������� � ��� ����
return (k);
}


void play (unsigned char buf[])
//������� ����������� ����� ������� � ������ buf � �� ���������������
{
unsigned char i = 0; //������� ����� �������
TCCR1A =  (1 << COM1A0); //�������� ����� ������� ��� ��������� A ������� 1; Normal mode
while (i < m_len)
  {
  parse_code (buf [i]); //�������� � ���������� ��������� ��� ������� -> flag_t,flag_o,flag_v,n
  //����������� ����� ����� ������ ������ ���������� ����� ���
  unsigned char s = 1; //������� ����� �����
  unsigned char j = i+1; //����� ��������� ����
  while (j < m_len)
    {
	if (buf [i] == buf [j]) //����������� �����
	   { s++; j++; i++; }
    else
	   j = m_len; //����� �����
	} 
  //����� s - ����� ���������� ������ ������ ����� ���
  n1 = tone_code (flag_o,n); //���������� ��������� ����������� ���� ��� ������� 1
  //���������� � ��������� ���� ����
  pause = 1;
  if (n1 > 0) //����, ����� - ����� (� ������ �� ������������)
     {
	 pause = 0;  //����, �� �����
	 strobe = 0; //��� ������ "�������"
     TCCR1B = 0; //������� ������� 1
     OCR1A = TCNT1 + n1; //�������� ����� ��������� � ������� ��������� A ������� 1
     TIMSK &= ~(1 << OCIE1B); //��������� ���������� �� ���������� B ������� 1
	 //��������� ������ "�������", ���� �����
	 if (flag_v)
	    {
		OCR1B = TCNT1 + T1B_1;
        TIMSK  |= (1 << OCIE1B); //��������� ���������� �� ���������� B ������� 1
		}
     TCCR1B |= (1 << CS11); //���� ������� 1 � ������������� 8
     TIMSK  |= (1 << OCIE1A);  //��������� ���������� �� ���������� A ������� 1
	 //�� ����� ���������, ������������ ���������� ���� �����������
	 //��������� ��������, ����� ��������� ��������� ���� ����
	 } //����
  //������������ ����� ������������ ��� ����� ���������� ���
  for (j=0;j<s;j++)
    { 
    if (flag_t)
       _delay_ms (temp1); //������������ ���� ��� �������� �����
    else
       _delay_ms (temp0); //������������ ���� ��� ���������� �����
	}
  //���������� ��������� ������� ����
  TIMSK &= ~(1 << OCIE1A); //��������� ���������� �� ���������� A ������� 1
  TIMSK &= ~(1 << OCIE1B); //��������� ���������� �� ���������� B ������� 1
  TCCR1B = 0; //������� ������� 1
  i++;
  } //while (i < m_len)
  TCCR1A = 0; //��������� ����� PB3 �� ������� 1
  PORTB &= ~(1 << PB3); //������� ������� �� ������ PB3 - ����� ���� ������ ������
}


ISR (TIMER1_COMPA_vect)
//=== ���������� ���������� �� ���������� A ��� ��������� �������/�������� 1
//    ��� ���������� �������� �����
//������� ������������ ���������� ������� ������ � http://alex.starspirals.net
{
//������ ��������� ����������
OCR1A += n1;
}


ISR (TIMER1_COMPB_vect)
//=== ���������� ���������� �� ���������� B ��� ��������� �������/�������� 1
//    ��� ������������ ������� "�������"
{
if (pause == 0) //��� ������� ������������ ���� (�� �����)
   {
   if (strobe) //���� ������ ������������ �������
      {
	  strobe = 0;    //��������� �����, ����������� ���� 
	  TCCR1B = 0;    //���������� ������ 1
	  OCR1A = TCNT1 + n1;  //���������������� ������������ ������ ����������� ��� ���� ����
	  OCR1B = TCNT1 + T1B_1; //������� � ������� ��������� B ������� 1 ������������ ���������� ������
	  TCCR1B |= (1 << CS11);   //��������� ������ 1
      TIMSK  |= (1 << OCIE1A); //��������� ���������� �� ���������� A ������� 1
	  }
   else //���� ������ ���������
      {
	  strobe = 1; //�������� �����, ������������� ���� 
	  TCCR1B = 0; //���������� ������ 1
	  OCR1B = TCNT1 + T1B_0; //������� � ������� ��������� B ������� 1 ������������ ������
	  TCCR1B |= (1 << CS11);    //��������� ������ 1
      TIMSK  &= ~(1 << OCIE1A); //��������� ���������� �� ���������� A ������� 1
	  } 
   }  
}


/*
ISR (INT0_vect)
//=== ���������� �������� ���������� INT0 �� ������� ������
{
...
EIFR  |= (1 << INTF0);//���������� ���� ���������� INT0 ���������� ��� � 1
GIMSK |= (1 << INT0); //��������� ���������� �� ����� INT0
}
*/


ISR (WDT_OVERFLOW_vect)
//=== ���������� ���������� �� ����������� �������
{
wdt_reset (); //����� ����������� �������
WDTCSR |= (1 << WDIE); //���������� ���������� �� ����������� �������
//TCNT0 = 0; //������ �������������� ������� ����������� � ������ ��������� ����� 
}



//========================================================
int main (void)
{

//��������� ������ � �������
DDRD &= ~((1 << PD2) | (1 << PD6)); //������� ������� 
PORTD |= (1 << PD2) | (1 << PD6);   //�������� ��� ��� ������������� ���������
DDRB  |= ((1 << PB0) | (1 << PB3)); //���������� PB3, PB0 ��� ������
PORTB &= ~(1 << PB0); //PB0 -> 0
//��������� ���������� � ��������
TCCR1B = 0; //������� ������� 1
TCCR1A = 0; //��������� ����� PB3 �� ������� 1
PORTB &= ~(1 << PB3); //������� ������� �� ������ PB3
TCNT1 = 0;
TIMSK &= ~(1 << OCIE1A); //��������� ���������� �� ���������� A ������� 1
TIMSK &= ~(1 << OCIE1B); //��������� ���������� �� ���������� B ������� 1
TCCR0B = 0; //������� ������� 0
TCCR0A = 0; //Normal mode ��� ������� 0
TCCR0B = (1 << CS02) | (1 << CS00); //���� ������� 0 � ������������� 1024 (������� 7.8125 ���)
//TCCR1B |= (1 << CS11); //���� ������� 1 � ������������� 8
//��� ������� ������ ������ �������� �������� T0 ����� ��������� ��������� �����
//MCUCR |= ~((1 << ISC01) | (1 << ISC00));  //���������� INT0 - �� ����� 
//GIMSK |= ((1 << INT0));   //��������� ���������� INT0
//EIFR  |= ((1 << INTF0)); //���������� ���� ���������� INT0
wdt_enable (WDTO_250MS); //�������� ���������� ������
WDTCSR |= (1 << WDIE); //��������� ���������� �� ����������� �������

_delay_ms (500); //��� ��������� ���������� ���������
while (check_pressed ()); //�����, ���� ������ ������
sei (); //��������� ��������� ����������

m = 0; //��������� ��� ������� ��� ������ ����������������� ��������
while (1) //
{

//�������� ������� ������ ������
if (safe_check_pressed ())
   {
   //���������, ����������� �� ��������� ���������� ������ �������
   if (check_jmp ())
      m = random_m (); //������������� ��������� ����� �������
   if (m >= nm) 
      m = 0; //������ � ������
   //�������� ����������� ������ ������� �� ������� ������ � �����
   mem_read_buf (melody_addr (m), buf); 
   //��������� ������� �� �����, ����������� � �����
   PORTB |= (1 << PB0); //PB0 -> 1
   play (buf);
   PORTB &= ~(1 << PB0); //PB0 -> 0
   //����� ��������� �������
   while (check_pressed ()); //�����, ���� �� ����� �������� ������
   _delay_ms (500); //����� ����� ��������� ���������� ������� ������
   m++; //��������� ����� ������� (��� ����������������� ��������)
   } //���������� ��������� ������� ������ ������

} //while (1)

}


