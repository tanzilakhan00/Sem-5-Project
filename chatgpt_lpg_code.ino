#include <Servo.h> // Include the Servo library

// Pin definitions
const int gasSensorPin = A0;  // Analog pin connected to the gas sensor (MQ sensor)
const int relayPin = 8;       // Digital pin connected to the relay
const int buzzerPin = 9;      // Digital pin connected to the buzzer
const int servoPin = 10;      // Digital pin connected to the servo motor

const int thresholdValue = 300; // Threshold value for gas detection (adjust as needed)
Servo myServo;                  // Create a servo object

void setup() {
    // Initialize serial communication for debugging and GSM
    Serial.begin(9600);
    
    // Set pin modes
    pinMode(gasSensorPin, INPUT);
    pinMode(relayPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    
    // Set initial states
    digitalWrite(relayPin, LOW); // Ensure fan is off at the beginning
    digitalWrite(buzzerPin, LOW); // Ensure buzzer is off at the beginning
    
    // Attach the servo motor to the designated pin
    myServo.attach(servoPin);
    myServo.write(0); // Set the servo to the closed position (open valve)

    // Send an initial message
    sendSMS("Gas detection system initialized.");
}

void loop() {
    // Read the value from the gas sensor
    int gasValue = analogRead(gasSensorPin);
    
    // Print the sensor value to the Serial Monitor for debugging
    Serial.print("Gas Sensor Value: ");
    Serial.println(gasValue);
    
    // If gas is detected (above the threshold), activate the components
    if (gasValue > thresholdValue) {
        Serial.println("Gas detected! Activating components...");
        
        // Close the gas valve using the servo
        myServo.write(90); // Adjust this angle based on your valve mechanism
        
        // Turn on the fan (activate the relay)
        digitalWrite(relayPin, HIGH);
        
        // Turn on the buzzer
        digitalWrite(buzzerPin, HIGH);
        
        // Send an SMS notification
        sendSMS("Warning! Gas detected!");

        // Make a call to the emergency number
        makeCall("+1234567890"); // Replace with the recipient's phone number
    } else {
        Serial.println("No gas detected. Deactivating components...");
        
        // Open the gas valve using the servo
        myServo.write(0); // Adjust this angle to open the valve
        
        // Turn off the fan (deactivate the relay)
        digitalWrite(relayPin, LOW);
        
        // Turn off the buzzer
        digitalWrite(buzzerPin, LOW);
    }
    
    // Delay for a short while before the next sensor reading
    delay(1000);
}

// Function to send SMS
void sendSMS(const char* message) {
    Serial.println("Sending SMS...");
    Serial.print("AT+CMGF=1\r"); // Set SMS mode to text
    delay(100);
    Serial.print("AT+CMGS=\"+1234567890\"\r"); // Replace with the recipient's phone number
    delay(100);
    Serial.print(message); // Send the message
    delay(100);
    Serial.write(26); // Send Ctrl+Z to indicate end of message
    delay(1000); // Wait for a response
}

// Function to make a call
void makeCall(const char* phoneNumber) {
    Serial.print("ATD");
    Serial.print(phoneNumber); // Phone number to call
    Serial.println(";"); // End the command
    delay(100); // Wait for a short time to process the call command
}