
#include <ArduinoBLE.h>

const int ledPin = LED_BUILTIN;
bool lastState = false;
byte value = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // initialize the BLE hardware
  BLE.begin();

  Serial.println("BLE Central - LED control");

  // start scanning for peripherals
  BLE.scanForUuid("19b10010-e8f2-537e-4f6c-d104768a1214");
}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral)
  {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    if (peripheral.localName() != "Temperature") 
    {
      return;
    }

    // stop scanning
    BLE.stopScan();

    controlLed(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForUuid("19b10010-e8f2-537e-4f6c-d104768a1214");
  }
}

void controlLed(BLEDevice peripheral)
{
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect())
  {
    Serial.println("Connected");
  } 
  
  else
  {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes())
  {
    Serial.println("Attributes discovered");
  } 
  
  else 
  {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // retrieve the LED characteristic
  BLECharacteristic ledCharacteristic = peripheral.characteristic("19b10011-e8f2-537e-4f6c-d104768a1214");
  BLECharacteristic buttonCharacteristic = peripheral.characteristic("19B10012-E8F2-537E-4F6C-D104768A1214");

  if (!switchCharacteristic)
  {
    Serial.println("Peripheral does not have a temperature characteristic!");
    peripheral.disconnect();
    return;
  } 
  
  else if (!switchCharacteristic.canRead()) 
  {
    Serial.println("Peripheral does not have a writable temp characteristic!");
    peripheral.disconnect();
    return;
  }

  while (peripheral.connected()) 
  {
    // while the peripheral is connected


      Serial.println(buttonCharacteristic.read());   
  }

  Serial.println("Peripheral disconnected");
}
