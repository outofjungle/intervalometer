#include <ColorLCDShield.h>

LCDShield lcd;

int PIN_ir = 12; 
byte contrast = 40;  // Good center value for contrast
int width = 16;  // width of the screen

int PIN_button[3] = {
    3, 4, 5};
int wait = 30;
int count;

int line[9][2] = {
    { 
        0, 2     }
    , { 
        13,2     }
    , { 
        26, 2     }
    ,
    { 
        39, 2     }
    , { 
        52, 2     }
    , { 
        65, 2     }
    ,
    { 
        78, 2     }
    , { 
        91, 2     }
    , { 
        104, 2     }
};

boolean set;

void setup() {
    Serial.begin(9600);
    pinMode(PIN_ir, OUTPUT); // init IR pin
    lcd.init(EPSON);  // Initialize the LCD, try using PHILLIPS if it's not working
    lcd.contrast(contrast);  // Initialize contrast
    lcd.clear(BLACK);  // Set background to white

    // Set up the button pins as inputs, set pull-up resistor
    for (int i=0; i<3; i++) {
        pinMode(PIN_button[i], INPUT);
        digitalWrite(PIN_button[i], HIGH);
    }

    set = true;  
    count = 0;
}

void loop() {

    takePicture();
    String message = "Delay: " + String( wait ) + " sec";  
    write_line( message, 0);
    message = "Photo #: " + String( ++count );
    write_line( message, 2);
    delay( wait * 1000 );

}

void set_wait() {
    while (!digitalRead(PIN_button[2]));

    while(digitalRead(PIN_button[2])) {
        if (!digitalRead(PIN_button[0])) {
            wait -= 10;
            if ( wait < 1 ) {
                wait = 1;
            }
        }

        if (!digitalRead(PIN_button[1])) {
            wait += 10;
            if ( wait > 3600 ) {
                wait = 3600;
            }
        }

        String message = "Delay: " + String( wait ) + " sec";  
        write_line( message, 0); 
    }
}
void write_line( String &str, int num ) {
    int size = width + 1;  
    char copy[size];
    str.toCharArray( copy, size );
    write_line( copy, num );
}

void write_line( char *str, int num ) {
    if ( num < 0 ) { 
        num = 0; 
    }
    if ( num > 8 ) { 
        num = 8; 
    }
    sprintf( str, "%-16s", str );
    lcd.setStr( str, line[num][0],line[num][1], WHITE, BLACK );
}

// sets the pulse of the IR signal.
void pulseON(int pulseTime) {
    unsigned long endPulse = micros() + pulseTime;        // create the microseconds to pulse for
    while( micros() < endPulse) {
        digitalWrite(PIN_ir, HIGH);                       // turn IR on
        delayMicroseconds(13);                              // half the clock cycle for 38Khz (26.32×10-6s) - e.g. the 'on' part of our wave
        digitalWrite(PIN_ir, LOW);                        // turn IR off
        delayMicroseconds(13);                              // delay for the other half of the cycle to generate wave/ oscillation
    }
}

void pulseOFF(unsigned long startDelay) {
    unsigned long endDelay = micros() + startDelay;       // create the microseconds to delay for
    while(micros() < endDelay);
}

void takePicture() {
    for (int i=0; i < 2; i++) {
        pulseON(2000);                                      // pulse for 2000 uS (Microseconds)
        pulseOFF(27850);                                    // turn pulse off for 27850 us
        pulseON(390);                                       // and so on
        pulseOFF(1580);
        pulseON(410);
        pulseOFF(3580);
        pulseON(400);
        pulseOFF(63200);
    }                                                     // loop the signal twice.
}

