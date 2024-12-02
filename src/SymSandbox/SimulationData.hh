#ifndef SIMULATIONAPP_SIMULATIONDATA_HH
#define SIMULATIONAPP_SIMULATIONDATA_HH

namespace sym
{
  struct SimulationData
  {
    // Bezier Cube
    static float s_a;  // side length
    static float s_m;  // mass
    static float s_c1; // elasticity coefficient
    static float s_k;  // damping coefficient
  };

} // namespace sym

#endif // SIMULATIONAPP_SIMULATIONDATA_HH
