#include <Servo.h>

Servo baseServo;
Servo shoulderServo;
Servo elbowServo;

int basePos = 115;      // Default Base position
int shoulderPos = 35;   // Default Shoulder position
int elbowPos = 185;     // Default Elbow position

int basePositions[9];
int shoulderPositions[9];
int elbowPositions[9];
int currentPositionIndex = 0; // Tracks the current save slot

bool isPlayingBack = false;
bool joystickMode = false;

const int joystickYPin = A0;  // Y-axis analog pin
const int joystickXPin = A1;  // X-axis analog pin
const int joystickBtnPin = 2; // Button pin

int elbowMinPos = 90;   // Minimum elbow position
int elbowMaxPos = 180;  // Maximum elbow position
bool elbowUp = true;    // Track the direction of the elbow

void setup() {
  baseServo.attach(9);
  shoulderServo.attach(10);
  elbowServo.attach(11);
  
  // Move servos to default positions at startup
  baseServo.write(basePos);
  shoulderServo.write(shoulderPos);
  elbowServo.write(elbowPos);
  
  Serial.begin(115200);
  Serial.println("Robot arm control initialized");
  Serial.println("Current Positions - Base: 115, Shoulder: 35, Elbow: 185");
  Serial.println("Send 'h' for help.");

  pinMode(joystickBtnPin, INPUT_PULLUP); // Joystick button with pull-up resistor
}

void loop() {
  if (joystickMode) {
    handleJoystickControl();
  } else {
    if (Serial.available() > 0) {
      char receivedChar = Serial.read();
      switch (receivedChar) {
        case 'm':
          isPlayingBack = false;
          Serial.println("Playback stopped by user.");
          break;
        case '0':
          moveToHomePosition();
          break;
        case 'z':
          adjustBasePosition(basePos - 5);
          break;
        case 'x':
          adjustBasePosition(basePos + 5);
          break;
        case 'a':
          adjustShoulderPosition(shoulderPos - 5);
          break;
        case 's':
          adjustShoulderPosition(shoulderPos + 5);
          break;
        case 'q':
          adjustElbowPosition(elbowPos - 5);
          break;
        case 'w':
          adjustElbowPosition(elbowPos + 5);
          break;
        case 'h':
          printHelpMenu();
          break;
        case 'l':
          listSavedPositions();
          break;
        case 'e':
          echoCurrentPositions();
          break;
        case 'k':
          saveCurrentPosition();
          break;
        case '/':
          clearSavedPositions();
          break;
        case 'n':
          playbackPositions();
          break;
        case 'j':
          toggleJoystickMode();
          break;
        default:
          if (receivedChar >= '1' && receivedChar <= '9') {
            recallSavedPosition(receivedChar);
          } else {
            Serial.println("Invalid command. Send 'h' for help.");
          }
          break;
      }
      delay(10); // Small delay to keep the loop responsive
    }
  }
}

void adjustBasePosition(int newPos) {
  baseServo.write(newPos);
  basePos = newPos;
}

void adjustShoulderPosition(int newPos) {
  // Limit the shoulder position to a maximum of 60 degrees
  if (newPos > 60) {
    newPos = 60;
  }
  shoulderServo.write(newPos);
  shoulderPos = newPos;
}

void adjustElbowPosition(int newPos) {
  elbowServo.write(newPos);
  elbowPos = newPos;
}

void moveToHomePosition() {
  baseServo.write(90);
  shoulderServo.write(60);  // Set to the maximum allowed position
  elbowServo.write(90);
  basePos = 90;
  shoulderPos = 60;  // Set to the maximum allowed position
  elbowPos = 90;
  Serial.println("Moved to home position.");
}

void printHelpMenu() {
  Serial.println("Help Menu:");
  Serial.println("z/x - Decrease/Increase Base Position");
  Serial.println("a/s - Decrease/Increase Shoulder Position (limited to 60 degrees)");
  Serial.println("q/w - Decrease/Increase Elbow Position");
  Serial.println("k - Save Current Position");
  Serial.println("1-9 - Recall Saved Position");
  Serial.println("l - List Saved Positions");
  Serial.println("e - Echo Current Positions");
  Serial.println("/ - Clear Saved Positions");
  Serial.println("n/m - Start/Stop Position Playback");
  Serial.println("0 - Move to Home Position");
  Serial.println("j - Toggle Joystick Mode");
}

void listSavedPositions() {
  if (currentPositionIndex > 0) {
    for (int i = 0; i < currentPositionIndex; i++) {
      Serial.print("Slot ");
      Serial.print(i + 1);
      Serial.print(": Base ");
      Serial.print(basePositions[i]);
      Serial.print(", Shoulder ");
      Serial.print(shoulderPositions[i]);
      Serial.print(", Elbow ");
      Serial.println(elbowPositions[i]);
    }
  } else {
    Serial.println("No positions saved.");
  }
}

void echoCurrentPositions() {
  Serial.print("Current Positions - Base: ");
  Serial.print(basePos);
  Serial.print(", Shoulder: ");
  Serial.print(shoulderPos);
  Serial.print(", Elbow: ");
  Serial.println(elbowPos);
}

void saveCurrentPosition() {
  if (currentPositionIndex < 9) {
    basePositions[currentPositionIndex] = basePos;
    shoulderPositions[currentPositionIndex] = shoulderPos;
    elbowPositions[currentPositionIndex] = elbowPos;
    currentPositionIndex++;
    Serial.println("Position saved.");
  } else {
    Serial.println("Maximum save slots reached.");
  }
}

void recallSavedPosition(char slot) {
  int index = slot - '1';
  if (index >= 0 && index < currentPositionIndex) {
    baseServo.write(basePositions[index]);
    shoulderServo.write(shoulderPositions[index]);
    elbowServo.write(elbowPositions[index]);
    Serial.print("Moved to position ");
    Serial.println(index + 1);
  } else {
    Serial.println("Invalid position slot.");
  }
}

void clearSavedPositions() {
  currentPositionIndex = 0;
  Serial.println("All positions cleared.");
}

void playbackPositions() {
  Serial.println("Playback started. Press 'm' to stop.");
  isPlayingBack = true;

  while (isPlayingBack) { // Loop to keep playing back positions until stopped
    for (int posIndex = 0; posIndex < currentPositionIndex && isPlayingBack; ++posIndex) {
      baseServo.write(basePositions[posIndex]);
      shoulderServo.write(shoulderPositions[posIndex]);
      elbowServo.write(elbowPositions[posIndex]);

      delay(500); // Adjust this delay for the speed of movement between positions

      // Update current positions to the target for the next round
      basePos = basePositions[posIndex];
      shoulderPos = shoulderPositions[posIndex];
      elbowPos = elbowPositions[posIndex];

      // Check for stop command
      if (Serial.available() > 0) {
        char cmd = Serial.read();
        if (cmd == 'm') {
          Serial.println("Playback stopped.");
          isPlayingBack = false;
          break; // Exit the loop
        }
      }
    }
  }
}

void toggleJoystickMode() {
  joystickMode = !joystickMode;
  if (joystickMode) {
    Serial.println("Joystick mode activated.");
  } else {
    Serial.println("Joystick mode deactivated.");
  }
}

void handleJoystickControl() {
  int yValue = analogRead(joystickYPin); // Read Y-axis
  int xValue = analogRead(joystickXPin); // Read X-axis
  bool buttonPressed = digitalRead(joystickBtnPin) == LOW; // Read button (active low)

  // Map joystick values to servo positions (inverted for X-axis)
  int newBasePos = map(xValue, 0, 1023, 180, 0); // Invert left-right control
  int newShoulderPos = map(yValue, 0, 1023, 0, 60); // Limited to 60 degrees

  adjustBasePosition(newBasePos);
  adjustShoulderPosition(newShoulderPos);

  if (buttonPressed) {
    if (elbowUp) {
      adjustElbowPosition(elbowMaxPos);
    } else {
      adjustElbowPosition(elbowMinPos);
    }
    elbowUp = !elbowUp; // Toggle the elbow direction
    Serial.print("Button pressed - Elbow moved to ");
    Serial.println(elbowUp ? "max" : "min");
    delay(300); // Debounce delay to prevent multiple triggers
  }

  delay(100); // Small delay to avoid too rapid updates
}
