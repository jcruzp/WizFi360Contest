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
   C language programs using Visual Studio Code

```web/aws-iot-device-sdk-js-v2```
   AWS IoT Javascript SDK to create web page

