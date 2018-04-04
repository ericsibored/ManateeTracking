//Revision log
/* -----------------------------------------------------------
Mar-14-2018: program creation
Mar-29-2018: integration with Azure IoT and PowerBI
Apr-04-2018: cleaned unused variables and added comments
---------------------------------------------------------------*/
//User notes
/* -----------------------------------------------------------
When importing this code onto a new Particle program, you MUST
manually include the libraries below otherwise this program will
compile properly.

**Only for flashing device OTA via build.particle.io
---------------------------------------------------------------*/
// This #include statement was automatically added by the Particle IDE.
#include <AssetTracker.h>

// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_GPS.h>


/* -----------------------------------------------------------
This example shows a lot of different features. As configured here
it will check for a good GPS fix every 10 minutes and publish that data
if there is one. If not, it will save you data by staying quiet. It also
registers 3 Particle.functions for changing whether it publishes,
reading the battery level, and manually requesting a GPS reading.
---------------------------------------------------------------*/


// Set whether you want the device to publish data to the internet by default here.
// 1 will Particle.publish AND Serial.print, 0 will just Serial.print
// Extremely useful for saving data while developing close enough to have a cable plugged in.
// You can also change this remotely using the Particle.function "tmode" defined in setup()
int transmittingData = 1;

//Time between publishes in seconds
int inSeconds = 20;

// Creating an AssetTracker named 't' for us to reference
AssetTracker t = AssetTracker();

// A FuelGauge named 'fuel' for checking on the battery state
FuelGauge fuel;
String Data;

String Coordinates;
String B;

String azureData;

// setup() and loop() are both required. setup() runs once when the device starts
// and is used for registering functions and variables and initializing things
void setup() {https://build.particle.io/build/59ff8fc1e6eed0d6fa000517#verify
    // Sets up all the necessary AssetTracker bits
    t.begin();

    // Enable the GPS module. Defaults to off to save power.
    // Takes 1.5s or so because of delays.
    t.gpsOn();
    
    //t.antennaExternal(); //use external antenna

    // Opens up a Serial port so you can listen over USB
    Serial.begin(9600);

    // These three functions are useful for remote diagnostics. Read more below.
    Particle.function("tmode", transmitMode);
    Particle.function("writeData", writeData);


    Particle.function("SetAntInt", SetAntInt);
    Particle.function("SetAntExt", SetAntExt);

   //Test Variable contains GPS coordinates and battery level
    Particle.variable("Data", Data);


}

// loop() runs continuously
void loop() {
    // You'll need to run this every loop to capture the GPS output
    t.updateGPS();

        // Dumps the full NMEA sentence to serial in case you're curious
        Serial.println(t.preNMEA());

        // GPS requires a "fix" on the satellites to give good data,
        // so we should only publish data if there's a fix
            // Only publish if we're in transmittingData mode 1;
            if (transmittingData) 
            {
                // Short publish names save data!
                if (t.gpsFix()) 
                {
                    Coordinates = t.readLatLon();
                    
                    String lat = String::format("%f",t.readLatDeg());
                    String lon = String::format("%f",t.readLonDeg());
                    
                    azureData = "{ \"lat\": " + String::format("%f",t.readLatDeg()) +
                    ", \"lng\": "+ String::format("%f",t.readLonDeg()) + " }";
                    Particle.publish("locateTracker", azureData, 60, PRIVATE);
                    //publishes JSON formatted Latitude, Longitude
                    //TODO: include battery level, hardware timestamp, device ID
                    
                    delay(inSeconds*1000);
                    //time between data collection
                    //used for rapid collection for verification purposes with Zoo
                    //field testing will incorporate "hibernate" function to save battery
                  
                } 
                else 
                {
                    Coordinates = "NULL";
                    azureData = "NULL";
                }
    
                B = "v:" + String::format("%.2f",fuel.getVCell()) +
                ",c:" + String::format("%.2f",fuel.getSoC());
                
                Data = Coordinates+B;
                //Saves GPS coordinates and battery level for Particle console testing purposes
                // Particle.publish("Data",Data,60, PRIVATE);
            }
}


//Particle Console Test Functions

// Allows you to remotely change whether a device is publishing to the cloud
// or is only reporting data over Serial. Saves data when using only Serial!
// Change the default at the top of the code.
int transmitMode(String command) {
    transmittingData = atoi(command);
    return 1;
}

// Actively ask for a GPS reading if you're impatient. Only publishes if there's
// a GPS fix, otherwise returns '0'
int writeData(String command) {
    
    t.updateGPS();

    if (t.gpsFix()) {
        
        Coordinates = t.readLatLon();
        // uncomment next line if you want a manual publish to reset delay counter
        // lastPublish = millis();
    } else {
        Coordinates = "NULL";
    }
    
        B = "v:" + String::format("%.2f",fuel.getVCell()) +
          ",c:" + String::format("%.2f",fuel.getSoC());
    
    Data = Coordinates+B;
    // Particle.publish("Data",Data,60, PRIVATE);
    
    return (1);
}

//Internal Antenna control (if attached)
int SetAntInt(String command) {
    
    t.antennaInternal();
    
    return (1);
}

//External Antenna Control (if attached)
int SetAntExt(String command) {
    
    t.antennaExternal();
    
    return (1);
}
