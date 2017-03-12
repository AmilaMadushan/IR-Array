#define F_CPU 8000000UL
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#define PWM_out DDRD
void forwardLeftMotor(unsigned int duty);
void forwardRightMotor(unsigned int duty);
void backwordLeftMotor(unsigned int duty);
void backwordRightMotor(unsigned int duty);
void stopMotors();
#define IRDir DDRB // IR sensor array direction
#define IRIN PORTB
#define IRPin PINB
unsigned char c={};
int main(void)
{
DDRC=0xff;
unsigned char uartVal=0;
PWM_out = (1<<PD4) | (1<<PD5); // PWM outputs
init_PWM(1000);
IRDir = 0x00; // sensor signals are outputs
int setpoint=0x18;
 while (1)
 {
int j=IRPin-setpoint;
int m=abs(j);
if(j>0x00){
backwordRight(650,j);
_delay_ms(10);
stopMotors();
}else if(j<0x00){
backwordLeft(10,m);
_delay_ms(10);
stopMotors();
}else{
stopMotors();
}
stopMotors();
}
return 0;
}
//////////////////////////// This is PWM init and pwm1 fuction that is used for PWM Controls
////////////////////////////////////////
void init_PWM(unsigned int topVal){
// Set up Timer 1. Timer 1 should reset when
// it reaches TOP = ICR1 (WGM13:0 = 1110b). On
// compare match clear output, at TOP set (COM1A1:0 = 10b).
TCCR1A = _BV(COM1A1) | !_BV(COM1A0) // Both PWM outputs set at TOP,
| _BV(COM1B1) | !_BV(COM1B0) // clear on compare match
| !_BV(FOC1A) | !_BV(FOC1B) // PWM mode, can't force output
13
| _BV(WGM11) | !_BV(WGM10); // Fast PWM, TOP = ICR1
TCCR1B = !_BV(ICNC1) | !_BV(ICES1) // Disable input capture noise canceller,
// edge select to negative.
| _BV(WGM13) | _BV(WGM12) // Fast PWM, TOP = ICR1
| !_BV(CS12) | _BV(CS11) | !_BV(CS10); // clk(i/o) / 8
ICR1 = topVal; // PWM period ( 50 Hz)
}
void pwm1(unsigned char channel,unsigned int duty){
if(channel){
OCR1A =duty ;
}else{
OCR1B =duty;
}
}
////////////////////////////////// End of the PWM Functions
////////////////////////////////////////////////////////////////////////////
// bellow functions are motor contorl functions, but they are not basic motor control functions.///////////
void forwadLeft(){
OCR1A = 600;
OCR1B = 10;
PORTC = (1<<PC0) | (1<<PC2);
}
void forwardRight(){
OCR1A = 400;
OCR1B = 50;
PORTC = (1<<PC0) | (1<<PC2);
}
void forward(){
OCR1A = 300;
OCR1B = 0;
PORTC |= (1<<PC0) | (1<<PC2);
}
void forwardNormal(){
OCR1A = 320 ;
OCR1B = 20;
PORTC = (1<<PC0) | (1<<PC2);
}
// backword
void backwordLeft(int k,int l){
OCR1A = 10;
OCR1B = 600+(k/l)*150;
PORTC = 0x00;
}
backwordLeft1(){
OCR1A = 200;
OCR1B = 1000;
PORTC = (0<<PC0) | (1<<PC2);
}
void backwordRight(int k,int l){
OCR1A = (k/l)*150;
OCR1B = 10;
PORTC = 0x00;
}
void backwordRight1(){
14
OCR1A = 1000 ;
OCR1B = 200;
PORTC = (1<<PC0) | (0<<PC2);
}
void backword(){
OCR1A = 1000 ;
OCR1B = 1000;
PORTC = 0x00;
}
void backwordNormal(){
OCR1A = 700;
OCR1B = 700;
PORTC = 0x00;
}
void backwordSlow(){
OCR1A = 950;
OCR1B = 950;
PORTC = 0x00;
}
/////////////// End of the Motor control function ///////////////////////////////////////////////////////////
/////////////// This is Motor Control basic Functions ///////////////////////////////////////////////////////
void forwardLeftMotor(unsigned int duty){
OCR1A = duty;
PORTC |= (1<<PC2);
}
void forwardRightMotor(unsigned int duty){
OCR1B = duty;
PORTC |= (1<<PC0);
}
void backwordLeftMotor(unsigned int duty){
OCR1A = duty;
PORTC &= ~(1<<PC2);
}
void backwordRightMotor(unsigned int duty){
OCR1A = duty;
PORTC &= ~(1<<PC0);
}
/////////////// End of the Motor Control basic Fuction //////////////////////////////////////////////////////
void stopMotors(){
OCR1A = 1000;
OCR1B = 1000;
PORTC = (1<<PC0) | (1<<PC2);
}