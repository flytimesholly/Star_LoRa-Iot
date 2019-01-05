#ifndef __SPI1_INIT_H__
#define __SPI1_INIT_H__

#include <rtthread.h>
#include <rtdevice.h>

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

/*Function declaration area*/
static void SPI1_RccConfiguration(void);
static void SPI1_GpioConfiguration_Master(void);
static void SPI1_GpioConfiguration_Slave(void);

static int SPI1_HwInit(void);
int rt_hw_spi1_init(void);

#endif
