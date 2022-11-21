// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <frc/TimedRobot.h>
#include <frc/XboxController.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/motorcontrol/Spark.h>
#include <frc/motorcontrol/MotorControllerGroup.h>
#include <frc/Joystick.h>
#include <frc/Compressor.h>
#include <frc/DoubleSolenoid.h>
#include "rev/CANSparkMax.h"

/**
 * This is a demo program showing the use of the DifferentialDrive class.
 * Runs the motors with split arcade steering and an Xbox controller.
 */
class Robot : public frc::TimedRobot {

//Motor controllors for each side and intake
  frc::Spark m_frontLeft{0};
  frc::Spark m_backLeft{1};
  frc::Spark m_frontRight{2};
  frc::Spark m_backRight{3};
  frc::Spark m_intake{4};
  
//NEO
  rev::CANSparkMax m_NEO_1{1, rev::CANSparkMax::MotorType::kBrushless};



//Compressor
  frc::Compressor Compressor{0, frc::PneumaticsModuleType::CTREPCM};
//Left Solenoid
  frc::DoubleSolenoid leftSolenoid{frc::PneumaticsModuleType::CTREPCM, 0, 1};
//Right Solenoid
  frc::DoubleSolenoid rightSolenoid{frc::PneumaticsModuleType::CTREPCM, 2, 3};

//Left and right motor groups
  frc::MotorControllerGroup m_left{m_frontLeft, m_backLeft};
  frc::MotorControllerGroup m_right{m_frontRight, m_backRight};

//Control
  frc::DifferentialDrive m_robotDrive{m_left, m_right};
  frc::XboxController m_XboxController{0};
  frc::Joystick m_stick{1};


 public:
  void RobotInit() override {
    // We need to invert one side of the drivetrain so that positive voltages
    // result in both sides moving forward. Depending on how your robot's
    // gearbox is constructed, you might have to invert the left side instead.
    m_right.SetInverted(true);
    rightSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
    leftSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
    Compressor.EnableDigital();
  }

  void TeleopInit() override {
    
  }

  void TeleopPeriodic() override {
    // Drive with split arcade style
    // That means that the Y axis of the left stick moves forward
    // and backward, and the X of the right stick turns left and right.
    m_robotDrive.ArcadeDrive(-m_XboxController.GetLeftY(),
                             m_XboxController.GetRightX());
                          
    //Intake motor toggling (foward: Right Bumper; backward:Left Bumper)
      if (m_XboxController.GetRightBumperPressed()){
        m_intake.Set(1);
      }else if (m_XboxController.GetRightBumperReleased()){
        m_intake.Set(0);
      }else if (m_XboxController.GetLeftBumperPressed()){
        m_intake.Set(-1);
      }else if (m_XboxController.GetLeftBumperReleased()){
        m_intake.Set(0);
      }  

    //Compressor enabling and disabling
      if (m_XboxController.GetBButtonPressed()){
        Compressor.Disable();
      }else if (m_XboxController.GetYButtonPressed()){
        Compressor.EnableDigital();
      }
    //Solenoid toggling                      
      if (m_XboxController.GetXButtonPressed()) {
        rightSolenoid.Toggle();
        //leftSolenoid.Toggle();
      }

    //NEO motor 1
      if (m_stick.GetTriggerPressed()){
        m_NEO_1.Set(1);
      }
  }
};

#ifndef RUNNING_FRC_TESTS
int main(){
  return frc::StartRobot<Robot>();
}
#endif