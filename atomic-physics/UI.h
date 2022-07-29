#pragma once
#include "pch.h"
#include "Event.h"
#include "Renderer.h"

#include <memory>

// ImGui ------------------------
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "implot.h"
// ------------------------------
enum ParticleDetailsColumnID
{
    ParticleDetailsColumnID_ID,
    ParticleDetailsColumnID_Name,
    ParticleDetailsColumnID_Mass,
    ParticleDetailsColumnID_Position,
    ParticleDetailsColumnID_Velocity
};

struct ParticleDetails
{
    int         ID;
    const char* Name;
    unsigned int Mass;

    // We have a problem which is affecting _only this demo_ and should not affect your code:
    // As we don't rely on std:: or other third-party library to compile dear imgui, we only have reliable access to qsort(),
    // however qsort doesn't allow passing user data to comparing function.
    // As a workaround, we are storing the sort specs in a static/global for the comparing function to access.
    // In your own use case you would probably pass the sort specs to your sorting/comparing functions directly and not use a global.
    // We could technically call ImGui::TableGetSortSpecs() in CompareWithSortSpecs(), but considering that this function is called
    // very often by the sorting algorithm it would be a little wasteful.
    
    static const ImGuiTableSortSpecs* s_current_sort_specs;

    // Compare function to be used by qsort()
    static int CompareWithSortSpecs(const void* lhs, const void* rhs)
    {
        const ParticleDetails* a = (const ParticleDetails*)lhs;
        const ParticleDetails* b = (const ParticleDetails*)rhs;
        for (int n = 0; n < s_current_sort_specs->SpecsCount; n++)
        {
            // Here we identify columns using the ColumnUserID value that we ourselves passed to TableSetupColumn()
            // We could also choose to identify columns based on their index (sort_spec->ColumnIndex), which is simpler!
            const ImGuiTableColumnSortSpecs* sort_spec = &s_current_sort_specs->Specs[n];
            int delta = 0;
            switch (sort_spec->ColumnUserID)
            {
            case ParticleDetailsColumnID_ID:             delta = (a->ID - b->ID);                break;
            case ParticleDetailsColumnID_Name:           delta = (strcmp(a->Name, b->Name));     break;
            case ParticleDetailsColumnID_Mass:           delta = (a->Mass - b->Mass);            break;
            default: IM_ASSERT(0); break;
            }
            if (delta > 0)
                return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? +1 : -1;
            if (delta < 0)
                return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? -1 : +1;
        }

        // qsort() is instable so always return a way to differenciate items.
        // Your own compare function may want to avoid fallback on implicit sort specs e.g. a Name compare if it wasn't already part of the sort specs.
        return (a->ID - b->ID);
    }
};

class UI
{
public:
	UI() noexcept;
	UI(const UI&) = delete;
	void operator=(const UI&) = delete;
    ~UI() noexcept;

	void Render(const std::unique_ptr<Renderer>& renderer) noexcept;

	D3D11_VIEWPORT GetViewport() const noexcept { return m_viewport; }

private:
    void OnPlayPauseChanged(bool isPlaying) noexcept;
    void OnParticleAdded(const Particle& particle, unsigned int particleCount) noexcept;
    void OnParticleRemoved(unsigned int particleIndex) noexcept;

	void CreateDockSpaceAndMenuBar() noexcept;
	void MenuBar() noexcept;
	void SimulationDetailsWindow(const std::unique_ptr<Renderer>& renderer) noexcept;
	void LogWindow() noexcept;


	void PerformanceWindow() noexcept;
	void PerformanceFPS() noexcept;
	void PerformanceProfile() noexcept;

	void SceneEditWindow(const std::unique_ptr<Renderer>& renderer) noexcept;
	void SceneLighting(const std::unique_ptr<Renderer>& renderer) noexcept;

	ImGuiIO& m_io;
	D3D11_VIEWPORT m_viewport;

	float m_top, m_left;
	float m_height, m_width;
	float m_windowOffsetX, m_windowOffsetY;

    ImVector<ParticleDetails>   m_particleDetails;
    ImVector<int>               m_selectedParticles;

    bool m_simulationIsPlaying;

    // Event Tokens
    EventToken t_playPause;
    EventToken t_particleAdded;
    EventToken t_particleRemoved;
};