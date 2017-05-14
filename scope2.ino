

#include <Adafruit_NeoPixel.h>
#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>

#define LEDPIN PB1
#define PIN PB0
#define BUTTON PB3
#define ENCODER1  PB1
#define ENCODER2  PB2


#define LEDCOUNT  45

#define BUTTON_DELAY  500

#define WHITE 0
#define RED 1
#define GREEN 2
#define BLUE 3
#define BALANCE 4
#define SPLIT 5
#define BLANK 6

#define WHOLE 0
#define LEFT_SIDE 1
#define RIGHT_SIDE 2
#define EGG 3
#define TUNE 4
#define SELECT 5

#define LIMIT  200

Encoder myEnc(ENCODER1,ENCODER2);

long position  = -999;

int sensorPin = A0;    // select the input pin for the potentiometer

char mode;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDCOUNT, PIN, NEO_GRB + NEO_KHZ800);

void colorWipe(uint32_t c, uint8_t wait);
void theaterChase(uint32_t c, uint8_t wait);
void theaterChaseRainbow(uint8_t wait);


void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(LEDPIN, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}

uint8_t red,green,blue,dred,dgreen,dblue;

void loop() {
  // put your main code here, to run repeatedly:

bool split,trip;
int i,button;
int split_type = 0;
int pos,oldpos,newpos;

oldpos=1;
newpos =2;
pos=1;
split = false;
  mode = WHITE;
  trip = false;



// Let me know we're starting

  strip.setPixelColor(0,255,0,0);
  strip.show();
  delay(500);
  strip.setPixelColor(0,0,0,0);
  strip.show();
  delay(500);
  strip.setPixelColor(LEDCOUNT,0,0,255);
  strip.show();
  delay(500);
  

// while 1
// if button down mode++
// switch on mode
//  case white
//     intensity = pot value
//     color 255,255,255
//  case red/green/blue
//      red/green/blue = pot value
//  case balance
//      color red,green,blue
//  case split
//     split = pos mod3
//  end switch mode
//  switch split
//      case 1
//        colorwipe(rgb,0)
//      case 2
//          for i=0 to LEDCOUNT/2
//            strip.setPixelColor(i,red,green,blue);
//      case 3
//          for i = LEDCOUNT/2 to LEDCOUNT
//            strip.setPixelColor(i,red,green,blue);
//  end switch split

        
//    
  while (1) {

    // Check the button
    
    if((digitalRead(BUTTON) == 0) & (trip == false)) {   // is the button pushed for the first time
       button++;                                         // increment counter
       if(button>LIMIT) {                               // If it's been pressed long enough
          trip = true;                                  // set trip flag, increment mode
          mode++;
          button = 0;
       }
    } else {                                            // else reset the counter
      button = 0;
      if(trip == true) {                                // if the flag is set, clear it.
        trip = false;
      }
    }  // end else if 

  switch (mode) {
    case WHITE:               // White illumination
        dred = dgreen = dblue = 255;
        break;    
    case RED:                 // Custom color balance - Red
        red = (uint8_t)(analogRead(sensorPin)/4);
        dred = red;
        dgreen = 0;
        dblue = 0;
        break;
    case GREEN:               // Custom color balance - Green
        green = (uint8_t)(analogRead(sensorPin)/4);
        dred = 0;
        dgreen = green;
        dblue = 0;
        break;
    case BLUE:                // Custom color balance - Blue
        blue = (uint8_t)(analogRead(sensorPin)/4);
        dred = 0;
        dgreen = 0;
        dblue = blue;
        break;
    case BALANCE:             // Turn on custom color balance
        dred = red;
        dgreen = green;
        dblue = blue;
        break;     
    case SPLIT:                 // Select whole ring, left, right half
        split_type = SELECT;
        break;  
    case BLANK:                 // Turn it off
        split_type = oldpos;
        dred = dgreen = dblue = 0;
        split = false;
        break;
        
    default:
        mode = 0;
        break;
  }
     switch (split_type) {
          case WHOLE:                       // Full Circle
            for(i=0;i<=LEDCOUNT;i++) {
              strip.setPixelColor(i,dred,dgreen,dblue);
            }
            break;  
          case LEFT_SIDE:                       // Left half
            for(i=0;i<=LEDCOUNT/2-1;i++) {
              strip.setPixelColor(i,dred,dgreen,dblue);
              strip.setPixelColor(i+LEDCOUNT/2,0,0,0);
            }
            break;
          case RIGHT_SIDE:                       // Right Half
             for(i=0;i<LEDCOUNT/2;i++) {
              strip.setPixelColor(i,0,0,0);
              strip.setPixelColor(i+LEDCOUNT/2,dred,dgreen,dblue);
            }
            break;
          case EGG:                     // Easter Egg
            theaterChase(strip.Color(127, 127, 127),20);
            split_type = 0;
            mode = WHITE;
            break;
          case TUNE:
//            for(i=(LEDCOUNT-(LEDCOUNT/4));i<(LEDCOUNT+LEDCOUNT/4);i++) {
            position = (int)myEnc.read()/4;
            if (position < 0) {
              position = 0;
              myEnc.write(0);                   // set position to min
            }

            for(i=position;i<position + LEDCOUNT/2;i++) {
              strip.setPixelColor((i + position) % LEDCOUNT,0,0,0);
              strip.setPixelColor((i+position+LEDCOUNT/2)%LEDCOUNT,dred,dgreen,dblue);
            }
            break;
          case SELECT:                     //  Set the display type
           //colorWipe(strip.Color(0,0,0),0);
           //myEnc.write(0);                   // set position to min
            if (!split) {
              colorWipe(strip.Color(0,0,0),0);
              myEnc.write(0);                   // set position to min
              split = true;
            }
          
            newpos = (int)myEnc.read()/4;
            if (newpos < 0) {                   // if we've gone off the bottom end
              newpos = 0;                       // set to 0
              myEnc.write(0);                   // set position to min
            }
            if(newpos > LEDCOUNT) {             // if we've gone off the top end
              newpos = LEDCOUNT-1; 
              myEnc.write(LEDCOUNT-1);          // set position = max
            }  
            if (newpos != oldpos) {      // if the position has changed 
             strip.setPixelColor(oldpos,0,0,0);           // erase the old pixel
             switch (newpos) {
              case 0:
              case 1:
              case 2:
                strip.setPixelColor(newpos,255,255,0);      // else color yellow
                break;
              case 3:
                strip.setPixelColor(newpos,255,0,255);      // if >= EGG color purple
                break;
              case 4:
                strip.setPixelColor(newpos,0,255,255);      // else color cyan
                break;
              default:
                strip.setPixelColor(newpos,255,0,0);      // else color yellow
                break;
             }
             oldpos = newpos;                             // Save where we are
            }
            break;
          default: 
            mode = WHITE;
//            strip.setPixelColor(0,dred,dgreen,dblue);
            break;     
    }    
    strip.setBrightness((uint8_t)(analogRead(sensorPin)/4));
    strip.show();
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
 //     delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
      delay(wait);    
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}
