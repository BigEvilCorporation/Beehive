/*
 * Setup the memory map of the Innovasic SBC 
 * stack grows down from high memory.
 *
 * The memory map for the ROM model looks like this:
 *
 * +--------------------+ <-address 0 in Flash
 * | .vector_table      |
 * +--------------------+ <- low memory
 * | .text              |
 * |        _etext      |
 * |        ctor list   | the ctor and dtor lists are for
 * |        dtor list   | C++ support
 * +--------------------+
 * | DCACHE_CODE        | code to be loaded into DCACHE
 * |     _dcache_start  |
 * |     _dcache_end    |
 * +--------------------+
 * | .data              | initialized data goes here
 * +--------------------+
 * .                    .
 * .                    .
 * .                    .
 * +--------------------+ <- The beginning of the SRAM area
 * | .data              | a wriable copy of data goes here.
 * |        _edata      |
 * +--------------------+
 * | .bss               |
 * |        __bss_start | start of bss, cleared by crt0
 * |        _end        | start of heap, used by sbrk()
 * |        _heapend    |    End   of heap, used by sbrk()
 * +--------------------+
 * .                    .
 * .                    .
 * .                    .
 * |        __stack     | top of stack
 * +--------------------+
 *
 *
 * The memory map for the RAM model looks like this:
 *
 * +--------------------+ <- The beginning of the SRAM or SDRAM area.
 * | .vector_table      |
 * +--------------------+ <- low memory
 * | .text              |
 * |        _etext      |
 * |        ctor list   | the ctor and dtor lists are for
 * |        dtor list   | C++ support
 * +--------------------+
 * | DCACHE_CODE        | code to be loaded into DCACHE
 * |     _dcache_start  |
 * |     _dcache_end    |
 * +--------------------+
 * | .data              | initialized data goes here
 * |        _edata      |
 * +--------------------+
 * | .bss               |
 * |        __bss_start | start of bss, cleared by crt0
 * |        _end        | start of heap, used by sbrk()
 * |        _heapend    |    End   of heap, used by sbrk()
 * +--------------------+
 * .                    .
 * .                    .
 * .                    .
 * |        __stack     | top of stack
 * +--------------------+
 */

STARTUP(fido-ram-crt0.o)
OUTPUT_ARCH(m68k)
ENTRY(_start);
GROUP(-lunhosted -lfido -lc -lgcc)

MEMORY {
  /* Flash ROM.  */
  rom (rx)      : ORIGIN = 0x0000000, LENGTH = 0x800000
  /* Internal SRAM.  */
  int_ram (rwx) : ORIGIN = 0x1000000, LENGTH = 0x6000
  /* External SDRAM.  */
  sdram (rwx)   : ORIGIN = 0x2000000, LENGTH = 0x800000
  /* External SRAM.  */
  sram (rwx)     : ORIGIN = 0x03000000, LENGTH = 0x00100000
}

SECTIONS {
  /* The interrupt vector is placed at the beginning of sdram,
     as required at reset.  */
  .vector_table : {
    *(.vector_table)
  } > sdram

  /* Text section.  */
  .text :
  {
    *(.text .text.*)
    *(.gnu.linkonce.t.*)

    . = ALIGN(0x4);
    /* These are for running static constructors and destructors under ELF.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*crtend.o(.ctors))
    KEEP (*crtbegin.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*crtend.o(.dtors))

    . = ALIGN(0x4);
    KEEP (*crtbegin.o(.jcr))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .jcr))
    KEEP (*crtend.o(.jcr))

    *(.rodata .rodata.*)
    *(.gnu.linkonce.r.*)
    *(.gcc_except_table) 
    *(.eh_frame)

    . = ALIGN(0x2);
    _init = . ;
    LONG (0x4e560000)	/* linkw %fp,#0 */
    *(.init)
    SHORT (0x4e5e)	/* unlk %fp */
    SHORT (0x4e75)	/* rts */

    _fini = . ;
    LONG (0x4e560000)	/* linkw %fp,#0 */
    *(.fini)
    SHORT (0x4e5e)	/* unlk %fp */
    SHORT (0x4e75)	/* rts */
    . = ALIGN(0x800);   /* align to a 2K dcache boundary */
    _dcache_start = .;
    *(DCACHE_CODE)
    _dcache_end = .;
    _etext = .;
    *(.lit)
    . = ALIGN(0x4);
    __start_romdata = .;
  } > sdram

  /* Initialized data section.  */
  .data :
  {
    _data = .;
    *(.got.plt) *(.got)
    *(.shdata);
    *(.data .data.*)
    *(.gnu.linkonce.d.*)
    _edata_cksum = .;
    *(checksum);
    _edata = .;
  } > sdram  

  /* Zero-initialized data.  */ 
  .bss :
  {
    . = ALIGN(0x4);
    __bss_start = . ;
    *(.shbss)
    *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    _end =  ALIGN (0x8);
    __end = _end;
  } > sdram

  /* Specially designated data is placed in the internal RAM.  */
  fast_memory :
  {
    . = ALIGN(0x4);
    __fast_start = .;
    *(FAST_RAM)
    __fast_stop = .;
  } > int_ram
}

PROVIDE (__stack = 0x021ffffc);

PROVIDE (_heapend = 0x02180000);
