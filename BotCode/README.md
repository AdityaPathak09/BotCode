Created by Aditya Pathak, last update: 10th oct 2023

Controlling instructions:
=
    	forward: 1
    	backward: 2
    	leftturn: 3
    	rightturn: 4
    	leftspin: 5
    	rightspin: 6
    	stop: 7
    	camservo: 8
    	gpsdata: 9
    	pump1: 10
    	pump2: 11
    	backcentre: 12
    	backedge: 13
    	clickImage: 14

Pin configuration:
=
    	motorPinlf 13 
    	motorPinlb 14
    	motorPinrf 27
    	motorPinrb 26
    	motorPinbE 25
    	motorPinbC 33
    	camTrig 19
    	pump1 5
    	pump2 18
    	servoPincam 15

### UART:
		gpsRX 34
		gpsTX 4
		
		gsmRX 16
		gsmTX 17

		camersTX 14
		cameraRX 12 //not used, just defined. prefered reading esp32 gpio12 documentations before using

###	I2C:
		1. compass:
		SCL 22
		SDA 21

Here is a simple flow chart:

```mermaid
graph TD;
    A-->B;
    A-->C;
    B-->D;
    C-->D;
```

