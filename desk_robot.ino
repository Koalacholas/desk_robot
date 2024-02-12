#include <Servo.h>

Servo baseServo;
Servo shoulderServo;
Servo elbowServo;

int basePos = 90;
int shoulderPos = 90;
int elbowPos = 90;

int basePositions[9];
int shoulderPositions[9];
int elbowPositions[9];
int currentPositionIndex = 0; // Tracks the current save slot

bool isPlayingBack = false;

enum InterpolationMode {
  LINEAR,
  SCURVE
};

InterpolationMode currentMode = LINEAR;

void setup() {
  baseServo.attach(9);
  shoulderServo.attach(10);
  elbowServo.attach(11);
  
  Serial.begin(115200);
  Serial.println("Robot arm control initialized");
  Serial.println("Send 'h' for help.");
}

void loop() {
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
      case 't': // Toggle interpolation mode
        toggleInterpolationMode();
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

void toggleInterpolationMode() {
  currentMode = (currentMode == LINEAR) ? SCURVE : LINEAR;
  Serial.print("Interpolation mode set to: ");
  Serial.println(currentMode == LINEAR ? "LINEAR" : "SCURVE");
}

void adjustPositionWithInterpolation(Servo& servo, int& currentPosition, int targetPosition) {
  int steps = 50; // Number of steps for interpolation
  int delayTime = 20; // Delay between steps to control speed
  for (int i = 0; i <= steps; i++) {
    float t = (float)i / (float)steps;
    float newPos;

    if (currentMode == LINEAR) {
      newPos = currentPosition + (targetPosition - currentPosition) * t;
    } else { // SCURVE
      float s = sCurveInterpolation(t);
      newPos = currentPosition + (targetPosition - currentPosition) * s;
    }

    servo.write((int)newPos);
    delay(delayTime);
  }
  currentPosition = targetPosition;
}

float sCurveInterpolation(float t) {
  // S-Curve equation, for example, ease-in-out cubic
  return t < 0.5 ? 4 * t * t * t : 1 - pow(-2 * t + 2, 3) / 2;
}

void adjustBasePosition(int newPos) {
  // Apply interpolation based on current mode
  if (currentMode == SCURVE) {
    // S-curve interpolation logic here
  }
  baseServo.write(newPos); // Placeholder for actual movement logic
  basePos = newPos;
}

void adjustShoulderPosition(int newPos) {
  // Apply interpolation based on current mode
  if (currentMode == SCURVE) {
    // S-curve interpolation logic here
  }
  shoulderServo.write(newPos); // Placeholder for actual movement logic
  shoulderPos = newPos;
}

void adjustElbowPosition(int newPos) {
  // Apply interpolation based on current mode
  if (currentMode == SCURVE) {
    // S-curve interpolation logic here
  }
  elbowServo.write(newPos); // Placeholder for actual movement logic
  elbowPos = newPos;
}

void moveToHomePosition() {
  baseServo.write(90);
  shoulderServo.write(90);
  elbowServo.write(90);
  basePos = shoulderPos = elbowPos = 90;
  Serial.println("Moved to home position.");
}

void printHelpMenu() {
  Serial.println("Help Menu:");
  Serial.println("z/x - Decrease/Increase Base Position");
  Serial.println("a/s - Decrease/Increase Shoulder Position");
  Serial.println("q/w - Decrease/Increase Elbow Position");
  Serial.println("k - Save Current Position");
  Serial.println("1-9 - Recall Saved Position");
  Serial.println("l - List Saved Positions");
  Serial.println("e - Echo Current Positions");
  Serial.println("/ - Clear Saved Positions");
  Serial.println("n/m - Start/Stop Position Playback");
  Serial.println("0 - Move to Home Position");
  Serial.println("t - Toggle Interpolation Mode");
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

int calculateNewPosition(int currentPos, int targetPos, float t, InterpolationMode mode) {
  float newPos;
  if (mode == LINEAR) {
    newPos = currentPos + (targetPos - currentPos) * t;
  } else { // SCURVE
    float s = sCurveInterpolation(t);
    newPos = currentPos + (targetPos - currentPos) * s;
  }
  return (int)newPos;
}

void playbackPositions() {
  Serial.println("Playback started. Press 'm' to stop.");
  isPlayingBack = true;
  const int steps = 50; // Number of steps for interpolation
  const int delayTime = 20; // Delay between steps to control speed

  while (isPlayingBack) { // Loop to keep playing back positions until stopped
    for (int posIndex = 0; posIndex < currentPositionIndex && isPlayingBack; ++posIndex) {
      for (int step = 0; step <= steps; step++) {
        float t = (float)step / (float)steps;

        // Calculate new positions for each servo at this step
        int newBasePos = calculateNewPosition(basePos, basePositions[posIndex], t, currentMode);
        int newShoulderPos = calculateNewPosition(shoulderPos, shoulderPositions[posIndex], t, currentMode);
        int newElbowPos = calculateNewPosition(elbowPos, elbowPositions[posIndex], t, currentMode);

        // Apply the new positions to the servos
        baseServo.write(newBasePos);
        shoulderServo.write(newShoulderPos);
        elbowServo.write(newElbowPos);

        delay(delayTime);

        // Check for stop command
        if (Serial.available() > 0) {
          char cmd = Serial.read();
          if (cmd == 'm') {
            Serial.println("Playback stopped.");
            isPlayingBack = false;
            break; // Exit the step loop
          }
        }
      }
      // Update current positions to the target for the next round
      basePos = basePositions[posIndex];
      shoulderPos = shoulderPositions[posIndex];
      elbowPos = elbowPositions[posIndex];

      if (!isPlayingBack) break; // Exit if playback was stopped
    }
  }
}



