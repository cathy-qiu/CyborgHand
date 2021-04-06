# Cyborg Hand

<p float="left">
  <img src="https://user-images.githubusercontent.com/36112125/113521681-14eb4600-9569-11eb-9436-58c40825cad0.png" alt="Your image title" width="300"/>
  <img src="https://user-images.githubusercontent.com/36112125/113521683-17e63680-9569-11eb-9afa-3083d6fe26a2.png" alt="Your image title" width="270"/>
  <img src="https://user-images.githubusercontent.com/36112125/113521684-1b79bd80-9569-11eb-81b7-6acd9adf7852.png" alt="Your image title" width="240"/>
 </p>

:mechanical_arm: This code is used to control a prosthetic hand that performs grasping and pinching movements. A PID motor control algorithm was implemented in C to actuate the hand with 93% accuracy.

This project is intended to restore the ability of basic grasping to the user with a minimalistic device.
The robot arm aims to support people with a forearm, hand/palm, and wrist, but without functional fingers, or without fingers altogether. 
Their lack of a fully functioning hand causes them to experience disruptions in everyday life, mainly due to their inability to grasp. 
A common disruption would be a daily moment as simple as the inability to grab a cup of coffee or water, which to the average person is a task that requires minimal effort.

A PID algorithm was used to control the pointer fingers’ approach to the thumb, which enabled a proper and consistent pinch. 
We found the target encoder limits by accessing the motor port view in the EV3, which shows encoder values  relative to the starting 0 position which was the reset/rest position.

The [PID—proportional derivative control algorithm](https://www.matthewpeterkelly.com/tutorials/pdControl/index.html) is a loop that continuously checks the difference between the setpoint (target) and the real-time encoder value. The algorithm sets a motor speed that is proportionalised by a constant that permits the motor to 
calmly and accurately approach the target position rather than a [bang-bang control](https://en.wikipedia.org/wiki/Bang%E2%80%93bang_control).  
The integral part ensures that there is no [steady state error](https://www.electrical4u.com/steady-state-error-analysis/#:~:text=Steady%2Dstate%20error%20is%20defined,response%20for%20a%20linear%20system.), 
giving more precision.  
The derivative part of the algorithm ensures that larger position changes will speed up the motor and keep it on track to reaching the set point.

### Principle of Design ###
The main concept of the software design for this project is to break down each task into many small functions then integrate them together with intermediate functions. 
This way, we can perform unit tests on smaller portions of the program, and it becomes easy to diagnose when problems occur. 
The hierarchy of functions is seen below.

<img src="https://user-images.githubusercontent.com/36112125/113522200-955f7600-956c-11eb-9ced-f97620943669.PNG" alt="Your image title" width="500"/>

This top-down structure allows control functions to be easily readable, since it does not contain the low-level details regarding sensors and motors.


**Check out a video of how it works [here](https://www.youtube.com/watch?v=mdVFzBSiMns)**

Team members: Kevin Lin, Maahir Gupta, Jupi, Cathy Qiu
