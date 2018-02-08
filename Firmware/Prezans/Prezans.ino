// Include all external libraries
#include <SPI.h>
#include <SD.h>
#include <MFRC522.h>
#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h>

// Define the RFID and SD Car pins
#define RFID_SS_PIN 15
#define RFID_RST_PIN 2
#define SD_SS_PIN 3

// Instance of the class of RFID reader
MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN); 
MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[3];

// Save the register at the format: "dd/mm/aaaa;hh:mm:ss;UUUUUUUU\n"
String prezansRegister;
File prezansRegisters;

void setup() {
  // Initialize the Serial and print a connected message
  Serial.begin(9600); // 9600 bps
  Serial.println("Connected!");

  // Get the time from the RTC
  setSyncProvider(RTC.get);
  if(timeStatus()!= timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");     

  // Initialize the RFID Reader
  SPI.begin(); 
  rfid.PCD_Init(); 

  // Clear the UID buffer
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  // Initialize the SD Card Reader
  if (!SD.begin(SD_SS_PIN)) {
    Serial.println("SD initialization failed!");
    return;
  }

}

void loop() {
  // Check if the time was set in the RTC
  if (timeStatus() == timeSet) {
    digitalClockDisplay();
  } else {
    Serial.println("The time has not been set.  Please run the Time");
    Serial.println("TimeRTCSet example, or DS1307RTC SetTime example.");
    Serial.println();
    delay(4000);
  }
  delay(1000);
  
  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if (!rfid.PICC_ReadCardSerial())
    return;

  // Look for a new card and store the new UID
  if (rfid.uid.uidByte[0] != nuidPICC[0] ||
    rfid.uid.uidByte[1] != nuidPICC[1] ||
    rfid.uid.uidByte[2] != nuidPICC[2] ||
    rfid.uid.uidByte[3] != nuidPICC[3] ) {

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
    
    // Clears the buffer
    prezansRegister = "";
    
    // Save the UID on the buffer
    prezansRegister += byteToHex(rfid.uid.uidByte, rfid.uid.size);
    prezansRegister += ";";

    //Get the timestamp
    //Save the date on the buffer
    prezansRegister += now();
    prezansRegister += "\n";
    
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    prezansRegisters = SD.open("Prezans.csv", FILE_WRITE);
    // if the file opened okay, write to it:
    if (prezansRegisters) {
      prezansRegisters.println(prezansRegister);
      // close the file:
      prezansRegisters.close();
      Serial.print(prezansRegister);
    }
    else {
      // if the file didn't open, print an error:
      Serial.println("Unable to save the data on SD Card");
    }
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

// Helper routine to dump a byte array as hex values to Serial.
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? "0" : "");
    Serial.print(buffer[i], HEX);
  }
}

// Function to convert the buffer byte to a Hex string
String byteToHex(byte *buffer, byte bufferSize) {
  String str = "";
  for (byte i = 0; i < bufferSize; i++) {
    str += String(buffer[i] < 0x10 ? "0" : "");
    str += String(buffer[i],HEX);
  }
  return str;
}

// Digital clock display of the time to serial
void digitalClockDisplay(){
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

// utility function for digital clock display
// prints preceding colon and leading 0
void printDigits(int digits){
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
