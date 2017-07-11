//Programa : Comunicacao Xbee utilizando Arduino Xbee Shield
//Autor : FILIPEFLOP

//Armazena os valores recebidos da serial
int i = 0;
//Armazena o estado do led
String myString;
byte mByte;
byte bytesRecebidos[100];

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
    mByte = Serial3.read();
    //estado = Serial3.readString();
    //Serial.println(mByte);
    if (mByte != 60) {
      bytesRecebidos[i] = mByte;
      i++;
    } else {
      bytesRecebidos[i] = 0;
      myString = String((char*)bytesRecebidos);
      i = 0;
    }
    if (myString != '~') {
      Serial.println(myString);
    }
  }
}

//myString = String((char*)bytesRecebidos);
