
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include "imgui_internal.h"
//#include "imgui_freetype.h"
#include "image.h"
#include "font.h"
#include <dwmapi.h>
#include "byteicon.h"
#include "icon.h"
#include <fstream>
#include <iostream>
#include <string>
#include "imgui_settings.h"
#include "vars.h"
// Data
static LPDIRECT3D9 g_pD3D = nullptr;
static LPDIRECT3DDEVICE9 g_pd3dDevice = nullptr;
static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


/****************************************/
namespace font
{
    ImFont* inter_bold = nullptr;
    ImFont* inter_default = nullptr;
    ImFont* icon = nullptr;
    ImFont* icons = nullptr;
}

bool info_bar = true;


const char* cheat_name = "  MENU";
const char* developer = "#Noe1010";

const char* ping = "45ms";
const char* world_time = "12:20am";
namespace pic
{
    IDirect3DTexture9* background = nullptr;

    IDirect3DTexture9* aimbot = nullptr;
 
}
static bool rgb = false;
static bool buny = true;
// Función de callback que se ejecutará cuando se haga clic en el texto "RGB"
void OnRGBTextClicked() {
    // Cambia el valor de la bandera rgb
    rgb = !rgb;
}


DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;
namespace ImGui
{
    int rotation_start_index;
    void ImRotateStart()
    {
        rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
    }

    ImVec2 ImRotationCenter()
    {
        ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

        const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
        for (int i = rotation_start_index; i < buf.Size; i++)
            l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

        return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
    }


    void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
    {
        float s = sin(rad), c = cos(rad);
        center = ImRotate(center, s, c) - center;

        auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
        for (int i = rotation_start_index; i < buf.Size; i++)
            buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
    }
}
void particulasrafa()
{
    ImVec2 screen_size = { (float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN) };

    static ImVec2 partile_pos[100];
    static ImVec2 partile_target_pos[100];
    static float partile_speed[100];
    static float partile_size[100];
    static float partile_radius[100];
    static float partile_rotate[100];

    for (int i = 1; i < 70; i++)
    {
        if (partile_pos[i].x == 0 || partile_pos[i].y == 0)
        {
            partile_pos[i].x = rand() % (int)screen_size.x + 1;
            partile_pos[i].y = 15.f;
            partile_speed[i] = 1 + rand() % 35;
            partile_radius[i] = rand() % 4;
            partile_size[i] = rand() % 8;

            partile_target_pos[i].x = rand() % (int)screen_size.x;
            partile_target_pos[i].y = screen_size.y * 2;
        }

        partile_pos[i] = ImLerp(partile_pos[i], partile_target_pos[i], ImGui::GetIO().DeltaTime * (partile_speed[i] / 60));
        partile_rotate[i] += ImGui::GetIO().DeltaTime;

        if (partile_pos[i].y > screen_size.y)
        {
            partile_pos[i].x = 0;
            partile_pos[i].y = 0;
            partile_rotate[i] = 0;
        }
        float randomRed = static_cast<float>(rand()) / RAND_MAX;
        float randomGreen = static_cast<float>(rand()) / RAND_MAX;
        float randomBlue = static_cast<float>(rand()) / RAND_MAX;
        ImVec4 randomColor = ImVec4(255, 0, 0, 1.0f);

        randomRed = static_cast<float>(rand()) / RAND_MAX;
        randomGreen = static_cast<float>(rand()) / RAND_MAX;
        randomBlue = static_cast<float>(rand()) / RAND_MAX;


        ImVec4 randomLineColor = ImVec4(255, 0, 0, 1.0f);



        ImGui::ImRotateStart();
        ImGui::GetWindowDrawList()->AddCircleFilled(partile_pos[i], partile_size[i], ImColor(c::accent), 1);
        ImGui::GetWindowDrawList()->AddShadowCircle(partile_pos[i], 8.f, ImColor(c::accent), 58.f + partile_size[i], ImVec2(0, 0), 0, 1);
        ImGui::ImRotateEnd(partile_rotate[i]);
    }
}


/********************/
// Main code
int main(int, char**)
{
    // Create application window
    // ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX9 Example", WS_POPUP, 0, 0, 1, 1, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(GetConsoleWindow(), SW_HIDE);
    ::ShowWindow(hwnd, SW_HIDE);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows


    ImFontConfig cfg;
    // cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_LoadColor;
    font::inter_default = io.Fonts->AddFontFromMemoryTTF(inter_medium, sizeof(inter_medium), 17.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::inter_bold = io.Fonts->AddFontFromMemoryTTF(inter_bold, sizeof(inter_bold), 17.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    font::icons = io.Fonts->AddFontFromMemoryTTF(&font_awesome_binary, sizeof(font_awesome_binary), 20, &icons_config, icon_ranges);
    font::icon = io.Fonts->AddFontFromMemoryTTF(icon_font, sizeof(icon_font), 15.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    io.Fonts->AddFontFromMemoryTTF(museo500_binary, sizeof(museo500_binary), 19);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();


    // ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Load Fonts
    // io.Fonts->AddFontDefault();

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            g_d3dpp.BackBufferWidth = g_ResizeWidth;
            g_d3dpp.BackBufferHeight = g_ResizeHeight;
            g_ResizeWidth = g_ResizeHeight = 0;
            ResetDevice();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGuiStyle* style = &ImGui::GetStyle();
            style->WindowPadding = ImVec2(0, 0);
            style->WindowBorderSize = 0;
            style->ItemSpacing = ImVec2(20, 20);
            style->ScrollbarSize = 8.f;
            ImGuiStyle* styles = &ImGui::GetStyle();
            styles->WindowPadding = ImVec2(0, 0);
            styles->WindowBorderSize = 0;
            styles->ItemSpacing = ImVec2(20, 20);
            styles->ScrollbarSize = 8.f;
            //    ImGui::SetNextWindowSize({ c::bg::size });
                //   ImGui::GetBackgroundDrawList()->AddImage(pic::background, ImVec2(-10, 0), ImVec2(1920, 1080), ImVec2(500, 0), ImVec2(500, 1), ImColor(255, 255, 255, 255));
            static float color[4] = { 0.0f, 244.0f / 255.0f, 255.0f / 255.0f, 200.0f / 255.0f };
            static float colorGlow[4] = { 0.0f, 244.0f / 255.0f, 255.0f / 255.0f, 200.0f / 255.0f };
            c::accent = ImVec4(color[0], color[1], color[2], 1.0f);
            ImVec4 textColor = ImColor(101, 102, 105, 255);
            ImGui::PushFont(font::icons);
            ImGui::SetNextWindowSize({ c::bg::size });
            //***************************************************************************************//
            static float rotation_speed = 1.0f;
          

            ImGui::Begin("IMGUI", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
            {
                ///It won't let me add the checkbox which gives me an access violation when reading
                 particulasrafa();
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(c::accent));
                const ImVec2& pos = ImGui::GetWindowPos();
                const ImVec2& spacing = style->ItemSpacing;
                const ImVec2& region = ImGui::GetContentRegionMax();

                ImGui::GetBackgroundDrawList()->AddRectFilled(pos, pos + region, ImGui::GetColorU32(c::bg::background), c::bg::rounding);

                ImGui::GetBackgroundDrawList()->AddRectFilled(pos + spacing, pos + ImVec2(180, 62 + +spacing.y), ImGui::GetColorU32(c::child::background), c::child::rounding);
                ImGui::GetBackgroundDrawList()->AddRect(pos + spacing, pos + ImVec2(180, 62 + spacing.y), ImGui::GetColorU32(c::accent), c::child::rounding);

                ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(0, 62 + spacing.y) + spacing, pos + ImVec2(180, region.y - spacing.y), ImGui::GetColorU32(c::child::background), c::child::rounding);
                ImGui::GetBackgroundDrawList()->AddRect(pos + ImVec2(0, 62 + spacing.y) + spacing, pos + ImVec2(180, region.y - spacing.y), ImGui::GetColorU32(c::accent), c::child::rounding);

                ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(180, 0) + spacing, pos + ImVec2(region.x - (spacing.x * 2) - 62, 62 + spacing.y), ImGui::GetColorU32(c::child::background), c::child::rounding);
                //CUADRO DE MENU PARTE SUPERIOR IZQUIERDO
                ImGui::GetBackgroundDrawList()->AddRect(pos + ImVec2(180, 0) + spacing, pos + ImVec2(region.x - (spacing.x * 2) - 140, 62 + spacing.y), ImGui::GetColorU32(c::accent), c::child::rounding);
                ///*******
                ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(region.x - (spacing.x * 2) - 62, 0) + spacing, pos + ImVec2(region.x - spacing.x, 62 + spacing.y), ImGui::GetColorU32(c::child::background), c::child::rounding);

                ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(region.x - (spacing.x * 2) - 62, 0) + spacing, pos + ImVec2(region.x - spacing.x, 62 + spacing.y), ImGui::GetColorU32(c::child::background), c::child::rounding);
                //CUADRO DE AJUSTES PARTE SUPERIOR DERECHO
                ImGui::GetBackgroundDrawList()->AddRect(pos + ImVec2(region.x - (spacing.x * 2) - 150, 0) + spacing, pos + ImVec2(region.x - spacing.x, 62 + spacing.y), ImGui::GetColorU32(c::accent), c::child::rounding);
                //***




                ImGui::GetBackgroundDrawList()->AddRect(pos + ImVec2(1, 1), pos + ImVec2(960, 610), ImGui::GetColorU32(c::accent), c::bg::rounding, ImDrawCornerFlags_All, 2.0f);
                ImGui::GlowText(font::inter_bold, 17.f, cheat_name, pos + (ImVec2(180, 62 + spacing.y) + spacing - ImGui::CalcTextSize(cheat_name)) / 2, ImGui::GetColorU32(c::accent));
                static int tabs = 0, sub_tabs = 0;
                static int sub_tabss = 0;
                ImVec4 colors = ImVec4(0.0f, 244.0f / 255.0f, 255.0f / 255.0f, 200.0f / 255.0f); // Color inicial (rojo)
                ImGui::SetCursorPos(ImVec2(spacing.x * 2, 62 + spacing.y * 3));
                ImGui::BeginGroup();
                {
                    ImVec4 text_color = ImVec4(colors.x, colors.y, colors.z, colors.w);
                    if (ImGui::Tabs(0 == tabs, pic::aimbot, "op1", ImVec2(180 - spacing.x * 3, 40), NULL)) tabs = 0;
                    ImGui::SetCursorPos(ImVec2(spacing.x * 2, 62 + spacing.y * 24));
                    ImGui::Text("  Usuario");
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 15);
                    ImGui::Text("sss");
                }
                ImGui::EndGroup();


                switch (tabs)
                {
                case 0:

                    ImGui::SetCursorPos(ImVec2(180 + spacing.x, spacing.y));
                    ImGui::BeginGroup();
                    {
                        if (ImGui::SubTab(0 == sub_tabs," op1", ImVec2(120, 62))) sub_tabs = 0;

                    }
                    ImGui::EndGroup();
                    ImGui::SetCursorPos(ImVec2(180, 62 + spacing.y) + spacing);
                    ImGui::BeginGroup();
                    {
                        ImGui::CustomBeginChild("op", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 2.0f + 40));
                        {
                            static bool aimbot_notification_shown = false;
                            ///It won't let me add the checkbox which gives me an access violation when reading
                            ImGui::Checkbox("btn", &vars::a::check);
                           // ImGui::Keybind("Keybind", &vars::a::key, &vars::a::misc, true);
                           // ImGui::SliderInt("FOV", &vars::a::d, 1, 180);

                        }
                        ImGui::CustomEndChild();
                    }
                    ImGui::EndGroup();

                    break;

                default:
                    break;
                }

                ImGui::PopStyleColor();

            }

            ImGui::End();
            ImGui::PopFont();

        }



        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions
bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // Present with vsync
    // g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice)
    {
        g_pd3dDevice->Release();
        g_pd3dDevice = nullptr;
    }
    if (g_pD3D)
    {
        g_pD3D->Release();
        g_pD3D = nullptr;
    }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            // const int dpi = HIWORD(wParam);
            // printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
