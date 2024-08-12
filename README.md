# Desk Robot Firmware

This repository contains Arduino firmware for a customizable desk robot, specifically designed for use with Arduino Nano and standard 9g microservos. The firmware facilitates precise control over the robot's movements through three servos that operate the base, shoulder, and elbow. Ideal for robotics hobbyists, this setup uses widely available 9g microservos, making it accessible for those interested in building their own 3D printed robot chassis.

## Features

- **Servo Control**: Precise management of three 9g microservos.
- **Manual Operation**: Direct adjustment of positions via serial commands or a connected joystick.
- **Joystick Mode**: Control the base and shoulder using a joystick, with a button to control the elbow's position.
- **Position Memory**: Capability to save and recall up to 9 distinct servo positions.
- **Automated Playback**: Automated playback of saved positions for smooth motion transitions.
- **Help Menu**: An integrated list of commands for straightforward operation.

## Getting Started

### Prerequisites

- Arduino IDE.
- Servo library (install via the Arduino IDE Library Manager).
- Arduino Nano.
- Three 9g microservos.
- Joystick module (optional for joystick mode).

### Hardware Setup

The project is designed around a 3D printed chassis tailored for 9g microservos, offering flexibility for personal customization or adaptation from existing designs.

1. **9g Microservos**: Attach the microservos to the designated mounts on your 3D printed chassis for the base, shoulder, and elbow.
    - **Base Servo**: Connect to pin D9.
    - **Shoulder Servo**: Connect to pin D10.
    - **Elbow Servo**: Connect to pin D11.
2. **Joystick Module** (Optional): Connect the joystick for manual control.
    - **Y-axis**: Connect to analog pin A0.
    - **X-axis**: Connect to analog pin A1.
    - **Button**: Connect to digital pin D2.
3. Ensure the servos and joystick are properly secured and aligned with the robot's joints for optimal performance.

### Software Setup

1. **Install the Arduino IDE** and the **Servo library**.
2. **Load the firmware**: Clone this repository and open the firmware sketch in the Arduino IDE.
3. **Configure and upload**: With your Arduino Nano connected, select the appropriate board and port in the IDE, then upload the sketch.

### Serial Monitor Connection

1. **Access the Serial Monitor** in the Arduino IDE with the Arduino Nano connected.
2. **Adjust the baud rate** to 115200 to align with the firmware settings.
3. **Control Commands**:
    - `z/x`: Adjust the base left/right.
    - `a/s`: Adjust the shoulder up/down (limited to 60 degrees).
    - `q/w`: Adjust the elbow up/down.
    - `k`: Save current configuration.
    - `1-9`: Recall saved configurations.
    - `n/m`: Begin/End automated playback.
    - `j`: Toggle joystick mode.
    - `0`: Reset to home position.
    - `h`: Display help menu.

### Joystick Mode

In joystick mode, the robot can be controlled using a joystick module:
- **Y-axis**: Controls the shoulder position (0-60 degrees).
- **X-axis**: Controls the base position (inverted left/right).
- **Button**: Toggles the elbow position between two preset angles (e.g., 90 and 180 degrees).

To activate or deactivate joystick mode, use the `j` command in the Serial Monitor.

## Contributing

Contributions are welcome. For feature additions or bug fixes, please fork the repository, make your changes, and submit a pull request.

## License

Licensed under the Apache License 2.0. For more details, see the [LICENSE](LICENSE) file.
