#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include <stm32f10x.h>

void SPI1_Init(void);
void SPI1_Write(uint8_t data);
uint8_t SPI1_Read(void);

#endif