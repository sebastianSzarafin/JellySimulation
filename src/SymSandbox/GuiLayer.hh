#ifndef SYM_BASE_GUILAYER_HH
#define SYM_BASE_GUILAYER_HH

#include "SymBase.hh"

#include "DockSpaceLayer.hh"

using namespace sym_base;

namespace sym
{
  class GuiLayer : public Layer
  {
   public:
    GuiLayer()  = default;
    ~GuiLayer() = default;

    void update(float dt) override
    {
      m_fps_manager.m_frames_count++;
      m_fps_manager.m_current_time += dt;
      if (m_fps_manager.m_current_time >= m_fps_manager.m_display_time)
      {
        m_fps_manager.m_current_fps  = 1.f / dt;
        m_fps_manager.m_current_time = 0;
      }
    }

    void imgui_update(float dt) override
    {
      ImGui::Begin(DockWinId::s_settings.c_str());
      {
        bool simulation_reset = false;

        ImGui::Text("FPS: %.1f", m_fps_manager.m_current_fps);
        ImGui::Text("Simulation time: %.1f", Clock::now());
        ImGui::Spacing();
        // --------------------------------------- PARAMETERS ---------------------------------------
        ImGui::SeparatorText("Parameters");
        /* -------------------- Time step -------------------- */
        {
          static float s_dt = SimulationData::s_dt;
          ImGui::SliderFloat("Time step (dt)", &s_dt, .001f, .2f);
          if (std::fabs(s_dt - SimulationData::s_dt) > FLT_EPSILON && ImGui::IsItemDeactivatedAfterEdit())
          {
            simulation_reset     = true;
            SimulationData::s_dt = s_dt;
          }
          ImGui::Spacing();
        }
        /* -------------------- Mass -------------------- */
        {
          static float s_m = SimulationData::s_m;
          ImGui::SliderFloat("Mass (m)", &s_m, .01f, 100.f, "%.2f");
          if (std::fabs(s_m - SimulationData::s_m) > FLT_EPSILON && ImGui::IsItemDeactivatedAfterEdit())
          {
            simulation_reset    = true;
            SimulationData::s_m = s_m;
          }
          ImGui::Spacing();
        }
        /* -------------------- Elasticity (jelly) -------------------- */
        {
          static float s_c1 = SimulationData::s_c1;
          ImGui::SliderFloat("Elasticity (c1) - jelly", &s_c1, .01f, 100.f, "%.2f");
          if (std::fabs(s_c1 - SimulationData::s_c1) > FLT_EPSILON && ImGui::IsItemDeactivatedAfterEdit())
          {
            simulation_reset     = true;
            SimulationData::s_c1 = s_c1;
          }
          ImGui::Spacing();
        }
        /* -------------------- Elasticity (steering cube) -------------------- */
        {
          static float s_c2 = SimulationData::s_c2;
          ImGui::SliderFloat("Elasticity (c2) - steering cube", &s_c2, .01f, 100.f, "%.2f");
          if (std::fabs(s_c2 - SimulationData::s_c2) > FLT_EPSILON && ImGui::IsItemDeactivatedAfterEdit())
          {
            simulation_reset     = true;
            SimulationData::s_c2 = s_c2;
          }
          ImGui::Spacing();
        }
        /* -------------------- Damping -------------------- */
        {
          static float s_k = SimulationData::s_k;
          ImGui::SliderFloat("Damping (k)", &s_k, 0, 100.f, "%.2f");
          if (std::fabs(s_k - SimulationData::s_k) > FLT_EPSILON && ImGui::IsItemDeactivatedAfterEdit())
          {
            simulation_reset    = true;
            SimulationData::s_k = s_k;
          }
          ImGui::Spacing();
        }
        /* -------------------- Rebound elasticity -------------------- */
        {
          static float s_gamma = SimulationData::s_gamma;
          ImGui::SliderFloat("Rebound elasticity (gamma)", &s_gamma, 0, 1.f, "%.2f");
          if (std::fabs(s_gamma - SimulationData::s_gamma) > FLT_EPSILON && ImGui::IsItemDeactivatedAfterEdit())
          {
            simulation_reset        = true;
            SimulationData::s_gamma = s_gamma;
          }
          ImGui::Spacing();
        }
        //  ------------------------------------ MOVEMENT SECTION ------------------------------------
        ImGui::SeparatorText("Movement");
        if (ImGui::BeginTable("TransformTableRowLabels", 1, 0, ImVec2(100, 0)))
        {
          ImGui::TableNextRow();
          ImGui::TableSetColumnIndex(0);
          ImGui::Dummy(ImVec2(0, 15));
          ImGui::TableNextRow();
          ImGui::TableSetColumnIndex(0);
          ImGui::Text("Translation");
          ImGui::TableNextRow();
          ImGui::TableSetColumnIndex(0);
          ImGui::Dummy(ImVec2(0, 2.5f));
          ImGui::Text("Rotation");
          ImGui::EndTable();
        }
        ImGui::SameLine();
        static ImGuiTableFlags flags = ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersInnerV;
        if (ImGui::BeginTable("TransformTable", 3, flags))
        {
          ImGui::TableSetupColumn("X");
          ImGui::TableSetupColumn("Y");
          ImGui::TableSetupColumn("Z");
          ImGui::TableHeadersRow();

          ImGui::TableNextRow();
          ImGui::TableSetColumnIndex(0);
          ImGui::PushItemWidth(-FLT_MIN);
          ImGui::DragFloat("##TranslationX", &SimulationData::s_translation.x, .05f);
          ImGui::TableSetColumnIndex(1);
          ImGui::PushItemWidth(-FLT_MIN);
          ImGui::DragFloat("##TranslationY", &SimulationData::s_translation.y, .05f);
          ImGui::TableSetColumnIndex(2);
          ImGui::PushItemWidth(-FLT_MIN);
          ImGui::DragFloat("##TranslationZ", &SimulationData::s_translation.z, .05f);
          ImGui::PopItemWidth();

          ImGui::TableNextRow();
          ImGui::TableSetColumnIndex(0);
          ImGui::PushItemWidth(-FLT_MIN);
          ImGui::DragFloat("##RotationX", &SimulationData::s_rotation.x, .01f, -3.14f, 3.14f);
          ImGui::TableSetColumnIndex(1);
          ImGui::PushItemWidth(-FLT_MIN);
          ImGui::DragFloat("##RotationY", &SimulationData::s_rotation.y, .01f, -3.14f, 3.14f);
          ImGui::TableSetColumnIndex(2);
          ImGui::PushItemWidth(-FLT_MIN);
          ImGui::DragFloat("##RotationZ", &SimulationData::s_rotation.z, .01f, -3.14f, 3.14f);
          ImGui::PopItemWidth();

          ImGui::EndTable();
        }
        /* -------------------- Display -------------------- */
        ImGui::SeparatorText("Display");
        ImGui::Begin(DockWinId::s_settings.c_str());
        {
          ImGui::ColorEdit3("Jelly color", glm::value_ptr(SimulationData::s_jelly_color));
          ImGui::End();
        }
        ImGui::Checkbox("Surface", &SimulationData::s_display_surface);
        ImGui::Checkbox("Points", &SimulationData::s_display_points);
        ImGui::Checkbox("Springs", &SimulationData::s_display_springs);
        ImGui::Spacing();

        ImGui::SeparatorText("");
        static bool simulation_paused = false;
        if (ImGui::Button("Reset") || simulation_reset)
        {
          // critical
          {
            std::lock_guard<std::mutex> lock(SimulationContext::s_mtx);
            SimulationContext::reset_jelly();
          }
          SimulationData::s_translation = { 0, 0, 0 };
          SimulationData::s_rotation    = { 0, 0, 0 };
          Application::get().reset_simulation();
          simulation_paused = false;
        }
        ImGui::SameLine();
        if (simulation_paused)
        {
          if (ImGui::Button("Resume"))
          {
            Application::get().resume_simulation();
            simulation_paused = false;
          }
        }
        else if (ImGui::Button("Pause"))
        {
          Application::get().pause_simulation();
          simulation_paused = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Start"))
        {
          // critical
          {
            std::lock_guard<std::mutex> lock(SimulationContext::s_mtx);
            SimulationContext::reset_jelly();
          }
          Application::get().start_simulation();
          simulation_paused = false;
        }
      }
      ImGui::End();
    }

   private:
    struct
    {
      int m_frames_count         = 0;
      float m_current_fps        = 0;
      float m_current_time       = 0;
      const float m_display_time = .25f;
    } m_fps_manager;
  };
} // namespace sym

#endif // SYM_BASE_GUILAYER_HH
