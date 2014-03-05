
/*

 Udp NTP Client

 Get the time from a Network Time Protocol (NTP) time server
 Demonstrates use of UDP sendPacket and ReceivePacket
 For more on NTP time servers and the messages needed to communicate with them,
 see http://en.wikipedia.org/wiki/Network_Time_Protocol

 created 4 Sep 2010
 by Michael Margolis
 modified 9 Apr 2012
 by Tom Igoe

 This code is in the public domain.

 */

#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Time.h>

int status = WL_IDLE_STATUS;
char ssid[] = "Groupon Mobile";  //  your network SSID (name)
char pass[] = "Sn0wf@ll";       // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int IRledPin =  2;    // LED connected to digital pin


unsigned int localPort = 2390;      // local port to listen for UDP packets

IPAddress timeServer(129, 6, 15, 28); // time.nist.gov NTP server

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

/* Set this to the offset (in seconds) to your local time
   This example is GMT - 6 */
const long timeZoneOffset = -21600L;

/* Syncs to NTP server every 15 seconds for testing,
   set to 1 hour or more to be reasonable */
unsigned int ntpSyncTime = 60;

// Keeps track of how long ago we updated the NTP server
unsigned long ntpLastUpdate = 0;
// Check last time clock displayed (Not in Production)
time_t prevDisplay = 0;

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if ( fv != "1.1.0" )
    Serial.println("Please upgrade the firmware");

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Udp.begin(localPort);

  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // Udp.begin(localPort);

  int trys = 0;
  while (!getTimeAndDate() && trys < 10) {
    trys++;
  }
  if (trys < 10) {
    Serial.println("ntp server update success");
  }
  else {
    Serial.println("ntp server update failed");
  }
}

void loop()
{

  // Update the time via NTP server as often as the time you set at the top
  if (now() - ntpLastUpdate > ntpSyncTime) {
    int trys = 0;
    while (!getTimeAndDate() && trys < 10) {
      trys++;
    }
    if (trys < 10) {
      Serial.println("ntp server update success");
    }
    else {
      Serial.println("ntp server update failed");
    }
  }

  // Display the time if it has changed by more than a second.
  if ( now() != prevDisplay) {
    prevDisplay = now();
    clockDisplay();
  }

}


