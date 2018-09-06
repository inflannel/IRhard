/* IRextInt.h header version 1.0.0 by u/inflannel, 5 September 2018. <-- Check the date.
 *       By the time you see this, you may be looking for a different IRextInt.h.
 *
 *    This is a replacement for u/inflannel's IRhard library, simplifying the whole mess.
 * This header can be in a library or simply copied to the same folder as the sketch that
 * includes it. You can even copy and paste the contents into your sketch, to "do it solo"
 * with no headers or libraries. No copyright overhead to contend with or anything. This
 * is "fell off a truck" code with no requirement to mention where it was found or who was
 * involved. It's presented "as is" without "where from".
 *    Free to steal, copy, distribute, modify and most importantly, improve and expand.
 * This is a basic infrared reciever for NEC codes only, and does not transmit anything.
 * It uses external interrupt for IR detector signal, such as pin 2 or 3 on Arduino Uno.
 *
 * Usage:
 *   #include IRextInt.h
 *   IR_init(pin [,rcid] [,validate]); // hooks the external interrupt to the specified pin
 *   // int: pin = hardware interrupt pin connected to IR detector. Pin 2 or 3 on Uno.
 *   // unsigned int: rcid = remote control ID - if specified, ignores codes with any other rcid.
 *   //   Alternately: can set IR_RCID to a specific remote control ID at any time.
 *   //   Although: the preferred method is to handle any IR_rcid filtering yourself.
 *   // bool: validate - if true, ignore codes unless code is key and ~key. Default is false.
 *
 *    IR data is available in 3 global variables: IR_code, IR_key, and IR_rcid. The complete
 * 32-bit NEC code includes a 16-bit remote control ID (IR_rcid) and a 16-bit command code
 * (IR_code), which is itself usually made up of an 8-bit key value (IR_key) and its inverse.
 * Once a command is detected and stored in IR_code, further decoding is disabled until you
 * reset it (IR_code = 0;) to resume decoding. Check IR_code for non-zero (or greater than 0
 * since it's unsigned) to indicate a new code.
 */
#ifndef IRextInt_h
#define IRextInt_h
volatile unsigned int IR_code = 7, IR_rcid = 7;
volatile uint8_t IR_key = 77;
unsigned int IR_RCID = 0;
bool IR_No_Validate = true, IRextInt_loaded = false;

void _IR_interrupt() {
  if (IR_code == 0) { // don't do anything unless IR_code == 0 (don't overwrite code)
    static unsigned long oldTime = 0, acc = 0, abit = 1;
    unsigned long nowTime = micros();
    unsigned long wl = nowTime - oldTime; // wl = wavelength, from last pulse-start to this pulse-start
    if (wl > 844) { // got short wavelenghts at times, possibly interrupted pulses, and this hack made it work
      if (wl > 2500) { acc = 0; abit = 1; } // 110% of bit-1 wavelength is "too long", causing reset
      else {
        if (wl > 1688) { acc |= abit; } // test midway between 0-bit (1125us) and 1-bit (2250us)
        if (abit < 0x80000000) { abit <<= 1; } // if not 32 bits, adjust to keep reading
        else { // accumulated 32 bits, so handle it as a complete code
          if ((IR_RCID == 0) || ((acc & 0xFFFF) == IR_RCID)) { // correct remote.
            // If validating, make sure code byte matches the inverse byte (ie 0xED12: 0xED + 0x12 == 0xFF)
            if (IR_No_Validate || (((acc & 0xFF000000) >> 8) + (acc & 0x00FF0000) == 0x00FF0000)) {
              IR_code = (acc >> 16) & 0xFFFF; // store command code in IR_code
              IR_rcid = acc & 0xFFFF; // store remote control ID in IR_rcid
              IR_key = (acc >> 16) & 0xFF; // store key identifier in IR_key
            }
          }
          acc = 0; abit = 1; // reset accumulator and bit pointer for next time
        }
      }
      oldTime = nowTime; // update time for calculating wavelength next time
    }
  }
}
void IR_init(uint8_t pin, unsigned int id = 0, bool b = false) {
  int IR_int_num = digitalPinToInterrupt(pin);
  if (IRextInt_loaded || (IR_int_num < 0)) { // can't attach interrupt to specified pin
    IR_code = 7;  // code set to an initial value of 7 if the wrong pin number is passed
    IR_key = 77;  // key set to an initial value of 77 if the wrong pin number is passed
    IR_rcid = 7;  // RC ID set to an initial value of 7 if the wrong pin number is passed
  }
  else { // An external interrupt pin is passed, as is the normal (working) case.
    IRextInt_loaded = true;
    pinMode(pin, INPUT_PULLUP);
    IR_RCID = id;  // user specified remote control ID, or 0 for "any remote"
    IR_No_Validate = !b; // true (skips validation) if user passes false (do not validate)
    IR_code = 0; // the NEC code as 16 bits gets stored here
    IR_key = 0;  // the button-specific 8 bits of the NEC code go here
    IR_rcid = 0; // basicly, the remote control's ID number
    attachInterrupt(IR_int_num, _IR_interrupt, FALLING); // enable the external interrupt
  } // The IR detector output (our pin input) goes LOW when IR is detected, hence FALLING
}
#endif
