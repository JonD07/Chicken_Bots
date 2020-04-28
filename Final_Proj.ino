/*

 */

//
/// Global constants
const int SENSOR_TEST = 0;
const int SPEED = 75;
const int LED = 4;

// Distance sensors
const int echoPin1 = 2;		// Right sensor
const int trigPin1 = 3;
const int echoPin2 = 12;	// Left sensor
const int trigPin2 = 13;

// Motor Driver
const int m1LogicA = 5;		// Left Wheel
const int m1LogicB = 6;
const int m2LogicA = 10;	// Right Wheel
const int m2LogicB = 11;

// IR Sensors
// '1' => line detected
// '0' => no line detected
const int leftIRPin = 8;
const int rightIRPin = 7;

const int sensorPin = A0;

// States
const int A = 0;
const int B = 1;
const int C = 2;
const int D = 3;
const int E = 4;
const int F = 5;
const int G = 6;
const int H = 7;
const int I = 8;
const int J = 9;
const int K = 10;


//
/// Global variables
int left_sensor;
int right_sensor;
int lightCal;
int lightVal;

int state;
int test;

float distance1, distance2;

void setup() {
	//
	/// Set pin-modes
	pinMode(LED, OUTPUT);
	// Dist sensors
	pinMode(trigPin1, OUTPUT);
	pinMode(echoPin1, INPUT);
	pinMode(trigPin2, OUTPUT);
	pinMode(echoPin2, INPUT);
	// Motor Driver
	pinMode(m1LogicA, OUTPUT);
	pinMode(m1LogicB, OUTPUT);
	pinMode(m2LogicA, OUTPUT);
	pinMode(m2LogicB, OUTPUT);
	// IR sensors
	pinMode(leftIRPin, INPUT);
	pinMode(rightIRPin, INPUT);
	
	//
	/// Initial config
	state = A;
	test = 0;
	// Set motor driver pins LOW
	digitalWrite(m1LogicA, LOW);
	digitalWrite(m1LogicB, LOW);
	digitalWrite(m2LogicA, LOW);
	digitalWrite(m2LogicB, LOW);
	
	// Start serial write
	Serial.begin(9600);
	
	delay(3000);
}

void loop() {
	if(SENSOR_TEST) {
		test++;
		if(test & 0x064) {
			digitalWrite(LED, HIGH);
		}
		else {
			digitalWrite(LED, LOW);
		}
		// Gather Data
		distance2 = getDistanceL();
		distance1 = getDistanceR();
		left_sensor = digitalRead(leftIRPin);
		right_sensor = digitalRead(rightIRPin);

		lightVal = analogRead(sensorPin);


		Serial.print("Dist R: ");
		Serial.print(distance1);
		Serial.print("\tDist L: ");
		Serial.print(distance2);
	//	Serial.print("\tPhoto: ");
	//	Serial.println(lightVal);
		Serial.print("\tL: ");
		Serial.print(left_sensor);
		Serial.print("\tR: ");
		Serial.println(right_sensor);
	}
	else {
		state = runStateMachine();
	}
}

int runStateMachine() {
	switch(state) {
	
		case A: {
			if(digitalRead(leftIRPin)) {
				Brake();
				return G;
			}
			if(digitalRead(rightIRPin)) {
				Brake();
				return H;
			}
			
			// Check left distance sensor
			if(getDistanceL() < 10.0) {
				Right();
				return A;
			} 
			// Check right distance sensor
			if(getDistanceR() < 10.0) {
				Left();
				return A;
			}
			
			/*
			TODO: add D trans here
			*/
			
			forward();
			return A;
		}
		
		case G: {
			left_sensor = digitalRead(leftIRPin);
			right_sensor = digitalRead(rightIRPin);
			if(left_sensor && right_sensor) {
				Right();
				return I;
			}
			if(left_sensor) {
				forward();
				return G;
			}
			
			Brake();
			digitalWrite(LED, HIGH);
			return K;
		}
		
		case H: {
			left_sensor = digitalRead(leftIRPin);
			right_sensor = digitalRead(rightIRPin);
			if(left_sensor && right_sensor) {
				Left();
				return J;
			}
			if(right_sensor) {
				forward();
				return H;
			}
			
			Brake();
			digitalWrite(LED, HIGH);
			return K;
		}
		
		case I: {
			left_sensor = digitalRead(leftIRPin);
			right_sensor = digitalRead(rightIRPin);
			if(left_sensor && right_sensor) {
				Right();
				return I;
			}
			if(left_sensor) {
				Brake();
				return G;
			}
			
			forward();
			return I;
		}
		
		case J: {
			left_sensor = digitalRead(leftIRPin);
			right_sensor = digitalRead(rightIRPin);
			if(left_sensor && right_sensor) {
				Left();
				return J;
			}
			if(right_sensor) {
				Brake();
				return H;
			}
			
			forward();
			return J;
		}
		
		case K: {
			left_sensor = digitalRead(leftIRPin);
			right_sensor = digitalRead(rightIRPin);
			if(left_sensor) {
				Left_left();
				return K;
			}
			if(right_sensor) {
				Right_right();
				return K;
			}
		
			// Check left distance sensor
			if(getDistanceL() < 10.0) {
				Brake();
				return K;
			}
			// Check right distance sensor
			if(getDistanceR() < 10.0) {
				Brake();
				return K;
			}
			
			forward();
			digitalWrite(LED, HIGH);
			return K;
		}
		
	}
}

// forward
void forward() {
	digitalWrite(m1LogicA, LOW);
	analogWrite(m1LogicB, SPEED);
	digitalWrite(m2LogicA, LOW);
	analogWrite(m2LogicB, SPEED);
}

// Brake
void Brake() {
	digitalWrite(m1LogicA, HIGH);
	digitalWrite(m1LogicB, HIGH);
	digitalWrite(m2LogicA, HIGH);
	digitalWrite(m2LogicB, HIGH);
}

// Right(), turn right by reversing
void Right() {
	digitalWrite(m1LogicA, HIGH);
	digitalWrite(m1LogicB, HIGH);
	analogWrite(m2LogicA, SPEED);
	digitalWrite(m2LogicB, LOW);
}

// Left(), turn left by reversing
void Left() {
	analogWrite(m1LogicA, SPEED);
	digitalWrite(m1LogicB, LOW);
	digitalWrite(m2LogicA, HIGH);
	digitalWrite(m2LogicB, HIGH);
}

// right(), turn right by moving forward
void right() {
	digitalWrite(m1LogicA, LOW);
	analogWrite(m1LogicB, SPEED);
	digitalWrite(m2LogicA, HIGH);
	digitalWrite(m2LogicB, HIGH);
}

// left(), turn right by moving forward
void left() {
	digitalWrite(m1LogicA, HIGH);
	digitalWrite(m1LogicB, HIGH);
	digitalWrite(m2LogicA, LOW);
	analogWrite(m2LogicB, SPEED);
}

// Left_left(), turn left by spinning
void Left_left() {
	analogWrite(m1LogicA, SPEED);
	digitalWrite(m1LogicB, LOW);
	digitalWrite(m2LogicA, LOW);
	analogWrite(m2LogicB, SPEED);
}

// Right_right(), turn right by spinning
void Right_right() {
	digitalWrite(m1LogicA, LOW);
	analogWrite(m1LogicB, SPEED);
	analogWrite(m2LogicA, SPEED);
	digitalWrite(m2LogicB, LOW);
}

float getDistanceR() {
	float dist;
	
	digitalWrite(trigPin1, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPin1, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin1, LOW);
	dist = pulseIn(echoPin1, HIGH);
	
	return (dist*0.0343)/2.0;
}

float getDistanceL() {
	float dist;
	
	digitalWrite(trigPin2, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPin2, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin2, LOW);
	dist = pulseIn(echoPin2, HIGH);
	
	return (dist*0.0343)/2.0;
}





