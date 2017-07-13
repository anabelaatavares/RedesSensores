#include <SphericalUtil_Arduino.h>

#include <WaspGPS.h>

#include <WaspXBee802.h>
#include <WaspFrame.h>


// Destination MAC address
//////////////////////////////////////////
char RX_ADDRESS[] = "0013A2004090A8FC";
//////////////////////////////////////////

// Define the Waspmote ID
char WASPMOTE_ID[] = "Node_Corredor1";

#define TIMEOUT 240




// define status variable for GPS connection
bool status, checkpoint;
double lat, lng;

// define variable
uint8_t error;

long tempoAtual, tempoInicio;

int intervalo = 5000;
int numFrame = 0;


void setup()
{
  xbee802.OFF();
  GPS.OFF();
  // init USB port
  USB.ON();
  USB.println(F("GPS_EXAMPLE"));
  // store Waspmote identifier in EEPROM memory
  frame.setID(WASPMOTE_ID);
  // init XBee
  //xbee802.ON();
  GPS.ON();

}


void loop()
{
  while(!checkpoint) {
    sacarCoordenadas();
    //coiso
    toRadians(123.123);
    //coordenadas dos checkpoints
    pos coord_chekpoint;
    coord_chekpoint.lt = 40.5396671;
    coord_chekpoint.ln = -7.2800064;

    pos coord_user;
    coord_user.lt = lat;
    coord_user.ln = lng;
    //calcular a distancia entre checkpoint e gps do user
    double distance = computeDistanceBetween (coord_chekpoint, coord_user);
    //Coords coordenadas(10,10);
    USB.print(distance);
    USB.print(F("\n"));

    if(distance < 215) {
      checkpoint = true;
    }

  }
  enviarPacote();
  
  
  delay(intervalo);
}

void enviarPacote() {
  GPS.OFF();
  xbee802.ON();

  // Create new frame (ASCII)
  frame.createFrame(ASCII);
  //frame.addSensor(SENSOR_STR, numFrame);
  frame.addSensor(SENSOR_GPS, lat, lng);
  // Prints frame
  frame.showFrame();

  // send XBee packet
  error = xbee802.send(RX_ADDRESS, frame.buffer, frame.length );

  // check TX flag
  if ( error == 0 ) {
    USB.println(F("send ok"));
  } else {
    USB.println(F("send error"));
  }

  numFrame++;
  USB.print(numFrame);  
  checkpoint = false;
  GPS.ON();
}

void sacarCoordenadas() {
  xbee802.OFF();
  
  status = GPS.waitForSignal(TIMEOUT);

  if( status == true ) {
    USB.println(F("\n----------------------"));
    USB.println(F("Connected"));
    USB.println(F("----------------------"));
    lat = GPS.convert2Degrees(GPS.latitude, GPS.NS_indicator);
    lng = GPS.convert2Degrees(GPS.longitude, GPS.EW_indicator);
  } else {
    USB.println(F("\n----------------------"));
    USB.println(F("GPS TIMEOUT. NOT connected"));
    USB.println(F("----------------------"));
    lat = 0.0;
    lng = 0.0;
  }
  
}

