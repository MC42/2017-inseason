#include "RobotInfo.h"
#include "Robot.h"

#include "WPILib.h"

#include "lib/GreyCompressor.h"
#include "lib/logging/LogSpreadsheet.h"
#include "lib/WrapDash.h"

#include "subsystems/BallIntake.h"
#include "subsystems/Shooter.h"
#include "subsystems/Drive.h"
#include "subsystems/Hanger.h"
#include "subsystems/Turret.h"

#include "CANTalon.h"

using namespace frc;

namespace frc973 {

Robot::Robot(void
	) :
	CoopMTRobot(),
	JoystickObserver(),
	m_logger(nullptr),
	m_pdp(new PowerDistributionPanel()),
	m_driverJoystick(nullptr),
	m_operatorJoystick(nullptr),
	m_tuningJoystick(nullptr),
	m_leftDriveTalon(nullptr),
	m_rightDriveTalon(nullptr),
	m_drive(nullptr),
	m_shooter(nullptr),
	m_hanger(nullptr),
	m_ballIntake(nullptr),
	m_turret(nullptr),
	m_autoState(0),
	m_autoTimer(0),
	m_battery(nullptr),
	m_time(nullptr),
	m_state(nullptr),
	m_messages(nullptr),
	m_buttonPresses(nullptr)
{
	m_driverJoystick = new ObservableJoystick(DRIVER_JOYSTICK_PORT, this, this);
	m_operatorJoystick = new ObservableJoystick(OPERATOR_JOYSTICK_PORT, this, this);
	m_tuningJoystick = new ObservableJoystick(2, this, this);
	fprintf(stderr, "Joystick Initialized...\n");

	m_leftDriveTalon = new frc::Talon(DRIVE_LEFT_PWM);
	m_rightDriveTalon = new frc::Talon(DRIVE_RIGHT_PWM);
	fprintf(stderr, "Initialized drive victors\n");

	m_turretMotor = new CANTalon(SHOOTER_TURRET_CAN_ID);

	m_logger = new LogSpreadsheet(this);
	m_drive = new Drive(this, m_leftDriveTalon, m_rightDriveTalon,
			nullptr, nullptr, nullptr, m_logger);

	m_ballIntake = new BallIntake(this);

	m_battery = new LogCell("Battery voltage");

	m_time = new LogCell("Time (ms)");
	m_state = new LogCell("Game State");
	m_messages = new LogCell("Robot messages", 100, true);
	m_buttonPresses = new LogCell("Button Presses (disabled only)", 100, true);

	m_logger->RegisterCell(m_battery);
	m_logger->RegisterCell(m_time);
	m_logger->RegisterCell(m_buttonPresses);

	m_shooter = new Shooter(this, m_logger);
	m_hanger = new Hanger(this);
	m_turret = new Turret(this, m_logger);

}

Robot::~Robot(void) {
}

void Robot::Initialize(void) {
	m_logger->InitializeTable();
}

void Robot::AllStateContinuous(void) {
	m_battery->LogPrintf("%f", DriverStation::GetInstance().GetBatteryVoltage());
	m_time->LogDouble(GetSecTime());
	m_state->LogPrintf("%s", GetRobotModeString());
}
void Robot::ObserveJoystickStateChange(uint32_t port, uint32_t button,
			bool pressedP) {
	fprintf(stderr, "joystick state change port %d button %d state %d\n", port, button, pressedP);
	if (this->IsOperatorControl()){
		HandleTeleopButton(port, button, pressedP);
	}
	else if (this->IsDisabled()){
		HandleDisabledButton(port, button, pressedP);
	}
}
}

#include "Disabled.h"
#include "Autonomous.h"
#include "Teleop.h"
#include "Test.h"

START_ROBOT_CLASS(frc973::Robot);