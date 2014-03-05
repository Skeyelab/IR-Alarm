// Do not alter this function, it is used by the system
int getTimeAndDate() {
   int flag=0;
   sendNTPpacket(timeServer);
   delay(1000);
   if (Udp.parsePacket()){
     Udp.read(packetBuffer,NTP_PACKET_SIZE);  // read the packet into the buffer
     unsigned long highWord, lowWord, epoch;
     highWord = word(packetBuffer[40], packetBuffer[41]);
     lowWord = word(packetBuffer[42], packetBuffer[43]);  
     epoch = highWord << 16 | lowWord;
     epoch = epoch - 2208988800 + timeZoneOffset;
     flag=1;
     setTime(epoch);
     ntpLastUpdate = now();
   }
   return flag;
}
 
//// Do not alter this function, it is used by the system
//unsigned long sendNTPpacket(IPAddress& address)
//{
//  memset(packetBuffer, 0, NTP_PACKET_SIZE);
//  packetBuffer[0] = 0b11100011;
//  packetBuffer[1] = 0;
//  packetBuffer[2] = 6;
//  packetBuffer[3] = 0xEC;
//  packetBuffer[12]  = 49;
//  packetBuffer[13]  = 0x4E;
//  packetBuffer[14]  = 49;
//  packetBuffer[15]  = 52;                  
//  Udp.beginPacket(address, 123);
//  Udp.write(packetBuffer,NTP_PACKET_SIZE);
//  Udp.endPacket();
//}
 
// Clock display of the time and date (Basic)
void clockDisplay(){
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
}
 
// Utility function for clock display: prints preceding colon and leading 0
void printDigits(int digits){
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
