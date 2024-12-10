#include "Point.hh"
#include "SimulationData.hh"
#include "Spring.hh"

#define MAX_DEPTH 3

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

  static uint32_t recursion_depth = 0;
  void MassPoint::detect_collision(float dt)
  {
    auto boundary = SimulationData::s_A / 2;
    bool collide  = false;
    if (m_position.x > boundary)
    {
      m_velocity.x *= m_velocity.x > 0 ? -SimulationData::s_gamma : SimulationData::s_gamma;
      collide = true;
    }
    if (m_position.x < -boundary)
    {
      m_velocity.x *= m_velocity.x < 0 ? -SimulationData::s_gamma : SimulationData::s_gamma;
      collide = true;
    }
    if (m_position.y > boundary)
    {
      m_velocity.y *= m_velocity.y > 0 ? -SimulationData::s_gamma : SimulationData::s_gamma;
      collide = true;
    }
    if (m_position.y < -boundary)
    {
      m_velocity.y *= m_velocity.y < 0 ? -SimulationData::s_gamma : SimulationData::s_gamma;
      collide = true;
    }
    if (m_position.z > boundary)
    {
      m_velocity.z *= m_velocity.z > 0 ? -SimulationData::s_gamma : SimulationData::s_gamma;
      collide = true;
    }
    if (m_position.z < -boundary)
    {
      m_velocity.z *= m_velocity.z < 0 ? -SimulationData::s_gamma : SimulationData::s_gamma;
      collide = true;
    }

    recursion_depth++;
    if (collide && recursion_depth <= MAX_DEPTH)
    {
      m_position += m_velocity * dt;
      detect_collision(dt);
    }
    recursion_depth = 0;
  }
} // namespace sym