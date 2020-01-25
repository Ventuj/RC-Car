# Remote 4x4 Car
Three different version 4x4 RC car with arduino.
## V1.0 (IR)
###### Components
- 1x RGB Led (Optional)
- 2x 1kohm resistors
- 2x 2 Relay
- 4x DC Motors
- 1x HC-SR04 (Ultrasonic Sensor)
- 1x 1838B (iR Receiver)
- 1x Arduino Nano or Arduino UNO
- 1x Buzzer (Optional)
- 2x Battery pack 7-12V

###### PinOut
LED Status
- D3 -> Red
- D4 -> Green

Signal Reciver
- D5 -> IR reciver

Ultrasonic Sensor
- D6 -> Echo 
- D7 -> Trig

Relay
- D8 -> SX1
- D9 -> SX2
- D10 -> DX1
- D11 -> DX2

Sound
- D12 -> Buzzer

## V2.0 (Bluetooth)
###### Components
- 1x Custom Motherboard PCB
- 4x 1kohm resistors (R1-R2-R3-R4)
- 2x 33kohm resistors (R5-R6)
- 1x L298N DC Motor driver
- 1x Buzzer (optional)
- 1x RGB Led (optional)
- 1x HC-05 (Bluetooth Module)
- 2x Tectile switch
- 1x 2P PCB connector
- 3x 40P Female Header [1x(1x8), 4x(1x6), 1x(1x5), 5x(1x4), 1x(2x4), 1x(1x3), 3x(1x2)]
- 1x 8P Male Header [1x(2x4)]
- 4x HC-SR04 (Ultrasonic Sensor)
- 4x DC Motors

###### PinOut

## V3.0 (WI-FI)
###### Components
- 1x Custom Motherboard PCB
- 4x 1kohm resistors (R1-R2-R3-R4)
- 2x 33kohm resistors (R5-R6)
- 1x L298N DC Motor driver
- 1x Buzzer (Optional)
- 1x RGB Led (Optional)
- 1x ESP8266 (WI-FI Module)
- 2x Tectile switch
- 1x 2P PCB connector
- 3x 40P Female Header [1x(1x8), 4x(1x6), 1x(1x5), 5x(1x4), 1x(2x4), 1x(1x3), 3x(1x2)]
- 1x 8P Male Header [1x(2x4)]
- 4x HC-SR04 (Ultrasonic Sensor)
- 4x DC Motors

###### PinOut

## Docs
###### Wire diagram 

###### Gerber file PCB
Top view of PCB

![Top](https://i.imgur.com/KZjgPiB.png)

Bottom view of PCB

![Bottom](https://i.imgur.com/OTyLNAK.png)
