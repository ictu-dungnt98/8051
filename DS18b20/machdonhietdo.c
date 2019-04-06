#include <regx51.h>

#define scan_seg	P1
#define dt_seg	  P2

//Khai bao chan giao tiep LCD16x2 4bit
#define LCD_RS P2_0
#define LCD_RW P2_1
#define LCD_EN P2_2
#define LCD_D4 P2_4
#define LCD_D5 P2_5
#define LCD_D6 P2_6
#define LCD_D7 P2_7


sbit DQ = P3^6;            // chan ket noi voi DS1820

#include <stdio.h>
 
void delay_18B20(unsigned int i); 
void Init_18B20(void) ;        // khoi tao DS1820
unsigned char ReadOneChar(void);    // doc mot byte trong DS1820
void WriteOneChar(unsigned char dat);      // Ghi byte vao DS1820
void ReadTemperature(void);                // Doc gia tri nhiet do ket qu cuoi cung luu o temp
 
unsigned char data_rs232,temp,temp_1,k;


/************************************/
void delay_us(unsigned int t){
        unsigned int i;
        for(i=0;i<t;i++);
}
void delay_ms(unsigned int t){
        unsigned int i,j;
        for(i=0;i<t;i++)
        for(j=0;j<125;j++);
}
/**************Ctr giao tiep LCD 16x2 4bit**********************/
void LCD_Enable(void){
        LCD_EN =1;
        delay_ms(1);
        LCD_EN=0;
        delay_ms(2); 
}
//Ham Gui 4 Bit Du Lieu Ra LCD
void LCD_Send4Bit(unsigned char Data){
        LCD_D4=Data & 0x01;
        LCD_D5=(Data>>1)&1;
        LCD_D6=(Data>>2)&1;
        LCD_D7=(Data>>3)&1;
}
// Ham Gui 1 Lenh Cho LCD
void LCD_SendCommand(unsigned char command){
        LCD_Send4Bit(command >>4);/* Gui 4 bit cao */
        LCD_Enable();
        LCD_Send4Bit(command); /* Gui 4 bit thap*/
        LCD_Enable();
}
void LCD_Clear(){// Ham Xoa Man Hinh LCD
        LCD_SendCommand(0x01); 
        delay_us(10);
}
// Ham Khoi Tao LCD
void LCD_Init(){
        LCD_Send4Bit(0x00);
        delay_ms(20);
        LCD_RS=0;
        LCD_RW=0;
        LCD_Send4Bit(0x03);
        LCD_Enable();
        delay_ms(5);
        LCD_Enable();
        delay_ms(1);
        LCD_Enable();
        LCD_Send4Bit(0x02);
        LCD_Enable();
        LCD_SendCommand( 0x28 ); // giao thuc 4 bit, hien thi 2 hang, ki tu 5x8
        LCD_SendCommand( 0x0c); // cho phep hien thi man hinh
        LCD_SendCommand( 0x06 ); // tang ID, khong dich khung hinh
        LCD_SendCommand(0x01); // xoa toan bo khung hinh
		delay_ms(2);
}
void LCD_Gotoxy(unsigned char x, unsigned char y){
        unsigned char address;
        if(!y)address=(0x80+x);
        else address=(0xc0+x);
        delay_ms(1);
        LCD_SendCommand(address);
        delay_ms(1);
}
void LCD_PutChar(unsigned char Data){//Ham Gui 1 Ki Tu
        LCD_RS=1;
        LCD_SendCommand(Data);
        LCD_RS=0 ;
}
void LCD_Puts (char *s){//Ham gui 1 chuoi ky tu
        while (*s){
                LCD_PutChar(*s);
                s++;
        }
}
//*********************// chuong trinh cho DS18B20
void delay_18B20(unsigned int i)
{
    while(i--);
}
 
void Init_18B20(void)
{
    unsigned char x=0;
    DQ = 1;       
    delay_18B20(8);
    DQ = 0;       
    delay_18B20(80);
    DQ = 1;       
    delay_18B20(14);
    x=DQ;         
    delay_18B20(10);
}
 
unsigned char ReadOneChar(void)
{
    unsigned char i=0;
    unsigned char dat = 0;
    for (i=8;i>0;i--)
    {
          DQ = 0;
          dat>>=1;
          DQ = 1;
          if(DQ)
          dat |= 0x80;
          delay_18B20(4);
    }
    return(dat);
}
void WriteOneChar(unsigned char dat)
{
    unsigned char i=0;
    for (i=8; i>0; i--)
    {
          DQ = 0;
        DQ = dat&0x01;
        delay_18B20(5);
        DQ = 1;
        dat>>=1;
    }
}
 
 
void ReadTemperature(void)
{
    unsigned char a=0;
    unsigned char b=0;
 
    Init_18B20();
    WriteOneChar(0xCC);        // Bo qua ROM
    WriteOneChar(0x44); 
 
    delay_18B20(100);      // this message is very important
 
    Init_18B20();
    WriteOneChar(0xCC); 
    WriteOneChar(0xBE);    //Doc nhiet do
 
    delay_18B20(100);
 
    a=ReadOneChar();        //Read temp low value
    b=ReadOneChar();          //Read temp high value
 
    temp=((b*256+a)>>4);    //gia tri nhiet do
 
}


void hienthitemp(void)
{
	LCD_Gotoxy(0,0);
	ReadTemperature();
	LCD_Puts("Nhiet do: ");
	LCD_PutChar((temp/100)+48);//Tram
	LCD_PutChar((temp%100/10)+48);//Chuc
	LCD_PutChar((temp%10)+48);//Don vi 
	LCD_Puts("'C");

}


// chuong trinh chinh
void main(void)
{
	//Tao xung clock cho ADC0808 dung ngat timer

	//init	
	LCD_Init();//Khoi tao LCD
    delay_ms(200);
	LCD_Puts("Do nhiet do...");//Gui chuoi len LCD
    delay_ms(500);
    LCD_Clear();//Xoa man hinh
	LCD_Gotoxy(0,1);
	LCD_Puts("ICTU");
	while(1)
	{
		hienthitemp();
		delay_ms(1000);

	}
}