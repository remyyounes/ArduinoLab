ArduinoLab
==========

On March 31st we got to play with an LCDMatrix and an Arduino at Fishbon. 
The trickiest part for me was figuring out the wiring. 
Thankfully,  Nick Winters wrote down a sketch on paper which I now have transcribed on a spreadsheet and added to the repo.
After getting this working, the next step was to create a way to easily print any message. So Nick and I came up with FishbonLedMatrix.
I wanted to make a scrolling version so I wrote ScrolLedMatrix. I'm sure there are already a few libraries that handle scrolling but I wanted to write a quick simple one because my C++ is rusty.

The character-set is different in ScrollLedMatrix because I redrew them to my liking using this ressource:  http://mil.ufl.edu/4744/docs/lcdmanual/characterset.html


- FishbonLedMatrix  
- ScrollLedMatrix  
- wire_diagram.ods

TODO:
-add uppercase characters
-add numbers and symbols
-control scroll speed
-control intensity
