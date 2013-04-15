ArduinoLab
==========

UPDATE: 4/14/13 - The scroll Code has been moved to its own library.

About the LCD Matrix
********************

On March 31st we got to play with an LCDMatrix and an Arduino at Fishbon. 
The trickiest part for me was figuring out the wiring. 
Thankfully,  Nick Winters wrote down a sketch on paper which I now have transcribed on a spreadsheet and added to the repo.
After getting this working, the next step was to create a way to easily print any message. So Nick and I came up with FishbonLedMatrix.
I wanted to make a scrolling version so I wrote ScrolLedMatrix. I'm sure there are already a few libraries that handle scrolling but I wanted to write a quick simple one because my C++ is rusty.
 
- wire_diagram.ods

WIRING PROBLEMS: I originally had some weird issues with the wiring. After providing a dedicated path to ground for pin 4 and 9 (as opposed to sharing the same wire), everything went back to normal... Eventhough this makes absolutely no sense to me, it will hopefully help some of you getting your matrix working. 
