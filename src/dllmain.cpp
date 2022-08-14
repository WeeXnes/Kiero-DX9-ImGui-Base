#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Thread>
#include <cstdint>
#include "../external/kiero/kiero.h"
#include <d3d9.h>
#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_win32.h"
#include "../external/imgui/imgui_impl_dx9.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
static EndScene oEndScene = NULL;

WNDPROC oWndProc;
static HWND window = NULL;

void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX9_Init(pDevice);
}


bool init = false;

// Declare the detour function
long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{

    if (!init)
    {
        InitImGui(pDevice);
        init = true;
    }

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("ImGui Window");
    ImGui::End();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    return oEndScene(pDevice);
}


LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
    DWORD wndProcId;
    GetWindowThreadProcessId(handle, &wndProcId);

    if (GetCurrentProcessId() != wndProcId)
        return TRUE; // skip to next window

    window = handle;
    return FALSE; // window found abort search
}

HWND GetProcessWindow()
{
    window = NULL;
    EnumWindows(EnumWindowsCallback, NULL);
    return window;
}

DWORD WINAPI Setup(const HMODULE instance){
    bool attached = false;
    do
    {
        if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success)
        {
            kiero::bind(42, (void**)& oEndScene, hkEndScene);
            while(window == NULL){
                window = GetProcessWindow();
            }
            oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);
            attached = true;
        }
    } while (!attached);
    return TRUE;
}

BOOL WINAPI DllMain(
        const HMODULE instance,
        const std::uintptr_t reason,
        const void* reserved
)
{
    if(reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(instance);
        const auto thread = CreateThread(nullptr,
                                         0,
                                         reinterpret_cast<LPTHREAD_START_ROUTINE>(Setup),
                                         instance,
                                         0,
                                         nullptr);
        if(thread)
            CloseHandle(thread);
    }
    else if(reason == DLL_PROCESS_DETACH){
        kiero::shutdown();
    }
    return TRUE;
}