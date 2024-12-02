#include "Point.hh"
#include "Spring.hh"

namespace sym
{
  glm::vec3 MassPoint::compute_force()
  {
    auto total_force = glm::vec3(0.f);
    for (const auto& spring : m_connections)
    {
      if (this == spring->m_p1) { total_force += spring->compute_force(Spring::P1toP2); }
      else { total_force += spring->compute_force(Spring::P2toP1); }
    }
    return total_force;
  }
} // namespace sym