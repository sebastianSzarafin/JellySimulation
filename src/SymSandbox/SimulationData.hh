#ifndef SIMULATIONAPP_SIMULATIONDATA_HH
#define SIMULATIONAPP_SIMULATIONDATA_HH

namespace sym
{
  struct SimulationData
  {
    static glm::vec3 s_translation;
    static glm::vec3 s_rotation;

    static float s_a;  // side length
    static float s_m;  // mass
    static float s_c1; // elasticity coefficient (bezier cube)
    static float s_c2; // elasticity coefficient (steering cube)
    static float s_k;  // damping coefficient
  };

  inline glm::vec3 SimulationData::s_translation = { 0, 0, 0 };
  inline glm::vec3 SimulationData::s_rotation    = { 0, 0, 0 };

  inline float SimulationData::s_a  = 1;
  inline float SimulationData::s_m  = 1;
  inline float SimulationData::s_c1 = 10;
  inline float SimulationData::s_c2 = 20;
  inline float SimulationData::s_k  = 1;

} // namespace sym

#endif // SIMULATIONAPP_SIMULATIONDATA_HH
