#include "BezierCube.hh"
#include "pch.hh"

namespace sym
{
  static const uint32_t top[16]    = { 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63 };
  static const uint32_t bottom[16] = { 3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12 };
  static const uint32_t left[16]   = { 12, 8, 4, 0, 28, 24, 20, 16, 44, 40, 36, 32, 60, 56, 52, 48 };
  static const uint32_t right[16]  = { 3, 7, 11, 15, 19, 23, 27, 31, 35, 39, 43, 47, 51, 55, 59, 63 };
  static const uint32_t near[16]   = { 15, 14, 13, 12, 31, 30, 29, 28, 47, 46, 45, 44, 63, 62, 61, 60 };
  static const uint32_t far[16]    = { 0, 1, 2, 3, 16, 17, 18, 19, 32, 33, 34, 35, 48, 49, 50, 51 };

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
    // Test 1
    //    static float time = 0, up = 1;
    //    glm::vec3 dir = glm::normalize(m_points[16]->get_position() - m_points[0]->get_position());
    //    if (time < 1)
    //    {
    //      time += dt;
    //      int a = up > 0 ? 0 : 48, b = a + 16;
    //      for (int i = a; i < b; i++)
    //      {
    //        m_points[i]->m_velocity += up * 2 * dt * dir;
    //      }
    //    }
    //    else if (1 <= time && time < 4) { time += dt; }
    //    else if (m_points[up > 0 ? 0 : 48]->m_velocity.y > 0)
    //    {
    //      int a = up > 0 ? 0 : 48, b = a + 16;
    //      for (int i = a; i < b; i++)
    //      {
    //        m_points[i]->m_velocity -= up * 2 * dt * dir;
    //      }
    //    }
    //    else
    //    {
    //      time = 0;
    //      up *= -1;
    //    }
    // Test 2
    //    for (int i = 0; i < 16; i++)
    //    {
    //      m_points[i]->m_velocity += glm::vec3(0, -dt, 0);
    //    }
    //

    for (auto& m_point : m_points)
    {
      m_point->move(m_point->m_velocity * dt);
      m_point->m_velocity += m_point->compute_force() / m_point->m_mass * dt;
    }

    for (auto& m_point : m_points)
    {
      m_point->detect_collision(dt);
    }

    prepare_batch();
  }

  std::vector<MassPoint*> BezierCube::get_corners() const
  {
    return { m_points[0].get(),  m_points[3].get(),  m_points[12].get(), m_points[15].get(),
             m_points[48].get(), m_points[51].get(), m_points[60].get(), m_points[63].get() };
  }

  void BezierCube::prepare_batch()
  {
    for (int i = 0; i < m_points.size(); i++)
    {
      m_batch.m_points[i] = m_points[i]->get_position();
    }

    for (int i = 0; i < m_springs.size(); i++)
    {
      if (m_springs[i]->m_type == Spring::Side)
      {
        m_batch.m_springs[i] = { m_springs[i]->m_p1->get_position(), m_springs[i]->m_p2->get_position() };
      }
    }

    auto idx = 0;
    for (const uint32_t i : top)
    {
      m_batch.m_sides[idx++] = m_points[i]->get_position();
    }
    for (const uint32_t i : bottom)
    {
      m_batch.m_sides[idx++] = m_points[i]->get_position();
    }
    for (const uint32_t i : left)
    {
      m_batch.m_sides[idx++] = m_points[i]->get_position();
    }
    for (const uint32_t i : right)
    {
      m_batch.m_sides[idx++] = m_points[i]->get_position();
    }
    for (const uint32_t i : near)
    {
      m_batch.m_sides[idx++] = m_points[i]->get_position();
    }
    for (const uint32_t i : far)
    {
      m_batch.m_sides[idx++] = m_points[i]->get_position();
    }
  }

  void BezierCube::init_points(float a, float m)
  {
    float side_dist  = a / 3;
    glm::vec3 offset = { -a / 2, -a / 2, -a / 2 };
    uint32_t p_idx   = 0;
    for (int y = 0; y < 4; ++y)
    {
      for (int z = 0; z < 4; ++z)
      {
        for (int x = 0; x < 4; ++x)
        {
          auto pos          = glm::vec3(x * side_dist, y * side_dist, z * side_dist) + offset;
          m_points[p_idx++] = std::make_shared<MassPoint>(pos, m);
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