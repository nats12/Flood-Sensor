
// Connect Pin 3 of the maxbotix to A0.
int analogPin = 0; 
// Variable to store the value read   
int rawVal = 0;   
// Variable to store the distance
int distance = 0;

// Initial depth
float depth = 0;    

void setup()

{
  // Outputs information to your serial port
  //  Setup serial
  Serial.begin(9600);  

   // delay(3000);

  // Wait for serial to connect
  while (!Serial){}
    
  int currentDistanceToRiverTopMM;

  // Check for incoming serial data:
  //if (Serial.available() > 0) {
    Serial.println("Set up");

    char initialRiverDepthMMInput[10];
    String inStr;

    // Whilst inStr is null, do nothing, skip
    while ((inStr = Serial.readString()) == NULL){}

    const int initialRiverDepthMM = inStr.toInt();
    Serial.println(initialRiverDepthMM); 
  //}
}

void loop()

{
  // Read a raw value
  rawVal = analogRead(analogPin);
  // As per datasheet (to get mm)   
  distance = rawVal * 5;  
  // Output value  
  Serial.println(distance);   

  // Check for incoming serial data:
  if (Serial.available() > 0) {
  
    String str;

    // Whilst str is null, do nothing, skip
    while ((str = Serial.readString()) == NULL){}
    
    // Convert string to int
    Serial.println(str.toInt());

  }
  
  delay(2000);
}
