/*
 * Copyright (c) 2006-2018,
 *
 * the sample base on STM32F103ZE
 *
 * Change Logs:
 * Date           Author      	 Notes
 * 2019-01-05     Mingfen.XIAO   first version
 *
 */
#include "spi1_init.h"
#include <stm32f10x.h>

/*
 * Configuration Spi1 clock
 * */
static void SPI1_RccConfiguration(void)
{
   RCC_APB2PeriphClockCmd( RCC_SPI1, ENABLE );
}

/*
 * Master Mode,Spi1 port and pin configuration
 * */
static void SPI1_GpioConfiguration_Master(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*
     * In the master mode:
     * NSS : Output
     * SCK : Output
     * MISO : Input
     * MOSI : Output
     * */
    GPIO_InitStructure.GPIO_Pin = SPI1_GPIO_NSS | SPI1_GPIO_SCK | SPI1_GPIO_MOSI;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;
    GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
    GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SPI1_GPIO_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);
}

/*
 * Slave Mode,Spi1 port and pin configuration
 * */
static void SPI1_GpioConfiguration_Slave(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*
     * In the Slave mode:
     * NSS : Input
     * SCK : Input
     * MISO : Output
     * MOSI : Input
     * */
    GPIO_InitStructure.GPIO_Pin = SPI1_GPIO_NSS | SPI1_GPIO_SCK | SPI1_GPIO_MOSI;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
    GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
    GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SPI1_GPIO_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);
}

/*
 * Set Spi1 mode
 * parameters : master and slave
 * */
static void SPI1_SetMode(Enum_SpiModeType mode)
{
	SPI_InitTypeDef  SPI_InitStructure;
	/*Input parameters check*/
	if((SPI_MODE_MASTER != mode) && (SPI_MODE_SLAVE != mode))
	{
		/*Reserved, shall add assertion in future*/
	}
	else
	{
		/*Doing nothing*/
	}

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);
}

static int SPI1_HwInit(void)
{
	int ret = 0x00;

	rt_kprintf("SPI1_HwInit...\n");

	return ret;
}

int rt_hw_spi1_init(void)
{
	return SPI1_HwInit();
}

INIT_BOARD_EXPORT(rt_hw_spi1_init);
