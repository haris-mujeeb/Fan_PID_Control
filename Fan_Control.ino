const int tachPin = A0;
const int pwmPin = 5;
int dutyCycle = 0;
double desiredTemp = 2000, pulseDuration, frequency, tempSensor, error, lastError, integral, derivative, pidOutput;

// PID constants
const float Kp = 0.1; // Proportional gain
const float Ki = 0.01; // Integral gain
const float Kd = 0.01; // Derivative gain

double getFanSpeed() { 
  pulseDuration = pulseIn(tachPin, LOW);
  if(pulseDuration > 0) {
    frequency = 1000000.0/pulseDuration;
    return frequency / 4 *60;
  }
  else
    return 0;
}

void setup() {
  pinMode(tachPin, INPUT);
  pinMode(pwmPin, OUTPUT);
  Serial.begin(19200);
}

void loop() {
  // Following a Sine Wave
  for (int i = 0; i < 3600; i++) {
    float sinValue = sin(radians(0.5*i))*1000;  // Calculate sine value
    desiredTemp = map(sinValue, -1*1000, 1*1000, 750, 3000);  // Map to PWM range (0-255)
    tempSensor = getFanSpeed();

    // PID 
    error = desiredTemp - tempSensor;
    integral += error;
    derivative = error - lastError;
    pidOutput = Kp * error + Ki * integral + Kd * derivative;
    dutyCycle = constrain(pidOutput, 0, 255); // saturation block

    analogWrite(pwmPin, dutyCycle);
    Serial.print(desiredTemp);
    Serial.print(" "); 
    Serial.print(dutyCycle);
    Serial.print(" "); 
    Serial.println(tempSensor);
  }
}
