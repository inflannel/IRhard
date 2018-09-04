/* IRhard library version 1.0.0 by u/inflannel, September 2018. <-- Check the date.
 *       By the time you see this, you may be looking for a different IRhard.
 * Free to steal, copy, distribute, modify and most importantly, improve and expand.
 * This is a basic infrared reciever for NEC codes only, and does not transmit anything.
 * It uses hardware interrupt for IR detector signal, such as pin 2 or 3 on Arduino Uno.
 *
 * Usage: IRhard instanceName(pin [,rcid] [,verify])
 * int: pin = hardware interrupt pin connected to IR detector. Pin 2 or 3 on Uno.
 * unsigned int: rcid = remote control ID - if specified, ignores codes with any other rcid.
 * bool: verify - if true, ignore codes unless code is key and ~key. Default is false.
 *
 *    The IR data is available in 3 instance members: code, key, and rcid. The complete
 * 32-bit NEC code includes a 16-bit remote control ID (rcid) and a 16-bit command code
 * (code), which is itself usually made up of an 8-bit key value (key) and its inverse.
 * Once a command is detected and stored in instance.code, further decoding is disabled
 * until you reset it to 0 (instance.code = 0;) to resume decoding.
 */
#ifndef IRhard_h
#define IRhard_h
#include "Arduino.h"

class IRhard {
  public:
    volatile unsigned int code, rcid;
    volatile uint8_t key;
    IRhard(uint8_t                    );
    IRhard(uint8_t, unsigned int      );
    IRhard(uint8_t,               bool);
    IRhard(uint8_t, unsigned int, bool);
    IRhard(uint8_t, bool, unsigned int);
  private:
    void IRhardInit(uint8_t, unsigned int, bool);
};
#endif
