TEAM 4:To sort objects based on Color and Size using Firebird V robot 
----------------------------------------------------------------------

Ruttika Maheshwari (9307045)
Arpita Agrawal (9307935)
Sukanya Basu (9305908)



PROJECT TITLE:  To sort object based on color and size usinf Firebird V robot
-----------------------------------------------------------------------------

The robot covers the entire arena according to a predetermined algorithm.
It detects objects and captures its image after proper alignment.
In ignores objects that are not of the desired color.
For the desired color objects it sorts the objects according to size and places them in seperate bins.


PROJECT OBJECTIVES
------------------

The main objective of this project is to develop a prototype of a farm assistant robot that will automatically detect and pick objects as per the specifications. It will sort the objects based on color and size, and place them separately.


The robot will move in its field and do the following:

1. The field of movement of the robot is bounded by a white line strip. The robot will move according to a predetermined algorithm to cover this field.
2. If it detects an object on any of its side sensors, it will turn to alogn itself to face the object.
3. It will move forward and stop at a particular distance from the object.
4. The robot  will then take a picture and send this image to Matlab on the PC. This communication is done via Zigbee.
5. In Matlab environment, first the color of the object is detected.
6. If it is not of required color, it is ignored.
7. If it is of required color, the size of the object is detected.
8. Accordingly, Matlab sends signal to the robot whether or not to pick the object, again through Zigbee.
9. If it is to pick up the object, the robot will move forward and pick the object using the robotic arm and place it in the respective basket.
10. When the robot is done with an object (it can either ignore, or pick and place an object), it will revert back to its original position on the path.
    It will then move fron here for the next object.
11. When the robot completes covering the entire field, it will come to a stop.


Hardware Platform
------------------

The following hardware parts has been used in the project.

1. Firebird V ATMEGA2560
2. Four IR Sharp Sensors of variety GP2D12
3. Three additional servo motors, in the robotic arm.
4. Zigbee module for serial communication.
5. 2 MegaPixel USB Camera



Software
----------


The following software platforms have been used.

1. AVR Studio 4. This is an IDE for compiling and building the project. It also burns the code on the robot.

2. Matlab 2008. Matlab is used for image processing.


Communication is done using zigbee. The information is sent to firebird from matlab using the serial communication zigbee interface.



DELIVERABLES
------------

Filename                                Contains
--------                                --------
C-code.tar.gz                           Commented Source Code of programs to be burnt on Robot

PC-interface.tar.gz                     Contains Matlab files

Documents.tar.gz                        Contains Project related doc files





CODE FILES
-----------

Filename                Purpose                                         Executes on
--------                -------                                         -----------
Main.c                  Main Program                                    Robot
servo.c                 Methods for servo motors                        Robot
cover_field.c           Methods for algorithm for covering the field    Robot
zigbee.c                Methods for zigbee communication                Robot
serial1.m               For serial communication                        PC




EXECUTION INSTRUCTIONS
----------------------

The following steps are to be followed in order to run the project.

1.Extract project files from C-code.tar.gz and PC-interface.tar.gz
2.Open project project.aps in AVR Studio 4
3.Compile and build the project
4.Set frequency at 11059200 Hz
5.Burn the code on the robot
6.Execute the Matlab code
7.Execute the AVR code





