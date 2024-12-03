#ifndef SIMULATIONAPP_POINT_HH
#define SIMULATIONAPP_POINT_HH

#include <glm/detail/type_quat.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

namespace sym
{
  struct Spring;

  struct Point
  {
    Point(glm::vec3 pos) : m_position{ pos }, m_velocity{ 0, 0, 0 } {}
    Point(const Point& other) = default;

    inline virtual glm::vec3 get_position() const { return m_position; }
    inline virtual void move(glm::vec3 vec) { m_position += vec; }

    glm::vec3 m_velocity;
    std::vector<Spring*> m_connections{};

   protected:
    glm::vec3 m_position;
  };

  struct MassPoint : public Point
  {
    MassPoint(glm::vec3 pos, float m) : Point(pos), m_mass{ m } {}
    MassPoint(const MassPoint& other) = default;

    glm::vec3 compute_force();

    float m_mass;
  };

  struct SteeringPoint : public Point
  {
    SteeringPoint(glm::vec3 pos) : Point(pos), m_translation{ 0, 0, 0 }, m_rotation{ 1, 0, 0, 0 } {}

    inline virtual glm::vec3 get_position() const override { return get_model_mat() * glm::vec4(m_position, 1.f); }
    inline virtual void move(glm::vec3 vec) override { m_translation += vec; }

    inline void set_translation(glm::vec3 vec) { m_translation = vec; }
    inline void set_rotation(glm::vec3 vec)
    {
      auto qx    = glm::angleAxis(vec.x, glm::vec3(1, 0, 0));
      auto qy    = glm::angleAxis(vec.y, glm::vec3(0, 1, 0));
      auto qz    = glm::angleAxis(vec.z, glm::vec3(0, 0, 1));
      m_rotation = qz * qy * qx;
    }

   private:
    inline glm::mat4 get_model_mat() const
    {
      return glm::translate(glm::mat4(1.f), m_translation) * glm::mat4_cast(m_rotation);
    }

   private:
    // TODO: add model mat placeholder
    glm::vec3 m_translation;
    glm::quat m_rotation;
  };
} // namespace sym

#endif // SIMULATIONAPP_POINT_HH
