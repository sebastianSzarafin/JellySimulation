#include "BezierCube.hh"

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

  glm::vec3 Spring::compute_force(Spring::Orientation orientation) const
  {
    auto dir  = m_p2->m_position - m_p1->m_position;
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

  BezierCube::BezierCube(float a, float m, float c, float k)
  {
    init_points(a, m);
    init_springs(a, c, k);

    prepare_batch();
  }

  void BezierCube::create_spring(MassPoint* p1,
                                 MassPoint* p2,
                                 uint32_t& idx,
                                 float l0,
                                 float c,
                                 float k,
                                 Spring::Type type)
  {
    m_springs[idx] = std::make_shared<Spring>(p1, p2, l0, c, k, type);
    p1->m_connections.emplace_back(m_springs[idx].get());
    p2->m_connections.emplace_back(m_springs[idx].get());
    idx++;
  }

  void BezierCube::update(float dt)
  {
    // Test
    static float time = 0, up = 1;
    glm::vec3 dir = glm::normalize(m_points[16]->m_position - m_points[0]->m_position);
    if (time < 1)
    {
      time += dt;
      int a = up > 0 ? 0 : 48, b = a + 16;
      for (int i = a; i < b; i++)
      {
        m_points[i]->m_velocity += up * 2 * dt * dir;
      }
    }
    else if (1 <= time && time < 4) { time += dt; }
    else if (m_points[up > 0 ? 0 : 48]->m_velocity.y > 0)
    {
      int a = up > 0 ? 0 : 48, b = a + 16;
      for (int i = a; i < b; i++)
      {
        m_points[i]->m_velocity -= up * 2 * dt * dir;
      }
    }
    else
    {
      time = 0;
      up *= -1;
    }
    //

    for (auto& m_point : m_points)
    {
      m_point->m_position += m_point->m_velocity * dt;
      m_point->m_velocity += m_point->compute_force() / m_point->m_mass * dt;
    }

    prepare_batch();
  }

  void BezierCube::prepare_batch()
  {
    for (int i = 0; i < m_points.size(); i++)
    {
      m_batch.m_points[i] = m_points[i]->m_position;
    }

    for (int i = 0; i < m_springs.size(); i++)
    {
      if (m_springs[i]->m_type == Spring::Side)
      {
        m_batch.m_springs[i] = { m_springs[i]->m_p1->m_position, m_springs[i]->m_p2->m_position };
      }
    }
  }

  void BezierCube::init_points(float a, float m)
  {
    float side_dist = a / 3;
    uint32_t p_idx  = 0;
    for (int y = 0; y < 4; ++y)
    {
      for (int z = 0; z < 4; ++z)
      {
        for (int x = 0; x < 4; ++x)
        {
          m_points[p_idx++] = std::make_shared<MassPoint>(glm::vec3(x * side_dist, y * side_dist, z * side_dist), m);
        }
      }
    }
  }

  void BezierCube::init_springs(float a, float c, float k)
  {
    float side_dist = a / 3;
    float diag_dist = a * sqrt(2) / 3;
    uint32_t s_idx  = 0;
    uint32_t p_idx  = 0;
    for (int y = 0; y < 4; ++y)
    {
      for (int z = 0; z < 4; ++z)
      {
        for (int x = 0; x < 4; ++x)
        {
          auto p1 = m_points[p_idx].get();
          // side neighbors
          if (x < 3) { create_spring(p1, m_points[p_idx + 1].get(), s_idx, side_dist, c, k, Spring::Side); }
          if (z < 3) { create_spring(p1, m_points[p_idx + 4].get(), s_idx, side_dist, c, k, Spring::Side); }
          if (y < 3) { create_spring(p1, m_points[p_idx + 16].get(), s_idx, side_dist, c, k, Spring::Side); }
          // xz diag neighbors
          if (x < 3 && z < 3)
          {
            create_spring(p1, m_points[p_idx + 5].get(), s_idx, diag_dist, c, k, Spring::Diagonal);
          }
          if (x < 3 && 0 < z)
          {
            create_spring(p1, m_points[p_idx - 3].get(), s_idx, diag_dist, c, k, Spring::Diagonal);
          }
          // xy diag neighbors
          if (x < 3 && y < 3)
          {
            create_spring(p1, m_points[p_idx + 1 + 16].get(), s_idx, diag_dist, c, k, Spring::Diagonal);
          }
          if (x < 3 && 0 < y)
          {
            create_spring(p1, m_points[p_idx + 1 - 16].get(), s_idx, diag_dist, c, k, Spring::Diagonal);
          }
          // yz diag neighbors
          if (z < 3 && y < 3)
          {
            create_spring(p1, m_points[p_idx + 4 + 16].get(), s_idx, diag_dist, c, k, Spring::Diagonal);
          }
          if (z < 3 && 0 < y)
          {
            create_spring(p1, m_points[p_idx + 4 - 16].get(), s_idx, diag_dist, c, k, Spring::Diagonal);
          }
          p_idx++;
        }
      }
    }
  }
} // namespace sym