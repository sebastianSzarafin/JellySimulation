#include "Spring.hh"
#include "pch.hh"

namespace sym
{
  glm::vec3 Spring::compute_force(Spring::Orientation orientation) const
  {
    auto dir = m_p2->get_position() - m_p1->get_position();
    if (glm::length(dir) < FLT_EPSILON) { return glm::vec3(0); }
    auto dist = glm::length(dir);
    auto l    = dist - m_l0;

    auto vel_relative = m_p2->m_velocity - m_p1->m_velocity;
    auto l_t          = glm::dot(glm::normalize(dir), vel_relative);

    auto f_scalar = -m_k * l_t - m_c * l;
    auto f_vec    = f_scalar * glm::normalize(dir);

    switch (orientation)
    {
    case P1toP2:
      return -f_vec;
    case P2toP1:
    default:
      return f_vec;
    }
  }
} // namespace sym