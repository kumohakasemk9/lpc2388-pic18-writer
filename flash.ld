MEMORY {
	FLASH (rx) : ORIGIN = 0x0 , LENGTH = 512K
	RAM (wx) : ORIGIN = 0x40000000 , LENGTH = 64K
}

SECTIONS {
	.text : {
		start.o(.text);
		*(.text*);
		*(.ARM.*);
		*(.rodata*);
		_idata = .;
	} >FLASH
	.data : {
		_sdata = .;
		*(.data);
		*(.data.*);
		_edata = .;
	} >RAM AT>FLASH
	.bss : {
		_sbss = .;
		*(.bss);
		*(.bss.*);
		*(COMMON);
		_ebss = .;
	} >RAM
	_stack = ORIGIN(RAM) + LENGTH(RAM);
}
