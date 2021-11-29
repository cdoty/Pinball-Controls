# Pinball-Controls
Software to allow the Duo pinball controller to work for Pinball FX2 and Pinball Arcade.

The implementation is inspired by Alex Atkin's post on the Digital Pinball Fans [forums](http://www.digitalpinballfans.com/showthread.php/6953-Duo-Pinball-controller-CAN-be-made-to-work-on-a-Windows-tablet).

This will not work for Pinball FX3 or later. I'm assuming they switched to DirectInput for keyboard reading.

The software allows switching from RETURN, for the plunger button, to SPACE.

There seems to be intermittent dropping of controls if the device sits idle for 10-15 seconds or so. Maybe it's something to do with how I'm handling the initialization message. It sends a message to reset all of the controls.