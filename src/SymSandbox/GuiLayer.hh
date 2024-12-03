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
        ImGui::Text("FPS: %.1f", m_fps_manager.m_current_fps);
        ImGui::Text("Simulation time: %.1f", Clock::now());
        ImGui::Spacing();

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
          ImGui::DragFloat("##RotationX", &SimulationData::s_rotation.x, .05f, 0.f, 360.f);
          ImGui::TableSetColumnIndex(1);
          ImGui::PushItemWidth(-FLT_MIN);
          ImGui::DragFloat("##RotationY", &SimulationData::s_rotation.y, .05f, 0.f, 360.f);
          ImGui::TableSetColumnIndex(2);
          ImGui::PushItemWidth(-FLT_MIN);
          ImGui::DragFloat("##RotationZ", &SimulationData::s_rotation.z, .05f, 0.f, 360.f);
          ImGui::PopItemWidth();

          ImGui::EndTable();
        }
      }
      ImGui::End();

      //      ImGui::ShowDemoWindow();
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
