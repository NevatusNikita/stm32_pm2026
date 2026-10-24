#include "display.h"
#include "spi.h"

#define CS_LOW()    (GPIOA->ODR &= ~GPIO_ODR_ODR4)  
#define CS_HIGH()   (GPIOA->ODR |=  GPIO_ODR_ODR4)  
#define DC_CMD()    (GPIOA->ODR &= ~GPIO_ODR_ODR1)  
#define DC_DATA()   (GPIOA->ODR |=  GPIO_ODR_ODR1)   
#define RES_LOW()   (GPIOB->ODR &= ~GPIO_ODR_ODR0)   
#define RES_HIGH()  (GPIOB->ODR |=  GPIO_ODR_ODR0)  

static void delay(volatile uint32_t count) {
    while (count--) { __NOP(); }
}

void display_cmd(uint8_t cmd) {
    CS_LOW();
    DC_CMD();
    delay(100);
    SPI1_Write(cmd);
    CS_HIGH();
}

void display_data(uint8_t data) {
    CS_LOW();
    DC_DATA();
    delay(100);
    SPI1_Write(data);
    CS_HIGH();
}

void display_init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    GPIOB->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);
    GPIOB->CRL |= GPIO_CRL_MODE0;

    RES_HIGH();
    delay(1000);
    RES_LOW();
    delay(100000);
    RES_HIGH();
    delay(100000);

    display_cmd(0xAE);         
    display_cmd(0x20); display_cmd(0x00);
    display_cmd(0xB0);          
    display_cmd(0xC8);          
    display_cmd(0x00);          
    display_cmd(0x10);          
    display_cmd(0x40);          
    display_cmd(0x81); display_cmd(0xFF); 
    display_cmd(0xA1);          
    display_cmd(0xA6);          
    display_cmd(0xA8); display_cmd(0x3F);  
    display_cmd(0xD3); display_cmd(0x00);  
    display_cmd(0xD5); display_cmd(0x80);  
    display_cmd(0xD9); display_cmd(0xF1);  
    display_cmd(0xDA); display_cmd(0x12);  
    display_cmd(0xDB); display_cmd(0x40); 
    display_cmd(0x8D); display_cmd(0x14);  
    display_cmd(0xAF);          
}

void display_clear(void) {
    for (uint8_t page = 0; page < 8; page++) {
        display_cmd(0xB0 + page);
        display_cmd(0x00);
        display_cmd(0x10);
        for (uint8_t col = 0; col < 128; col++) {
            display_data(0x00);
        }
    }
}

void display_draw_chessboard(void) {
    for (uint8_t page = 0; page < 8; page++) {
        display_cmd(0xB0 + page);
        display_cmd(0x00);          
        display_cmd(0x10);

        for (uint8_t col = 0; col < 128; col++) {
            uint8_t cell_x = col / 16;
            uint8_t cell_y = page;
            if (((cell_x + cell_y) & 1) == 0) {
                display_data(0xFF);
            } else {
                display_data(0x00);
            }
        }
    }
}