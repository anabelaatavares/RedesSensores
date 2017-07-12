String myString, idNodeMCU, NodeCorredor, ContaPassagem, CoordenadasGPS;
String valores [100];
String mByte;
byte bytesRecebidos[200];

long tempoAtual, tempoInicio;

void setup()
{
  //Define o pino 13 - LED embutido no Arduino - como saida
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  Serial3.begin(9600);
}

void loop()
{
  if (Serial3.available() > 0) {
    mByte = Serial3.readBytesUntil('<=>', bytesRecebidos, 128);
    if (mByte != 16) {
      myString = String((char*)bytesRecebidos);
      if (myString != "~") {
        idNodeMCU = getValue(myString, '#', 1);
        NodeCorredor = getValue(myString, '#', 2);
        ContaPassagem = getValue(myString, '#', 3);
        CoordenadasGPS = getValue(myString, '#', 4);
        Serial.println(idNodeMCU + " " + NodeCorredor + " " + ContaPassagem + " " + CoordenadasGPS);
      }

    }

  }
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
