#include <stdlib.h> 

/* --- VARIABLES --- */
int buttons[] = {13, 11, 9, 7}; // Contains pins buttons are connected to. RED = 13, BLUE = 11, GREEN = 9, YELLOW = 7
int lights[] = {12, 10, 8, 6}; // Contains pins LEDs are connected to. RED = 12, BLUE = 10, GREEN = 8, YELLOW = 6 

int buzzerPin = 5;
int tones[] = {252, 415, 209, 310}; // These are the tones that correspond with the various colours

int gameStarted = 0; // Used to determine if there is an active game

int maxLen = 5; // This is the length of the longest sequence
int sequence[5]; // This array is going to hold the sequence. 
int sequenceLen = 0; // This holds the current length of the sequence 

/* --- SETUP & LOOP --- */
void setup() {
	Serial.begin(9600);
	// Set Pin modes
	for (int i = 0; i < 4; i++) 
		pinMode(buttons[i], INPUT_PULLUP); // Buttons are going to be used for input 
	
	for (int i = 0; i < 4; i++)
		pinMode(lights[i], OUTPUT); // Lights are going to be used for output 

	// Function Declarations (prototypes)
	void playSequence(int *sequence);
	int checkPressed();
	void idle();	
	void gameOver();
	void reset();
	void win();

	struct timeval tm;
	gettimeofday(&tm, NULL);
	srandom(tm.tv_sec + tm.tv_usec * 1000000ul);
}

void loop() {
	if (!gameStarted)
		idle();
	else {
		if (sequenceLen == maxLen) 
			win();
		else {
			sequence[sequenceLen] = rand() % 4; // Sets a new element of the pattern to a random number between one and 4.
			sequenceLen++; // We have just added a new element to the sequence so the length has increased by one.
			delay(250);  
			playSequence(sequence, sequenceLen);
			getInput();
			delay(250);

		}
	}
}

/* --- FUNCTIONS --- */

// Plays a sequence and checks if the user wants to play the game
void idle() {
	int sequence[] = {0, 2, 1, 3};
	for (int i = 0; i < 4; i++) {
		digitalWrite(lights[sequence[i]], HIGH);
		delay(100);
		digitalWrite(lights[sequence[i]], LOW); 
		if (checkPressed() < 4) {
			gameStarted = 1;
			delay(250);
			return;
		}
	}
}

// plays a given sequence that is passed in  
void playSequence(int *sequence, int len) {
	for (int i = 0; i < len; i++) {
		digitalWrite(lights[sequence[i]], HIGH);
		tone(buzzerPin, tones[sequence[i]]);
		delay(250);
		digitalWrite(lights[sequence[i]], LOW);
		noTone(buzzerPin);
		delay(250);
	}
}

// Returns the button being pressed, if any.
int checkPressed() {
	if (digitalRead(buttons[0]) == LOW) 
		return 0;
	if (digitalRead(buttons[1]) == LOW) 
		return 1;
	if (digitalRead(buttons[2]) == LOW) 
		return 2;
	if (digitalRead(buttons[3]) == LOW) 
		return 3;
	return 4; // This means that no button ahs been pressed
}

// Gets input from the user
void getInput() {
	int index = 0; // The current element of the pattern that we are requesting input for. 
	int pressed;
	Serial.println(sequenceLen);
	while (index < sequenceLen) {
		delay(50);
		if ((pressed = checkPressed()) < 4) {
			tone(buzzerPin, tones[pressed]);
			digitalWrite(lights[pressed], HIGH);
			while (digitalRead(buttons[pressed]) == LOW); 
			digitalWrite(lights[pressed], LOW);
			noTone(buzzerPin);
			delay(100);
			if (pressed != sequence[index]) {
				gameOver();
				return;
			} index++;
		} 
	} 
}

// resets all of the game values
void reset() {
	int sequence[5];
	gameStarted = 0;
	sequenceLen = 0;
}

// Plays a sequence when a player loses the game
void gameOver() {
	reset();
	toggleLEDs(HIGH);
	tone(buzzerPin, 15);
	delay(250);
	toggleLEDs(LOW);
	noTone(buzzerPin);
	delay(500);
	toggleLEDs(HIGH);
	tone(buzzerPin, 30);
	delay(1000);
	noTone(buzzerPin);
	toggleLEDs(LOW);

}

// Plays a sequence when the player wins the game
void win() {
	reset();
	for (int i = 0; i < 4; i++) {
		digitalWrite(lights[i], HIGH);
		tone(buzzerPin, tones[i]);
		delay(100);
		digitalWrite(lights[i], LOW);
		noTone(buzzerPin);
	}
	for (int i = 3; i >= 0; i--) {
		digitalWrite(lights[i], HIGH);
		tone(buzzerPin, tones[i]);
		delay(100);
		digitalWrite(lights[i], LOW);
		noTone(buzzerPin);
	}
}

// Turns all the LEDs on and off
void toggleLEDs(int highOrLow) {
	for (int i = 0; i < 4; i++) 
		digitalWrite(lights[i], highOrLow);
}
