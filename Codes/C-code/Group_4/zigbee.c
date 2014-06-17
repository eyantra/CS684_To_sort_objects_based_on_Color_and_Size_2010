/**
 * Copyright (c) 2010, this is a property of ERTS Lab, IIT Bombay.
 * This project is developed as part of the Embedded Systems (Software) course CS 684.
 *
 * PROJECT TITLE: To sort object based on color and size usinf Firebird V robot
 *
 * @section PROJECT SUMMARY
 * The robot covers the entire arena according to a predetermined algorithm.
 * It detects objects and captures its image after proper alignment.
 * In ignores objects that are not of the desired color.
 * For the desired color objects it sorts the objects according to size and places them in seperate bins.
 * This piece of code, is converted to a hex file of ATMEGA2560 standard
 * and burnt onto the FIREBIRD V Robot.
 *
 * @file zigbee.c
 * This file contains definition of all the functions required for
 * serial communication between the PC and the robot using the Zigbee module
 * This file is included in the main file.
 * 
 * @author Group4. Arpita Agrawal, Ruttika Maheshwari, Sukanya Basu
 *
 * @date LAST MODIFIED: Nov 8, 2010
 * @version 1.0
 *
 */





/********************************************************************************
 Written by: Sachitanand Malewar, NEX Robotics Pvt. Ltd.
 AVR Studio Version 4.17, Build 666

 Date: 13th January 2010
 
 Application example: Robot control over serial port via ZigBee wireless comunication module 
 					  (located on the ATMEGA260 microcontroller adaptor board)

 Concepts covered:  serial communication
 
 Serial Port used: UART0

 There are two components to the motion control:
 1. Direction control using pins PORTA0 to PORTA3
 2. Velocity control by PWM on pins PL3 and PL4 using OC5A and OC5B.

 In this experiment for the simplicity PL3 and PL4 are kept at logic 1.
 
 Pins for PWM are kept at logic 1.
  
 Connection Details:  	
 						
  Motion control:		L-1---->PA0;		L-2---->PA1;
   						R-1---->PA2;		R-2---->PA3;
   						PL3 (OC5A) ----> Logic 1; 	PL4 (OC5B) ----> Logic 1; 


  Serial Communication:	PORTD 2 --> RXD1 UART1 receive for RS232 serial communication
						PORTD 3 --> TXD1 UART1 transmit for RS232 serial communication

						PORTH 0 --> RXD2 UART 2 receive for USB - RS232 communication
						PORTH 1 --> TXD2 UART 2 transmit for USB - RS232 communication

						PORTE 0 --> RXD0 UART0 receive for ZigBee wireless communication
						PORTE 1 --> TXD0 UART0 transmit for ZigBee wireless communication

						PORTJ 0 --> RXD3 UART3 receive available on microcontroller expainsion board
						PORTJ 1 --> TXD3 UART3 transmit available on microcontroller expainsion board

Serial communication baud rate: 9600bps
To control robot use number pad of the keyboard which is located on the right hand side of the keyboard.
Make sure that NUM lock is on.

Commands:
			Keyboard Key   ASCII value	Action
				8				0x38	Forward
				2				0x32	Backward
				4				0x34	Left
				6				0x36	Right
				5				0x35	Stop
				7				0x37	Buzzer on
				9				0x39	Buzzer off

 Note: 
 
 1. Make sure that in the configuration options following settings are 
 	done for proper operation of the code

 	Microcontroller: atmega2560
 	Frequency: 11059200
 	Optimization: -O0 (For more information read section: Selecting proper optimization options 
						below figure 4.22 in the hardware manual)

 2. Difference between the codes for RS232 serial, USB and wireless communication is only in the serial port number.
 	Rest of the things are the same. 

 3. For USB communication check the Jumper 1 position on the ATMEGA2560 microcontroller adaptor board

*********************************************************************************/

/********************************************************************************

   Copyright (c) 2010, NEX Robotics Pvt. Ltd.                       -*- c -*-
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

   * Source code can be used for academic purpose. 
	 For commercial use permission form the author needs to be taken.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. 

  Software released under Creative Commence cc by-nc-sa licence.
  For legal information refer to: 
  http://creativecommons.org/licenses/by-nc-sa/3.0/legalcode

********************************************************************************/


#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>


/*** Function for initialization ***/
void uart0_init(void)
{
 UCSR0B = 0x00;         //disable while setting baud rate
 UCSR0A = 0x00;
 UCSR0C = 0x06;
 UBRR0L = 0x47;         //set baud rate lo
 UBRR0H = 0x00;         //set baud rate hi
 UCSR0B = 0x98;
}



/*** ISR for receiving complete interrupt ***/
SIGNAL(SIG_USART0_RECV)
{
        data=UDR0;

	if((flag_rx == 0)&&( data == '0'))
	{
		UDR0='E';
	}
	else if(flag_rx == 1)
	{
		UDR0='0';
	}
	
	if (data == 'B')        //objects of larger area...to be kept on right
	{
		UDR0='R';
		flag_x=1;
        } 

	else if (data== 'C')    // object of smaller area to be kept on left
	{
		UDR0= 'R';
	        flag_x=1;
	} 
        
        else if (data == 'D')
        {
        	UDR0='R';
        	flag_x=1;
        }

        else
        {
        	UDR0 = '0';
        }
}


/*** Function To Initialize all The Devices ***/
void init_uart0()
{
 cli();         //Clears the global interrupts
 uart0_init();  //Initailize UART1 for serial communiaction
 sei();         //Enables the global interrupts
}


