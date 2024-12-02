#ifndef SYM_BASE_MYLAYER_HH
#define SYM_BASE_MYLAYER_HH

#include "SymBase.hh"
#include "pch.hh"

#include "BezierCube.hh"
#include "DockSpaceLayer.hh"

using namespace sym_base;

namespace sym
{
  class SimulationLayer : public Layer
  {
   public:
    SimulationLayer()
    {
      // jelly
      {
        m_jelly.m_model = std::make_shared<BezierCube>(m_jelly.a, m_jelly.m, m_jelly.c, m_jelly.k);

        BufferLayout layout = { { SharedDataType::Float3, "a_Position" } };

        // points
        {
          auto batch = m_jelly.m_model->get_batch_points();
          auto vertex_buffer =
              std::make_shared<VertexBuffer>(batch.data(), sizeof(batch), sizeof(Vertex), BufferType::DYNAMIC);
          vertex_buffer->set_layout(layout);

          m_jelly.m_points.m_va = std::make_shared<VertexArray>();
          m_jelly.m_points.m_va->add_vertex_buffer(vertex_buffer);

          m_jelly.m_points.m_shader = std::make_shared<Shader>("shaders/point.glsl");
        }

        // springs
        {
          auto batch = m_jelly.m_model->get_batch_springs();
          auto vertex_buffer =
              std::make_shared<VertexBuffer>(batch.data(), sizeof(batch), sizeof(Vertex), BufferType::DYNAMIC);
          vertex_buffer->set_layout(layout);

          m_jelly.m_springs.m_va = std::make_shared<VertexArray>();
          m_jelly.m_springs.m_va->add_vertex_buffer(vertex_buffer);

          m_jelly.m_springs.m_shader = std::make_shared<Shader>("shaders/point.glsl");
        }
      }
    }
    ~SimulationLayer() = default;

    void update(float dt) override
    {
      auto& camera = SimulationContext::s_camera;

      // jelly
      {
        m_jelly.m_model->update(dt);

        m_jelly.m_points.m_shader->bind();
        m_jelly.m_points.m_shader->upload_uniform_float3("u_Color", m_jelly.m_color);
        auto mvp = camera->get_projection() * camera->get_view() * m_jelly.get_model_mat();
        m_jelly.m_points.m_shader->upload_uniform_mat4("u_MVP", mvp);
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
      float a = 1, m = 1, c = 10, k = 1;
      glm::vec3 m_color       = { 1, 0, 0 };
      glm::vec3 m_translation = { -a / 2, -a / 2, -a / 2 };
      glm::quat m_rotation    = { 1, 0, 0, 0 };
      float m_scale           = 1.f;
      struct
      {
        std::shared_ptr<VertexArray> m_va;
        std::shared_ptr<Shader> m_shader;
      } m_points;

      struct
      {
        std::shared_ptr<VertexArray> m_va;
        std::shared_ptr<Shader> m_shader;
      } m_springs;

      glm::mat4 get_model_mat()
      {
        return glm::translate(glm::mat4(1.f), m_translation) * glm::mat4_cast(m_rotation) *
            glm::scale(glm::mat4(1.f), glm::vec3(m_scale));
      }

    } m_jelly;
  };
} // namespace sym

#endif // SYM_BASE_MYLAYER_HH
