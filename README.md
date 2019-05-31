# Eaonium_EWI_MIDIUSB
Eaonium is an Open Source Electronic Wind MIDI USB Instrument I wanted to provide to let more people have fun making computer assisted music.

The hardware is centered on a Arduino board and the case can be 3D printed.

The software part uses MIDI USB Lib, so the Arduino board must be atmega32u4 ARM based.

Photos, videos and blog here http://eaonsynergy.fr/category/maker/ewi-eaonium/

# Case designs
I created two designs
   1) Trompet like design : provides 10 buttons, this design was my first one with the idea to have something looking cool. I still need to improve the ergonomy but I really love the feeling playing it.
   2) Flute like design : provides 12 buttons, espacially design to be capacitive touch. I made this design to provide a case most people will be able to build with no specific tools or experience. All you will need is a hot glue gun to stick the Arduino and Adafruit boards, then slide and clip the case.

# Mecanical or Capacitive Touch
Two models are available :
   1) With mecanical buttons
   2) With Capative buttons thanks to an Adafruit MPR121 12-channel Capacitive touch sensor

# Notes Mapping
Because it is electronic, and yet better, you can customize the note to button mapping. Personnaly, I like to make it simple, so mapping provided by default is
1 button = 1 note (A,B,C,D,E,F,G)
1 button = Alt bemol (hold button playing A + button Alt plays Ab)
2 buttons two switch between octaves

However, I provided an xlsx files to generate the arduino code for custom mapping.
XLSX Left side : Select the pressed buttons
XLSX Center zone : Select the note to play for the combination
XLSX Right zone : Get the Arduino Switch code
