
#include "GL/glew.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include "MemMan.h"
#include "proc.h"
#include "Options.h"
using namespace Options;
#include "Offsets.h"
#include "Addr.h"
#include "Menu.h"
#include "DataTypes.h"

#define aModuleBase addr->aModuleBase
#define aLocalPlayer aProcess->aLocalPlayer
#define aEntList aProcess->aEntList
#define aClientGame aProcess->aClientGame


int main(int, char**)
{
    MemMan mem;

    /* ------- Setup menu window ------- */
    if (!glfwInit())
    {
        std::cout << "Failed to initialize glfw!\n";
        return 1;
    }     

    // GL+GLSL verion
    const char* glsl_version = "#version 130";

    // Get Primary monitor
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor)
    {
        std::cout << "Failed to get primary monitor!\n";
        return 0;
    }
    int width = glfwGetVideoMode(monitor)->width;
    int height = glfwGetVideoMode(monitor)->height;

    // Window Properties
    glfwWindowHint(GLFW_FLOATING, true);
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_MAXIMIZED, true);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (window == NULL)
    {
        return 1;
    }
    glfwSetWindowAttrib(window, GLFW_DECORATED, false);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize OpenGL loader!\n";
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);    

    /* -- Wait for game window to appear -- */
    HWND hGameWindow = NULL;
    std::cout << "Waiting for \"plutonium-bootstrapper-win32.exe\" to be opened...\n";
    while (hGameWindow == NULL)
    {
        hGameWindow = FindWindow(NULL, L"Call of Duty\xAE: Black Ops II - Zombies");
    }
    system("cls");
    std::cout << "Process Found!\n";

    DWORD procId = GetProcId(L"plutonium-bootstrapper-win32.exe");
    if (!procId)
    {
        std::cout << "\nProcess not found, press any key to exit";
        std::cin;
        return 0;
    }

    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    aModuleBase = GetModuleBaseAddress64(procId); // 0x400000

    aLocalPlayer = mem.ReadMem<uintptr_t>(hProcess, aModuleBase + oProcess->local_player); // 0x023427A0
    aEntList = mem.ReadMem<uintptr_t>(hProcess, aModuleBase + oProcess->entity_list); // 0x2330358
    aClientGame = mem.ReadMem<uintptr_t>(hProcess, aModuleBase + oProcess->client_game); // 0x1AB10107

    // std::cout << "0x" << std::hex << std::uppercase << aClientGame << std::endl;

    /* --------- Main Loop --------- */
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Menu overlay show/hide
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            Options::bMenu = !Options::bMenu;            
        }

        // Draw here
        if (Options::bMenu)
        {
            if (ImGui::Button("Exit"))
            {
                return 0;
            }
        }

        // Find Local Player again
        aLocalPlayer = mem.ReadMem<uintptr_t>(hProcess, aModuleBase + oProcess->local_player); // 0x023427A0        

        // Get View Matrix
        ViewMatrix = mem.ReadMem<Matrix>(hProcess, aClientGame + (uintptr_t)&oClientGame->viewmatrix);

        for (short int i = 0; i < 64; i++)
        {
            uintptr_t Entity = mem.ReadMem<uintptr_t>(hProcess, aEntList + i * 0x8C);
        }

        updateMenu(window);

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}