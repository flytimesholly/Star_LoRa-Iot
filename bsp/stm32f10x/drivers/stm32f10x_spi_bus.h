#ifndef __STM32F10X_SPI_BUS_H__
#define __STM32F10X_SPI_BUS_H__

#include <rtthread.h>
#include <rtdevice.h>
#include "stm32f10x.h"

#define USING_SPI_BUS_DEBUG 	/*should be delete in future,shall be implements in menuconfig tools*/
#ifdef USING_SPI_BUS_DEBUG
#define spi_bus_printf	rt_kprintf
#else
#define	spi_bus_printf
#endif


typedef struct stm32f10x_spi_cs_config_tag
{
	GPIO_TypeDef * GPIOx;
    uint16_t GPIO_Pin;
}stm32f10x_spi_cs_config;

/*Macro definition area*/
#define SPI1_GPIO 		GPIOA
#define SPI1_GPIO_NSS 	GPIO_Pin_4
#define SPI1_GPIO_SCK 	GPIO_Pin_5
#define SPI1_GPIO_MISO 	GPIO_Pin_6
#define SPI1_GPIO_MOSI 	GPIO_Pin_7
#define RCC_SPI1 		(	RCC_APB2Periph_SPI1 \
							| RCC_APB2Periph_GPIOA \
							| RCC_APB2Periph_AFIO	)

/*Type definition area*/
typedef enum enum_SpiModeType_TAG
{
	SPI_MODE_NONE,
	SPI_MODE_MASTER,
	SPI_MODE_SLAVE,
	SPI_MODE_MAX
} Enum_SpiModeType;

#define stm32f10x_spi_bus_channel(channel)	SPI##channel

/*Function declaration area*/
static int stm32f10x_spi_bus_RccConfig(void);

static int SPI1_HwInit(void);
int rt_hw_spi1_init(void);

#endif
