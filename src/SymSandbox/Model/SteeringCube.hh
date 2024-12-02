#ifndef SIMULATIONAPP_STEERINGCUBE_HH
#define SIMULATIONAPP_STEERINGCUBE_HH

#include <memory>

#include "Spring.hh"

namespace sym
{
  class SteeringCube
  {
   public:
    SteeringCube(const std::vector<MassPoint*>& points);

    auto& get_batch() { return m_batch.m_points; }

   private:
    void connect_edges(const std::vector<MassPoint*>& points);
    void create_spring(Point* p1, Point* p2, uint32_t idx);
    void prepare_batch();

   private:
    std::array<std::shared_ptr<Point>, 8> m_points;
    std::array<std::shared_ptr<Spring>, 8> m_springs;

    struct
    {
      std::array<glm::vec3, 8> m_points{};
    } m_batch;
  };
} // namespace sym

#endif // SIMULATIONAPP_STEERINGCUBE_HH
