### ESW - Final Project Submission

### Table of Contents

- [Introduction](#introduction)
- [Components](#components)
- [website](#website)
    - [login page](#login-page)
    - [experiments page](#experiments-page)
    - [graphs and video page](#graphs-and-video-page)
    - [theory page](#theory-page)
- [Data Flow](#data-flow)
- [ESP32 part](#esp32-part)
  - [Bulk update](#bulk-update)
  - [MQTT communication to take input -](#mqtt-communication-to-take-input--)
- [Collaborators](#collaborators)

## Introduction

Our aim is to control position of a motor using pid controller
A pid controller is a control loop mechanism employing feedback.A pid controller 
continuously calculates an error value and applies correction on basis of proportional,
Integral amd derivative terms.


## Components
 HARDWARE:
* ESP32
* ENCODER
* DC MOTOR
* PID CONTROLLER
* MOTOR DRIVER 

SOFTWARE:
* A dashboard that operates as a remote lab, takes required inputs from the user and displays the angle vs time graph.
* ESP microcontroller code.


## website

* We have basically 4 pages in our website 
* We have used HTML, CSS for the frontend of the website, and PHP and SQLite3 for the backend.
* We have used HTML, CSS for the frontend of the website, and PHP and SQLite3 for the backend.
  
  #### login page
  * we have made a login page using php and a database in sqlite
  * it has options to login and register
  * a table members has been created which stores the login info of the members 
  * if password is incorrect the user is directed to the same page again.
  * <img src="login.png" alt="Alt text" title="Optional title">
  
  #### experiments page
  * user can choose among various experiments available
  * <img src="experiments.png" alt="Alt text" title="Optional title">

  #### graphs and video page
    * a graph has been made using chart.js
    * We are dynamically updatin the graph using AJAX
    * livestreaming is done
    * user can change the values of ki,kp,kd and target angle using sliders provided
    * a session timer is running for 15 mins the page will expire once the time ends
    * The user is given an alert when 15 seconds are left. We have used JavaScript for implementing the session timer.
    * it also has a logout button which can be used to log out of the website
  *   <img src="livestream.jpeg" alt="Alt text" title="Optional title">
  *  <img src="graph.png" alt="Alt text" title="Optional title">
  
  ####  theory page
     * Once the user chooses our experiment , he is taken to a theory page , where the aim , working and procedure of the experiment is explained in detail. He can choose to conduct the experiment , which takes him to our experiments page.
    * <img src="theory.png" alt="Alt text" title="Optional title">
  
# <u> Data Flow : </u>
* The microcontroller uploads data(current relative position of motor) on ThingSpeak using Bulk Update. 
Our website fetches data from ThingSpeak every 3 seconds and dynamically plots
graphs using last 'n' observations of the data. 
* On the other hand, if user gives input, we push the data to broker. NodeMCU subscriber reads the given
data from broker and updates constants and angle.
## ESP32 part 
### Bulk update - 
* Sending data each time an observation was recorded,was not feasible, as making a HTTP Post requests
require some 1-2 seconds during which the motor rotates extra time, without constraints.
* We used Bulk-Update to ThingSpeak, in order to update values once in `updateInterval` and send the data to
ThingSpeak servers once in `postingInterval`.
* Once in `updateInterval` we call `updatesJson(char* jsonBuffer, int pos)` and append the new position to
`jsonBuffer` which is a string in JSON format. 
```C++
  if (millis() - lastUpdateTime >=  updateInterval) {
      updatesJson(jsonBuffer,pos);
```

```C++
void updatesJson(char* jsonBuffer, int pos){
   
    strcat(jsonBuffer,"{\"delta_t\":"); // delta is do determin
  
    ...
    
    strcat(jsonBuffer,temp);
    strcat(jsonBuffer,","); 
    long rssi = pos; 
    strcat(jsonBuffer, "\"field5\":");
    lengthT = String(rssi).length();
    String(pos).toCharArray(temp,lengthT+1);
    strcat(jsonBuffer,temp);
    strcat(jsonBuffer,"},");
  ```
* When `postingInterval` expires, we call `httpRequest(char* jsonBuffer) `, connect to server and free 
the `jsonBuffer`.
``` C++
    if(millis() - lastConnectionTime >=  postingInterval && countUpdate>=10) {
          size_t len = strlen(jsonBuffer);
          jsonBuffer[len-1] = ']';
          if(sum < 10 ) 
          
          setMotor(0,0);
          delay(2000);
          httpRequest(jsonBuffer);
   
      ...
    }
``` 
* In `httpRequest` function - 
```C++
if (client.connect(server1, 80)) { 
    ... 
    client.println(data);
    client.parseFloat();
    String resp = String(client.parseInt());
    Serial.println("Response code:"+resp);  
    jsonBuffer[0] = '['; //Reinitialize the jsonBuffer for next batch of data
    memset(&jsonBuffer[1], 0, 2000 -1);
 
    lastConnectionTime = millis();
    ... 
  }
```
### MQTT communication to take input - 
* The following setup happens in `void setup()`
```C++
// Configure the MQTT client
mqttClient.setServer( server, mqttPort );
// Set the MQTT message handler function.
mqttClient.setCallback( mqttSubscriptionCallback );
// Set the buffer to handle the returned JSON. NOTE: A buffer overflow of the message buffer will result in your callback not being invoked.
mqttClient.setBufferSize( 2048 ); 
```

* `mqttSubscriptionCallback` is a function which is called after a message is received by subscription.
```C
void mqttSubscriptionCallback( char* topic, byte* payload, unsigned int length ) {
    
  ...
  
  DeserializationError err = deserializeJson(doc, payload); // Conver json string to an object 
  if(err) { 
  Serial.print(F("deserializeJson() failed: "));
  Serial.println(err.c_str());
  }
  else {
  float f1 = doc["field1"];
  float f2 = doc["field2"];
  float f3 = doc["field3"];
  int f4 = doc["field4"];
  
  // Update values
  kp = f1;ki = f2;kd = f3; target = f4;
  }
```
* In the `void loop()` we use - 
```C++
 if (!mqttClient.connected()) {
         mqttConnect();
          mqttSubscribe( channel_num );
        }
    
     mqttClient.loop();
    
 ``` 
to first connect to broker and later to subcribe. `mqttClient.loop()` maintains connectiion with broker.


## Website part

### Fetching data and plottitng graphs - 
* We use `chart.js` to plot the graph. The data is fetched from Thingspeak.
```js
var Url = 'https://api.thingspeak.com/channels/1922379/feeds.json?results=110'
var getData = function() {
        $.ajax({
                url: Url,
                success: function(data) {
                        // process your data to pull out what you plan to use to update the chart
                      
                        time_arr = [];
                        value_arr = [];
                        const_arr = [];
                        var input4 = document.getElementById('inp4').value;
                        for(let i = 0;i < data['feeds'].length; i++) {
                                time_arr.push(data['feeds'][i]['created_at'].slice(11,-1));
                                value_arr.push(data['feeds'][i]['field5']);
                                const_arr.push(input4);
                        }
                        myChart.data.labels = time_arr;
                        myChart.data.datasets[0].data = value_arr;

                        myChart.data.datasets[1].data = const_arr;
                        // re-render the chart
                        myChart.update();
                }
        });
};
```
* This function is called regularly after some interval.
```js
setInterval(getData, 3000);
```
* On button click, after selecting values using sliders, we send the data on the ThingSpeak 
server, which is later is fetched.
```js
var inp_url = 'https://api.thingspeak.com/update?api_key=WRITE_KEY'
var degree = 0
$('#btn').click(function() {

        var input1 = document.getElementById('inp1').value;
        var input2 = document.getElementById('inp2').value;
        var input3 = document.getElementById('inp3').value;
        var input4 = document.getElementById('inp4').value;
        degree = input4;
        var final_url = inp_url + '&field1=' + String(input1)+ '&field2=' + String(input2) + '&field3=' + String(input3)+ '&field4=' + String(input4);
        $.post(final_url);
});
```

## Collaborators

- [Kyrylo Shyvam Kumar]
- [Akshat Sanghvi]
- [Khushi Wadhwa]
- [Vanshika Dhingra]

