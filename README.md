# teensyShell
C code to open admin meterpreter shell with a Teensy

TeensyShell is basically Teensyterpreter (https://github.com/cry0h/Teensyterpreter) for Teensy 2.0 (and variants), where Arduino only works sometimes. I configured it for Windows 8+, where you can use the Windows key + x, followed by "a" to open an Admin command prompt. If your victim is running Windows <8, you'll have to change to the method used in the original Teensyterpreter (winkey + R followed by "cmd" and enter). Make sure to change your port and IP. After you install the avr-gcc utilities (instructions here: https://www.pjrc.com/teensy/gcc.html), just type in "make" and load the resulting file ("example.hex") onto your Teensy!
