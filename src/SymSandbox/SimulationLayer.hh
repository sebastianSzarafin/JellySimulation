#ifndef SYM_BASE_MYLAYER_HH
#define SYM_BASE_MYLAYER_HH

#include "SymBase.hh"
#include "pch.hh"

#include "DockSpaceLayer.hh"
#include "Model/BezierCube.hh"
#include "Model/SteeringCube.hh"
#include "SimulationData.hh"

using namespace sym_base;

namespace sym
{
  class SimulationLayer : public Layer
  {
   public:
    SimulationLayer()
    {
      BufferLayout layout = { { SharedDataType::Float3, "a_Position" } };

      // jelly
      {
        m_jelly.m_model = std::make_shared<BezierCube>(SimulationData::s_a,
                                                       SimulationData::s_m,
                                                       SimulationData::s_c1,
                                                       SimulationData::s_k);

        m_jelly.m_shader = std::make_shared<Shader>("shaders/point.glsl");

        // points
        {
          auto batch = m_jelly.m_model->get_batch_points();
          auto vertex_buffer =
              std::make_shared<VertexBuffer>(batch.data(), sizeof(batch), sizeof(Vertex), BufferType::DYNAMIC);
          vertex_buffer->set_layout(layout);

          m_jelly.m_points.m_va = std::make_shared<VertexArray>();
          m_jelly.m_points.m_va->add_vertex_buffer(vertex_buffer);
        }

        // springs
        {
          auto batch = m_jelly.m_model->get_batch_springs();
          auto vertex_buffer =
              std::make_shared<VertexBuffer>(batch.data(), sizeof(batch), sizeof(Vertex), BufferType::DYNAMIC);
          vertex_buffer->set_layout(layout);

          m_jelly.m_springs.m_va = std::make_shared<VertexArray>();
          m_jelly.m_springs.m_va->add_vertex_buffer(vertex_buffer);
        }
      }

      // steering cube
      {
        m_steering_cube.m_model = std::make_shared<SteeringCube>(m_jelly.m_model->get_corners());

        auto batch = m_steering_cube.m_model->get_batch();
        auto vertex_buffer =
            std::make_shared<VertexBuffer>(batch.data(), sizeof(batch), sizeof(Vertex), BufferType::DYNAMIC);
        vertex_buffer->set_layout(layout);

        uint32_t indices[] = { 0, 1, 1, 3, 3, 2, 2, 0, 0, 4, 1, 5, 3, 7, 2, 6, 4, 5, 5, 7, 7, 6, 6, 4 };
        auto index_buffer  = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

        m_steering_cube.m_va = std::make_shared<VertexArray>();
        m_steering_cube.m_va->add_vertex_buffer(vertex_buffer);
        m_steering_cube.m_va->set_index_buffer(index_buffer);

        m_steering_cube.m_shader = std::make_shared<Shader>("shaders/point.glsl");
      }
    }
    ~SimulationLayer() = default;

    void update(float dt) override
    {
      auto& camera = SimulationContext::s_camera;

      // jelly
      {
        m_jelly.m_model->update(dt);

        m_jelly.m_shader->bind();
        m_jelly.m_shader->upload_uniform_float3("u_Color", m_jelly.m_color);
        auto mvp = camera->get_projection() * camera->get_view() * m_jelly.get_model_mat();
        m_jelly.m_shader->upload_uniform_mat4("u_MVP", mvp);
        // points
        {
          auto batch = m_jelly.m_model->get_batch_points();
          m_jelly.m_points.m_va->get_vertex_buffer(0)->send_data(0, sizeof(batch), batch.data());
          RenderCommand::set_draw_primitive(DrawPrimitive::POINTS);
          RenderCommand::set_point_size(5);
          Renderer::submit(m_jelly.m_points.m_va);
          m_jelly.m_points.m_va->unbind();
        }
        // springs
        {
          auto batch = m_jelly.m_model->get_batch_springs();
          m_jelly.m_springs.m_va->get_vertex_buffer(0)->send_data(0, sizeof(batch), batch.data());
          RenderCommand::set_draw_primitive(DrawPrimitive::LINES);
          RenderCommand::set_line_width(1);
          Renderer::submit(m_jelly.m_springs.m_va);
          m_jelly.m_springs.m_va->unbind();
        }
      }
      // steering cube
      {
        m_steering_cube.m_shader->bind();
        m_steering_cube.m_shader->upload_uniform_float3("u_Color", m_steering_cube.m_color);
        auto mvp = camera->get_projection() * camera->get_view() * m_steering_cube.get_model_mat();
        m_steering_cube.m_shader->upload_uniform_mat4("u_MVP", mvp);
        auto batch = m_steering_cube.m_model->get_batch();
        m_steering_cube.m_va->get_vertex_buffer(0)->send_data(0, sizeof(batch), batch.data());
        RenderCommand::set_draw_primitive(DrawPrimitive::LINES);
        RenderCommand::set_line_width(2);
        Renderer::submit(m_steering_cube.m_va);
        m_steering_cube.m_va->unbind();
      }
    }

    virtual void imgui_update(float dt)
    {
      // note: this should be in gui layer
      ImGui::Begin(DockWinId::s_settings.c_str());
      {
        ImGui::ColorEdit3("Jelly color", glm::value_ptr(m_jelly.m_color));
      }
      ImGui::End();
    }

   private:
    struct Vertex
    {
      glm::vec3 m_position;
    };

    struct
    {
      std::shared_ptr<BezierCube> m_model;
      glm::vec3 m_color       = { 1, 0, 0 };
      glm::vec3 m_translation = { -SimulationData::s_a / 2, -SimulationData::s_a / 2, -SimulationData::s_a / 2 };
      glm::quat m_rotation    = { 1, 0, 0, 0 };
      float m_scale           = 1.f;

      std::shared_ptr<Shader> m_shader;

      struct
      {
        std::shared_ptr<VertexArray> m_va;
      } m_points;

      struct
      {
        std::shared_ptr<VertexArray> m_va;
      } m_springs;

      glm::mat4 get_model_mat()
      {
        return glm::translate(glm::mat4(1.f), m_translation) * glm::mat4_cast(m_rotation) *
            glm::scale(glm::mat4(1.f), glm::vec3(m_scale));
      }

    } m_jelly;

    struct
    {
      std::shared_ptr<SteeringCube> m_model;
      glm::vec3 m_color       = { 0, 1, 0 };
      glm::vec3 m_translation = { -SimulationData::s_a / 2, -SimulationData::s_a / 2, -SimulationData::s_a / 2 };
      glm::quat m_rotation    = { 1, 0, 0, 0 };
      float m_scale           = 1.f;

      std::shared_ptr<VertexArray> m_va;
      std::shared_ptr<Shader> m_shader;

      glm::mat4 get_model_mat()
      {
        return glm::translate(glm::mat4(1.f), m_translation) * glm::mat4_cast(m_rotation) *
            glm::scale(glm::mat4(1.f), glm::vec3(m_scale));
      }

    } m_steering_cube;
  };
} // namespace sym

#endif // SYM_BASE_MYLAYER_HH
