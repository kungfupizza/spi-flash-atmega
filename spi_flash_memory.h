/*
 * spi_flash_memory.h
 * This file contain the function declaration & macro definition for flash initialization & reading & writing to flash memory
 * Created: 10/3/2018 1:40:18 PM
 *  Author: Sarthak
 */ 

#include <asf.h>
#ifndef SPI_FLASH_MEMORY_H_
#define SPI_FLASH_MEMORY_H_


	#define ERASE_4KB     0x20
	#define ERASE_32KB    0x52
	#define ERASE_64KB    0xD8
//	#define ERASE_ALL     0x60
	#define ERASE_ALL     0xC7	

void spi_flash_init(void);

uint8_t spi_flash_read(void);

void spi_flash_write(uint8_t value);

void flash_jdec_read_id(void);

void flash_read_id(void);

void flash_memory_write_enable(void);

void flash_memory_write_disable(void);

void flash_status_reg_enable(void);

uint8_t flash_read_status_reg_1(void);

uint8_t flash_read_status_reg_2(void);

void flash_write_status_reg(uint8_t data);

void flash_clear_memory(uint8_t size,uint32_t address);

void flash_byte_program(uint32_t address,uint8_t value);

void flash_aai_word_program(uint32_t address,uint8_t *value,uint8_t length);

void flash_busy_status(void);

void flash_read(uint32_t address,uint16_t length,uint8_t *value);

void flash_high_speed_read(uint32_t address,uint8_t length,uint8_t *value);

void flash_write_protect_disable(void);

void flash_write_protect_enable(void);

void write_block_protection(void);

void flash_global_BPR_unlock(void);

#endif /* SPI_FLASH_MEMORY_H_ */