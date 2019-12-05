/*
 display lcd hola mundo
 */
#pragma config FOSC = HS       
#pragma config PLLDIV = 1         
#pragma config MCLRE = ON  

#pragma config CPUDIV = OSC1_PLL2     
#pragma config BOR = OFF   

#pragma config WDT = OFF        
#pragma config LVP = OFF 

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <xc.h>
#include <pic18f4550.h>

#define _XTAL_FREQ 20000000

#define RS RD2  //Register Select (Character or Instruction)
#define EN RD3  //LCD Clock Enable PIN, Falling Edge Triggered

// 4 bit operation
#define D4 RD4  //Bit 4
#define D5 RD5  //Bit 5
#define D6 RD6  //Bit 6
#define D7 RD7  //Bit 7

// function prototypes
void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Clear();
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init();
void Lcd_Write_Char(char a);
void Lcd_Write_String(const char *a);
void Lcd_Shift_Right();
void Lcd_Shift_Left();
void Lcd_Write_Integer(int v);
void Lcd_Write_Float(float f);



void main(void) 
{
    
    TRISD=0x00;
   
    Lcd_Init();
    Lcd_Set_Cursor(1,5);
    Lcd_Write_String("Hola");
    Lcd_Set_Cursor(2,5);
    Lcd_Write_String("mundo");
    
    unsigned char cadena[5]; // 4 digitos como maximo
    uint16_t valor=1234;
    sprintf(cadena,"%i",valor);
    Lcd_Set_Cursor(2,12);
    Lcd_Write_String(cadena);
    
   
    //Lcd_Write_String(itoa(cadena, valor, 2));
  
    
    while(1)
    {
                 
    }
   
}



/*******************************************************************************
 * Function: void Lcd_Port (char a)
 *
 * Returns: Nothing
 *
 * Description: LCD Setup Routines
 ******************************************************************************/

void Lcd_Port(char a)
{
   if(a & 1)
      D4 = 1;
   else
      D4 = 0;

   if(a & 2)
      D5 = 1;
   else
      D5 = 0;

   if(a & 4)
      D6 = 1;
   else
      D6 = 0;

   if(a & 8)
      D7 = 1;
   else
      D7 = 0;
}

/*******************************************************************************
 * Function: void Lcd_Cmd (char a)
 *
 * Returns: Nothing
 *
 * Description: Sets LCD command
 ******************************************************************************/

void Lcd_Cmd(char a)
{
   RS = 0;             // => RS = 0
   Lcd_Port(a);
   EN  = 1;             // => E = 1
        __delay_ms(4);
        EN  = 0;             // => E = 0
}

/*******************************************************************************
 * Function: void Lcd_Clear()
 *
 * Returns: Nothing
 *
 * Description: Clears the LCD
 ******************************************************************************/

void Lcd_Clear()
{
   Lcd_Cmd(0);
   Lcd_Cmd(1);
}

/*******************************************************************************
 * Function: void Lcd_Set_Cursor(char a, char b)
 *
 * Returns: Nothing
 *
 * Description: Sets the LCD cursor position
 ******************************************************************************/

void Lcd_Set_Cursor(char a, char b)
{
   char temp,z,y;
   if(a == 1)
   {
     temp = 0x80 + b - 1;
      z = temp>>4;
      y = temp & 0x0F;
      Lcd_Cmd(z);
      Lcd_Cmd(y);
   }
   else if(a == 2)
   {
      temp = 0xC0 + b - 1;
      z = temp>>4;
      y = temp & 0x0F;
      Lcd_Cmd(z);
      Lcd_Cmd(y);
   }
}

/*******************************************************************************
 * Function: void Lcd_Init()
 *
 * Returns: Nothing
 *
 * Description: Initialises the LCD
 ******************************************************************************/

void Lcd_Init()
{
  Lcd_Port(0x00);
   __delay_ms(20);
  Lcd_Cmd(0x03);
   __delay_ms(5);
  Lcd_Cmd(0x03);
   __delay_ms(11);
  Lcd_Cmd(0x03);
  /////////////////////////////////////////////////////
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x08);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x0C);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x06);
}

/*******************************************************************************
 * Function: void Lcd_Write_Char (char a)
 *
 * Returns: Nothing
 *
 * Description: Writes a character to the LCD
 ******************************************************************************/

void Lcd_Write_Char(char a)
{
   char temp,y;
   temp = a&0x0F;
   y = a&0xF0;
   RS = 1;             // => RS = 1
   Lcd_Port(y>>4);             //Data transfer
   EN = 1;
   __delay_us(40);
   EN = 0;
   Lcd_Port(temp);
   EN = 1;
   __delay_us(40);
   EN = 0;
}

/*******************************************************************************
 * Function: void Lcd_Write_String (const char *a)
 *
 * Returns: Nothing
 *
 * Description: Writes a string to the LCD
 ******************************************************************************/

void Lcd_Write_String(const char *a)
{
   int i;
   for(i=0;a[i]!='\0';i++)
      Lcd_Write_Char(a[i]);
}

/*******************************************************************************
 * Function: void Lcd_Shift_Right()
 *
 * Returns: Nothing
 *
 * Description: Shifts text on the LCD right
 ******************************************************************************/

void Lcd_Shift_Right()
{
   Lcd_Cmd(0x01);
   Lcd_Cmd(0x0C);
}

/*******************************************************************************
 * Function: void Lcd_Shift_Left()
 *
 * Returns: Nothing
 *
 * Description: Shifts text on the LCD left
 ******************************************************************************/

void Lcd_Shift_Left()
{
   Lcd_Cmd(0x01);
   Lcd_Cmd(0x08);
}

/*******************************************************************************
 * Function: void Lcd_Write_Integer(int v)
 *
 * Returns: Nothing
 *
 * Description: Converts a string to an integer
 ******************************************************************************/

void Lcd_Write_Integer(int v)
{
    unsigned char buf[8];

    Lcd_Write_String(itoa(buf, v, 10));
}


/*******************************************************************************
 * Function: void Lcd_Write_Float(float f)
 *
 * Returns: Nothing
 *
 * Description: Converts a string to a float
 ******************************************************************************/

void Lcd_Write_Float(float f)
{
    char* buf11;
    int status;

    buf11 = ftoa(f, &status);

    Lcd_Write_String(buf11);
}
