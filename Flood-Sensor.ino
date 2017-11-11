int analogPin = 0;     // Connect Pin 3 of the maxbotix to A0. Pin 4 of the maxbotix must be disconnected!
int rawVal = 0;           // variable to store the value read
int distance = 0;

void setup()

{
  // Outputs information to your serial port
  Serial.begin(9600);          //  setup serial
}

void loop()

{
  rawVal = analogRead(analogPin);    // Read a raw value
  distance = rawVal * 5;             // As per datasheet (to get mm)
  Serial.println(distance);             // Output value
  delay(2000);
}
