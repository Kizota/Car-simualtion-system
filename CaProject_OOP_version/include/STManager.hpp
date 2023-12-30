#ifndef STMANAGER_H
#define STMANAGER_H


//idea 

/*
 the stmanager gonna perfrom the logic -> which logic> 

 there are button signal and joystick signal that need t o be read 
 when button presseed -> call new task wait for yoystick command 

*/

class STManager
{
    private: 
    
    public:

};
#endif




/*
//NOTE - STEERING Wheels ECU
 
 - then first review the FreeRTOS 

 - transmitter
  based id: 0x30


  - stearing whheel has two main component 
      - button 
      - 
  
  
  - intergation with FreeRTOS
   - one task for for first button - hone 
   - second task for second button - blinker 

   - button pressed -> task create? 
   - logic combine between signals would need a manager to perfrom the logic? 
   - will the manager is the object perform the logic to call the new task/thread also?

   - button press -
   - the command is send through the Queue? bla bla bla 

   what if 
  - reading the signal from the JoyStick and the button at the same time in combien for serveral mode 
  - conflict? -> mutex or queue? mostly mutex?
  - button press call new thread? have to wait to read signal from JoyStick?


  create sensor as a test check read with FreeRTOS!!!! amazing idea 
*/