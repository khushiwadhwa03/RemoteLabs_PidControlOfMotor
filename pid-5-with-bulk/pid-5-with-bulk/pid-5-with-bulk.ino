  #include <string.h>
  #include <WiFi.h>
  #include <WebServer.h>
  #include <ThingSpeak.h>
  #include <WiFiClient.h>
  #include <PubSubClient.h>
  #include <ArduinoJson.h>  
  #include<SPI.h> // Comment this to work with ESP8266 board
  // #include<WiFi101.h> // Comment this to work with ESP8266 board
  const char* ssid = "POCO M3";
  const char* pass = "correcthorsebatterystaple";
  // const char* ssid = "Redmi Note 10S";
  // const char* pass = "sam656199";

  const char mqttUserName[] = "PTAHIBkoDwgEAyYWOQ0yCDY";
  //const char clientID[] = "SPl4s/9Y0yM4MAL9C4ZOAgtC";
  const char mqttPass[] = "SPl4s/9Y0yM4MAL9C4ZOAgtC";


  char jsonBuffer[2000] = "["; // Initialize the jsonBuffer to hold data
  char server1[] = "api.thingspeak.com";
  const char* server = "mqtt3.thingspeak.com";// ThingSpeak Server // Is it even needed?
  unsigned long lastConnectionTime = 0; // Track the last connection time
  unsigned long lastUpdateTime = 0; // Track the last update time
  const unsigned long postingInterval = 15L * 1000L; // Post data every 5 seconds
  const unsigned long updateInterval = 0.2L * 1000L; // Update once every 0.2 seconds
  const long noPost = 10L * 1000L;
  const long Upd = 10;
  int countUpdate = 0;
  unsigned long channel_num = 1922379;

  #define mqttPort 1883
  WiFiClient client;
  int status = WL_IDLE_STATUS; 
  long lastPublishMillis = 0;
  int connectionDelay = 1;

  PubSubClient mqttClient( client );

  #define ENCA 12  // Yellow
  #define ENCB 14  // White

  #define MOTOR1 32
  #define MOTOR2 33

  #define PWM 18
  volatile int posi = 0;
  // analogWrite
  // 
  const int freq = 5000;
  const int ledChannel = 0;
  const int resolution = 8;
  // 
  // 
  long prevT = 0;
  float eprev = 0;
  float eintegral = 0;
  long sum = 0;
  //int target = 150;float kp = 4.6,ki = 1,kd =1;
  void setup() {

    Serial.begin(115200);

    pinMode(ENCA, INPUT);
    pinMode(ENCB, INPUT);

    pinMode(MOTOR1, OUTPUT);
    pinMode(MOTOR2, OUTPUT);
    pinMode(PWM, OUTPUT);
    int count = 0;
    attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);

    // configure LED PWM functionalitites
    ledcSetup(ledChannel, freq, resolution);
    
    // attach the channel to the GPIO to be controlled
    ledcAttachPin(PWM, ledChannel);
    Serial.println("target pos");

    WiFi.begin(ssid,pass);   
    while(WiFi.status()!=WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println(WiFi.localIP());   
  // Configure the MQTT client
    mqttClient.setServer( server, mqttPort ); 
    // Set the MQTT message handler function.
    mqttClient.setCallback( mqttSubscriptionCallback );
    // Set the buffer to handle the returned JSON. NOTE: A buffer overflow of the message buffer will result in your callback not being invoked.
    mqttClient.setBufferSize( 2048 );  
    if (!mqttClient.connected()) {
          mqttConnect();
          mqttSubscribe( channel_num );
        }
        Serial.println("before");
    mqttClient.loop();
  
  }

  // Updates the josnBuffer with data
  void updatesJson(char* jsonBuffer, int pos){
    /* JSON format for updates paramter in the API
    *  This examples uses the relative timestamp as it uses the "delta_t". You can also provide the absolute timestamp using the "created_at" parameter
    *  instead of "delta_t".
    *   "[{\"delta_t\":0,\"field1\":-70},{\"delta_t\":3,\"field1\":-66}]"
    */
    // Format the jsonBuffer as noted above
    sum += pos;
    countUpdate ++;
    strcat(jsonBuffer,"{\"delta_t\":");
    unsigned long deltaT = (1);
    size_t lengthT = String(deltaT).length();
    char temp[10];
    String(deltaT).toCharArray(temp,lengthT+1); // Current time converted
    strcat(jsonBuffer,temp);
    strcat(jsonBuffer,","); 
    long rssi = pos; 
    strcat(jsonBuffer, "\"field5\":");
    lengthT = String(rssi).length();
    String(pos).toCharArray(temp,lengthT+1);
    strcat(jsonBuffer,temp);
    strcat(jsonBuffer,"},");
    // If posting interval time has reached //2 minutes, update the ThingSpeak channel with your data
    if (// countUpdate >= Upd ){//
        millis() - lastConnectionTime >=  postingInterval && countUpdate>=10) {
          
          size_t len = strlen(jsonBuffer);
          jsonBuffer[len-1] = ']';
          if(sum < 10 ) 
          {
            sum = 0;
            jsonBuffer[0] = '['; //Reinitialize the jsonBuffer for next batch of data
            jsonBuffer[1] = '\0';
          //  
          }
        else 
        {
          setMotor(0,0);
          delay(2000);
            httpRequest(jsonBuffer);
        
            

        }
          sum = 0;
          countUpdate = 0;
          
    
    // Call the loop to maintain connection to the server.
          
    }
    lastUpdateTime = millis(); // Update the last update time
  }

  // Updates the ThingSpeakchannel with data
  void httpRequest(char* jsonBuffer) {
    /* JSON format for data buffer in the API
    *  This examples uses the relative timestamp as it uses the "delta_t". You can also provide the absolute timestamp using the "created_at" parameter
    *  instead of "delta_t".
    *   "{\"write_api_key\":\"YOUR-CHANNEL-WRITEAPIKEY\",\"updates\":[{\"delta_t\":0,\"field1\":-60},{\"delta_t\":15,\"field1\":200},{\"delta_t\":15,\"field1\":-66}]
    */
    // Format the data buffer as noted above
    char data[2000] = "{\"write_api_key\":\"TK0QTUF1PBS6TX25\",\"updates\":"; // Replace YOUR-CHANNEL-WRITEAPIKEY with your ThingSpeak channel write API key
    strcat(data,jsonBuffer);
    strcat(data,"}");
    // Close any connection before sending a new request
    client.stop();
    String data_length = String(strlen(data)+1); //Compute the data buffer length
    Serial.println(data);
    // POST data to ThingSpeak
    if (client.connect(server1, 80)) {
      client.println("POST /channels/1922379/bulk_update.json HTTP/1.1"); // Replace YOUR-CHANNEL-ID with your ThingSpeak channel ID
      client.println("Host: api.thingspeak.com");
      client.println("User-Agent: mw.doc.bulk-update (Arduino ESP8266)");
      client.println("Connection: close");
      client.println("Content-Type: application/json");
      client.println("Content-Length: "+data_length);
      client.println();
      client.println(data);
    }
    else {
      Serial.println("Failure: Failed to connect to ThingSpeak");
    }
    delay(250); //Wait to receive the response
    client.parseFloat();
    String resp = String(client.parseInt());
    Serial.println("Response code:"+resp); // Print the response code. 202 indicates that the server has accepted the response
    jsonBuffer[0] = '['; //Reinitialize the jsonBuffer for next batch of data
    memset(&jsonBuffer[1], 0, 2000 -1);
  // jsonBuffer[1] = '\0';
    lastConnectionTime = millis(); //Update the last conenction time
  }
  float kp = 8;float ki = 0.06;float kd = 3;int target = 300;
  void loop() {

    // Connect if MQTT client is not connected and resubscribe to channel updates.
    
    
       //  Serial.println("before");
  

    // PID constants
    Serial.println(kp);
    Serial.println(ki);
    Serial.println(kd);

    // time difference

    long currT = micros();

    float deltaT = ((float) (currT - prevT))/( 1.0e6 );

    prevT = currT;

    int pos = posi;
    int e = pos - target;
    // derivative
    float dedt = (e-eprev)/(deltaT);
    // integral
    eintegral = eintegral + e*deltaT;
    // control signal
    float u = kp*e + kd*dedt + ki*eintegral;
    // motor power
    Serial.print(e);Serial.print(" "); Serial.print(dedt);Serial.print(" ");Serial.print(eintegral); Serial.println();
    float pwr = fabs(u);

    if( pwr > 255 ){
      pwr = 255;
    }
    // motor direction
    int dir = 1;
    if(u<0){  
      dir = -1;
    }
    // signal the motor
    setMotor(-dir,pwr);
    // store previous error
    Serial.print(pwr);
    Serial.print(" ");
    eprev = e;
    Serial.print(target);

    Serial.print(" ");

    Serial.print(pos);

    Serial.println();  

    // If update time has reached 1 second, then update the jsonBuffer
    // if(millis() - lastUpdateTime <= noPost )
    // {
    //     delay(100);
    //     continue;
    // }
    if (millis() - lastUpdateTime >=  updateInterval) {
      // Serial.println("Update.");
      updatesJson(jsonBuffer,pos);
     if (!mqttClient.connected()) {
         mqttConnect();
          mqttSubscribe( channel_num );
        }
            Serial.println("Loop.");      
            mqttClient.loop();
      
    }

    
      //count ++ ;
    delay(100);
  }
  void readEncoder() {

    int b = digitalRead(ENCB);

    if (b > 0) {

      posi++;

    }

    else {

      posi--;
    }
  // Serial.println(posi);
  }
  void setMotor(int dir, int pwmVal) {

    ledcWrite(ledChannel, pwmVal);
    if (dir == 1) {

      digitalWrite(MOTOR1, HIGH);

      digitalWrite(MOTOR2, LOW);
      Serial.println(1);    
      
    }

    else if (dir == -1) {

      digitalWrite(MOTOR1, LOW);

      digitalWrite(MOTOR2, HIGH);
    Serial.println(-1);
    }

    else {

      digitalWrite(MOTOR1, LOW);

      digitalWrite(MOTOR2, LOW);
    }
  }
  DynamicJsonDocument doc(2048);


  // Function to handle messages from MQTT subscription.
  void mqttSubscriptionCallback( char* topic, byte* payload, unsigned int length ) {
    // Print the details of the message that was received to the serial monitor.
    Serial.print("Message arrived [");
    // Serial.print(topic);
    // Serial.print("] ");
    // for (int i = 0; i < length; i++) {
    //   Serial.print((char)payload[i]);
    // }
    // Serial.println();
    // Error checking should be added
    DeserializationError err = deserializeJson(doc, payload);
    if(err) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(err.c_str());
  }
  else {
    float f1 = doc["field1"];
    float f2 = doc["field2"];
    float f3 = doc["field3"];
    int f4 = doc["field4"];
    
    // Stop motor
    //setMotor(0,0); 
    // Update values
    kp = f1;ki = f2;kd = f3; target = f4;
  Serial.println(f1);Serial.println(f2);Serial.println(f3);Serial.println(f4);  // Continue
    }
  }

  void mqttSubscribe( long subChannelID ){
    String myTopic = "channels/"+String( subChannelID )+"/subscribe";
    mqttClient.subscribe(myTopic.c_str());
  }

  // Publish messages to a ThingSpeak channel.
  void mqttPublish(long pubChannelID, String message) {
    String topicString ="channels/" + String( pubChannelID ) + "/publish";
    mqttClient.publish( topicString.c_str(), message.c_str() );
  }


  // Connect to MQTT server.
  void mqttConnect() {
    // Loop until connected.
    while ( !mqttClient.connected() )
    { 
        //setMotor(0,0);    
      // Connect to the MQTT broker.
      if ( mqttClient.connect( mqttUserName/*clientID*/, mqttUserName, mqttPass ) ) {
        Serial.print( "MQTT to " );
        Serial.print( server );
        Serial.print (" at port ");
        Serial.print( mqttPort );
        Serial.println( " successful." );
      } else {
        Serial.print( "MQTT connection failed, rc = " );
        // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
        Serial.print( mqttClient.state() );
        Serial.println( " Will try again in a few seconds" );
        delay( connectionDelay*1000 );
      }
    }
  }
