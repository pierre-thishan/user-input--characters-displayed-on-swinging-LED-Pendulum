# changeable-character-displayed-on-swinging-LED-Pendulum
Task-
Display characters on the LED pendulum which have been typed on the PC.
When the user types text into the console window, it is sent via UART to the microcontroller which updates the text
string on the LED pendulum “on the fly”. Another interrupt source could be a button which changes the font on the
display or the background color

Detailed steps followed:
When the user types text into the console window, it is sent via UART to the microcontroller which updates the text string on
the LED pendulum “on the fly”. Another interrupt source could be a button which changes the font on the display or the
background color.

◼ Displays characters (0-9, A-Z) on the LED pendulum, which were previously typed into a console window on the PC

◼ Shows on the LED pendulum 10 characters during a swing from left to right (optional: and vice versa)

◼ Holds the characters to be displayed in a global array of char and updates the array whenever new values have been
received via UART

◼ Transmits characters between PC and microcontroller via UART0 at a bitrate and frame format of your choice

◼ Uses a push button connected to a GPIO port to toggle between two display formats: red on black or black on red (so
the push button inverts the colors of the pattern)

◼ UART and GPIO interrupts are used, i.e. a UART frame from PC and the push button shall raise interrupts and the update of global
variables shall be done in interrupt handlers

Further explaned:
In order to realize the displaying of characters on the LED pendulum in a way that they can be changed “on-the-fly” (by what you
typed on the PC) you should define a global array of char with 10 elements.

Characters sent by UART are coded in ASCII format (see table below). You may want to support only uppercase letters which
have ASCII codes 0x41 (A) to 0x5A (Z). Of course, an implementation of all letters and numbers is nice to have.In a second global
array you can define bit patterns for each of the letters (e.g. 8x5 dots).

The printing of the characters can be realized as follows: first the next character to be displayed is read from the 10-char array.
Let’s assume it is a ‘B’ (hex value: 0x42). Use the value 0x42 to find the corresponding bitmap entry for a ‘B’ in the bitmap array.
Display the columns of the bitmap of letter ‘B’ one after another as you did in lab task 2.
Proceed with the next letters until you reach the 10th letter - and the right edge of the pendulum.


References-HAW Hamburg (https://emil.haw-hamburg.de/pluginfile.php/4911113/mod_resource/content/0/MC_LabAssignment_Session4to6.pdf)




