#ifndef SIMULATIONAPP_BEZIERCUBE_HH
#define SIMULATIONAPP_BEZIERCUBE_HH

#include "pch.hh"

namespace sym
{
  struct Spring;

  struct MassPoint
  {
    MassPoint(glm::vec3 pos, float m) : m_position{ pos }, m_velocity{ 0, 0, 0 }, m_mass{ m } {}
    MassPoint(const MassPoint& other) = default;

    glm::vec3 m_position;
    glm::vec3 m_velocity;
    float m_mass;
    std::vector<Spring*> m_connections{};

    glm::vec3 compute_force();
  };

  struct Spring
  {
    enum Orientation
    {
      P1toP2 = -1,
      P2toP1 = 1
    };

    enum Type
    {
      Side,
      Diagonal
    };

    MassPoint* m_p1;
    MassPoint* m_p2;
    float m_l0; // rest length
    float m_c;  // damping coefficient
    float m_k;  // stiffness coefficient
    Type m_type;

    glm::vec3 compute_force(Orientation orientation) const;
  };

  class BezierCube
  {
   public:
    BezierCube(float a, float m, float c, float k);

    void update(float dt);

    auto& get_batch_points() { return m_batch.m_points; }
    auto& get_batch_springs() { return m_batch.m_springs; }

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
