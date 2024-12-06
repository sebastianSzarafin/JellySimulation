#ifndef SYM_BASE_SANDBOXAPP_HH
#define SYM_BASE_SANDBOXAPP_HH

#include "SymBase.hh"

#include "DockSpaceLayer.hh"
#include "FramebufferLayer.hh"
#include "GuiLayer.hh"
#include "InputLayer.hh"
#include "SimulationContext.hh"

using namespace sym_base;

namespace sym
{
  class SandboxApp : public Application
  {
   public:
    SandboxApp(const ApplicationParams& params) : Application(params)
    {
      // setup simulation
      SimulationContext::s_camera = std::make_shared<OrbitCamera>();
      SimulationContext::s_camera->set_position({ 0, 0, 5 });
      SimulationContext::reset_jelly();
      create_simulation(
          SimulationData::s_dt,
          [&]()
          {
            std::lock_guard<std::mutex> lock(SimulationContext::s_mtx);
            SimulationContext::s_jelly_cube->update(SimulationData::s_dt);
            SimulationContext::s_steering_cube->update(SimulationData::s_dt);
          },
          LoopStatus::running);

      // create application layers
      push_layer(new InputLayer());
      push_layer(new GuiLayer());
      push_layer(new FramebufferLayer());
      push_layer(new DockSpaceLayer());
    }

    ~SandboxApp() override {}

    virtual void update(float dt) override
    {
      if (Input::is_key_pressed(GLFW_KEY_ESCAPE)) { m_running = false; }
      Application::update(dt);
    }
  };
} // namespace sym

#endif // SYM_BASE_SANDBOXAPP_HH
