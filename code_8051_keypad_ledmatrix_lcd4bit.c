/*
        4x4 Keypad Interfacing with 8051(AT89s52)
        http://www.electronicwings.com
*/

#include <REGX51.H>
#include <stdio.h>
#include <string.h>

/************************************/
void delay_us(unsigned int t)
{
    unsigned int i;
    for (i = 0; i < t; i++)
        ;
}
void delay_ms(unsigned int t)
{
    unsigned int i, j;
    for (i = 0; i < t; i++)
        for (j = 0; j < 125; j++)
            ;
}

/********Khai bao chan giao tiep************/
#define LCD_RS P0_0
#define LCD_RW P0_1
#define LCD_EN P0_2
#define LCD_D4 P0_4
#define LCD_D5 P0_5
#define LCD_D6 P0_6
#define LCD_D7 P0_7

/**************Ctr giao tiep LCD 16x2 4bit**********************/
void LCD_Enable(void)
{
    LCD_EN = 1;
    delay_us(3);
    LCD_EN = 0;
    delay_us(50);
}
// Ham Gui 4 Bit Du Lieu Ra LCD
void LCD_Send4Bit(unsigned char Data)
{
    LCD_D4 = Data & 0x01;
    LCD_D5 = (Data >> 1) & 1;
    LCD_D6 = (Data >> 2) & 1;
    LCD_D7 = (Data >> 3) & 1;
}
// Ham Gui 1 Lenh Cho LCD
void LCD_SendCommand(unsigned char command)
{
    LCD_Send4Bit(command >> 4); /* Gui 4 bit cao */
    LCD_Enable();
    LCD_Send4Bit(command); /* Gui 4 bit thap*/
    LCD_Enable();
}
void LCD_Clear()
{  // Ham Xoa Man Hinh LCD
    LCD_SendCommand(0x01);
    delay_us(10);
}
// Ham Khoi Tao LCD
void LCD_Init()
{
    LCD_Send4Bit(0x00);
    delay_ms(20);
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_Send4Bit(0x03);
    LCD_Enable();
    delay_ms(5);
    LCD_Enable();
    delay_us(100);
    LCD_Enable();
    LCD_Send4Bit(0x02);
    LCD_Enable();
    LCD_SendCommand(0x28);  // giao thuc 4 bit, hien thi 2 hang, ki tu 5x8
    LCD_SendCommand(0x0c);  // cho phep hien thi man hinh
    LCD_SendCommand(0x06);  // tang ID, khong dich khung hinh
    LCD_SendCommand(0x01);  // xoa toan bo khung hinh
}
void LCD_Gotoxy(unsigned char x, unsigned char y)
{
    unsigned char address;
    if (!y)
        address = (0x80 + x);
    else
        address = (0xc0 + x);
    delay_us(1000);
    LCD_SendCommand(address);
    delay_us(50);
}
void LCD_PutChar(unsigned char Data)
{  // Ham Gui 1 Ki Tu
    LCD_RS = 1;
    LCD_SendCommand(Data);
    LCD_RS = 0;
}
void LCD_Puts(char *s)
{  // Ham gui 1 chuoi ky tu
    while (*s) {
        LCD_PutChar(*s);
        s++;
    }
}

/******** LCD ******************/

/**************** KEY PAD ***********/
#define keyport P1

unsigned char keypad[4][4] = {{'7', '8', '9', '/'},
                              {'4', '5', '6', 'x'},
                              {'1', '2', '3', '-'},
                              {' ', '0', '=', '+'}};

unsigned char colloc, rowloc;

unsigned char key_detect()
{
    keyport = 0xF0; /*set port direction as input-output*/
    do {
        keyport = 0xF0;
        colloc = keyport;
        colloc &= 0xF0;       /* mask port for column read only */
    } while (colloc != 0xF0); /* read status of column */

    do {
        do {
            delay_ms(20);              /* 20ms key debounce time */
            colloc = (keyport & 0xF0); /* read status of column */
        } while (colloc == 0xF0);      /* check for any key press */

        delay_ms(1);
        colloc = (keyport & 0xF0);
    } while (colloc == 0xF0);

    while (1) {
        /* now check for rows */
        keyport = 0xFE; /* check for pressed key in 1st row */
        colloc = (keyport & 0xF0);
        if (colloc != 0xF0) {
            rowloc = 0;
            break;
        }

        keyport = 0xFD; /* check for pressed key in 2nd row */
        colloc = (keyport & 0xF0);
        if (colloc != 0xF0) {
            rowloc = 1;
            break;
        }

        keyport = 0xFB; /* check for pressed key in 3rd row */
        colloc = (keyport & 0xF0);
        if (colloc != 0xF0) {
            rowloc = 2;
            break;
        }

        keyport = 0xF7; /* check for pressed key in 4th row */
        colloc = (keyport & 0xF0);
        if (colloc != 0xF0) {
            rowloc = 3;
            break;
        }
    }

    if (colloc == 0xE0) {
        return (keypad[rowloc][0]);
    } else if (colloc == 0xD0) {
        return (keypad[rowloc][1]);
    } else if (colloc == 0xB0) {
        return (keypad[rowloc][2]);
    } else {
        return (keypad[rowloc][3]);
    }
}
/**************** KEY PAD ***********/

/********************* LED **********************/
unsigned char code maquet[8] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};

unsigned char code maled[][8] = {
    0x1C, 0x22, 0x32, 0x2A, 0x26, 0x22, 0x1C, 0x00,  // 0
    0x08, 0x0C, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00,  // 1
    0x1C, 0x22, 0x20, 0x18, 0x04, 0x02, 0x3E, 0x00,  // 2
    0x1C, 0x22, 0x20, 0x1C, 0x20, 0x22, 0x1C, 0x00,  // 3
    0x10, 0x18, 0x14, 0x12, 0x3E, 0x10, 0x10, 0x00,  // 4
    0x3E, 0x02, 0x02, 0x1E, 0x20, 0x22, 0x1C, 0x00,  // 5
    0x18, 0x04, 0x02, 0x1E, 0x22, 0x22, 0x1C, 0x00,  // 6
    0x3E, 0x20, 0x10, 0x08, 0x04, 0x04, 0x04, 0x00,  // 7
    0x1C, 0x22, 0x22, 0x1C, 0x22, 0x22, 0x1C, 0x00,  // 8
    0x1C, 0x22, 0x22, 0x3C, 0x20, 0x10, 0x0C, 0x00,  // 9
};
unsigned char Buffer_display[8];

void T1_ISR() interrupt 3  // Dung timer 1 de quet led matrix
{
    static unsigned char z;  // Bien dem
    P2 = 0xFF;               // tat het LED
    P3 = Buffer_display[z];  // lay du lieu trong bo dem hien thi ra man hinh
    P2 = maquet[z];          // cho phep LED sang
    z++;
    if (z == 8)
        z = 0;
    TH1 = 0xFC;  // Nap gia tri cho TH1
    TL1 = 0x17;  // Nap gia tri cho TL1
}
void print_number(char number)
{
    int i;
    for (i = 0; i < 8; i++) {
        Buffer_display[i] = maled[number][i];
    }
}

void timer1_init()
{
    TMOD = 0x10;  // khoi tao ngat T1, 16bit
    ET1 = 1;      // cho phep ngat T1
    TF1 = 0;      // xoa co ngat T1
    TR1 = 1;      // khoi dong T1
    EA = 1;       // cho phep ngat toan cuc
}

int main(void)
{
    int i;
    char number;

    timer1_init();

    LCD_Init();  // Khoi tao LCD
    delay_ms(1000);
    LCD_Puts("Trong Hai");  // Gui chuoi len LCD
    LCD_Gotoxy(0, 1);  // Tro toi vi tri
    
    while (1) {
        number = key_detect();
        LCD_PutChar(number); /* Display which key is pressed */
        print_number(number - 48);
    }
}