# Surf5_LCD_library

You can use Surf 5 and LCD with VS Code. This library is a complete C library that does not use C++ or Python. 

Please put it in the **"...\W7500x-surf5\Projects\W7500x_StdPeriph_Examples"** directory and use it.

Communication uses **8Bits-bus communication**.
The SPI version will be uploaded later.

Please refer to the following when connecting the module and Surf 5.

  **LCD** -- **Surf 5**  <br />
  -------------- <br />
  VSS  -  GND  <br />
  VDD  -  5V  <br />
  VO   -  Variable Resistance  <br />
  
  RS   -  PA0  <br />
  RW   -  PA2  <br />
  E    -  PA1  <br />

  D0   -  PC0  <br />
  D1   -  PC1 <br />
  D2   -  PC2 <br />
  D3   -  PC3 <br />
  D4   -  PC4 <br />
  D5   -  PC5 <br />
  D6   -  PC8 <br />
  D7   -  PC9 <br />

  A    -  Resistance  -  GND <br />
  K    -  GND <br />
  
