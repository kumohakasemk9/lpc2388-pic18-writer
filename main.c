//lpc2388 pic writer program for PIC18
//available for PIC18F2XX0/2X21/2XX5/4XX0/4XX5 and PIC18F2XK20/4XK20
//Copyright (C) 2022 Kumohakase
//Please consider supporting me through kofi.com https://ko-fi.com/kumohakase

#include <lpc23xx.h>
#include <stdlib.h>
#include <string.h>

void delay(int); //2 delay = 1uS
void print(char*);
void println(char*);
void printC(char);
void printI(long,int);
void writecmd(int,int);
int readcmd(int);
void write4bitcmd(int);
void writeptr(int);
void strcpyc(char*,char*,int);
void programdelay();
void main() {
	SCS = 0x21;
	while((SCS & 0x40) == 0);
	CLKSRCSEL = 0x1;
	FIO1DIR = 1 << 18;
	FIO0DIR = 0x12; //PGD PGC - - PGM
	PINSEL0 |= 0x50;
	PCLKSEL0 = 0x40;
	U0LCR = 0x80;
	U0DLL = 4;
	U0FDR = 0x85;
	U0LCR = 0x3;
	U0FCR = 0x1;
	char buffer[255],*e,tmp[64];
	int bp = 0,i;
	while(1) {
		if((U0LSR & 0x1) != 0) {
			buffer[bp] = U0RBR;
			if(buffer[bp] == '\n') {
				buffer[bp] = '\0';
				bp = 0;
				FIO0SET = 0x10;
				delay(10);
				if(strcmp(buffer,"I") == 0) {
					println("LPC21ISP-PICPROG");
				} else if(buffer[0] == 'R' && strlen(buffer) == 9) {
					unsigned int p = strtol(buffer+1,&e,16);
					int l = (p >> 24) & 0xff;
					int a = p & 0xffffff;
					if(e != buffer+1 && 0 <= l && l <= 0xfe && 0 <= a && a + l <= 0x3fffff) {
							writeptr(a);
							for(i = 0;i < l + 1;i++) {
								printI(readcmd(9),2);
							}
							println("");
						} else { println("X"); }
				} else if(buffer[0] == 'C' && strlen(buffer) == 5) {
					int p = strtol(buffer+1,&e,16);
					int a = (p >> 8) & 0xff;
					int v = p & 0xff;
					if(e != buffer + 1 && 0 <= a && a <= 0xd && 0 <= v && v <= 0xff) {
						writecmd(0,0x8ea6);
						writecmd(0,0x8ca6);
						writeptr(0x300000 + a);
						if(a % 2 == 0) {writecmd(15,v);} else {writecmd(15,v << 8);}
						programdelay();
						println("O");
					} else { println("X"); }
				} else if(buffer[0] == 'P' && strlen(buffer) > 10) {
					strcpyc(tmp,buffer+1,6);
					int a = strtol(tmp,&e,16);
					int l = strlen(buffer+7);
					if(e != tmp && l % 4 == 0 && 0 <= a && a + (l / 2) - 1 <= 0x3fffff && l <= 64) {
						l = l / 4;
						writecmd(0,0x8ea6);
						writecmd(0,0x9ca6);
						writeptr(a);
						for(i = 0;i < l;i++) {
							strcpyc(tmp,buffer+7+i*4,4);
							int d = strtol(tmp,&e,16);
							if(e != tmp && 0 <= d && d <= 0xffff) {
								if(i != l - 1) {writecmd(13,d);} else {writecmd(15,d);}
							} else {
								println("X");
								break;
							}
						}
						if(i == l) {
								programdelay();
								println("O");
						}
					} else { println("X"); }
				} else if(strcmp(buffer,"E") == 0) {
					writeptr(0x3c0005);
					writecmd(12,0x3f3f);
					writeptr(0x3c0004);
					writecmd(12,0x8f8f);
					writecmd(0,0);
					write4bitcmd(0);
					delay(2000);
					println("O");
				} else { println("X"); }
				FIO0CLR = 0x10;
			} else if(bp < 254) {
				bp++;
			}
		}
	}
}

void delay(int w) {
	for(int i = 0;i < w;i++);
}

void print(char *d) {
	while(*d) {
		printC(*d);
		d++;
	}
}

void println(char *c) {
	print(c);
	printC('\n');
}

void printC(char c) {
	U0THR = c;
	while((U0LSR & 0x40) == 0);
}

void printI(long v, int l) {
	char buf[128];
	itoa(v,buf,16);
	int n = l - strlen(buf);
	if(n > 0) {for(int i = 0;i < n;i++) {printC('0');}}
	print(buf);
}

void write4bitcmd(int c) {
	FIO0DIR |= 0x1;
	for(int i = 0;i < 4;i++) {
		if((c & (1 << i)) == 0) {FIO0CLR = 0x1;} else {FIO0SET = 0x1;}
		FIO0SET = 0x2;
		delay(1);
		FIO0CLR = 0x2;
		delay(1);
	}
}

void writecmd(int c,int p) {
	write4bitcmd(c);
	for(int i = 0;i < 4;i++) {
		write4bitcmd(p >> (i * 4));
	}
}

int readcmd(int c) {
	int r = 0;
	write4bitcmd(c);
	FIO0DIR &= 0xfffffffe;
	for(int i = 0;i < 16;i++) {
		FIO0SET = 0x2;
		delay(1);
		if(i > 7 && (FIO0PIN & 0x1) != 0) {r += 1 << (i-8);}
		FIO0CLR = 0x2;
		delay(1);
	}
	return r;
}

void writeptr(int a) {
	for(int i = 0;i < 3;i++) {
		writecmd(0,0xe00 + ((a >> ((2-i) * 8)) & 0xff));
		writecmd(0,0x6ef8-i);
	}
}

void strcpyc(char* dst, char *src, int n) {
	for(int i = 0;i < n;i++) {
		*dst = *src;
		dst++;
		src++;
		if(*src == '\0') {break;}
	}
	*dst = '\0';
}

void programdelay() {
	FIO0DIR |= 0x1;
	for(int i = 0;i < 4;i++) {
		FIO0SET = 0x2;
		if(i == 3) {delay(1000);} else {delay(1);}
		FIO0CLR = 0x2;
		delay(1);
	}
	delay(100);
}
