#include <QTRSensors.h>

#define NUM_SENSORS   		8
#define TIMEOUT       		2500
#define EMITTER_PIN   		QTR_NO_EMITTER_PIN
#define PWM_A 		  		3
#define PWM_B	      		11
#define DIR_A		  		12
#define DIR_B		  		13
#define CALIBRATION_SPEED	160
#define TURN_SPEED	  		160
#define TURN_SPEED_SLOW     130
#define GENERAL_SPEED		160
#define MAX_PATH_LENGTH		100
#define RATE				115200

QTRSensorsRC qtrrc((unsigned char[]) { 2, 4, 5, 6, 7, 8, 9, 10 }, NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];
unsigned int linePosition;

void setup()
{
	Serial.begin(RATE);
	pinMode(PWM_A, OUTPUT);
	pinMode(PWM_B, OUTPUT);
	pinMode(DIR_A, OUTPUT);
	pinMode(DIR_B, OUTPUT);
	analogWrite(PWM_A, 0);
	analogWrite(PWM_B, 0);
	delay(2000);

	for (int i = 0; i <= 100; i++)
	{
		if (i == 0 || i == 60)
		{
			runEngine(DIR_A, LOW, PWM_A, CALIBRATION_SPEED, DIR_B, HIGH, PWM_B, CALIBRATION_SPEED);
		}
		else if (i == 20 || i == 100)
		{
			runEngine(DIR_A, HIGH, PWM_A, CALIBRATION_SPEED, DIR_B, LOW, PWM_B, CALIBRATION_SPEED);
		}

		qtrrc.calibrate();
	}

	linePosition = qtrrc.readLine(sensorValues);

	while (sensorValues[6] < 200)
	{
		linePosition = qtrrc.readLine(sensorValues);
	}

	analogWrite(PWM_A, TURN_SPEED_SLOW);
	analogWrite(PWM_B, TURN_SPEED_SLOW);

	while (linePosition > 4350)
	{
		linePosition = qtrrc.readLine(sensorValues);
	}

	analogWrite(PWM_B, 0);
	analogWrite(PWM_A, 0);
	delay(1000);
}

void loop()
{
	int pathLength = 0;
	char path[MAX_PATH_LENGTH];
	linePosition = qtrrc.readLine(sensorValues);

	while (1)
	{
		int foundDir[3] = { 0, 0, 0 };
		followLine();
		runEngine(DIR_A, LOW, PWM_A, GENERAL_SPEED, DIR_B, LOW, PWM_B, GENERAL_SPEED);
		delay(25);
		linePosition = qtrrc.readLine(sensorValues);

		if (sensorValues[0] > 200)
		{
			foundDir[2] = 1;
		}

		if (sensorValues[7] > 200)
		{
			foundDir[0] = 1;
		}

		runEngine(DIR_A, LOW, PWM_A, GENERAL_SPEED, DIR_B, LOW, PWM_B, GENERAL_SPEED);
		delay(300);
		linePosition = qtrrc.readLine(sensorValues);
		
		if (
			sensorValues[1] > 200 ||
			sensorValues[2] > 200 ||
			sensorValues[3] > 200 ||
			sensorValues[4] > 200 ||
			sensorValues[5] > 200 ||
			sensorValues[6] > 200
			)
		{
			foundDir[1] = 1;
		}

		if (
			sensorValues[1] > 600 &&
			sensorValues[2] > 600 &&
			sensorValues[3] > 600 &&
			sensorValues[4] > 600 &&
			sensorValues[5] > 600 &&
			sensorValues[6] > 600
			)
		{
			break;
		}

		path[pathLength++] = makeTurn(0, foundDir);
		shorteningPath(path, pathLength);
	}

	while (1)
	{
		analogWrite(PWM_A, 0);
		analogWrite(PWM_B, 0);
		linePosition = qtrrc.readLine(sensorValues);
		
		while (
			sensorValues[1] > 200 &&
			sensorValues[2] > 200 &&
			sensorValues[3] > 200 &&
			sensorValues[4] > 200 &&
			sensorValues[5] > 200 &&
			sensorValues[6] > 200
			)
		{
			linePosition = qtrrc.readLine(sensorValues);
			delay(50);
		}

		while (1)
		{
			linePosition = qtrrc.readLine(sensorValues);

			if (
				sensorValues[2] > 200 ||
				sensorValues[3] > 200 ||
				sensorValues[4] > 200 ||
				sensorValues[5] > 200 &&
				sensorValues[0] < 200 &&
				sensorValues[1] < 200 &&
				sensorValues[6] < 200 &&
				sensorValues[7] < 200
				)
			{
				break;
			}

			delay(50);
		}

		delay(2000);

		for (int i = 0; i < pathLength; i++)
		{
			followLine();
			runEngine(DIR_A, LOW, PWM_A, GENERAL_SPEED, DIR_B, LOW, PWM_B, GENERAL_SPEED);
			delay(300);
			makeTurn(path[i], NULL);
		}

		followLine();
		runEngine(DIR_A, LOW, PWM_A, GENERAL_SPEED, DIR_B, LOW, PWM_B, GENERAL_SPEED);
		delay(300);
	}
}

void followLine()
{
	float lastError = 0;

	while (1)
	{
		linePosition = qtrrc.readLine(sensorValues);

		switch (linePosition)
		{
		default:
			float error = (float)linePosition - 3500;
			float PV = 0.5 * error + 1 * (error - lastError);
			lastError = error;

			if (PV > 55)
			{
				PV = 55;
			}

			if (PV < -55)
			{
				PV = -55;
			}

			runEngine(DIR_A, LOW, PWM_A, GENERAL_SPEED - PV, DIR_B, LOW, PWM_B, GENERAL_SPEED + PV);
			break;
		}

		if (
			sensorValues[1] < 100 &&
			sensorValues[2] < 100 &&
			sensorValues[3] < 100 &&
			sensorValues[4] < 100 &&
			sensorValues[5] < 100 &&
			sensorValues[6] < 100
			)
		{
			break;
		}
		else if (
			sensorValues[0] > 200 ||
			sensorValues[7] > 200
			)
		{
			break;
		}
	}

}

char makeTurn(char dir, int foundDir[])
{
	if (!dir) 
	{
		if (foundDir[0])
		{
			dir = 'L';
		}
		else if (foundDir[1])
		{
			dir = 'S';
		}
		else if (foundDir[2])
		{
			dir = 'R';
		}
		else
		{
			dir = 'B';
		}
	}
	
	switch (dir)
	{
	case 'L':
		turnLeft();
		break;
	case 'R':
		turnRight();
		break;
	case 'B':
		turnRight();
		break;
	case 'S':
		break;
	}
	
	return dir;
}

void turnLeft()
{
	runEngine(DIR_A, HIGH, PWM_A, TURN_SPEED, DIR_B, LOW, PWM_B, TURN_SPEED);
	linePosition = qtrrc.readLine(sensorValues);

	while (sensorValues[6] < 200)
	{
		linePosition = qtrrc.readLine(sensorValues);
	}

	analogWrite(PWM_A, TURN_SPEED_SLOW);
	analogWrite(PWM_B, TURN_SPEED_SLOW);

	while (linePosition > 4350)
	{
		linePosition = qtrrc.readLine(sensorValues);
	}

	analogWrite(PWM_B, 0);
	analogWrite(PWM_A, 0);
}

void turnRight()
{
	runEngine(DIR_A, LOW, PWM_A, TURN_SPEED, DIR_B, HIGH, PWM_B, TURN_SPEED);
	linePosition = qtrrc.readLine(sensorValues);

	while (sensorValues[1] < 200)
	{
		linePosition = qtrrc.readLine(sensorValues);
	}

	analogWrite(PWM_A, TURN_SPEED_SLOW);
	analogWrite(PWM_B, TURN_SPEED_SLOW);

	while (linePosition < 3250)
	{
		linePosition = qtrrc.readLine(sensorValues);
	}

	analogWrite(PWM_A, 0);
	analogWrite(PWM_B, 0);
}

void shorteningPath(char (&path)[MAX_PATH_LENGTH], int &pathLength)
{
	int totalAngle = 0;

	if (pathLength < 3 || path[pathLength - 2] != 'B')
	{
		return;
	}

	for (int i = 1; i <= 3; i++)
	{
		switch (path[pathLength - i])
		{
		case 'R':
			totalAngle += 90;
			break;
		case 'L':
			totalAngle += 270;
			break;
		case 'B':
			totalAngle += 180;
			break;
		}
	}

	switch (totalAngle % 360)
	{
	case 0:
		path[pathLength - 3] = 'S';
		break;
	case 90:
		path[pathLength - 3] = 'R';
		break;
	case 180:
		path[pathLength - 3] = 'B';
		break;
	case 270:
		path[pathLength - 3] = 'L';
		break;
	}

	pathLength -= 2;
}

void runEngine(int dirA, int voltageA, int pwmA, int cycleA, int dirB, int voltageB, int pwmB, int cycleB)
{
	digitalWrite(dirA, voltageA);
	analogWrite(pwmA, cycleA);
	digitalWrite(dirB, voltageB);
	analogWrite(pwmB, cycleB);
}