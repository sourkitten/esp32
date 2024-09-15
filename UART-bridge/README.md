# UART bridge
This project is revolved around transforming an ESP32's Serial UART\
into a passthrough / bridge for a UART serial interface.

It works by reading the Tx pin of the UART interface of the target device\
and printing that to the ESP32's Serial UART (onboard).
It also writes to the ESP32's UART Serial input (onboard),\
which writes that data to the Rx pin of the UART of the target device.

Basically:
Read:  [PC]<-[ESP32 onboard USB/UART]<-[external UART]
Write: [PC]->[ESP32 onboard USB/UART]->[external UART]


### Why?
---
Why not? I started this project, as I wanted a UART adapter\
but didn't have the time to wait for one to be delivered to me.
I have many ESP32 based boards and procrastinate my studies,\
thus this project was born.


### Support
---
Currently implemented for the Wemos Lolin32 Lite,\
but can easily be modified to fit any ESP32 based board\
with an onboard UART/USB interface.
