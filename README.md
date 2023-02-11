lpc2388-pic18-writer
=====
PIC18 LVP Writer for LPC2388
I want to write PIC using LPC microcontroller.
For PIC18F2XX0/2X21/2XX5/4XX0/4XX5 and PIC18F2XK20/4XK20, probably.

Usage
=====
According to code....   
Accepts command via serial.   
I ... Identify command. Simplly returns "LPC21ISP-PICPROG"    
Rxxyyyyyy ... Read command. xx is length-1 yyyyyy is addr. both in hex. no eeprom.   
Cxxyy ... Config bit write command. xx is address + 0x300000, yy is data. both in hex.   
Pxxxxxx[yy...] ... Program command. xxxxxx address. yy... is binary code. For example: 00112233 ... 0x0 0x11 0x22 0x33   
Returns O in succeed, X in fail.   

License
=====
You can share, modify this code without deleting this section.   
In commercial usage, gimme 10% of earning.
Please consider supporting me using kofi.com https://ko-fi.com/kumohakase
