#ifndef SIMULATIONAPP_SPRING_HH
#define SIMULATIONAPP_SPRING_HH

#include "Point.hh"

namespace sym
{
  struct Spring
  {
    enum Orientation
    {
      P1toP2 = -1,
      P2toP1 = 1
    };

    enum Type
    {
      Side,
      Diagonal,
      Steering
    };

    Point* m_p1;
    Point* m_p2;
    float m_l0; // rest length
    float m_c;  // damping coefficient
    float m_k;  // stiffness coefficient
    Type m_type;

    glm::vec3 compute_force(Orientation orientation) const;
  };
} // namespace sym

#endif // SIMULATIONAPP_SPRING_HH
