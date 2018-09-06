# IRhard
Arduino header to decode NEC signals from an IR detector via hardware interrupt. Was a library, but need only be a header.

   Everything here is presented "as is" and initially considered amateur until further contributions verify, validate, or improve and correct the procedures and methods involved.
   
   The intent is an Arduino header consisting of a few global variables, and ISR to update those variables, and an initialization function to hook the ISR to the hardware interrupt. The new methodology has no class, and uses global variables instead of global pointers to instance members.
   
   To Do: Add code to count "repeat" signals, indicating how long a key was held beyond the initial keypress. This may add a function for resetting the count as it is retrieved. It may include two count variables: the number of repeats of the last key (since read/reset) before a more recent key was pressed, and the count of repeats from the recent key since it was pressed.
   
File structure of an IRhard folder within the the Arduino library folder should include:  
 * IRhard.h  
 * keywords.txt  
 * library.properties
 * examples/IRdemo/IRdemo.ino

IRhard.h is the only file required for use, and may be copied the the same folder as a sketch that includes it. This may be preferred while trying and testing, rather than messing with the library.
