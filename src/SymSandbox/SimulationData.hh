#ifndef SIMULATIONAPP_SIMULATIONDATA_HH
#define SIMULATIONAPP_SIMULATIONDATA_HH

namespace sym
{
  struct SimulationData
  {
    // steering cube movement
    static glm::vec3 s_translation;
    static glm::vec3 s_rotation;
    // jelly cube / steering cube
    static float s_a;  // side length
    static float s_m;  // mass
    static float s_c1; // elasticity coefficient (bezier cube)
    static float s_c2; // elasticity coefficient (steering cube)
    static float s_k;  // damping coefficient
    // bounding cube
    static const float s_A;
    static float s_gamma;
  };

  inline glm::vec3 SimulationData::s_translation = { 0, 0, 0 };
  inline glm::vec3 SimulationData::s_rotation    = { 0, 0, 0 };

  inline float SimulationData::s_a  = 1;
  inline float SimulationData::s_m  = 1;
  inline float SimulationData::s_c1 = 30;
  inline float SimulationData::s_c2 = 25;
  inline float SimulationData::s_k  = 4;

  inline const float SimulationData::s_A = 5;
  inline float SimulationData::s_gamma   = .5f;

} // namespace sym

#endif // SIMULATIONAPP_SIMULATIONDATA_HH
