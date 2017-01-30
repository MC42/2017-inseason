/*
 * RobotInfo.h
 *
 *  Created on: Oct 9, 2015
 *      Author: Andrew
 */

#ifndef ROBOTINFO_H_
#define ROBOTINFO_H_

#include "lib/util/Util.h"

namespace frc973 {

//#define PROTO_BOT_PINOUT
//#define PRACTICE_BOT_PINOUT

/**
 * PWM pin-out
 */
constexpr int DRIVE_RIGHT_PWM = 1;
constexpr int DRIVE_LEFT_PWM = 0;

constexpr int SHOOTER_PWM = 9;

constexpr int RIGHT_DRIVE_ENCODER_A_DIN = 12;
constexpr int RIGHT_DRIVE_ENCODER_B_DIN = 13;
constexpr int LEFT_DRIVE_ENCODER_A_DIN = 24;
constexpr int LEFT_DRIVE_ENCODER_B_DIN = 25;

constexpr int GEAR_INTAKE_BANNER_DIN = 18;
/**
 * USB port-out (driver-station)
 */
constexpr int DRIVER_JOYSTICK_PORT = 0;
constexpr int OPERATOR_JOYSTICK_PORT = 1;

//solenoids
constexpr int DRIVE_SHIFT_SOL = 0;
constexpr int GEAR_INTAKE_GRIP = 1;
constexpr int GEAR_INTAKE_POS = 2;

constexpr int POWER_TAKEOFF_SOL_A = 3;
constexpr int POWER_TAKEOFF_SOL_B = 4;
//CANTalon
constexpr int SHOOTER_TURRET_CAN_ID = 2;
constexpr int HANGER_CAN_ID = 4;

constexpr int FLYWHEEL_PRIMARY_CAN_ID = 0;
constexpr int FLYWHEEL_REPLICA_CAN_ID = 1;

constexpr int LEFT_INDEXER_CAN_ID = 2;
constexpr int RIGHT_INDEXER_CAN_ID = 3;

constexpr int LEFT_AGITATOR_CAN_ID = 4;
constexpr int RIGHT_AGITATOR_CAN_ID = 5;

constexpr int BALL_INTAKE_CAN_ID = 6;
constexpr int BALL_INTAKE_B_CAN_ID = 7;
constexpr int BALL_CONVEYOR_CAN_ID = 8;

constexpr int FLYWHEEL_CONTROL_PERIOD_MS = 1;
/**
 * Distance (in inches) of the drive per click of the encoder
 */
constexpr double DRIVE_WHEEL_DIAMETER = 4.0;
constexpr double DRIVE_CLICKS_PER_REVOLUTION = 360.0;
constexpr double DRIVE_GEAR_RATIO = 1.0;
constexpr double DRIVE_DIST_PER_REVOLUTION = DRIVE_WHEEL_DIAMETER *
		Constants::PI;
constexpr double DRIVE_DIST_PER_CLICK = DRIVE_DIST_PER_REVOLUTION *
		DRIVE_CLICKS_PER_REVOLUTION;

constexpr double SHOOTER_CLICKS_PER_REVOLUTION = 360.0;
}

#endif /* SRC_ROBOTINFO_H_ */
