# IRhard
Arduino library to decode NEC signals from an IR detector via hardware interrupt.

   Everything here is presented "as is" and initially considered amateur until further contributions verify, validate, or improve and correct the procedures and methods involved.
   
   The intent is an Arduino library consisting of a class with direct access to data via members rather than methods, and a hardware interrupt routine that updates the class instance members. The initial methodology allows the hardware interrupt routine to update class instance members via global pointers, and does not include any class methods other than instantiation, but either or both of these may change.
   
   To Do: Add code to count "repeat" signals, indicating how long a key was held beyond the initial keypress. This may add a method to the class for resetting the count as it is retrieved. It may include two count variables: the number of repeats of the last key (since read/reset) before a more recent key was pressed, and the count of repeats from the recent key since it was pressed.
   
File structure of an IRhard folder within the the Arduino library folder should include:  
 * README.md  
 * IRhard.h  
 * IRhard.cpp  
 * keywords.txt  
 * library.properties
 * examples/IRreader/IRreader.ino
