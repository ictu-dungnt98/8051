#include <REGX51.H>

void delay(unsigned int t)
{
	while(t--)
	{
		TMOD = 0x01; // timer0 mode1
		TH0 = 0xfc;
		TL0 = 0x18;
	
		TR0 = 1;
		while(TF0 == 0){}
		TR0 = 0;
		TF0 = 0;
	}
}
void main()
{
	P1 = 0x00;
	delay(1000);
	P1 = 0xff;
	delay(1000);	


}