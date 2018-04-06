# ManateeTracking
Asset tracking software for Particle Electron integration with Azure IoT Hub

## Usage
Set up the hardware:
- Particle Electron Cellular 3G IoT Board
- Particle Asset Tracker GPS Shield

## Flash Program to Microcontroller
- Go to build.particle.io
- Copy/paste this program
- Manually include the libraries <AssetTracker.h> and <Adafruit_GPS.h> into your program tab if flashing over the air
- Program will not successfully flash if you do not complete this step
## Particle Integration with Azure IoT
- Create Azure account
- On Azure, create a new resource "IoT Hub"
- Record policy name and Shared Access Policy Key
- Use these details to set up integration of Particle with Azure IoT on console.particle.io
- Create a custom JSON under "Advanced Settings"
![alt text](https://github.com/ericsibored/ManateeTracking/ParticleJSON.png

## Azure Data Processing & Visualization
- Create a new resource "Stream Analytics"
- Establish IoT hub as an input and PowerBI as an output
- Begin the Stream Analytics job

## PowerBI Visualization
- Wait for satellite fix (blinking red LED)
- Observe Particle Console for successful data publication to the cloud
- Open PowerBI > Streaming Data Set > (Your Data Set)
- Perform desired data visualizations or manipulations. In this case, we are plotting GPS coordinates on a map.
- Save Report & Deploy to Web Service as embed code
- Paste into web app

To demo our visualization, visit http://manatee.azurewebsites.net

## License
2018, M.A.N.A.T.E.E. , Eric Zhang
