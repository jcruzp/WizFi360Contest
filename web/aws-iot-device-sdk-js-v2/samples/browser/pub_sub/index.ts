/*****************************************************************************
* | File      	:   index.ts
* | Author      :   Jose A. Cruz P.
* | Function    :
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2022-08-22
* | Info        :   Typescript file that generate new index.js javascript 
******************************************************************************/
/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

import { mqtt, iot, auth } from "aws-iot-device-sdk-v2";
import * as AWS from "aws-sdk";

const Settings = require("./settings");
const $ = require("jquery");

const DEBUG = true;

//Pump control and status
var is_pump_on = false;
const cmd_pump_on = 'On';
const cmd_pump_off = 'Off';
const cmd_get_value = "Get";

//Tank level status
const tank_empty = "Empty";
const tank_low = "Low";
const tank_medium = "Medium";
const tank_full = "Full";

//Full tank value from ADS1115 must be set
const tank_sensor_full_value =  0.655;

//Tank and Level module topics
const topic_level_status = "tlc/level/status";
const topic_level_value = "tlc/level/value";
const topic_pump_status = "tlc/pump/status";
const topic_pump_value = "tlc/pump/value";
const mqtt_filter_value = "tlc/+/value"

function log(msg: string) {
  if (DEBUG) $("#console").append(`<pre>${msg}</pre>`);
}

//Read sensor TL231 0.655 is FULL / 0.345 is Medium / > 0.345 Low / 0 Empty
function readPercentWaterSensor(distance: number) {
  var value = Math.round(Math.max(0,distance) * 100.0 / tank_sensor_full_value);
  return Math.min(value,100);
}

//Show general status value % based
function showTankStatus(value: number) {
  log('Tank Status:' + value);

  if (value <= 0) {
    $("#TankLevel").html(" " + tank_empty + " ");
    $("#TankLevel").prop("class", 'label label-danger')
  }
  else if (value > 0 && value < 50) {
    $("#TankLevel").html(" " + tank_low + " ");
    $("#TankLevel").prop("class", 'label label-warning');
  }
  else if (value >= 50 && value < 100) {

    $("#TankLevel").html(" " + tank_medium + " ");
    $("#TankLevel").prop("class", 'label label-info');
  }
  else {
    $("#TankLevel").html(" " + tank_full + " ");
    $("#TankLevel").prop("class", 'label label-success');
  }
}


/**
* AWSCognitoCredentialOptions. The credentials options used to create AWSCongnitoCredentialProvider.
*/
interface AWSCognitoCredentialOptions {
  IdentityPoolId: string,
  Region: string
}

/**
* AWSCognitoCredentialsProvider. The AWSCognitoCredentialsProvider implements AWS.CognitoIdentityCredentials.
*
*/
export class AWSCognitoCredentialsProvider extends auth.CredentialsProvider {
  private options: AWSCognitoCredentialOptions;
  private source_provider: AWS.CognitoIdentityCredentials;
  private aws_credentials: auth.AWSCredentials;
  constructor(options: AWSCognitoCredentialOptions, expire_interval_in_ms?: number) {
    super();
    this.options = options;
    AWS.config.region = options.Region;
    this.source_provider = new AWS.CognitoIdentityCredentials({
      IdentityPoolId: options.IdentityPoolId
    });
    this.aws_credentials =
    {
      aws_region: options.Region,
      aws_access_id: this.source_provider.accessKeyId,
      aws_secret_key: this.source_provider.secretAccessKey,
      aws_sts_token: this.source_provider.sessionToken
    }

    setInterval(async () => {
      await this.refreshCredentialAsync();
    }, expire_interval_in_ms ?? 3600 * 1000);
  }

  getCredentials() {
    return this.aws_credentials;
  }

  async refreshCredentialAsync() {
    return new Promise<AWSCognitoCredentialsProvider>((resolve, reject) => {
      this.source_provider.get((err) => {
        if (err) {
          reject("Failed to get cognito credentials.")
        }
        else {
          this.aws_credentials.aws_access_id = this.source_provider.accessKeyId;
          this.aws_credentials.aws_secret_key = this.source_provider.secretAccessKey;
          this.aws_credentials.aws_sts_token = this.source_provider.sessionToken;
          this.aws_credentials.aws_region = this.options.Region;
          resolve(this);
        }
      });
    });
  }
}

// Connect to MQTT AWS IoT Core
async function connect_websocket(provider: auth.CredentialsProvider) {
  return new Promise<mqtt.MqttClientConnection>((resolve, reject) => {
    let config = iot.AwsIotMqttConnectionConfigBuilder.new_builder_for_websocket()
      .with_clean_session(true)
      .with_client_id(`pub_sub_sample(${new Date()})`)
      .with_endpoint(Settings.AWS_IOT_ENDPOINT)
      .with_credential_provider(provider)
      .with_use_websockets()
      .with_keep_alive_seconds(30)
      .build();

    log("Connecting websocket...");
    const client = new mqtt.MqttClient();

    const connection = client.new_connection(config);
    connection.on("connect", (session_present) => {
      resolve(connection);
    });
    connection.on("interrupt", (error) => {
      log(`Connection interrupted: error=${error}`);
    });
    connection.on("resume", (return_code, session_present) => {
      log(`Resumed: rc: ${return_code} existing session: ${session_present}`);
    });
    connection.on("disconnect", () => {
      log("Disconnected");
    });
    connection.on("error", (error) => {
      reject(error);
    });
    connection.connect();
  });
}

//Main Control function
async function main() {

  /** Set up the credentialsProvider */
  const provider = new AWSCognitoCredentialsProvider({
    IdentityPoolId: Settings.AWS_COGNITO_IDENTITY_POOL_ID,
    Region: Settings.AWS_REGION
  });
  /** Make sure the credential provider fetched before setup the connection */
  await provider.refreshCredentialAsync();

  connect_websocket(provider)
    .then((connection) => {
      // Subscribe to topics tlc/pump/value and tlc/level/value
      connection
        .subscribe(
          mqtt_filter_value,
          mqtt.QoS.AtLeastOnce,
          (topic, payload, dup, qos, retain) => {
            const decoder = new TextDecoder("utf8");
            let message = decoder.decode(new Uint8Array(payload));
            log(`Message received: topic=${topic} message=${message}`);
            // Set initial status to pump button and level value
            if (topic === topic_level_value) {
              let value = readPercentWaterSensor(+message)
              $("#valuebar").width(value);
              $("#SensorLevelPercent").html(`${value}%`);
              showTankStatus(value);
            } else if (topic === topic_pump_value) {
              is_pump_on = message === cmd_pump_on;
              $("#pumpbtn").prop("checked", is_pump_on ? 'checked' : '');
            }
          }
        )
        .then((subscription) => {

          // When document is loaded in browser adjust javascript componentes and properties
          $(document).ready(function () {
            //Show title and version doc
            $("#titleId").html($(document).attr("title"));
            //Pump button is disabled
            $("#pumpbtn").prop("disabled", false);
            //Wifi connected icon
            $("#wifiIconId").attr("src","images/wifi_on_icon.png");
            //Publish pump On or Off value
            $("#pumpbtn").click(async function () {
              //const msg = `Get`;
              //connection.publish("tlc/level/status", msg, subscription.qos);
              connection.publish(topic_pump_status, is_pump_on ? cmd_pump_off : cmd_pump_on, subscription.qos);
              //sendMessage(topicPumpValue, ispumpOn ? pumpOff : pumpOn);
              is_pump_on = !is_pump_on;
            });
          });

          log(`Get level and pump status`)
          connection.publish(topic_pump_status, cmd_get_value, subscription.qos);
          connection.publish(topic_level_status, cmd_get_value, subscription.qos);

          /** Keep publishing get level every 10 seconds.*/
          setInterval(() => {           
            connection.publish(topic_level_status, cmd_get_value, subscription.qos);
          }, 10000);

        });
    })
    .catch((reason) => {
      log(`Error while connecting: ${reason}`);
    });

}


main();

