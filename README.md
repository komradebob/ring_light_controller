# ring_light_controller
Ring Light Controller

A while back I was inspired by the 'Lights Aziz' project to build a ring light controller for a microscope. While ordering up some WS2812 LEDs to build my own variant, I came across pre built rings of LEDS on eBay. Ordered a few up, and here we are. 

This is a project to drive a microscope ring light created from a strip of WS2811/2812 LEDs using a DigiSpark Arduino clone.

While I generally prefer bare hardware, having a large suite of libraries already written to drive the LEDs along with reading a rotary encoder was atteractive. The sub $2 US price tag and having half a dozen in my personal stash didn't hurt either. My aplogies to the purists.

The controller allows you to driver any size ring. Typically commercially available sizes are 16,32,45 LEDs. Find them on ebay. Each ring connects with two connectors, each with 3 wires, 5V, Data In/Out, GND. Rings may be cascaded using the output of one to the input of the next.

The controller is pretty simple. A rotary encoder/switch allows you to choose from one of six modes: 
  1) Turn on the ring, white.
  2) Set red intensity
  3) Set green intensity
  4) set blue intensity
  5) Turn on the ring, custom color set above.
  6) Set the light configuration to one of:
    a) Whole ring
    b) Left half
    c) right half
    d) movable half - In this mode half of the ring is lit and you can move it around with the rotary encoder. This allows you to move shadows around, which can be very helpful. 

Pin PB5 is an analog intensity control. Connect to the wiper of a 10k pot set up as a voltage divider.

The transition from mode to mode is accomplished by pushing on the encoder, closing it's switch, which is connected to PB3. (the other end of the switch is grounded)

To operate, turn on the power. The Digispark takes about 15-20 sec to initialize (this can be changed, but I'm lazy). Then the whole ring will light up. The pot will vary the intensity. Push the button on the encoder to skip to the next mode. 

Mode 6, light configuration: In this mode, LEDs 0-5 will illuminate, indicating what mode you are in. Rotate to illuminate the led representing the LED config you want. The first three are yellow, the next purple, the next cyan, and anything after that is red indicating not valid.

After using this a while, I think the Left/Right options are probably redundant now that I have the rotation available. 

The RGB set modes could be avoided with 3 pots and 3 analog ports. But that means moving to a different controller. But slide pots are a pain to cut panels for. :)



The circuit is pretty simple: Pin PB0 (aka pin 1) is the output to the LED ring. Pins PB1/2 to the rotary encoder, PB3 to the encoder switch. PB5 to the wiper of a 10k linear taper pot connecting 5V to ground. 5V comes from a wall wart, passes through a switch on the pot, then feeds the pot, the LED ring, the power LED, and the Digispark. 
