#include <Wire.h>

// 24C08N EEPROM specifications
#define EEPROM_SIZE 1024        // 1024 bytes total
#define PAGE_SIZE 16            // 16 bytes per page
#define TOTAL_PAGES 64          // 64 pages total
#define BASE_ADDRESS 0x50       // Base I2C address for 24C08N



void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.println("// 24C08N EEPROM Dump - Ready for programming");
  Serial.println("// Copy this data into your write sketch");
  Serial.println("// Format: byte eepromData[] = { ... };");
  Serial.println();

  delay(1000);

  dumpEEPROMForProgramming();

}

void loop() {
  // Nothing to do in loop
}

void dumpEEPROMForProgramming() {
  Serial.println("byte eepromData[1024] = {");

  for (int address = 0; address < EEPROM_SIZE; address++) {
    // Add proper indentation
    if (address % 16 == 0) {
      if (address > 0) {
        Serial.println(); // End previous line
      }
      Serial.print("  "); // Indent
    }

    byte data = readByte(address);

    // Format as 0x## with proper spacing
    Serial.print("0x");
    if (data < 16) Serial.print("0");
    Serial.print(data, HEX);

    // Add comma except for last byte
    if (address < EEPROM_SIZE - 1) {
      Serial.print(",");

      // Add space between bytes on same line
      if ((address + 1) % 16 != 0) {
        Serial.print(" ");
      }
    }

    delay(10); // Small delay between reads
  }

  Serial.println();
  Serial.println("};");
  Serial.println();
  Serial.println("// End of EEPROM dump");
  Serial.println("// Total bytes: 1024");
}

// Alternative format - Intel HEX format
void dumpIntelHex() {
  Serial.println("// Intel HEX format dump");

  for (int address = 0; address < EEPROM_SIZE; address += 16) {
    int bytesToRead = min(16, EEPROM_SIZE - address);

    // Start code
    Serial.print(":");

    // Byte count (2 hex digits)
    if (bytesToRead < 16) Serial.print("0");
    Serial.print(bytesToRead, HEX);

    // Address (4 hex digits)
    if (address < 0x1000) Serial.print("0");
    if (address < 0x100) Serial.print("0");
    if (address < 0x10) Serial.print("0");
    Serial.print(address, HEX);

    // Record type (00 = data)
    Serial.print("00");

    // Calculate checksum
    byte checksum = bytesToRead + (address >> 8) + (address & 0xFF) + 0x00;

    // Data bytes
    for (int i = 0; i < bytesToRead; i++) {
      byte data = readByte(address + i);
      if (data < 16) Serial.print("0");
      Serial.print(data, HEX);
      checksum += data;
    }

    // Checksum (two's complement)
    checksum = (~checksum + 1) & 0xFF;
    if (checksum < 16) Serial.print("0");
    Serial.print(checksum, HEX);

    Serial.println();
  }

  // End of file record
  Serial.println(":00000001FF");
}

// Raw binary format for copy/paste programming
void dumpRawArrayFormat() {
  Serial.println("// Raw array format - 16 bytes per line");
  Serial.println("const byte eepromData[1024] PROGMEM = {");

  for (int page = 0; page < TOTAL_PAGES; page++) {
    Serial.print("  "); // Indent

    for (int i = 0; i < PAGE_SIZE; i++) {
      int address = page * PAGE_SIZE + i;
      byte data = readByte(address);

      Serial.print("0x");
      if (data < 16) Serial.print("0");
      Serial.print(data, HEX);

      if (address < EEPROM_SIZE - 1) {
        Serial.print(",");
      }

      if (i < PAGE_SIZE - 1) {
        Serial.print(" ");
      }
    }

    Serial.print("  // Page ");
    Serial.print(page);
    Serial.print(" (0x");
    if (page < 16) Serial.print("0");
    Serial.print(page, HEX);
    Serial.println(")");

    delay(50);
  }

  Serial.println("};");
}

byte readByte(int address) {
  // Calculate device address and memory address
  int deviceAddress = BASE_ADDRESS | ((address >> 8) & 0x03);
  int memoryAddress = address & 0xFF;

  // Set the read address
  Wire.beginTransmission(deviceAddress);
  Wire.write(memoryAddress);
  Wire.endTransmission();

  // Request one byte
  Wire.requestFrom(deviceAddress, 1);

  if (Wire.available()) {
    return Wire.read();
  } else {
    return 0xFF; // Return 0xFF if read failed
  }
}

// Function to write the EEPROM data back (example usage)
void writeEEPROMData(const byte* data, int size) {
  Serial.println("Writing EEPROM data...");

  for (int address = 0; address < size && address < EEPROM_SIZE; address++) {
    writeByte(address, data[address]);

    // Page write delay
    if ((address + 1) % PAGE_SIZE == 0) {
      delay(10); // Wait for page write to complete
    }

    // Progress indicator
    if (address % 64 == 0) {
      Serial.print(".");
    }
  }

  Serial.println();
  Serial.println("Write complete!");
}

void writeByte(int address, byte data) {
  // Calculate device address and memory address
  int deviceAddress = BASE_ADDRESS | ((address >> 8) & 0x03);
  int memoryAddress = address & 0xFF;

  // Write the byte
  Wire.beginTransmission(deviceAddress);
  Wire.write(memoryAddress);
  Wire.write(data);
  Wire.endTransmission();

  delay(5); // Write cycle delay
}
