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
import { auth } from "aws-iot-device-sdk-v2";
/**
* AWSCognitoCredentialOptions. The credentials options used to create AWSCongnitoCredentialProvider.
*/
interface AWSCognitoCredentialOptions {
    IdentityPoolId: string;
    Region: string;
}
/**
* AWSCognitoCredentialsProvider. The AWSCognitoCredentialsProvider implements AWS.CognitoIdentityCredentials.
*
*/
export declare class AWSCognitoCredentialsProvider extends auth.CredentialsProvider {
    private options;
    private source_provider;
    private aws_credentials;
    constructor(options: AWSCognitoCredentialOptions, expire_interval_in_ms?: number);
    getCredentials(): auth.AWSCredentials;
    refreshCredentialAsync(): Promise<AWSCognitoCredentialsProvider>;
}
export {};
