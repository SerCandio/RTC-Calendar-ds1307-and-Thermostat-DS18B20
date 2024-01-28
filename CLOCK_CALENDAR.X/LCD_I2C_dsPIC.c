#include "LCD_I2C_dsPIC.h"
#include <stdint.h>
#include "Configuracion.h"
#include <libpic30.h>

void LCD_I2C_dsPIC_DataBus(uint8_t a){
//   //0000 0001
//    if(a & 0x01) D4=1; else D4=0;
//    //0000 0010
//    if(a & 0x02) D5=1; else D5=0;
//    //0000 0100
//    if(a & 0x04) D6=1; else D6=0;
//     //0000 1000
//    if(a & 0x08) D7=1; else D7=0;
    
    /*Send data on 4 bit mode*/
    lcdParalellData&=~(1<<D7) & ~(1<<D6) & ~(1<<D5) & ~(1<<D4) ; //Borrar el bus de datos D[i]
    lcdParalellData|=(a & 0x0F)<<4; //Pre-cargar al buffer  D[i] = a[i]  ...bit a bit el nuevo dato
    /*Transferir el dato por I2C*/
    pcf8574_SendData(PCF8574_LCD_HITACHHI_ADDR_WR_MODE, lcdParalellData); 
    
}
void LCD_I2C_dsPIC_Cmd(uint8_t a){
//    E=0;
//    E=1;
//    LCD_dsPIC_DataBus(a);    //Datos
//    __delay_ms(2);
//    E=0;
    
    lcdParalellData&=~(1<<E); //Pre-cargar al buffer ..............E=0
    pcf8574_SendData(PCF8574_LCD_HITACHHI_ADDR_WR_MODE, lcdParalellData);
    
    lcdParalellData|=(1<<E); //Pre-cargar al buffer ..............E=1
    pcf8574_SendData(PCF8574_LCD_HITACHHI_ADDR_WR_MODE, lcdParalellData);
    
    LCD_I2C_dsPIC_DataBus(a);    //Datos
    
    lcdParalellData&=~(1<<E); //Pre-cargar al buffer ..............E=0
    pcf8574_SendData(PCF8574_LCD_HITACHHI_ADDR_WR_MODE, lcdParalellData);
    
}
void LCD_I2C_dsPIC_Init(void){
    
    /*Pull I/O Paralell to 0*/
    pcf8574_SendData(PCF8574_LCD_HITACHHI_ADDR_WR_MODE, 0x00);
    
    LCD_I2C_dsPIC_Cmd(0x03);
    LCD_I2C_dsPIC_Cmd(0x03);
    LCD_I2C_dsPIC_Cmd(0x03);
    LCD_I2C_dsPIC_Cmd(0x02);
    LCD_I2C_dsPIC_Cmd(0x02);
    LCD_I2C_dsPIC_Cmd(0x08);
    LCD_I2C_dsPIC_Cmd(0x00);
    LCD_I2C_dsPIC_Cmd(0x08);
    LCD_I2C_dsPIC_Cmd(0x00);
    LCD_I2C_dsPIC_Cmd(0x01);
    LCD_I2C_dsPIC_Cmd(0x00);
    LCD_I2C_dsPIC_Cmd(0x0C);
    
}
void LCD_I2C_dsPIC_SetCursor(uint8_t x, uint8_t y){
   // RS=0;
    
    lcdParalellData&=~(1<<RS); //Pre-cargar al buffer ..............RS=0
    pcf8574_SendData(PCF8574_LCD_HITACHHI_ADDR_WR_MODE, lcdParalellData);
    
    uint8_t m,n;
     switch(y){
        case 1:
            m=0x80+x;
            n=(0x80+x)>>4;//Corrimiento de bits
            LCD_I2C_dsPIC_Cmd(n); 
            LCD_I2C_dsPIC_Cmd(m);  
            break;
        case 2: 
            m=0xC0+x;
            n=(0xC0+x)>>4;//Corrimiento de bits
            LCD_I2C_dsPIC_Cmd(n); 
            LCD_I2C_dsPIC_Cmd(m);              
            break;
        case 3: 
            m=0x94+x;
            n=(0x94+x)>>4;//Corrimiento de bits
            LCD_I2C_dsPIC_Cmd(n); 
            LCD_I2C_dsPIC_Cmd(m);              
            break;
        case 4: 
            m=0xD4+x;
            n=(0xD4+x)>>4;//Corrimiento de bits
            LCD_I2C_dsPIC_Cmd(n); 
            LCD_I2C_dsPIC_Cmd(m);              
           break;
    }
}
void LCD_I2C_dsPIC_Print_Char(char a){
    uint8_t m,n;
    m=a>>4;
    n=a;
   // RS=1;
    lcdParalellData|=(1<<RS); //Pre-cargar al buffer ..............RS=1
    pcf8574_SendData(PCF8574_LCD_HITACHHI_ADDR_WR_MODE, lcdParalellData);
    
    LCD_I2C_dsPIC_Cmd(m); 
    LCD_I2C_dsPIC_Cmd(n);        
}
void LCD_I2C_dsPIC_Print_String(char *a){
    char *ptr;
    ptr = a;
    while (*ptr) 
    LCD_I2C_dsPIC_Print_Char(*ptr++);
}
void LCD_I2C_dsPIC_Clear(void){
    //RS=0;
    lcdParalellData&=~(1<<RS); //Pre-cargar al buffer ..............RS=0
    pcf8574_SendData(PCF8574_LCD_HITACHHI_ADDR_WR_MODE, lcdParalellData);
    
    LCD_I2C_dsPIC_Cmd(0x00);
    LCD_I2C_dsPIC_Cmd(0x01);
}
void LCD_I2C_dsPIC_Home(void){
   // RS=0;
    lcdParalellData&=~(1<<RS); //Pre-cargar al buffer ..............RS=0
    pcf8574_SendData(PCF8574_LCD_HITACHHI_ADDR_WR_MODE, lcdParalellData);
    
    LCD_I2C_dsPIC_Cmd(0x00);
    LCD_I2C_dsPIC_Cmd(0x02);
}
void LCD_I2C_dsPIC_New_Char(uint8_t a,uint8_t b,uint8_t c,uint8_t d,uint8_t e,uint8_t f,uint8_t g,uint8_t h,uint8_t i){
    uint8_t m,n;
    switch(a){
        case 1:
        LCD_I2C_dsPIC_Cmd(0x04);
        LCD_I2C_dsPIC_Cmd(0x00);
        //RS=1;
        lcdParalellData|=(1<<RS); //Pre-cargar al buffer ..............RS=1
        pcf8574_SendData(PCF8574_LCD_HITACHHI_ADDR_WR_MODE, lcdParalellData);
        m=b>>4;
        n=b;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=c>>4;
        n=c;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=d>>4;
        n=d;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=e>>4;
        n=e;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=f>>4;
        n=f;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=g>>4;
        n=g;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=h>>4;
        n=h;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=i>>4;
        n=i;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        //RS=0;
        lcdParalellData&=~(1<<RS); //Pre-cargar al buffer ..............RS=0
        pcf8574_SendData(PCF8574_LCD_HITACHHI_ADDR_WR_MODE, lcdParalellData);
        LCD_I2C_dsPIC_Cmd(0x00);
        LCD_I2C_dsPIC_Cmd(0x01);
        __delay_ms(100);
        break;
        case 2:
        LCD_I2C_dsPIC_Cmd(0x04);
        LCD_I2C_dsPIC_Cmd(0x08);
        //RS = 1;
        lcdParalellData|=(1<<RS); //Pre-cargar al buffer ..............RS=1
        pcf8574_SendData(PCF8574_LCD_HITACHHI_ADDR_WR_MODE, lcdParalellData);
        m=b>>4;
        n=b;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=c>>4;
        n=c;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=c>>4;
        n=c;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=d>>4;
        n=d;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=e>>4;
        n=e;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=f>>4;
        n=f;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=g>>4;
        n=g;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=h>>4;
        n=h;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=i>>4;
        n=i;    
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
       // RS = 0;
        lcdParalellData&=~(1<<RS); //Pre-cargar al buffer ..............RS=0
        pcf8574_SendData(PCF8574_LCD_HITACHHI_ADDR_WR_MODE, lcdParalellData);
        LCD_I2C_dsPIC_Cmd(0x00);
        LCD_I2C_dsPIC_Cmd(0x01);
        __delay_ms(100);
        break;
        case 3:
        LCD_I2C_dsPIC_Cmd(0x04);
        LCD_I2C_dsPIC_Cmd(0xFF);
        //RS=1;
        lcdParalellData|=(1<<RS); //Pre-cargar al buffer ..............RS=1
        pcf8574_SendData(PCF8574_LCD_HITACHHI_ADDR_WR_MODE, lcdParalellData);
        m=b>>4;
        n=b;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=c>>4;
        n=c;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=d>>4;
        n=d;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=e>>4;
        n=e;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=f>>4;
        n=f;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=g>>4;
        n=g;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=h>>4;
        n=h;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
        m=i>>4;
        n=i;
        LCD_I2C_dsPIC_Cmd(m);
        LCD_I2C_dsPIC_Cmd(n);
       // RS=0;
        lcdParalellData&=~(1<<RS); //Pre-cargar al buffer ..............RS=0
        pcf8574_SendData(PCF8574_LCD_HITACHHI_ADDR_WR_MODE, lcdParalellData);
        LCD_I2C_dsPIC_Cmd(0x00);
        LCD_I2C_dsPIC_Cmd(0x01);
        __delay_ms(100);
        break;
    }
}
void LCD_dsPIC_Write_New_Char(uint8_t a){
    //RS = 1;
    lcdParalellData|=(1<<RS); //Pre-cargar al buffer ..............RS=1
    pcf8574_SendData(PCF8574_LCD_HITACHHI_ADDR_WR_MODE, lcdParalellData);
    LCD_I2C_dsPIC_Cmd(0x00);
    LCD_I2C_dsPIC_Cmd(a-1);
   // RS = 0;
    lcdParalellData&=~(1<<RS); //Pre-cargar al buffer ..............RS=0
    pcf8574_SendData(PCF8574_LCD_HITACHHI_ADDR_WR_MODE, lcdParalellData);
}

