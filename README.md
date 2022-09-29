## Tank Level Control using WizFi360-EVB-Pico and AWS Services

![Imagen](http://maker.wiznet.io/upload/project/7446/cover.jpg)

It is a system to control the water level in a tank and activate a filling pump, with web page, using some Aws services.

In places where the filling pump and the main tank are separated, we need devices like these, which allow us to control the water level inside the tank, without having to raise and open the tank. 

Inside the tank there is a pressure sensor which captures the data and sends it to a level module and this sends the information to AWS through the secure MQTT protocol. 

In AWS, the IoT Core service is used, which allows other devices to be connected. 

Additionally, we have the pump module, which, being connected to AWS, is responsible for activating or deactivating the pump, when it receives the respective commands, allowing the filling pump to be remotely activated. 

We control both modules through a web page that can be loaded on a mobile device or on the PC. 
This page is hosted on AWS using the S3 service.
 
![Imagen](http://maker.wiznet.io/upload/ckeditor5/944403055_1663570124.png)

 
### Gettting Started

The project description could be found at 
 http://maker.wiznet.io/jcruzp/contest/tank-level-control-using-wizfi360-evb-pico-and-aws-services/
 
 
 
### Files and folders description

```c_firmware```
- c language programs using Visual Studio Code
- 
- libraries/ADS1115 interface lib for ADS1115 
- libraries/TLC_Libs al project libs include mqtt, socket, and ws2812 Led interface 
- libraries/aws-iot-device-sdk-embedded-C AWS pico sdk lib
- examples/TLC_Level Firmware for level module in C 
- examples/TLC_Pump  Firmware for pump module in C


```web/aws-iot-device-sdk-js-v2```
- AWS IoT Javascript SDK 

```setDeviceId.m5f``` 
- UIFlow program for set a device_id in AWS IoT Edukit EEprom memory

```setDeviceId.py``` 
- Micropython source code for setDeviceId.m5f

```/images folder``` 
- Have all png images files resources. 
- Need to be upload to AWS IoT Edukit using [M5Burner](https://shop.m5stack.com/pages/download) 

```/wav folder```
- Have all wav sound files resources. 
- Need to be upload too to AWS IoT EduKit

```/mp3 folder``` 
- Have source mp3 sound files created using [Amazon Polly](https://aws.amazon.com/polly/?nc1=h_ls). 
- They were converted to wav files using [Online Converter](https://www.online-convert.com/). 
- Need to be upload too to AWS IoT EduKit

```/lcad/DMHC_Host_Probe_Base.ldr``` 
- [Cad lego](http://www.melkert.net/LDCad) file, with all step instructions for base construction 

```/lcad/DMHC_Host_Probe_Base.pdf``` 
- All lego base construction instructions in pdf format

```/lambda_functions/dmhc_TimeStamp.js``` 
- Lambda function in Node.Js used to create in json message received a timestamp field server_time before put it in AWS IoT Analytics data store

```/lambda_functions/dmhc_ResetDevice.js``` 
- Lambda function in Node.Js used to reset device and send Slack and SMS notification to AWS SNS topic
