# 2013-2015 Honda Civic Automatic Climate Control Conversion

This device allows for a plug-and-play conversion of the manual climate control provided in the 2013-2015 Civic LX, GX, HF, and Si (US) to the automatic climate control provided in the EX and EX-L trims.

All files required to build and program the device are included in this repository.

Refer to [this](https://youtu.be/0c5TGwSvMb4) YouTube video for instructions if you have never built a black box before.

Questions? Comments? Concerns? Email me at [aidancivicminded@gmail.com](aidancivicminded@gmail.com).

## PCB Fabrication

The Gerber files required for PCB fabrication can be downloaded from Hardware/PCB. This folder also contains the EasyEDA PCB and schematic files in JSON form. These can be loaded into the EasyEDA online editor for part number references as shown in the instruction video. The parts lists can be found under ./Bill of Materials. To place an order from DigiKey, use the file "BOM_9th gen Civic Automatic Climate Control Conversion_BulkOrder.csv"

## Enclosure Fabrication

The enclosure model files are included in Hardware/Enclosure.

## Programming

This device uses an AVR Atmega328P microcontroller with Arduino bootloader. The Arduino software is free to download from [this page](https://www.arduino.cc/en/software), and the Arduino Uno can be ordered from the same website. All code for this device can be found under Software/Auto_Climate_CAN_Filter.