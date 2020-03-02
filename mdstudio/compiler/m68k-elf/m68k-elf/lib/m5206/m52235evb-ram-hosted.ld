STARTUP(cf-hosted-crt0.o)
OUTPUT_ARCH(m68k)
ENTRY(__start)
SEARCH_DIR(.)
GROUP(-lc -lhosted -lcf)
__DYNAMIC  =  0;

MEMORY
{
  
  ram (rwx) : ORIGIN = 512M, LENGTH = 32K
}

/* Place the stack at the end of memory, unless specified otherwise. */
PROVIDE (__stack = 512M + 32K);

SECTIONS
{
  .text :
  {
    CREATE_OBJECT_SYMBOLS
    *(.interrupt_vector)
    
    cf-hosted-crt0.o(.text)
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

    . = ALIGN(0x4);
    *(.gcc_except_table) 

    . = ALIGN(0x4);
    *(.eh_frame)

    . = ALIGN(0x4);
    _init = . ;
    LONG (0x4e560000)	/* linkw %fp,#0 */
    *(.init)
    SHORT (0x4e5e)	/* unlk %fp */
    SHORT (0x4e75)	/* rts */

    . = ALIGN(0x4);
    _fini = . ;
    LONG (0x4e560000)	/* linkw %fp,#0 */
    *(.fini)
    SHORT (0x4e5e)	/* unlk %fp */
    SHORT (0x4e75)	/* rts */

    *(.lit)

    . = ALIGN(4);
    _etext = .;
  } >ram

  .data :
  {
    __data_load = LOADADDR (.data);
    __data_start = .;
    *(.got.plt) *(.got)
    *(.shdata)
    *(.data .data.*)
    *(.gnu.linkonce.d.*)
    . = ALIGN (4);
    _edata = .;
  } >ram 

  .bss :
  {
    __bss_start = . ;
    *(.shbss)
    *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN (8);
    _end = .;
    __end = _end;
  } >ram 

  .stab 0 (NOLOAD) :
  {
    *(.stab)
  }

  .stabstr 0 (NOLOAD) :
  {
    *(.stabstr)
  }

  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }

}
