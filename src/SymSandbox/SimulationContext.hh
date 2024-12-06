#ifndef SIMULATIONAPP_SIMULATIONCONTEXT_HH
#define SIMULATIONAPP_SIMULATIONCONTEXT_HH

#include <mutex>

#include "Model/BezierCube.hh"
#include "Model/SteeringCube.hh"
#include "SimulationData.hh"
#include "SymBase.hh"

using namespace sym_base;

namespace sym
{
  struct SimulationContext
  {
    static void reset_jelly();

    static std::shared_ptr<OrbitCamera> s_camera;
    static std::shared_ptr<BezierCube> s_jelly_cube;
    static std::shared_ptr<SteeringCube> s_steering_cube;

    static std::mutex s_mtx;
  };

  inline std::shared_ptr<OrbitCamera> SimulationContext::s_camera         = nullptr;
  inline std::shared_ptr<BezierCube> SimulationContext::s_jelly_cube      = nullptr;
  inline std::shared_ptr<SteeringCube> SimulationContext::s_steering_cube = nullptr;

  inline std::mutex SimulationContext::s_mtx;

  void SimulationContext::reset_jelly()
  {
    SimulationContext::s_jelly_cube    = std::make_shared<BezierCube>(SimulationData::s_a,
                                                                   SimulationData::s_m,
                                                                   SimulationData::s_c1,
                                                                   SimulationData::s_k);
    SimulationContext::s_steering_cube = std::make_shared<SteeringCube>(SimulationContext::s_jelly_cube->get_corners());
  }
} // namespace sym

#endif // SIMULATIONAPP_SIMULATIONCONTEXT_HH
