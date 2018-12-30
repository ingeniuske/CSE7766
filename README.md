# CSE7766
CSE7766 ESP8266 Arduino library
https://github.com/ingeniuske/CSE7766.git
/*
CSE7766_basic

Created stand alone library for the Sonoff Pow R2 power switch with energy monotor witch uses the CSE7766 as power monitor sensor.
Based upon the excelent espurna platform of Xose Pérez "Home automation firmware for ESP8266-based devices"

https://github.com/xoseperez/espurna/blob/dev/code/espurna/sensors/CSE7766Sensor.h

ATTENTION!

The serial port ground from the Sonoff device is directly connected to AC-mains..
Be carfull it could potentially destroy your devices if connected directly!

For this reason I using a telnet as remote debug connection from Joao Lopes and OTA updates.
https://github.com/JoaoLopesF/RemoteDebug


Copyright (C) 2016-2018 by Xose Pérez <xose dot perez at gmail dot com>
Copyright (C) 2018 by Ingeniuske  <ingeniuske at gmail dot com>


This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
