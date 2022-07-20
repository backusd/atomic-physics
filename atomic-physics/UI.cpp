#include "UI.h"
#include "HLSLStructures.h"

#include <algorithm>
#include <string>
#include <vector>

UI::UI() noexcept :
	m_io(ImGui::GetIO()),
	m_viewport(),
	m_left(0.0f),
	m_top(0.0f),
	m_height(0.0f),
	m_width(0.0f),
	m_windowOffsetX(0.0f),
	m_windowOffsetY(0.0f)
{
	PROFILE_FUNCTION();
}

void UI::Render(const std::unique_ptr<Renderer>& renderer) noexcept
{
	PROFILE_FUNCTION();

	CreateDockSpaceAndMenuBar();

	// DEMO
	{
		PROFILE_SCOPE("ImGui::ShowDemoWindow()");
		ImGui::ShowDemoWindow();
	}
	{
		PROFILE_SCOPE("ImPlot::ShowDemoWindow()");
		ImPlot::ShowDemoWindow();
	}

	SimulationDetailsWindow();
	LogWindow();
	PerformanceWindow();
	SceneEditWindow(renderer);

	

	m_viewport = CD3D11_VIEWPORT(
		m_left,
		m_top,
		m_width,
		m_height
	);
}

void UI::CreateDockSpaceAndMenuBar() noexcept
{
	PROFILE_FUNCTION();

	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	window_flags |= ImGuiWindowFlags_NoBackground; 

	ImGui::Begin("Main DockSpace", 0, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);



	MenuBar();

	ImGui::End();
}

void UI::MenuBar() noexcept
{
	PROFILE_FUNCTION();

	if (ImGui::BeginMenuBar())
	{
		m_top = ImGui::GetWindowContentRegionMin().y;
		m_windowOffsetY = ImGui::GetWindowPos().y;
		m_windowOffsetX = ImGui::GetWindowPos().x;

		//ImVec2 regionMin = ImGui::GetWindowContentRegionMin();
		//ImVec2 regionMax = ImGui::GetWindowContentRegionMax();
		//regionMin.x += ImGui::GetWindowPos().x;
		//regionMin.y += ImGui::GetWindowPos().y;
		//regionMax.x += ImGui::GetWindowPos().x;
		//regionMax.y += ImGui::GetWindowPos().y;
		//ImGui::GetForegroundDrawList()->AddRect(regionMin, regionMax, IM_COL32(255, 255, 0, 255));

		// File
		if (ImGui::BeginMenu("File"))
		{
			// New
			if (ImGui::MenuItem("New")) 
			{
			}
			// Open
			if (ImGui::MenuItem("Open", "Ctrl+O")) 
			{
			}
			// Open Recent
			if (ImGui::BeginMenu("Open Recent"))
			{
				ImGui::MenuItem("Some recent file 1");
				ImGui::MenuItem("Some recent file 2");
				ImGui::EndMenu();
			}
			// Save
			if (ImGui::MenuItem("Save", "Ctrl+S")) 
			{
			}
			// Save As...
			if (ImGui::MenuItem("Save As...")) 
			{
			}


			ImGui::EndMenu();
		}

		// Edit
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Edit 1"))
			{
			}
			if (ImGui::MenuItem("Edit 2"))
			{
			}

			ImGui::EndMenu();
		}

		// View
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("View 1"))
			{
			}
			if (ImGui::MenuItem("View 2"))
			{
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}	
}

void UI::SimulationDetailsWindow() noexcept
{
	PROFILE_FUNCTION();

	ImVec2 padding = ImVec2(5.0f, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, padding);
	ImGui::Begin("Simulation");
	ImGui::PopStyleVar();

	m_left = ImGui::GetWindowContentRegionMax().x + padding.x;

	ImGui::Text(" Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_io.Framerate, m_io.Framerate);
	
	if (SimulationManager::SimulationIsPlaying())
	{
		if (ImGui::Button("Pause##Simulation_Details"))
			SimulationManager::SwitchPlayPause();
	}
	else
	{
		if (ImGui::Button("Play##Simulation_Details"))
			SimulationManager::SwitchPlayPause();
	}
	
	
	ImGui::End();
}

void UI::LogWindow() noexcept
{
	PROFILE_FUNCTION();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
	ImGui::Begin("Log");
	ImGui::PopStyleVar();

	m_height = ImGui::GetWindowPos().y - m_windowOffsetY - m_top;

	ImGui::Text("  Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_io.Framerate, m_io.Framerate);
	ImGui::End();
}

void UI::PerformanceWindow() noexcept
{
	PROFILE_FUNCTION();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
	ImGui::Begin("Performance");
	ImGui::PopStyleVar();

	// Temporary hack using std::max. For some reason, the width comes up as negative
	// when the program is launched which causes an issue setting the viewport
	m_width = std::max(ImGui::GetWindowPos().x - m_windowOffsetX - m_left, 1.0f);

	PerformanceFPS(); 
#ifdef PROFILE
	PerformanceProfile();
#endif



	ImGui::End();
}

void UI::PerformanceFPS() noexcept
{
	PROFILE_FUNCTION();

	if (ImGui::CollapsingHeader("FPS", ImGuiTreeNodeFlags_None))
	{
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / m_io.Framerate, m_io.Framerate);

		static std::vector<float> fps(2000);
		static float totalTime = 0.0f;
		static std::vector<float> time(2000);
		static int offset = 0;
		static float history = 10.0f;

		totalTime += m_io.DeltaTime;
		time[offset] = totalTime;

		fps[offset] = m_io.Framerate;
		offset = (offset + 1) % fps.size();

		float maxY = *std::max_element(fps.begin(), fps.end());
		float minY = *std::min_element(fps.begin(), fps.end());

		static ImPlotAxisFlags flags = ImPlotAxisFlags_None; // ImPlotAxisFlags_NoTickLabels;

		if (ImPlot::BeginPlot("FPS##Scrolling", ImVec2(-1, 150))) {
			ImPlot::SetupAxes(NULL, NULL, flags, flags);
			ImPlot::SetupAxisLimits(ImAxis_X1, totalTime - history, totalTime, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, std::max(0.0f, minY - 10.0f), maxY + 10.0f, ImGuiCond_Always);
			ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
			ImPlot::PlotLine("FPS", &time[0], &fps[0], static_cast<int>(fps.size()), 0, offset, sizeof(float));
			ImPlot::EndPlot(); 
		}
	}
}

void UI::PerformanceProfile() noexcept
{
	PROFILE_FUNCTION();

	if (ImGui::CollapsingHeader("Profile", ImGuiTreeNodeFlags_None))
	{
		ImGui::Indent();

		if (Instrumentor::Get().SessionIsActive())
		{
			ImGui::Text("Active Session: %s", Instrumentor::Get().SessionName().c_str());
		}
		else
		{
			static char resultsFile[128] = "profile/results.json";
			ImGui::SetNextItemWidth(200.0f);
			ImGui::InputText("Output Results File", resultsFile, IM_ARRAYSIZE(resultsFile));

			static int numFramesToCapture = 0;
			ImGui::SetNextItemWidth(125.0f);
			ImGui::InputInt("Frames to Capture", &numFramesToCapture);
			ImGui::SameLine();
			if (ImGui::Button("Capture##Frames_to_capture"))
			{
				Instrumentor::Get().CaptureFrames(numFramesToCapture, std::string("Capture Number of Frames"), std::string(resultsFile));
			}

			static int numSecondsToCapture = 0;
			ImGui::SetNextItemWidth(125.0f);
			ImGui::InputInt("Seconds to Capture", &numSecondsToCapture);
			ImGui::SameLine();
			if (ImGui::Button("Capture##Seconds_to_capture"))
			{
				Instrumentor::Get().CaptureSeconds(numSecondsToCapture, std::string("Capture Number of Seconds"), std::string(resultsFile));
			}
		}

		ImGui::Unindent();
	}
}

void UI::SceneEditWindow(const std::unique_ptr<Renderer>& renderer) noexcept
{
	PROFILE_FUNCTION();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
	ImGui::Begin("Scene Edit");
	ImGui::PopStyleVar();

	SceneLighting(renderer);

	ImGui::End();
}

void UI::SceneLighting(const std::unique_ptr<Renderer>& renderer) noexcept
{
	if (ImGui::CollapsingHeader("Lighting", ImGuiTreeNodeFlags_None))
	{
		ImGui::Indent(); // 1

		Lighting* lighting = renderer->GetLighting();
		LightProperties* properties = lighting->GetLightProperties();

		if (ImGui::ColorEdit4("Global Ambient", (float*)(&properties->GlobalAmbient)))
			lighting->UpdateLightingProperties();

		std::vector<std::string> lightNames(MAX_LIGHTS);
		for (unsigned int iii = 0; iii < MAX_LIGHTS; ++iii)
			lightNames[iii] = std::format("Light {0}", iii + 1);

		static int selectedIndex = 0;
		static int selectedType = properties->Lights[selectedIndex].LightType;
		if (ImGui::BeginCombo("Light##Light_Selector", lightNames[selectedIndex].c_str()))
		{
			for (unsigned int iii = 0; iii < MAX_LIGHTS; ++iii)
			{
				const bool is_selected = (selectedIndex == iii);
				if (ImGui::Selectable(lightNames[iii].c_str(), is_selected))
				{
					selectedIndex = iii;
					selectedType = properties->Lights[selectedIndex].LightType;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		ImGui::Indent(); // 2

		// Visibility
		if (ImGui::Checkbox("Visible##Scene_Light", (bool*)(&properties->Lights[selectedIndex].Enabled)))
			lighting->UpdateLightingProperties();

		// Light Type
		if (ImGui::Combo("Light Type##Scene_Light", &properties->Lights[selectedIndex].LightType, "Directional Light\0Point Light\0Spot Cone\0\0"))
			lighting->UpdateLightingProperties();

		// Position
		float positionMax = renderer->GetBox()->GetBoxSize().x * 1.5f; // allow the light to go 50% outside the box
		float positionDragSpeed = 0.1f;
		if (ImGui::DragFloat3("Position##Scene_Light", (float*)(&properties->Lights[selectedIndex].Position), positionDragSpeed, -positionMax, positionMax))
			lighting->UpdateLightingProperties();

		// Direction
		if (ImGui::DragFloat3("Direction##Scene_Light", (float*)(&properties->Lights[selectedIndex].Direction), 0.01f, -1.0f, 1.0f))
			lighting->UpdateLightingProperties();

		// Color
		if (ImGui::ColorEdit4("Color##Scene_Light", (float*)(&properties->Lights[selectedIndex].Color)))
			lighting->UpdateLightingProperties();

		// Spot Angle (Only applicable for spot lights)
		if (properties->Lights[selectedIndex].LightType == LightType::SpotLight)
		{
			if (ImGui::DragFloat("Spot Angle##Scene_Light", &properties->Lights[selectedIndex].SpotAngle, 0.01f, 0.01f, DirectX::XM_PI, "%.03f"))
				lighting->UpdateLightingProperties();
		}

		ImGui::Text("Attenuation:");

		// Constant Attenuation
		if (ImGui::DragFloat("Constant##Scene_Light", &properties->Lights[selectedIndex].ConstantAttenuation, 0.05f, 0.0f, 5.0f, "%.02f"))
			lighting->UpdateLightingProperties();

		// Linear Attenuation
		if (ImGui::DragFloat("Linear##Scene_Light", &properties->Lights[selectedIndex].LinearAttenuation, 0.01f, 0.0f, 1.0f, "%.03f"))
			lighting->UpdateLightingProperties();

		// Quadratic Attenuation
		if (ImGui::DragFloat("Quadratic##Scene_Light", &properties->Lights[selectedIndex].QuadraticAttenuation, 0.0001f, 0.0f, 0.03f, "%.03f"))
			lighting->UpdateLightingProperties();


		ImGui::Unindent(); // 2
		ImGui::Unindent(); // 1
	}
}