#ifndef SIMULATIONAPP_SIMULATIONCONTEXT_HH
#define SIMULATIONAPP_SIMULATIONCONTEXT_HH

#include "Model/BezierCube.hh"
#include "Model/SteeringCube.hh"
#include "SymBase.hh"

using namespace sym_base;

namespace sym
{
  struct SimulationContext
  {
    static std::shared_ptr<OrbitCamera> s_camera;
    static std::shared_ptr<BezierCube> s_jelly_cube;
    static std::shared_ptr<SteeringCube> s_steering_cube;
  };

  inline std::shared_ptr<OrbitCamera> SimulationContext::s_camera         = nullptr;
  inline std::shared_ptr<BezierCube> SimulationContext::s_jelly_cube      = nullptr;
  inline std::shared_ptr<SteeringCube> SimulationContext::s_steering_cube = nullptr;
} // namespace sym

#endif // SIMULATIONAPP_SIMULATIONCONTEXT_HH
