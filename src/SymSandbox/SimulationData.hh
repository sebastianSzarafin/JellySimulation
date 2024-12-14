#ifndef SIMULATIONAPP_SIMULATIONDATA_HH
#define SIMULATIONAPP_SIMULATIONDATA_HH

namespace sym
{
  struct SimulationData
  {
    static float s_dt;
    // steering cube movement
    static glm::vec3 s_translation;
    static glm::vec3 s_rotation;
    // jelly cube / steering cube
    static const float s_a; // side length
    static float s_m;       // mass
    static float s_c1;      // elasticity coefficient (bezier cube)
    static float s_c2;      // elasticity coefficient (steering cube)
    static float s_k;       // damping coefficient
    // bounding cube
    static const float s_A;
    static float s_gamma; // rebound elasticity
    // display
    static glm::vec3 s_jelly_color;
    static bool s_display_surface;
    static bool s_display_points;
    static bool s_display_springs;
    static bool s_display_rubber_duck;
  };

  inline float SimulationData::s_dt = .005f;

  inline glm::vec3 SimulationData::s_translation = { 0, 0, 0 };
  inline glm::vec3 SimulationData::s_rotation    = { 0, 0, 0 };

  inline const float SimulationData::s_a = 1;
  inline float SimulationData::s_m       = 1;
  inline float SimulationData::s_c1      = 30;
  inline float SimulationData::s_c2      = 25;
  inline float SimulationData::s_k       = 4;

  inline const float SimulationData::s_A = 5;
  inline float SimulationData::s_gamma   = .5f;

  inline glm::vec3 SimulationData::s_jelly_color    = { 1, 0, 0 };
  inline bool SimulationData::s_display_surface     = true;
  inline bool SimulationData::s_display_points      = false;
  inline bool SimulationData::s_display_springs     = false;
  inline bool SimulationData::s_display_rubber_duck = false;
} // namespace sym

#endif // SIMULATIONAPP_SIMULATIONDATA_HH
