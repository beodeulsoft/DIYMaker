#include <ESP8266WiFi.h>
#include "plotly_streaming_wifi_ESP8266.h"

// Sign up to plotly here: https://plot.ly
// View your API key and streamtokens here: https://plot.ly/settings
#define nTraces 2
// View your tokens here: https://plot.ly/settings
// Supply as many tokens as data traces
// e.g. if you want to ploty A0 and A1 vs time, supply two tokens
char *tokens[nTraces] = {"token_1", "token_2"};
// arguments: username, api key, streaming token, filename
plotly graph = plotly("plotly_username", "plotly_api_key", tokens, "your_filename", nTraces);

char ssid[] = "wifi_network_name"; //  your network SSID (name) 
char pass[] = "wifi_network_password"; // // your network password

void wifi_connect(){
    // attempt to connect using WPA2 encryption:
    Serial.println("... Attempting to connect to WPA network...");
    WiFi.begin(ssid, pass);
    // if you're not connected, stop here:
    while ( WiFi.status() != WL_CONNECTED) { 
      delay(500);
      Serial.print(".");
    } 
    // if you are connected, print out info about the connection:
    Serial.println("... Connected to network");
	Serial.print("  IP address: ");
	Serial.println(WiFi.localIP());
}

void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.println("Putting WiFi into station mode...");
  WiFi.mode(WIFI_STA);
  
  wifi_connect();

  graph.fileopt="overwrite"; // See the "Usage" section in https://github.com/plotly/arduino-api for details
  bool success;
  success = graph.init();
  if(!success){while(true){}}
  graph.openStream();
}

unsigned long x;
int y;

void loop() {
  graph.plot(millis(), analogRead(A0), tokens[0]);
  graph.plot(millis(), analogRead(A1), tokens[1]);

  delay(200);

}