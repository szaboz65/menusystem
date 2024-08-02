# Menu system for simple devices

## General services of the menu system
This menu system is designed for simple devices. Its control is basically achieved through 4 functions (next-prev-select-back).
The display is also separated through an interface function, where even a serial line/character LCD display/graphic TFT display implementation is possible.
The activity in the menu system can be observed via a listening object and the installation of the menu system can be done with simple constant descriptors.
The menu system ensures that the main menu can be defined with sub-menus of any depth. In the menu items, external actions can be started or other components can be accessed.
Components can be:
- list selection component
- text field component with different usable character sets (e.g. letter, letter and number, number, url, etc.)
- integer input field

See the code in the menusystem library.

## Task manager
Performing multiple tasks at the same time is usually a problem that requires a lot of attention in low-power devices. In order to support this, a taskmanager works in the background, which periodically calls the given tasks at the specified time, thus supporting non-preemptive multitasking.
This block is independent of the menu system, but is used for simple control of the menu system.
See the code in the taskman directory.

## Control of the menu system
The menu system is basically controlled using the previously mentioned 4 functions. There are many ways to implement this depending on the device, for example with 4 push buttons or a rotary switch, etc.
The classes in the btnctrl library help to serve them easily where the xxxCtrl classes are device dependent.
See the code in the menunav library.

## Defining the menu system
The elements of the menu system can be defined with constant descriptors, as can the relationships between them. Listeners can be connected to the components, in which individual reactions can be performed. In this case, they also include saving the data entry fields in EEPROM.
Communication functions between subsystems and server tasks for individual actions were also defined here.
See the menudef directory for the code.

## Displaying the menu system
The display is also separated from the base classes, and the device dependent different implementations can be created through an interface.
See the renderer directory for the code.

## Sample application
To demonstrate the operation of the menu system, a TTGO application was created in the Arduino IDE environment, which contains a rotary switch, 2 push buttons, and a graphic TFT display.
In this model, a rotary switch provides the input for the next-prev services, while 2 push buttons provide the select-bak services.
The code is in the root directory.
