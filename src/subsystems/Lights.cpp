#include "Lights.h"
#include "lib/WrapDash.h"

namespace frc973{
  Lights::Lights(TaskMgr *scheduler) :
    m_scheduler(scheduler),
    m_lightsTimer(0),
    m_flashOrder(0),
    m_lightMode(LightMode::off),
    m_pixyLight(new Solenoid(BOILER_PIXY_LIGHT_SOL))
    {
      m_scheduler->RegisterTask("Lights", this, TASK_PERIODIC);
      m_pixyLight->Set(false);
    }

    Lights::~Lights(){
      m_scheduler->UnregisterTask(this);
    }

    void Lights::EnableLights(){
      m_pixyLight->Set(true);
      m_lightMode = on;
    }

    void Lights::DisableLights(){
      m_pixyLight->Set(false);
      m_lightMode = off;
    }

    void Lights::NotifyFlash(int n){
      m_lightMode = LightMode::blinkingOn;
      m_flashOrder = n;
      m_lightsTimer = GetMsecTime();
    }

    void Lights::TaskPeriodic(RobotMode mode){
      switch(m_lightMode){
        case on:
          m_pixyLight->Set(true);
          break;
        case off:
          m_pixyLight->Set(false);
          break;
        case blinkingOn:
          m_pixyLight->Set(true);
          if(GetMsecTime() - m_lightsTimer >= 250){
            m_lightsTimer = GetMsecTime();
            m_lightMode = LightMode::blinkingOff;
          }
          break;
        case blinkingOff:
          m_pixyLight->Set(false);
          if (GetMsecTime() - m_lightsTimer >= 250){
            m_lightsTimer = GetMsecTime();
            m_flashOrder--;
            if (m_flashOrder > 0) {
              m_lightMode = LightMode::blinkingOn;
            }
            else {
              this->DisableLights();
            }
          }
          break;
      }
    }
}
