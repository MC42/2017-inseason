#include "RobotInfo.h"
#include "Robot.h"

#include "WPILib.h"

#include "lib/GreyCompressor.h"
#include "lib/logging/LogSpreadsheet.h"
#include "lib/WrapDash.h"
#include "lib/SingleThreadTaskMgr.h"

#include "subsystems/GearIntake.h"
#include "subsystems/BallIntake.h"
#include "subsystems/Shooter.h"
#include "subsystems/Drive.h"
#include "subsystems/Hanger.h"

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
	m_leftDriveTalonA(nullptr),
	m_leftDriveTalonB(nullptr),
	m_rightDriveTalonA(nullptr),
	m_rightDriveTalonB(nullptr),
	m_drive(nullptr),
	m_hanger(nullptr),
	m_ballIntake(nullptr),
	m_gearIntake(nullptr),
	m_autoRoutine(AutonomousRoutine::NoAuto),
	m_autoDirection(0.0),
	m_autoState(0),
	m_autoTimer(0),
	m_battery(nullptr),
	m_time(nullptr),
	m_state(nullptr),
	m_messages(nullptr),
	m_buttonPresses(nullptr)
{
    SingleThreadTaskMgr *sepTask =
        new SingleThreadTaskMgr(*this, 1.0 / 50.0);
	m_driverJoystick = new ObservableJoystick(DRIVER_JOYSTICK_PORT, this, this);
	m_operatorJoystick = new ObservableJoystick(OPERATOR_JOYSTICK_PORT, this, this);
	m_tuningJoystick = new ObservableJoystick(2, this, this);
	fprintf(stderr, "Joystick Initialized...\n");

	m_leftDriveTalonA = new CANTalon(DRIVE_LEFT_A_CAN);
	m_leftDriveTalonB = new CANTalon(DRIVE_LEFT_B_CAN);
	m_rightDriveTalonA = new CANTalon(DRIVE_RIGHT_A_CAN);
	m_rightDriveTalonB = new CANTalon(DRIVE_RIGHT_B_CAN);

    m_leftDriveTalonA->SetControlMode(CANSpeedController::ControlMode::kPercentVbus);
    m_leftDriveTalonB->SetControlMode(CANSpeedController::ControlMode::kFollower);
    m_leftDriveTalonB->Set(m_leftDriveTalonA->GetDeviceID());

    m_rightDriveTalonB->SetControlMode(CANSpeedController::ControlMode::kPercentVbus);
    m_rightDriveTalonA->SetControlMode(CANSpeedController::ControlMode::kFollower);
    m_rightDriveTalonA->Set(m_rightDriveTalonB->GetDeviceID());
	fprintf(stderr, "Initialized drive victors\n");

	m_logger = new LogSpreadsheet(this);
	m_drive = new Drive(this,
            m_leftDriveTalonA, m_rightDriveTalonB,
			nullptr, nullptr, nullptr, m_logger);

	m_battery = new LogCell("Battery voltage");

	m_time = new LogCell("Time (ms)");
	m_state = new LogCell("Game State");
	m_messages = new LogCell("Robot messages", 100, true);
	m_buttonPresses = new LogCell("Button Presses (disabled only)", 100, true);

	m_logger->RegisterCell(m_battery);
	m_logger->RegisterCell(m_time);
	m_logger->RegisterCell(m_buttonPresses);

	m_shooter = new Shooter(sepTask, m_logger);
	m_hanger = new Hanger(sepTask);
	m_ballIntake = new BallIntake(sepTask);
	m_gearIntake = new GearIntake(sepTask);

	m_airPressureSwitch = new DigitalInput(AIR_PRESSURE_DIN);
	m_compressorRelay = new Relay(COMPRESSOR_RELAY, Relay::kForwardOnly);
	m_compressor = new GreyCompressor(m_airPressureSwitch, m_compressorRelay, sepTask);
    sepTask->Start();

    fprintf(stderr, "initializing aliance\n");
	if(DriverStation::GetInstance().GetAlliance() == DriverStation::Alliance::kRed){
		m_autoDirection = 1.0;
	}
	else{
		m_autoDirection = -1.0;
	}
    fprintf(stderr, "done w/ constructor\n");
}

Robot::~Robot(void) {
}

void Robot::Initialize(void) {
    printf("gonna initialize logger\n");
	m_logger->InitializeTable();
    printf("initialized\n");
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
