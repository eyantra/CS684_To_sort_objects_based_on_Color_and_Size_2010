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
 * @file main.c
 * This is the main file.
 * It contains implementation of the algorithm and calls to all the functions.
 * 
 * @author Group4. Arpita Agrawal, Ruttika Maheshwari, Sukanya Basu
 *
 * @date LAST MODIFIED: Nov 8, 2010
 * @version 1.0
 *
 * @section LICENSE
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 *
 * @section COMPILATION
 * Compile using the AVR studio for ATMEGA2560 and F_CPU=11059200
 * Write the .hex file to the FIREBIRD V robot
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <util/delay.h>
#include <math.h>
#include "cover_field.c"
#include "servo.c"
#include "zigbee.c"


unsigned int value_left, value_center, value_right,value_center_left,value_center_right;
unsigned int temp,count,temp1, rev_dis;
unsigned int flag_turn = 0;
unsigned int flag = 0;
unsigned int r_count=0;
unsigned int flag_rx = 1;
unsigned char data;
unsigned int flag_x = 0;



int main(void)
{

/*** Calls to initialization functions ***/
init_devices();
init_devices_servo();
timer5_init();
INIT_PORTS();
lcd_set_4bit();
lcd_init();
INIT_INTERRUPT();
uart0_init();


/*** Setting initial position of arm.. SERVO_1 = Gripper, SERVO_2 = UP/DOWN, SERVO_3 = LEFT/RIGHT ***/
	
	servo_1(180);
	_delay_ms(200);
	servo_2(50);
	_delay_ms(200);
	servo_3(85);
	
	Left_Speed(148);
	Right_Speed(160);
	FORWARD();

while(1)
{
	flag_rx = 1;
	flag_turn = 0;
	flag = 0;
	r_count=0;
	flag_x = 0;

	
	sharp_left = ADC_Conversion(9);
	value_left = Sharp_GP2D12_estimation(sharp_left);
	lcd_print(2,1,value_left,3); 
	
	sharp_right = ADC_Conversion(13);
	value_right = Sharp_GP2D12_estimation(sharp_right);
	lcd_print(2,13,value_right,3);


	sharp_center_left=ADC_Conversion(10);
	value_center_left=Sharp_GP2D12_estimation(sharp_center_left);
	lcd_print(1,1,value_center_left,3); 

	sharp_center = ADC_Conversion(11);
	value_center = Sharp_GP2D12_estimation(sharp_center);
	lcd_print(1,7,value_center,3);
	
	
	white_1 = ADC_Conversion(1);
	white_2 = ADC_Conversion(2);
	white_3 = ADC_Conversion(3);
			
        /* To check if white line is detected */
	if (white_1<30 || white_3<30)
	{
		sharp_left = ADC_Conversion(9);
		value_left = Sharp_GP2D12_estimation(sharp_left);
	        lcd_print(2,1,value_left,3); 
	
	        sharp_right = ADC_Conversion(13);
        	value_right = Sharp_GP2D12_estimation(sharp_right);
        	lcd_print(2,13,value_right,3);


	        sharp_center_left=ADC_Conversion(10);
	        value_center_left=Sharp_GP2D12_estimation(sharp_center_left);
	        lcd_print(1,1,value_center_left,3); 

	        sharp_center = ADC_Conversion(11);
	        value_center = Sharp_GP2D12_estimation(sharp_center);
	        lcd_print(1,7,value_center,3);
				
		white_1 = ADC_Conversion(1);
        	white_2 = ADC_Conversion(2);
		white_3 = ADC_Conversion(3);

                /* Taking U turn alternately in opposite directions */
		FORWARD();
		if(flag%2 == 0)
		{
		        REVERSE ();
		        LINEAR_DISTANCE(150);
		        STOP();

			RIGHT_TURN();
			ANGLE_ROTATE(90);
				
			FORWARD();
			LINEAR_DISTANCE(250);
						
			FORWARD();
			RIGHT_TURN();
			ANGLE_ROTATE(90);
		}
		else
		{
			REVERSE ();
			LINEAR_DISTANCE(150);
			STOP();
			
			LEFT_TURN();
			ANGLE_ROTATE(90);
					
			FORWARD();
			LINEAR_DISTANCE(250);
						
			FORWARD();
			LEFT_TURN();
			ANGLE_ROTATE(90);
		}
		flag++;
	}

	/* While object is not detected in the left or right sensors  */
	if (((value_left > 230) && (value_left < 270))|| ((value_right > 230) &&  (value_right < 270)))
	{
		STOP();
		count = 0;
		temp = 0;
		/* Logic to turn and align when object is detected on the left side */
		if (value_left < 270 && value_left > 230) 
		{
			flag_turn=1;
			temp = value_left;
			FORWARD();
			LEFT_TURN();
			ANGLE_ROTATE(80);

			sharp_center_left=ADC_Conversion(10);
			value_center_left=Sharp_GP2D12_estimation(sharp_center_left);
			lcd_print(1,1,value_center_left,3); 

			sharp_center = ADC_Conversion(11);
			value_center = Sharp_GP2D12_estimation(sharp_center);
			lcd_print(1,7,value_center,3);
		
			/* Fine alignment with object */
		backl:
			while(1)
			{
			        if (((value_center_left <= (temp-40)) && (value_center <= (temp-40))) && (value_center < value_center_left))
	                       	{
			                break;
		                }
			        else
			        {
			         	count++;
				        FORWARD();
				        LEFT_TURN_SOFT();
				        _delay_ms(100);
				        STOP();

				        sharp_center_left=ADC_Conversion(10);
				        value_center_left=Sharp_GP2D12_estimation(sharp_center_left);
				        lcd_print(1,1,value_center_left,3); 

				        sharp_center = ADC_Conversion(11);
				        value_center = Sharp_GP2D12_estimation(sharp_center);
				        lcd_print(1,7,value_center,3);

			        }
			}

			STOP();
			_delay_ms(1000);
			if (((value_center_left <= (temp-40)) && (value_center <= (temp-40))) && (value_center < value_center_left))
			{}
			else
			{
			        goto backl;
			}
		}

                /* Turn and align when object is detected on the right side */
		else if ((value_right < 270) && (value_right > 230))
		{
			flag_turn=2;
			temp = value_right;
			
			FORWARD();
			RIGHT_TURN();
			ANGLE_ROTATE(80);
			/* Front IR On */
			sharp_center_left=ADC_Conversion(10);
			value_center_left=Sharp_GP2D12_estimation(sharp_center_left);
			lcd_print(1,1,value_center_left,3); 

			sharp_center = ADC_Conversion(11);
			value_center = Sharp_GP2D12_estimation(sharp_center);
			lcd_print(1,7,value_center,3);
			
                /* Fine alignment with object */
		backr:
			while(1)
			{
			        if ((value_center_left <= (temp-40)) && (value_center <= (temp-40)))
			        {
			
				        break;
			        }

			        else
			        {
			                count++;
				        FORWARD();
				        RIGHT_TURN_SOFT();
				        _delay_ms(100);
				        STOP();


				        sharp_center_left=ADC_Conversion(10);
				        value_center_left=Sharp_GP2D12_estimation(sharp_center_left);
				        lcd_print(1,1,value_center_left,3); 

				        sharp_center = ADC_Conversion(11);
				        value_center = Sharp_GP2D12_estimation(sharp_center);
				        lcd_print(1,7,value_center,3);
	
			        }


			}
				
			STOP();
			_delay_ms(1000);
			if (((value_center_left <= (temp-40)) && (value_center <= (temp-40))) && (value_center < value_center_left))
			{}
			else
			{
			goto backr;
			}

		}

	

		/* Set bot at particular distance from object */

		sharp_center = ADC_Conversion(11);
		value_center = Sharp_GP2D12_estimation(sharp_center);
		lcd_print(1,7,value_center,3);


                sharp_center_left=ADC_Conversion(10);
		value_center_left=Sharp_GP2D12_estimation(sharp_center_left);
		lcd_print(1,1,value_center_left,3); 

		if(value_center <= value_center_left)
		{
			temp1=value_center-82;
		}
		else if(value_center > value_center_left)
		{
			temp1=value_center_left-82;
		}
		

                if (value_center > 230 )
        	{
        		r_count=0;
        		FORWARD();
        		LINEAR_DISTANCE(temp1);
        		STOP();
               	}	
		
		else
	        {
		     	r_count=0;
			FORWARD();
		        LINEAR_DISTANCE(temp1);
		        STOP();
	        }
	

		rev_dis= temp1;
		flag_rx=0;
		flag_x=0;

                while(flag_x == 0)
                {
                }


                /* Logic to pick and place  */

		if (data == 'B')  //objects of larger area...to be kept on right
		{

			servo_3(85);
			_delay_ms(2000);
			
			servo_2(105);
			_delay_ms(2000);

			FORWARD();
			LINEAR_DISTANCE(50);
			STOP();
			servo_1(0);
			_delay_ms(2000);
			servo_2(65);
			_delay_ms(2000);
			servo_3(30); 
			_delay_ms(2000);
			servo_1(180);
			_delay_ms(2000);
			servo_3(85);
			_delay_ms(2000);

			REVERSE();
			LINEAR_DISTANCE(rev_dis);
			STOP();
		} 

	        else if (data== 'C') // object of smaller area to be kept on left
		{
			servo_3(85);
			_delay_ms(2000);
			
			servo_2(105);
			_delay_ms(2000);
			
			FORWARD();
			LINEAR_DISTANCE(50);
			STOP();

			servo_1(0);
			_delay_ms(2000);
			servo_2(65);
			_delay_ms(2000);
			servo_3(130);
			_delay_ms(2000);
			servo_1(180);
			_delay_ms(2000);
			servo_3(85);
			_delay_ms(2000);

			REVERSE();
			LINEAR_DISTANCE(rev_dis);
			STOP();

		} 

        	while(r_count!=0)
        	{
		        REVERSE();
		        r_count--;
		        _delay_ms(50);
		        STOP();
		        sharp_center = ADC_Conversion(11);
		        value_center = Sharp_GP2D12_estimation(sharp_center); // Not mandatory but added just to provide same time lag
		        lcd_print(1,7,value_center,3);

		}

        	/*  To bring the bot to the same position on the path where it started from, before detecting the object */
	
		if(flag_turn == 1)   // If Left turn earlier falg == 1 and if right turn earlier flag == 2 else flag ==0
		{
	        	while(!(count == 0))
			{
				REVERSE();
				RIGHT_REVERSE();
				count--;
				_delay_ms(50);
				STOP();


				sharp_center_left=ADC_Conversion(10);
				value_center_left=Sharp_GP2D12_estimation(sharp_center_left);
				lcd_print(1,1,value_center_left,3); 

				sharp_center = ADC_Conversion(11);
				value_center = Sharp_GP2D12_estimation(sharp_center);
				lcd_print(1,7,value_center,3);
	

			}
			FORWARD();
			RIGHT_TURN();
			ANGLE_ROTATE(80);
			flag_turn=0;

		}
		else if(flag_turn == 2)
		{
			
			while(!(count == 0))
			{
				REVERSE();
				LEFT_REVERSE();
				count--;
				_delay_ms(50);
				STOP();


				sharp_center_left=ADC_Conversion(10);
				value_center_left=Sharp_GP2D12_estimation(sharp_center_left);
				lcd_print(1,1,value_center_left,3); 

				sharp_center = ADC_Conversion(11);
				value_center = Sharp_GP2D12_estimation(sharp_center);
				lcd_print(1,7,value_center,3);

			}
			REVERSE();
			LEFT_TURN();

			ANGLE_ROTATE(80);
			
			flag_turn=0;
		}
				

		STOP();
		sharp_left = ADC_Conversion(9);
	        value_left = Sharp_GP2D12_estimation(sharp_left);
	        lcd_print(2,1,value_left,3); 
	
	        sharp_right = ADC_Conversion(13);
	        value_right = Sharp_GP2D12_estimation(sharp_right);
	        lcd_print(2,13,value_right,3);


	        sharp_center_left=ADC_Conversion(10);
	        value_center_left=Sharp_GP2D12_estimation(sharp_center_left);
	        lcd_print(1,1,value_center_left,3); 

	        sharp_center = ADC_Conversion(11);
	        value_center = Sharp_GP2D12_estimation(sharp_center);
	        lcd_print(1,7,value_center,3);
			
		white_1 = ADC_Conversion(1);
		white_2 = ADC_Conversion(2);
		white_3 = ADC_Conversion(3);
		
	}
	FORWARD();
}
}
