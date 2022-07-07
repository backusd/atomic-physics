#include "UI.h"

UI::UI() noexcept :
	m_io(ImGui::GetIO())
	//m_left(0.0f),
	//m_top(0.0f),
	//m_right(0.0f),
	//m_bottom(0.0f)
{
}

void UI::Render() noexcept
{
	CreateDockSpaceAndMenuBar();

	// DEMO
	ImGui::ShowDemoWindow();

	SimulationDetails();
	LogWindow();
	PerformanceWindow();
	// SimulationViewport();

	//ImVec2 min = ImVec2(m_leftScreenSpace + m_leftWindowSpace, m_topScreenSpace + m_topWindowSpace);
	//ImVec2 max = ImVec2(m_rightScreenSpace + m_leftWindowSpace, m_bottomScreenSpace + m_topWindowSpace);
	//ImGui::GetForegroundDrawList()->AddRect(min, max, IM_COL32(255, 255, 0, 255));

	m_viewport = CD3D11_VIEWPORT(
		m_leftScreenSpace,
		m_topScreenSpace,
		m_rightScreenSpace - m_leftScreenSpace,
		m_bottomScreenSpace - m_topScreenSpace
	);

	// --------------------------------------------------

	//ImGui::Begin("Render Stats");
	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_io.Framerate, m_io.Framerate);
	//ImGui::End();

	// --------------------------------------------------
	// CreateDockSpace();

	/*
	// Demonstrate the various window flags. Typically you would just use the default!
	static bool no_titlebar = true;
	static bool no_scrollbar = false;
	static bool no_menu = false;
	static bool no_move = false;
	static bool no_resize = false;
	static bool no_collapse = false;
	static bool no_close = false;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = false;
	static bool no_docking = false;
	static bool unsaved_document = false;

	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (no_docking)         window_flags |= ImGuiWindowFlags_NoDocking;
	if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
	//if (no_close)           p_open = NULL; // Don't pass our bool* to Begin

	// We specify a default position/size in case there's no data in the .ini file.
	// We only do it to make the demo applications a little more welcoming, but typically this isn't required.
	//const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	//ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
	//ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

	// Main body of the Demo window starts here.
	if (!ImGui::Begin("UI", 0, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}


	DrawMenuBar();

	//ImGui::Checkbox("No titlebar", &no_titlebar);

	ImGui::End();
	*/
}

void UI::CreateDockSpaceAndMenuBar() noexcept
{
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
	if (ImGui::BeginMenuBar())
	{
		ImVec2 regionMin = ImGui::GetWindowContentRegionMin();
		ImVec2 regionMax = ImGui::GetWindowContentRegionMax();
		m_topScreenSpace = regionMin.y;

		regionMin.x += ImGui::GetWindowPos().x;
		regionMin.y += ImGui::GetWindowPos().y;
		regionMax.x += ImGui::GetWindowPos().x;
		regionMax.y += ImGui::GetWindowPos().y;

		// ImGui::GetForegroundDrawList()->AddRect(regionMin, regionMax, IM_COL32(255, 255, 0, 255));

		m_topWindowSpace = regionMin.y;

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

void UI::SimulationDetails() noexcept
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Simulation");
	ImGui::PopStyleVar();

	ImVec2 regionMin = ImGui::GetWindowContentRegionMin();
	ImVec2 regionMax = ImGui::GetWindowContentRegionMax();
	m_leftScreenSpace = regionMax.x;

	regionMin.x += ImGui::GetWindowPos().x;
	regionMin.y += ImGui::GetWindowPos().y;
	regionMax.x += ImGui::GetWindowPos().x;
	regionMax.y += ImGui::GetWindowPos().y;

	// ImGui::GetForegroundDrawList()->AddRect(regionMin, regionMax, IM_COL32(255, 255, 0, 255));

	m_leftWindowSpace = regionMax.x;

	ImGui::Text(" Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_io.Framerate, m_io.Framerate);
	ImGui::End();
}

void UI::LogWindow() noexcept
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Log");
	ImGui::PopStyleVar();

	ImVec2 regionMin = ImGui::GetWindowContentRegionMin();
	ImVec2 regionMax = ImGui::GetWindowContentRegionMax();

	regionMin.x += ImGui::GetWindowPos().x;
	regionMin.y += ImGui::GetWindowPos().y;
	regionMax.x += ImGui::GetWindowPos().x;
	regionMax.y += ImGui::GetWindowPos().y;

	// ImGui::GetForegroundDrawList()->AddRect(regionMin, regionMax, IM_COL32(255, 255, 0, 255));

	m_bottomWindowSpace = regionMin.y;
	m_bottomScreenSpace = regionMin.y - m_topWindowSpace + m_topScreenSpace - 19; // minus 19 because that is how many pixels high the tab region (non-content) is

	ImGui::Text("  Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_io.Framerate, m_io.Framerate);
	ImGui::End();
}

void UI::PerformanceWindow() noexcept
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Performance");
	ImGui::PopStyleVar();

	ImVec2 regionMin = ImGui::GetWindowContentRegionMin();
	ImVec2 regionMax = ImGui::GetWindowContentRegionMax();

	regionMin.x += ImGui::GetWindowPos().x;
	regionMin.y += ImGui::GetWindowPos().y;
	regionMax.x += ImGui::GetWindowPos().x;
	regionMax.y += ImGui::GetWindowPos().y;

	// ImGui::GetForegroundDrawList()->AddRect(regionMin, regionMax, IM_COL32(255, 255, 0, 255));

	m_rightWindowSpace = regionMin.x;
	m_rightScreenSpace = regionMin.x - m_leftWindowSpace + m_leftScreenSpace;


	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_io.Framerate, m_io.Framerate);
	ImGui::End();
}

void UI::SimulationViewport() noexcept
{
	
	ImGui::Begin("Viewport");


	// m_viewport = CD3D11_VIEWPORT(pos.x, pos.y, width, height);
	ImGui::End();
}