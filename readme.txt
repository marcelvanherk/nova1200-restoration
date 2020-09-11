This repository is about restoration of a 1972 Data General Nova1210 computer I proudly posses.

It came without a frontpanel, hence creation of what is the main contents sofar.

In the mean time, this repository contains several projects:

teensy_nova: for those of you who want a tiny emulated Nova1200 computer, just add a breadboard, a Teensy (3.2, 3.2, 3.5, 4.0 or 4.1) a few switches and wires and an SDI ILI9341 TFT display (note that pinouts can differ, mines are without touch and have 9 pins on one side). Also works very nicely (no wires required) on a Featherwing 2.4 inch TFT touch display with a Feather-Teensy adaptor. On the Teensy 3.5, 4.1, or Featherwing TFT  you can add an SD card and a PS2 keyboard. Runs the 1969 Nova single user basic on the TFT display!

arduino_novarev1pcb: arduino code for my handwired Nova frontpanel (defunct)

arduino_novarev2pcb: arduino/teensy code for my first PCB Nova frontpanel
nova1200rev2pcb: corresponding Kikad PCB design, updated for found errors
This frontpanel is in use on my actual Nova1210 computer
See: https://www.youtube.com/watch?v=n7GDlJNpB8Y

arduino_novarev3pcb: Teensy code for my second PCB frontpanel, also simulates a Nova!
nova1200rev3pcb: corresponding Kikad PCB design
This frontpanel is NOT in use on my actual Nova1210 computer but runs a Nova simulator
See: https://www.youtube.com/watch?v=eXXXlYzDSzM

lua_assembler: My first attempt at writing CT reconstruction in assembly languange. The required assembler is home written in Lua.
See: https://www.youtube.com/watch?v=n7GDlJNpB8Y

Note that all frontpanels and the simulator include a monitor that allows loading software. They can read the output from my Lua Nova assembler, DG punched tapes, and this Nova assembler https://github.com/Quantx/dgnsdk using the -mv format.

10 September 2020

Marcel van Herk