/*
 * spi_flash_memory.c
 *This file contain the function definition for flash initialization & reading & writing to flash memory
 * Created: 10/3/2018 1:40:40 PM
 *  Author: Sarthak
 */ 
#include "spi_flash_memory.h"
struct spi_device flash_ce ={
	.id=DF_SS
};



/***********************************************************************************************************************************/
/*
*	Function Name: spi_flash_init
*	Description:   Initializes the SPI & configures the CS pin.
*/
/***********************************************************************************************************************************/
void spi_flash_init(void)
{
	
	spi_master_init(&SPIC);
	spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);  // setting it to 100Khz.
	spi_enable(&SPIC);
	flash_memory_write_enable();
	flash_write_status_reg(0x00); // the user can access the whole flash memory of 4MB
}


/******************************************************************************************************************************/
/*
 *	Read SPI Data register
*/
/******************************************************************************************************************************/
uint8_t spi_flash_read(void)
{
	uint8_t data;
	while(!(SPIC.STATUS & 0x80));
	data=SPIC_DATA;
	return data;
	
}

/******************************************************************************************************************************/
/*
 *	Write to SPI Data register
*/
/******************************************************************************************************************************/
void spi_flash_write(uint8_t value)
{
	SPIC_DATA = value;
	while(!(SPIC.STATUS & 0x80));
}


/**************************************************************************************************************************************/
/*
*	Function name: flash_jdec_read_id
*	Description:   Reads Manufacturer Id, Memory type & Memory Capacity of Flash memory
*/
/*************************************************************************************************************************************/
void flash_jdec_read_id(void)
{
	uint8_t data;
	spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);    \
	spi_select_device(&SPIC, &flash_ce);  // high to low transition of CE
	spi_flash_write(0x9F);  // read id command for flash for getting deviceid
	
	for(int i=0;i<4;i++)
	{	
		data=spi_flash_read();
		spi_flash_write(0xff);
	}
	spi_deselect_device(&SPIC, &flash_ce);  
spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);  // low to high transition of CE

}

/**********************************************************************************************************************************/
/*
*	Function name: flash_memory_write_enable
*	Description:   it allows to perform any changes in the flash memory like
*					writing to memory,status reg,erase,etc.
*/
/**********************************************************************************************************************************/
void flash_memory_write_enable(void)
{
	spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);    
	spi_select_device(&SPIC, &flash_ce);  // high to low transition of CE
	spi_flash_write(0x06);               // OPCODE FOR ENABLING WRITE TO MEMORY
	spi_deselect_device(&SPIC, &flash_ce);  
	spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);  // low to high transition of CE
}


/************************************************************************************************************************************/
/*
*	Function name: flash_memory_write_disable
*	Description:   It does not allow to perform any changes in the flash memory like
*					writing to memory,status reg,erase,etc.
*/
/************************************************************************************************************************************/
void flash_memory_write_disable(void)
{
	spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);    
	spi_select_device(&SPIC, &flash_ce);  // high to low transition of CE
	spi_flash_write(0x04);               // OPCODE FOR DISABLING WRITES TO MEMORY
	spi_deselect_device(&SPIC, &flash_ce);  
	spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);  // low to high transition of CE
}

/************************************************************************************************************************************/
/*
*	Function name: flash_status_reg_enable
*	Description:   it allows to write to status reg only if memory write enable is enabled
*/
/************************************************************************************************************************************/
void flash_status_reg_enable(void)
{
	spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);    
	spi_select_device(&SPIC, &flash_ce);  // high to low transition of CE
	spi_flash_write(0x50);               // OPCODE FOR ENABLING TO WRITE TO STATUS REG.
	spi_deselect_device(&SPIC, &flash_ce);  
spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);  // low to high transition of CE
}

/*********************************************************************************************************************************/
/*
*	Function name: flash_read_status_reg_1
*	Description:   it gets the status of flash memory
*/
/**********************************************************************************************************************************/
uint8_t flash_read_status_reg_1(void)
{
	uint8_t data;
	uint8_t check=0x01;
	
	spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);    
	spi_select_device(&SPIC, &flash_ce);  // high to low transition of CE
	spi_flash_write(0x05);               // OPCODE FOR READING STATUS REG
	spi_flash_read();
	while(check)
	{
		spi_flash_write(0XFF);
		data=spi_flash_read();
		check=data & 0x01;
	}
	spi_deselect_device(&SPIC, &flash_ce);  
	spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);  // low to high transition of CE
	return data;
}


/*********************************************************************************************************************************/
/*
*	Function name: flash_read_status_reg_2
*	Description:   it gets the status of flash memory
*/
/**********************************************************************************************************************************/
uint8_t flash_read_status_reg_2(void)
{
	uint8_t data;
	
	spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);    
	spi_select_device(&SPIC, &flash_ce);  // high to low transition of CE
	spi_flash_write(0x35);               // OPCODE FOR READING STATUS REG
	spi_flash_read();
	spi_flash_write(0XFF);
	data=spi_flash_read();
	spi_deselect_device(&SPIC, &flash_ce);
	spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);  // low to high transition of CE
	return data;
}


/**************************************************************************************************************************************/
/*
*	Function name: flash_write_status_reg
*	Description:   writes the data to status reg. it internally calls flash_memory_write_enable(); & flash_status_reg_enable();
*/
/****************************************************************************************************************************************/
void flash_write_status_reg(uint8_t data)
{
	flash_memory_write_enable();
	spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);
	spi_select_device(&SPIC, &flash_ce);  // high to low transition of CE
	spi_flash_write(0x01);               // OPCODE FOR WRITING TO STATUS REG
	spi_flash_write(data);
	spi_flash_write(data);
	spi_deselect_device(&SPIC, &flash_ce);  
	spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);  // low to high transition of CE
	
}


/**********************************************************************************************************************************************/
/*
*	Function name	: flash_clear_memory
*	Description		:   It clears the flash memory as per the parameter send.
*	Parameter		:  ERASE_4KB / ERASE_32KB / ERASE_64KB / ERASE_ALL
*					:  address
*/
/**********************************************************************************************************************************************/
void flash_clear_memory(uint8_t size,uint32_t address)
{
	ds1343_interrupt_disable();
	flash_memory_write_enable();
	delay_ms(1);
	switch(size)
	{
		case ERASE_4KB:
				/* erasing only 4KB of memory  */
		        spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);
		        spi_select_device(&SPIC, &flash_ce);
				spi_flash_write(ERASE_4KB); 
				spi_flash_write((address &0x00ff0000)>>16);
				spi_flash_write((address &0x0000ff00)>>8);
				spi_flash_write(address &0x000000ff);
				spi_deselect_device(&SPIC, &flash_ce);  
				spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);
				delay_ms(400);
		break;
		case ERASE_32KB:
				/* erasing only 32KB of memory  */
		        spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);
		        spi_select_device(&SPIC, &flash_ce);
		        spi_flash_write(ERASE_32KB);
		        spi_flash_write((address &0x00ff0000)>>16);
		        spi_flash_write((address &0x0000ff00)>>8);
		        spi_flash_write(address &0x000000ff);
		        spi_deselect_device(&SPIC, &flash_ce);  
				spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);
        break;
		case ERASE_64KB:
				/* erasing only 64KB of memory  */
		        spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);
		        spi_select_device(&SPIC, &flash_ce);
		        spi_flash_write(ERASE_64KB);
		        spi_flash_write((address &0x00ff0000)>>16);
		        spi_flash_write((address &0x0000ff00)>>8);
		        spi_flash_write(address &0x000000ff);
		        spi_deselect_device(&SPIC, &flash_ce);  
				spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);
        break;
		case ERASE_ALL:
				/* erasing whole 4MB of memory  */
				spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);
				spi_select_device(&SPIC, &flash_ce);
				spi_flash_write(ERASE_ALL);
				spi_deselect_device(&SPIC, &flash_ce);  
				spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);
		break;
		default:
				break;
	}
	flash_read_status_reg_1();
	flash_memory_write_disable();
	ds1343_interrupt_init();
}

/**********************************************************************************************************************************************/
/*
*	Function name	:	flash_byte_program
*	Description		:	it is used to write single byte of data to flash memory
*	Parameter		:	address & data.
*/
/*********************************************************************************************************************************************/
void flash_byte_program(uint32_t address,uint8_t value)
{

	flash_memory_write_enable();
	spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);
	spi_select_device(&SPIC, &flash_ce);
	spi_flash_write(0x02);                    // OPCODE FOR WRITING SINGLE BYTE
	spi_flash_write((address & 0x00ff0000)>>16);
	spi_flash_write((address & 0x0000ff00)>>8);
	spi_flash_write(address & 0x000000ff);
	spi_flash_write(value);
	spi_deselect_device(&SPIC, &flash_ce);  
spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);

}


/************************************************************************/
/*
*	Function name: flash_aai_word_program
*	Description:   it is used to write number of byte of data to flash memory
*	Parameter:     address, data & length (no of bytes).
*	Note: length should be always even numbers.
*/
/************************************************************************/
void flash_aai_word_program(uint32_t address,uint8_t *value,uint8_t length)
{
	uint8_t loc_len=0;
	ds1343_interrupt_disable();
		loc_len = (address+length)%256;
		if(loc_len>=length)
		{
			loc_len=0;
		}
		length=length-loc_len;
		flash_memory_write_enable();
		spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);
		spi_select_device(&SPIC, &flash_ce);
		spi_flash_write(0x02);                     // OPCODE FOR ENABLING AAI MODE WRITING
		spi_flash_write((address & 0x00ff0000)>>16);
		spi_flash_write((address & 0x0000ff00)>>8);
		spi_flash_write(address & 0x000000ff);
		for(int i=0;i<length;i++)
		{
			spi_flash_write(value[i]);	
		}
		spi_deselect_device(&SPIC, &flash_ce);  
		spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);
		delay_ms(5);
		if(loc_len>0)
		{
			address=address+length;
			flash_memory_write_enable();
			spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);
			spi_select_device(&SPIC, &flash_ce);
			spi_flash_write(0x02);                     // OPCODE FOR ENABLING AAI MODE WRITING
			spi_flash_write((address & 0x00ff0000)>>16);
			spi_flash_write((address & 0x0000ff00)>>8);
			spi_flash_write(address & 0x000000ff);
			for(int i=length;i<(loc_len+length);i++)
			{
				spi_flash_write(value[i]);
			}
			spi_deselect_device(&SPIC, &flash_ce);
			spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);
		}
	flash_memory_write_disable();
	ds1343_interrupt_init();	
}


/**********************************************************************************************************************************************/
/*
*	Function name: flash_read
*	Description:   it gets the data from flash memory. it runs at max 25mhz
*	Parameter:     address,length (no of bytes),array or pointer for storing a read data.
*/
/**********************************************************************************************************************************************/
void flash_read(uint32_t address,uint16_t length,uint8_t *value)
{
	ds1343_interrupt_disable();
	spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);
	spi_select_device(&SPIC, &flash_ce);
	spi_flash_write(0x03);                     // OPCODE FOR READ MEMORY
	spi_flash_write((address &0x00ff0000)>>16);
	spi_flash_write((address &0x0000ff00)>>8);
	spi_flash_write(address &0x000000ff);
	spi_flash_read();
	spi_flash_write(0xff);
	//delay_s(1);
	for(int i=0;i<length;i++)
	{	
		value[i]=spi_flash_read();
		spi_flash_write(0xff);
		delay_ms(1);
		
	}
	spi_deselect_device(&SPIC, &flash_ce);  
	spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);
	ds1343_interrupt_init();
	
}

/*********************************************************************************************************************************************/
/*
*	Function name: flash_high_speed_read
*	Description:   it gets the data from flash memory. it runs at max 80mhz
*	Parameter:     address,length (no of bytes),array or pointer for storing a read data.
*/
/*********************************************************************************************************************************************/
void flash_high_speed_read(uint32_t address,uint8_t length,uint8_t *value)
{
	
	spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);
	spi_select_device(&SPIC, &flash_ce);
	spi_flash_write(0x0B);                   // OPCODE FOR HIGH SPEED READ
	spi_flash_write((address & 0x00ff0000)>>16);
	spi_flash_write((address & 0x0000ff00)>>8);
	spi_flash_write(address & 0x000000ff);
	spi_flash_write(0xff);
	spi_flash_read();
	for(int i=0;i<length;i++)
	{
		spi_flash_write(0xff);
		delay_ms(1);
		value[i]=spi_flash_read();
		
	}
	spi_deselect_device(&SPIC, &flash_ce);  
	spi_master_setup_device(&SPIC, &flash_ce,SPI_MODE_3,1000000,1);

}


