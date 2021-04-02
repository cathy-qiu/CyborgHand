// constants used for touch sensors
const int CLICK_TIME = 1000;
// constants used for gyro sensor
// both are time in ms
const int SURVEY_FREQ = 500;
const int MAX_WAIT_TIME = 120000;
// sensor functions
void configureSensors();
bool updateGyroValue(int &previousValue, int &timesSettled, bool &exit);
bool checkTouchValues(tSensors &touch, int &presses);
// finger motions
void resetFingers();
void powerGrab();
void pinch(float target, const float kp, const float kd, tMotor motorType);
void rockPaperScissors();
void weird();
void computer();
void rpsSequence();
void fist();
void helperPaper();
int obtainMinutes(int time_s);
int obtainSeconds(int time_s);

// other functions
void performAction(tSensors touch, int presses, int &settlement);
void beep(int freq);
void configureSensors() {
    displayBigTextLine(0, "Starting...");
    // set up sensor types
    SensorType[S1] = sensorEV3_Touch;
    SensorType[S2] = sensorEV3_Touch;
    SensorType[S3] = sensorEV3_Gyro;
    wait1Msec(100);
    // set gyro mode
    SensorMode[S3] = modeEV3Gyro_RateAndAngle;
    wait1Msec(100);
    setSoundVolume(100);
playSoundFile("Start up");
sleep(2000);
displayBigTextLine(0, "Running...");
}

bool updateGyroValue(int &previousValue, int &timesSettled, bool &exit) {
    int currentValue = getGyroDegrees(S3); 
    if (time1[T1] > SURVEY_FREQ) { 
        if (previousValue == currentValue) { 
            timesSettled += time1[T1]; 
            if (timesSettled >= MAX_WAIT_TIME) { 
                exit = true; 
                return false; 
            } 
        } 
        else 
            timesSettled = 0; 
            clearTimer(T1); 
            previousValue = currentValue; 
    } 
    int deltaT = (MAX_WAIT_TIME - timesSettled)/1000; 
    displayString(3, "%dm %ds till shutting down", obtainMinutes(deltaT),  
    obtainSeconds(deltaT)); 
    return true; 
} 

bool checkTouchValues(tSensors &touch, int &presses) { 
    // no touch sensors are pressed 
    if (SensorValue[S1] != 1 && SensorValue[S2] != 1) 
        return true; 
    else { 
        if (SensorValue[S1] == 1) 
        touch = S1 ; 
        else 
            touch = S2; 
            clearTimer(T4); 
            resetBumpedValue(touch);
            
        while(time1[T4] < CLICK_TIME) {} 
        presses = getBumpedValue(touch); 
        if (touch == S1) 
            displayString(9, "S1 Press: %d", presses); 
        else 
            displayString(9, "S2 Press: %d", presses); 
        return false; 
    } 
} 

void performAction(tSensors touch, int &presses, int &settlement) { 
    beep(784); 
    if (touch == S1) { 
        if (presses == 1) { 
            displayBigTextLine(8, "Action: Power"); 
            powerGrab(); 
        } 
        else if (presses == 2) { 
            displayBigTextLine(8, "Action: Fist"); 
            fist(); 
            while(SensorValue[S1] != 1) {}
        } 
        else if (presses == 3) { 
            displayBigTextLine(8, "Action: RPS"); 
            rpsSequence(); 
            wait1Msec(500); 
            rockPaperScissors(); 
        } 
        // protection against double click will just loop resetFingers() again 
    }
    else { 
        if (presses == 1) { 
            displayBigTextLine(8, "Action: Pinch"); 
            pinch(64, 0.5, 0.2, motorA); 
        } 
        else if (presses == 2) { 
            displayBigTextLine(8, "Action: Typing"); 
            computer(); 
        } 
        else { 
            displayBigTextLine(8, "Action: Weird");
            weird(); 
        } 
    } 
    presses = 0; 
    settlement = 0; 
} 

void resetFingers() { 
    // move thumb out of the way 
    motor[motorD] = -20; 
    motor[motorA] = motor[motorB] = motor[motorC] = -20; 
    wait1Msec(1500); 
    // close the three fingers in all the way 
    motor[motorA] = motor[motorB] = motor[motorC] = 40; 
    wait1Msec(1000); 
    motor[motorA] = motor[motorB] = motor[motorC] = 0; 
    // reset encoder, stretch the fingers out and reset encoder again 
    nMotorEncoder[motorA] = nMotorEncoder[motorB] = nMotorEncoder[motorC] = 0; 
    motor[motorA] = -20; 
    while(nMotorEncoder[motorA] > -120) {} 
    motor[motorA] = 0; 
    motor[motorB] = -20; 
    while(nMotorEncoder[motorB] > -120) {} 
    motor[motorB] = 0; 
    motor[motorC] = -20; 
    while(nMotorEncoder[motorC] > -120) {} 
    motor[motorC] = 0; 
    motor[motorA] = motor[motorB] = motor[motorC] = 0; 
    nMotorEncoder[motorA] = nMotorEncoder[motorB] = nMotorEncoder[motorC] = 0; wait1Msec(200); 
    // same procedure with the thumb 
    motor[motorD] = 30; 
    wait1Msec(1000); 
    nMotorEncoder[motorD] = 0; 
    motor[motorD] = -10; 
    while(nMotorEncoder[motorD] > -45) {}  
    //-35 works for the red shorter finger length pieces. 
    motor[motorD] = 0; 
    nMotorEncoder[motorD] = 0; 
} 

void powerGrab() { 
    // palm opens up 
    motor[motorA] = motor[motorB] = motor[motorC] = -20;
    motor[motorD] = -20; 
    wait1Msec(1000); 
    // grabs the object 
    motor[motorA] = motor[motorB] = motor[motorC] = 20; 
    motor[motorD] = 15; 
    wait1Msec(1000); 
    motor[motorA] = motor[motorB] = motor[motorC] = motor[motorD] = 0; 
    while(SensorValue[S1] == 0){} 
    motor[motorA] = motor[motorB] = motor[motorC] = motor[motorD] = 0; 
} 

void weird() { 
    motor[motorB] = motor[motorC] = 30; 
    wait1Msec(500); 
    motor[motorD] = 15; 
    wait1Msec(200); 
    int motor_power = 10, max = 10; 
    while(SensorValue[S2] == 0){ 
        int index = 0; 
        while (SensorValue[S2] == 0 && index < max) { 
            motor[motorA] = motor_power; 
            wait1Msec(40); 
            index++; 
        }
        motor_power *= -1; 
    }
    motor[motorA] = 0; 
} 

void pinch(float target, const float kp, const float kd, tMotor motorType) { // const int angleTargetPointerFinger = 70; 
    // et point: angleNeededToReachForpinchOfIndexFinger 
    // const int kp = 100000, kd = 100000; 
    SensorType[S1] = sensorEV3_Touch; 
    float error = 100, tempError = 0, derivativeError = 0, proportionalOutput = 0, total =  0, derivativeOutput = 0; 
    nMotorEncoder[motorType] = 0; 
    setMotorBrakeMode(motorD, motorBrake); 
    setMotorSpeed(motorD, 0); 
    while(error < -3 || error > 3) {
        tempError = error; 
        error = (target - (nMotorEncoder[motorType])); 
        // motorEncoder is realtime position - the process variable 
        derivativeError = (error - tempError); 
        proportionalOutput = (kp * error); 
        derivativeOutput = -(kd * derivativeError); 
        /* 
        shouldn't normally have a huge derivativeError value, but if it does... 
        meaning a sudden large change compared to the last 
        it will correct the motors and force more speed onto the motor 
        control variable to meet the set point requirement. 
        */ 
        // no overshoot when going to set point with PD control 
        total = proportionalOutput + derivativeOutput; 
        if(total > 100){ 
        total = 100; 
        } 
        else if(total < 0){ 
        total = 0; 
        } 
        // in case constants take the motor value higher than max 100. 
        // once error is 0, motor will stop, 0 + 0 = 0 
        motor[motorType] = total; //motor is control variable 
    } 
    motor[motorType] = 5; 
    while(SensorValue[S2] == 0){ 
    } 
    motor[motorType] = 0; 
} 

void fist(){ 
    motor[motorD] = -20; 
    wait1Msec(300); 
    motor[motorD] = 0; 
    motor[motorA] = motor[motorB] = motor[motorC] = 30; 
    wait1Msec(600); 
    motor[motorA] = motor[motorB] = motor[motorC] = 0; 
    motor[motorD] = 30; 
    wait1Msec(300); 
    motor[motorD] = 0; 
    beep(784);
    
} 

// helper functions for Rock Paper Scissors 
void helperPaper(){ 
    motor[motorD] = -30; 
    wait1Msec(500); 
    motor[motorD] = 0; 
    wait1Msec(200); 
    motor[motorA] = motor[motorB] = motor[motorC] = -25; 
    wait1Msec(550); 
    motor[motorA] = motor[motorB] = motor[motorC] = 0; 
    motor[motorD] = 30; 
    wait1Msec(1000); 
    motor[motorD] = 0; 
    beep(1000); 
} 

void Scissors(){ 
    motor[motorC] = 30; 
    wait1Msec(1000); 
    motor[motorC] = 0; 
    beep(1200); 
} 

void rpsSequence(){ 
    wait1Msec(1000); 
    fist(); 
    wait1Msec(200); 
    helperPaper(); 
    wait1Msec(400); 
    Scissors(); 
    wait1Msec(500); 
} 

void rockPaperScissors(){ 
    int randomNum = random(2); 
    wait1Msec(500); 
    if (randomNum == 0){ 
        fist(); 
        beep(900); 
    } 
    //some movement from rpsSequence to show that scissors was chosen 
    else if(randomNum == 1){ 
        motor[motorC] = -30; 
        wait1Msec(500); 
        motor[motorC] = 30; 
        wait1Msec(500); 
        motor[motorC] = 0; 
        beep(784); 
        beep(900); 
    } 
    else if(randomNum == 2){ 
        nMotorEncoder[motorC] = 0;
        motor[motorC] = -25; 
        while(nMotorEncoder[motorC] > -127){} 
        motor[motorC] = 0; 
        beep(784); 
        beep(900); 
    } 
    wait1Msec(2000); 
} 

void computer(){ 
    motor[motorC] = motor[motorB] = 20; 
    wait1Msec(1500); 
    motor[motorD] = 20; 
    wait1Msec(500); 
    motor[motorA] = 5; 
    wait1Msec(1000); 
    while(SensorValue[S2] == 0){} 
    motor[motorA] = motor[motorB] = motor[motorC] = motor[motorD] = 0; 
} 

void beep(int freq) { 
    playTone(freq, 15); 
} 

int obtainMinutes(int time_s) { 
    return time_s/60; 
} 

int obtainSeconds(int time_s) { 
    return time_s%60; 
} 

task main(){ 
    configureSensors(); 
    resetFingers(); 
    bool exit = false; 
    int previousGyroValue = 0; 
    int timesSettled = 0;
    while (!exit) { 
        tSensors sensorNum = S1; 
        int presses = 0; 
        while(checkTouchValues(sensorNum, presses) && updateGyroValue(previousGyroValue,  timesSettled, exit)) {} 
        if (!exit) { 
            performAction(sensorNum, presses, timesSettled); 
            wait1Msec(500); 
            resetFingers(); 
        } 
        timesSettled = 0; 
        clearTimer(T1); 
        displayBigTextLine(8, "Action: None"); 
    } 
    playSoundFile("Power down"); 
    sleep(2000); 
} 