#ifndef SYM_BASE_MYLAYER_HH
#define SYM_BASE_MYLAYER_HH

#include "SymBase.hh"
#include "pch.hh"

#include "DockSpaceLayer.hh"
#include "SimulationContext.hh"
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
        m_jelly.m_shader = std::make_shared<Shader>("shaders/point.glsl");

        // points
        {
          auto batch = SimulationContext::s_jelly_cube->get_batch_points();
          auto vertex_buffer =
              std::make_shared<VertexBuffer>(batch.data(), sizeof(batch), sizeof(Vertex), BufferType::DYNAMIC);
          vertex_buffer->set_layout(layout);

          m_jelly.m_points.m_va = std::make_shared<VertexArray>();
          m_jelly.m_points.m_va->add_vertex_buffer(vertex_buffer);
        }

        // springs
        {
          auto batch = SimulationContext::s_jelly_cube->get_batch_springs();
          auto vertex_buffer =
              std::make_shared<VertexBuffer>(batch.data(), sizeof(batch), sizeof(Vertex), BufferType::DYNAMIC);
          vertex_buffer->set_layout(layout);

          m_jelly.m_springs.m_va = std::make_shared<VertexArray>();
          m_jelly.m_springs.m_va->add_vertex_buffer(vertex_buffer);
        }
      }

      // steering cube
      {
        auto batch = SimulationContext::s_steering_cube->get_batch();
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

      // bezier surface
      {
        auto batch = SimulationContext::s_jelly_cube->get_batch_sides();
        auto vertex_buffer =
            std::make_shared<VertexBuffer>(batch.data(), sizeof(batch), sizeof(Vertex), BufferType::DYNAMIC);
        vertex_buffer->set_layout(layout);

        m_bezier_surface.m_va = std::make_shared<VertexArray>();
        m_bezier_surface.m_va->add_vertex_buffer(vertex_buffer);

        m_bezier_surface.m_shader = std::make_shared<Shader>("shaders/bezier_surface.glsl");
      }

      // bounding cube
      {
        float off          = m_bounding_cube.m_a / 2;
        Vertex vertices[8] = {
          { { -off, -off, off } },  { { off, -off, off } },  { { off, off, off } },  { { -off, off, off } },
          { { -off, -off, -off } }, { { off, -off, -off } }, { { off, off, -off } }, { { -off, off, -off } },
        };
        auto vertex_buffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices), sizeof(Vertex));
        vertex_buffer->set_layout(layout);

        // edges
        {
          uint32_t indices[] = { 0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 3, 7, 2, 6 };
          auto index_buffer  = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

          m_bounding_cube.m_va_edges = std::make_shared<VertexArray>();
          m_bounding_cube.m_va_edges->add_vertex_buffer(vertex_buffer);
          m_bounding_cube.m_va_edges->set_index_buffer(index_buffer);
        }

        // interior
        {
          uint32_t indices[] = { 0, 2, 1, 2, 0, 3, 1, 6, 5, 6, 1, 2, 4, 5, 6, 6, 7, 4,
                                 0, 4, 7, 7, 3, 0, 0, 1, 5, 0, 5, 4, 3, 6, 2, 3, 7, 6 };
          auto index_buffer  = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

          m_bounding_cube.m_va_interior = std::make_shared<VertexArray>();
          m_bounding_cube.m_va_interior->add_vertex_buffer(vertex_buffer);
          m_bounding_cube.m_va_interior->set_index_buffer(index_buffer);
        }

        m_bounding_cube.m_shader = std::make_shared<Shader>("shaders/bounding_cube.glsl");
      }

      // light
      {
        Vertex vertices[8] = {
          { { -1, -1, 1 } },  { { 1, -1, 1 } },  { { 1, 1, 1 } },  { { -1, 1, 1 } },
          { { -1, -1, -1 } }, { { 1, -1, -1 } }, { { 1, 1, -1 } }, { { -1, 1, -1 } },
        };
        auto vertex_buffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices), sizeof(Vertex));
        vertex_buffer->set_layout(layout);

        uint32_t indices[] = { 0, 1, 2, 2, 3, 0, 4, 6, 5, 6, 4, 7, 4, 0, 3, 3, 7, 4,
                               1, 5, 6, 6, 2, 1, 3, 2, 6, 6, 7, 3, 4, 5, 1, 1, 0, 4 };
        auto index_buffer  = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

        m_light.m_va = std::make_shared<VertexArray>();
        m_light.m_va->add_vertex_buffer(vertex_buffer);
        m_light.m_va->set_index_buffer(index_buffer);

        m_light.m_shader = std::make_shared<Shader>("shaders/point.glsl");
      }
    }
    ~SimulationLayer() = default;

    void update(float dt) override
    {
      auto& camera = SimulationContext::s_camera;

      // jelly
      {
        m_jelly.m_shader->bind();
        m_jelly.m_shader->upload_uniform_float3("u_Color", m_jelly.m_color);
        auto mvp = camera->get_projection() * camera->get_view();
        m_jelly.m_shader->upload_uniform_mat4("u_MVP", mvp);
        // points
        {
          auto batch = SimulationContext::s_jelly_cube->get_batch_points();
          m_jelly.m_points.m_va->get_vertex_buffer(0)->send_data(0, sizeof(batch), batch.data());
          RenderCommand::set_draw_primitive(DrawPrimitive::POINTS);
          RenderCommand::set_point_size(5);
          Renderer::submit(m_jelly.m_points.m_va);
          m_jelly.m_points.m_va->unbind();
        }
        // springs
        {
          auto batch = SimulationContext::s_jelly_cube->get_batch_springs();
          m_jelly.m_springs.m_va->get_vertex_buffer(0)->send_data(0, sizeof(batch), batch.data());
          RenderCommand::set_draw_primitive(DrawPrimitive::LINES);
          RenderCommand::set_line_width(1);
          Renderer::submit(m_jelly.m_springs.m_va);
          m_jelly.m_springs.m_va->unbind();
        }
      }
      // steering cube
      {
        SimulationContext::s_steering_cube->update(dt);

        m_steering_cube.m_shader->bind();
        m_steering_cube.m_shader->upload_uniform_float3("u_Color", m_steering_cube.m_color);
        auto mvp = camera->get_projection() * camera->get_view() * m_steering_cube.m_model;
        m_steering_cube.m_shader->upload_uniform_mat4("u_MVP", mvp);
        auto batch = SimulationContext::s_steering_cube->get_batch();
        m_steering_cube.m_va->get_vertex_buffer(0)->send_data(0, sizeof(batch), batch.data());
        RenderCommand::set_draw_primitive(DrawPrimitive::LINES);
        RenderCommand::set_line_width(2);
        Renderer::submit(m_steering_cube.m_va);
        m_steering_cube.m_va->unbind();
      }
      // bezier surface
      {
        m_bezier_surface.m_shader->bind();
        auto vp = camera->get_projection() * camera->get_view();
        m_bezier_surface.m_shader->upload_uniform_int("u_Tess.u", m_bezier_surface.m_tess_uv);
        m_bezier_surface.m_shader->upload_uniform_int("u_Tess.v", m_bezier_surface.m_tess_uv);
        m_bezier_surface.m_shader->upload_uniform_mat4("u_VP", vp);
        m_bezier_surface.m_shader->upload_uniform_mat4("u_M", glm::mat4(1));
        m_bezier_surface.m_shader->upload_uniform_float3("u_CameraPos", camera->get_position());
        m_bezier_surface.m_shader->upload_uniform_float3("u_Light.pos", m_light.m_position);
        m_bezier_surface.m_shader->upload_uniform_float3("u_Light.color", m_light.m_color);
        m_bezier_surface.m_shader->upload_uniform_float3("u_FragColor", m_bezier_surface.m_color);
        auto batch = SimulationContext::s_jelly_cube->get_batch_sides();
        m_bezier_surface.m_va->get_vertex_buffer(0)->send_data(0, sizeof(batch), batch.data());
        RenderCommand::set_draw_primitive(DrawPrimitive::PATCHES);
        RenderCommand::set_patch_size(m_bezier_surface.m_patch_size);
        Renderer::submit(m_bezier_surface.m_va);
        m_bezier_surface.m_va->unbind();
      }
      // bounding cube
      {
        m_bounding_cube.m_shader->bind();
        m_bounding_cube.m_shader->upload_uniform_float3("u_Color", m_bounding_cube.m_color);
        auto mvp = camera->get_projection() * camera->get_view() * m_bounding_cube.m_model;
        m_bounding_cube.m_shader->upload_uniform_mat4("u_MVP", mvp);
        // edges
        {
          m_bounding_cube.m_shader->upload_uniform_float("u_Alpha", 1);
          RenderCommand::set_draw_primitive(DrawPrimitive::LINES);
          RenderCommand::set_line_width(2);
          Renderer::submit(m_bounding_cube.m_va_edges);
          m_bounding_cube.m_va_edges->unbind();
        }
        // interior
        {
          m_bounding_cube.m_shader->upload_uniform_float("u_Alpha", .1f);
          RenderCommand::set_draw_primitive(DrawPrimitive::TRIANGLES);
          Renderer::submit(m_bounding_cube.m_va_interior);
          m_bounding_cube.m_va_interior->unbind();
        }
      }
      // light
      {
        m_light.m_shader->bind();
        m_light.m_shader->upload_uniform_float3("u_Color", m_light.m_color);
        auto mvp = camera->get_projection() * camera->get_view() * m_light.m_model;
        m_light.m_shader->upload_uniform_mat4("u_MVP", mvp);
        RenderCommand::set_draw_primitive(DrawPrimitive::TRIANGLES);
        Renderer::submit(m_light.m_va);
        m_light.m_va->unbind();
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
      glm::vec3 m_color = { 1, 0, 0 };
      std::shared_ptr<Shader> m_shader;

      struct
      {
        std::shared_ptr<VertexArray> m_va;
      } m_points;

      struct
      {
        std::shared_ptr<VertexArray> m_va;
      } m_springs;

    } m_jelly;

    struct
    {
      glm::vec3 m_color = { 0, 1, 0 };
      std::shared_ptr<VertexArray> m_va;
      std::shared_ptr<Shader> m_shader;
      const glm::mat4 m_model = glm::scale(glm::mat4(1), glm::vec3(1.01f));
    } m_steering_cube;

    struct
    {
      glm::vec3 m_color           = { 1, 0, 0 };
      const uint32_t m_patch_size = 16;
      const uint32_t m_tess_uv    = 16;
      std::shared_ptr<VertexArray> m_va;
      std::shared_ptr<Shader> m_shader;
    } m_bezier_surface;

    struct
    {
      float m_a         = SimulationData::s_A;
      glm::vec3 m_color = { .75f, .75f, .75f };
      std::shared_ptr<VertexArray> m_va_edges;
      std::shared_ptr<VertexArray> m_va_interior;
      std::shared_ptr<Shader> m_shader;
      const glm::mat4 m_model = glm::mat4(1);
    } m_bounding_cube;

    struct
    {
      const float m_size      = .1f;
      const glm::vec3 m_color = { 1, 1, 1 };
      const glm::vec3 m_position =
          glm::vec3(SimulationData::s_A / 2, SimulationData::s_A / 2, -SimulationData::s_A / 2);
      std::shared_ptr<VertexArray> m_va;
      std::shared_ptr<Shader> m_shader;
      const glm::mat4 m_model = glm::translate(glm::mat4(1), m_position + glm::vec3(-m_size, -m_size, m_size)) *
          glm::scale(glm::mat4(1), glm::vec3(m_size));
    } m_light;
  };
} // namespace sym

#endif // SYM_BASE_MYLAYER_HH
