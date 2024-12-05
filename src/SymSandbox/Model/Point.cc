#include "Point.hh"
#include "SimulationData.hh"
#include "Spring.hh"

#define COLLISION_EPS 1e-3f

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

  void MassPoint::detect_collision()
  {
    auto boundary = SimulationData::s_A / 2;
    auto new_pos  = boundary - COLLISION_EPS;
    if (std::abs(m_position.x) > boundary)
    {
      m_position.x > 0 ? m_position.x = new_pos : m_position.x = -new_pos;
      m_velocity.x *= -SimulationData::s_gamma;
    }
    if (std::abs(m_position.y) > boundary)
    {
      m_position.y > 0 ? m_position.y = new_pos : m_position.y = -new_pos;
      m_velocity.y *= -SimulationData::s_gamma;
    }
    if (std::abs(m_position.z) > boundary)
    {
      m_position.z > 0 ? m_position.z = new_pos : m_position.z = -new_pos;
      m_velocity.z *= -SimulationData::s_gamma;
    }
  }
} // namespace sym