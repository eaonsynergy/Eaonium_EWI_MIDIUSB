# Eaonium_EWI_MIDIUSB
Eaonium is an Open Source Electronic Wind MIDI USB Instrument I wanted to provide to let more people have fun making computer assisted music.

The hardware is centered on a Arduino board and the case can be 3D printed.

The software part uses MIDI USB Lib, so the Arduino board must be atmega32u4 ARM based.

Photos, videos and blog here http://eaonsynergy.fr/category/maker/ewi-eaonium/

The repo is not yet perfect, so I think you will need some Arduino, 3D printing and handcraft skills to go through it. The documentation is still really minimalist, and I have to draw the wiring plans.

I swear to do my best to improve the documentation, providing some wiring plans, components list, etc.

# Case designs
I created two designs
   1) Trompet like design : provides 10 buttons, this design was my first one with the idea to have something looking cool. I still need to improve the ergonomy but I really love the feeling playing it.
   2) Flute like design : provides 12 buttons, espacially design to be capacitive touch. I made this design to provide a case most people will be able to build with no specific tools or experience. All you will need is a hot glue gun to stick the Arduino and Adafruit boards, then slide and clip the case.

# Mecanical or Capacitive Touch
Two models are available :
   1) With mecanical buttons
   2) With Capative buttons thanks to an Adafruit MPR121 12-channel Capacitive touch sensor

# Notes Mapping
Because it is electronic, and yet better, you can customize the note to button mapping.
Find here a online map code generator to easily create your own key mapping : http://eaonsynergy.fr/eaonium-map-generator/
