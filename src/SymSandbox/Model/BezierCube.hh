#ifndef SIMULATIONAPP_BEZIERCUBE_HH
#define SIMULATIONAPP_BEZIERCUBE_HH

#include <memory>

#include "Spring.hh"

namespace sym
{
  class BezierCube
  {
   public:
    BezierCube(float a, float m, float c, float k);

    void update(float dt);

    auto& get_batch_points() { return m_batch.m_points; }
    auto& get_batch_springs() { return m_batch.m_springs; }
    std::vector<MassPoint*> get_corners() const;

   private:
    void create_spring(MassPoint* p1, MassPoint* p2, uint32_t& idx, float l0, float c, float k, Spring::Type type);
    void prepare_batch();

    void init_points(float a, float m);
    void init_springs(float a, float c, float k);

   private:
    std::array<std::shared_ptr<MassPoint>, 64> m_points;
    std::array<std::shared_ptr<Spring>, 360> m_springs;

    struct
    {
      std::array<glm::vec3, 64> m_points{};
      std::array<std::pair<glm::vec3, glm::vec3>, 360> m_springs{};
    } m_batch;
  };
} // namespace sym

#endif // SIMULATIONAPP_BEZIERCUBE_HH
