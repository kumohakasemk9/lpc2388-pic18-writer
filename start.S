//lpc2388 startup routine
//Copyright (C) 2022 Kumohakase
//Please consider supporting me through kofi.com https://ko-fi.com/kumohakase

.text
.arm
b			Reset_Handler
b			UnDef_Handler
b			SWI_Handler
b			PAbrt_Handler
b			DAbrt_Handler
.word		0
b			IRQ_Handler
b			FIQ_Handler

Reset_Handler:
ldr		sp,=_stack
ldr		r0,=_idata
ldr		r1,=_sdata
ldr		r2,=_edata
CopyDataLoop:
cmp		r1,r2
beq		EndCopyDataLoop
ldrb		r3,[r0],#1
strb		r3,[r1],#1
b			CopyDataLoop
EndCopyDataLoop:
ldr		r0,=_sbss
ldr		r1,=_ebss
mov		r2,#0
ZFillBssLoop:
cmp		r0,r1
beq		main
strb		r2,[r0],#1
b			ZFillBssLoop

UnDef_Handler:
SWI_Handler:
PAbrt_Handler:
DAbrt_Handler:
IRQ_Handler:
FIQ_Handler:
b			.
