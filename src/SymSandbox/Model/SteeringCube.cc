#include "SteeringCube.hh"
#include "SimulationData.hh"

namespace sym
{
  SteeringCube::SteeringCube(const std::vector<MassPoint*>& points)
  {
    for (size_t i = 0; i < points.size(); i++)
    {
      m_points[i] = std::make_shared<SteeringPoint>(points[i]->get_position());
    }

    connect_edges(points);
    prepare_batch();
  }

  void SteeringCube::update(float dt)
  {
    for (auto& point : m_points)
    {
      point->set_translation(SimulationData::s_translation);
      point->set_rotation(SimulationData::s_rotation);
    }

    prepare_batch();
  }

  void SteeringCube::connect_edges(const std::vector<MassPoint*>& points)
  {
    for (size_t i = 0; i < points.size(); i++)
    {
      create_spring(m_points[i].get(), points[i], i);
    }
  }

  void SteeringCube::create_spring(Point* p1, Point* p2, uint32_t idx)
  {
    m_springs[idx] =
        std::make_shared<Spring>(p1, p2, 0, SimulationData::s_c2, SimulationData::s_k, Spring::Type::Steering);
    p1->m_connections.emplace_back(m_springs[idx].get());
    p2->m_connections.emplace_back(m_springs[idx].get());
  }
  void SteeringCube::prepare_batch()
  {
    for (int i = 0; i < m_points.size(); i++)
    {
      m_batch.m_points[i] = m_points[i]->get_position();
    }
  }
} // namespace sym