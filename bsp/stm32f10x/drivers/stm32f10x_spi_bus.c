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
#include "stm32f10x_spi_bus.h"
#include <stm32f10x.h>

/*
 * configuration rcc base on spi channel
 * */
static int stm32f10x_spi_bus_RccConfig(void)
{
#ifdef RT_USING_SPI1	/*Configration spi1 clock*/
   RCC_APB2PeriphClockCmd( RCC_SPI1, ENABLE );
#endif

#ifdef RT_USING_SPI2	/*Configration spi2 clock*/

#endif

#ifdef RT_USING_SPI3	/*Configration spi2 clock*/

#endif
}

/*
 * configuration gpio base on spi channel in the master
 * */
static void stm32f10x_spi_bus_GpioConfig_Master(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*
     * In the master mode:
     * NSS : Output
     * SCK : Output
     * MISO : Input
     * MOSI : Output
     * */
    GPIO_InitStructure.GPIO_Pin = /*SPI1_GPIO_NSS |*/ SPI1_GPIO_SCK | SPI1_GPIO_MOSI;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;
    GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
    GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SPI1_GPIO_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);
}

/*
 * configuration gpio base on spi channel in the slave
 * */
static void stm32f10x_spi_bus_GpioConfig_Slave(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*
     * In the Slave mode:
     * NSS : Input,not setting the cs pin.
     * SCK : Input
     * MISO : Output
     * MOSI : Input
     * */
    GPIO_InitStructure.GPIO_Pin = /*SPI1_GPIO_NSS |*/ SPI1_GPIO_SCK | SPI1_GPIO_MOSI;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
    GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
    GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SPI1_GPIO_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);
}

/*
 * configuration spi mode base on spi channel
 * parameters : master and slave
 * */
static void stm32f10x_spi_bus_SetMode(Enum_SpiModeType mode)
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

/*
 * read one byte data from hardware spi bus base on spi channel
 * */
static int stm32f10x_spi_bus_ReadByte(SPI_TypeDef* SPIx, unsigned char *Data)
{
	int timeout_cnt = 0;
	/*wait the receive flag*/
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
	{
		timeout_cnt++;
		/*Wait the receive timeout check*/
		if(timeout_cnt>200)
		{
			return RT_ETIMEOUT;
		}
		else
		{
			/*doing noting*/
		}
	}

	*Data = SPI_I2S_ReceiveData(SPIx);/*read data from SPI bus*/
	return RT_EOK;
}

/*
 * write one byte data from hardware spi bus base on spi channel
 * */
static int stm32f10x_spi_bus_WriteByte(SPI_TypeDef* SPIx, unsigned char Data)
{
	int timeout_cnt = 0;
	/*wait the send flag is ready*/
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
	{
		timeout_cnt++;
		/*Wait the send timeout check*/
		if(timeout_cnt>200)
		{
			return RT_ETIMEOUT;
		}
		else
		{
			/*doing noting*/
		}
	}

	SPI_I2S_SendData(SPIx, Data);/*read data from SPI bus*/
	return RT_EOK;
}

/*
 * read size of byte data from hardware spi bus base on spi channel
 * pos : stm32f10x spi bus channel
 * */
int stm32f10x_spi_bus_ReadData(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
	rt_size_t i = 0x00;
	rt_err_t ret = RT_EOK;
    SPI_TypeDef *SPIx = SPI1;
    unsigned char buf[size];

	/*Check the device is valid*/

    /*pos parameters convert to SPI bus channel*/
    if(0x01 == pos)
    {
    	SPIx = stm32f10x_spi_bus_channel(1);
    }
    else if(0x02 == pos)
    {
    	SPIx = stm32f10x_spi_bus_channel(2);
    }
    else if(0x03 == pos)
    {
    	SPIx = stm32f10x_spi_bus_channel(3);
    }
    else
    {
    	ret = RT_ERROR;
    	return ret;
    }

	/*Read data from hardware spi bus*/
	for(i = 0x00; i < size; i++)
	{
		ret = stm32f10x_spi_bus_ReadByte(SPIx,&buf[i]);
		/*if read data happen error,exit this loop*/
		if(RT_EOK != ret)
		{
			break;
		}
	}

	/*Copy data to *buffer*/
	memcpy(buffer, buf,size);

	return ret;
}

/*
 * write size of byte data from hardware spi bus base on spi channel
 * pos : stm32f10x spi bus channel
 * */
int stm32f10x_spi_bus_WriteData(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
	rt_size_t i = 0x00;
	rt_err_t ret = RT_EOK;
	SPI_TypeDef *SPIx = SPI1;
	unsigned char buf[size];

	/*Check the device is valid*/

	/*pos parameters convert to SPI bus channel*/
	if(0x01 == pos)
	{
		SPIx = stm32f10x_spi_bus_channel(1);
	}
	else if(0x02 == pos)
	{
		SPIx = stm32f10x_spi_bus_channel(2);
	}
	else if(0x03 == pos)
	{
		SPIx = stm32f10x_spi_bus_channel(3);
	}
	else
	{
		ret = RT_ERROR;
		return ret;
	}

	/*Copy data to *buf*/
	memcpy(buf, buffer,size);

	/*Read data from hardware spi bus*/
	for(i = 0x00; i < size; i++)
	{
		ret = stm32f10x_spi_bus_WriteByte(SPIx,buf[i]);
		/*if read data happen error,exit this loop*/
		if(RT_EOK != ret)
		{
			break;
		}
	}

	return ret;
}

/*
 * pos : stm32f10x spi bus channel
 * */
int stm32f10x_spi_bus_Control(rt_device_t dev, int cmd, void *args)
{
	rt_size_t i = 0x00;
	rt_err_t ret = RT_EOK;

	/*Check the device is valid*/


	return ret;
}

static int stm32f10x_spi_bus_HwInit(void)
{
	int ret = 0x00;

	stm32f10x_spi_bus_RccConfig();
	stm32f10x_spi_bus_GpioConfig_Master();
	stm32f10x_spi_bus_SetMode(SPI_MODE_MASTER);

	return ret;
}

static const rt_uint32_t stm32f10x_spi_bus_config(struct rt_spi_device *device, struct rt_spi_configuration *configuration)
{
	rt_uint32_t ret = 0x00;
	spi_bus_printf("enter stm32f10x_spi_bus_config function\r\n");


	return ret;
}

static const rt_uint32_t stm32f10x_spi_bus_transfer_message(struct rt_spi_device *device, struct rt_spi_message *message)
{
	rt_uint32_t ret = 0x00;

	spi_bus_printf("enter stm32f10x_spi_bus_transfer_message function\r\n");

	return ret;
}

static struct rt_spi_bus spi_bus;
static const struct rt_spi_ops spi_ops =
{
		stm32f10x_spi_bus_config,
		stm32f10x_spi_bus_transfer_message
};
static struct rt_spi_device spi_device;
static stm32f10x_spi_cs_config spi_device_cs;

static rt_err_t stm32f10x_spi_bus_init(void)
{
	rt_err_t ret = 0x00;


	/*initialization the spi hardware*/
	stm32f10x_spi_bus_HwInit();

	spi_bus.parent.parent.type = RT_Object_Class_Device;

	spi_bus.parent.read = stm32f10x_spi_bus_ReadData;
	spi_bus.parent.write = stm32f10x_spi_bus_WriteData;
	spi_bus.parent.control = stm32f10x_spi_bus_Control;

	/*attach bus to device list*/
	ret = rt_spi_bus_register(&spi_bus,"spi1",&spi_ops);
	if(RT_EOK != ret)
	{
		spi_bus_printf("spi bus register failed !\r\n");
		return ret;
	}
	else
	{
		spi_bus_printf("spi bus register successful!\r\n");
	}


}

static rt_err_t stm32f10x_spi_device_init(void)
{
	rt_err_t ret = RT_EOK;
	char spi_device_name[] = "spi10";

	/*Configuration the spi cs pin mode and direct*/


	spi_device.bus = &spi_bus;
	//spi_device.



	ret = rt_spi_bus_attach_device(&spi_device, spi_device_name, "spi1", (void*)&spi_device_cs);
	if (ret != RT_EOK)
	{
		spi_bus_printf("spi device[%s] register failed !\r\n",spi_device_name);
		return ret;
	}
	else
	{
		spi_bus_printf("spi device[%s] register successful !\r\n",spi_device_name);
	}

	return ret;
}

int rt_hw_stm32f10x_spi_init(void)
{
	stm32f10x_spi_bus_init();
	stm32f10x_spi_device_init();
	return 0x00;
}

static void stm32f10x_spi_bus_test_sample(int argc, char *argv[])
{
	unsigned char name[] = "spi10";
	struct rt_spi_configuration spi_cfg;
	struct rt_spi_device* spi_device;
	static rt_uint8_t w25x_read_id = 0x90;
	static rt_uint8_t id[5] = {0};

	spi_device = (struct rt_spi_device *)rt_device_find(name);
	if (!spi_device)
	{
		spi_bus_printf("stm32f10x spi bus test sample run failed! can't find %s device!\n", name);
	}
	else
	{
		spi_bus_printf("stm32f10x spi bus test sample run successful! %s device exist is system!\n", name);
	}

	spi_bus_printf("the device type is 0x%x\n", spi_device->bus->lock.parent.parent.type);

	rt_spi_configure(spi_device,&spi_cfg);

	rt_spi_send_then_recv(spi_device, &w25x_read_id, 1, id, 1);
	spi_bus_printf("use rt_spi_send_then_recv() write w25q id is %x, read w25q ID is:%x\n", w25x_read_id,id[0]);
}

MSH_CMD_EXPORT(stm32f10x_spi_bus_test_sample, stm32f10x spi bus test sample);

INIT_BOARD_EXPORT(rt_hw_stm32f10x_spi_init);
