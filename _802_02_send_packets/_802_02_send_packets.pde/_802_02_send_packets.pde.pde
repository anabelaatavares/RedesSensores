#include <WaspXBee802.h>
#include <WaspFrame.h>

// Destination MAC address
//////////////////////////////////////////
char RX_ADDRESS[] = "0013A2004090A8FC";
//////////////////////////////////////////

// Define the Waspmote ID
char WASPMOTE_ID[] = "Node_Corredor1";


// define variable
uint8_t error;

long tempoAtual, tempoInicio;



void setup()
{
  // init USB port
  USB.ON();
  USB.println(F("Sending packets example"));

  // store Waspmote identifier in EEPROM memory
  frame.setID(WASPMOTE_ID);

  // init XBee
  xbee802.ON();

}


void loop()
{
  tempoAtual = millis();

  if (tempoAtual - tempoInicio > 2000) {
    tempoInicio = millis();

    // Create new frame (ASCII)
    frame.createFrame(ASCII);

    // set frame fields (String - char*)
    frame.addSensor(SENSOR_STR, "Bananas");
    frame.addSensor(SENSOR_STR, "k");
    // Prints frame
    frame.showFrame();
    ///////////////////////////////////////////
    // 2. Send packet
    ///////////////////////////////////////////
    // send XBee packet
    error = xbee802.send(RX_ADDRESS, frame.buffer, frame.length );

    // check TX flag
    if ( error == 0 )
    {
      USB.println(F("send ok"));
    }
    else
    {
      USB.println(F("send error"));
    }
  }
}
