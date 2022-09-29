## Tank Level Control using WizFi360-EVB-Pico and AWS Services

### Files and folders description

```web/aws-iot-device-sdk-js-v2```
   AWS IoT Javascript SDK 

```web/aws-iot-device-sdk-js-v2/samples/browser/pub_sub``` 
   Web page to control level and pump modules

- index.html source html file
- index.ts typescript source file to generate index.js
- settings.js constants with aws iot cores and some credentials
- scripts some utils scripts
- images some images used in web page 
- dist at here compile process generate index.js file

When update files to AWS S3 bucket web hosting, we need this files:
- index.html
- dist/index.js (place this file al same place like index.html)
- imges/*
- scripts/*

