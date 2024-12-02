#ifndef SIMULATIONAPP_POINT_HH
#define SIMULATIONAPP_POINT_HH

#include <glm/vec3.hpp>
#include <vector>

namespace sym
{
  struct Spring;

  struct Point
  {
    Point(glm::vec3 pos) : m_position{ pos }, m_velocity{ 0, 0, 0 } {}
    Point(const Point& other) = default;

    glm::vec3 m_position;
    glm::vec3 m_velocity;
    std::vector<Spring*> m_connections{};
  };

  struct MassPoint : public Point
  {
    MassPoint(glm::vec3 pos, float m) : Point(pos), m_mass{ m } {}
    MassPoint(const MassPoint& other) = default;

    float m_mass;

    glm::vec3 compute_force();
  };
} // namespace sym

#endif // SIMULATIONAPP_POINT_HH
