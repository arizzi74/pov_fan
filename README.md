# pov_fan
Reprogram a Series 45 POV Fan

http://zjembedded.blogspot.com/2015/07/led-pov-fan-update.html

Tested with: Arduino 1.6.5 and Python 2.7.5/3.4.3

*************************
# 2025 udpate
In fodler 2025_update you find an updated version with new character map adapted to a more recent version of these PoV Fans. 

Python script updated to Python3 and sketch to Arduino 2.3.6

# Instructions:
- You can optionally dump the exisitng eeprom using the provided Arduino sketch

To program:
- Update demo_pov.py with text you want to display
- run python3 demo_pov.py
- copy output code at the beginning of the arduino sketch
- tested only with buffers < 256 bytes
