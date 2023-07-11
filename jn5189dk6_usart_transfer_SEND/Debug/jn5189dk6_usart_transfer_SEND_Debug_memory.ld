/*
 * GENERATED FILE - DO NOT EDIT
 * Copyright (c) 2008-2013 Code Red Technologies Ltd,
 * Copyright 2015, 2018-2019 NXP
 * (c) NXP Semiconductors 2013-2023
 * Generated linker script file for JN5189
 * Created from memory.ldt by FMCreateLinkMemory
 * Using Freemarker v2.3.30
 * MCUXpresso IDE v11.7.1 [Build 9221] [2023-03-28] on 2023. 7. 11. 오전 9:14:22
 */

MEMORY
{
  /* Define each memory region */
  PROGRAM_FLASH (rx) : ORIGIN = 0x0, LENGTH = 0xa0000 /* 640K bytes (alias Flash) */  
  SRAM (rwx) : ORIGIN = 0x4000000, LENGTH = 0x15fe0 /* 90080 bytes (alias RAM) */  
  SRAM1 (rwx) : ORIGIN = 0x4020000, LENGTH = 0x10000 /* 64K bytes (alias RAM2) */  
}

  /* Define a symbol for the top of each memory region */
  __base_PROGRAM_FLASH = 0x0  ; /* PROGRAM_FLASH */  
  __base_Flash = 0x0 ; /* Flash */  
  __top_PROGRAM_FLASH = 0x0 + 0xa0000 ; /* 640K bytes */  
  __top_Flash = 0x0 + 0xa0000 ; /* 640K bytes */  
  __base_SRAM = 0x4000000  ; /* SRAM */  
  __base_RAM = 0x4000000 ; /* RAM */  
  __top_SRAM = 0x4000000 + 0x15fe0 ; /* 90080 bytes */  
  __top_RAM = 0x4000000 + 0x15fe0 ; /* 90080 bytes */  
  __base_SRAM1 = 0x4020000  ; /* SRAM1 */  
  __base_RAM2 = 0x4020000 ; /* RAM2 */  
  __top_SRAM1 = 0x4020000 + 0x10000 ; /* 64K bytes */  
  __top_RAM2 = 0x4020000 + 0x10000 ; /* 64K bytes */  
