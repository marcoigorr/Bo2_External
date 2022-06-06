
#include "GL/glew.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>
#include <GLFW/glfw3.h> // sorry rake, next time i will make it with DirectX
#include "proc.h"
#include "utils.h"
#include "Options.h"
using namespace Options;
#include "Offsets.h"
#include "Addr.h"
#include "Values.h"
using namespace Values;
#include "DataTypes.h"

#define aModuleBase aProcess->aModuleBase
#define aLocalPlayer aProcess->aLocalPlayer
#define aEntList aProcess->aEntList
#define aClientGame aProcess->aClientGame

int main(int, char**)
{
    /* ------- Wait for game window ------- */
    HWND hGameWindow = NULL;
    std::cout << "Waiting for \"plutonium-bootstrapper-win32.exe\" to be opened...\n";
    while (hGameWindow == NULL)
    {
        hGameWindow = FindWindow(NULL, L"Call of Duty\xAE: Black Ops II - Zombies");
    }
    system("cls");
    std::cout << "Process Found!\n";

    /* ------- Setup menu window ------- */
    if (!glfwInit())
    {
        std::cout << "Failed to initialize glfw!\n";
        return 0;
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
    windowWidth = glfwGetVideoMode(monitor)->width;
    windowHeight = glfwGetVideoMode(monitor)->height;

    // Window Properties
    glfwWindowHint(GLFW_FLOATING, true);
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_MAXIMIZED, true);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
    if (window == NULL) return 1;
    
    ShowWindow(GetConsoleWindow(), SW_HIDE); // Hide console

    glfwSetWindowAttrib(window, GLFW_DECORATED, false);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize OpenGL loader!\n";
        return 0;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.WantSaveIniSettings = false;
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 18.0f);

    // Setup Dear ImGui style    
    static int tab = 0;
    static ImVec2 tabBtnSize = ImVec2(150, 40);

    static float menuW = 820.0f, menuH = 500.0f;
    ImGui::StyleColorsDark();   
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { menuW, menuH });    

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Get procId and handle
    if (!GetProcInfo())
        return 0;

    // Calculate Process Addresses
    MemMan mem;

    aModuleBase = GetModuleBaseAddress64(procId); // 0x400000
    aLocalPlayer = mem.ReadMem<uintptr_t>(hProcess, aModuleBase + oProcess->local_player); // 0x023427A0
    aEntList = mem.ReadMem<uintptr_t>(hProcess, aModuleBase + oProcess->entity_list); // 0x2330358
    aClientGame = mem.ReadMem<uintptr_t>(hProcess, aModuleBase + oProcess->client_game); // 0x1AB10107

    /* --------- Main Loop --------- */
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();    

        // Menu overlay show/hide
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            bMenu = !bMenu;            
        }

        // Draw here
        if (bMenu)
        {
            ImGui::SameLine();
            if (ImGui::Button("ESP", tabBtnSize)) tab = 1;
            ImGui::SameLine();
            if (ImGui::Button("Rapid Fire", tabBtnSize)) tab = 2;
            ImGui::SameLine();
            if (ImGui::Button("No Recoil", tabBtnSize)) tab = 3;
            ImGui::SameLine();
            if (ImGui::Button("Misc", tabBtnSize)) tab = 4;
            ImGui::SameLine();
            if (ImGui::Button("Configuration", tabBtnSize)) tab = 5;

            switch (tab)
            {
            case 1:
                ImGui::SetCursorPos(ImVec2(30, 100));
                ImGui::Checkbox("Active", &bESP);
                break;
            case 2:
                ImGui::SetCursorPos(ImVec2(30, 100));
                ImGui::Checkbox("Active", &bFireRate);
                break;
            case 3:
                ImGui::SetCursorPos(ImVec2(30, 100));
                ImGui::Checkbox("Active", &bRecoil);
                break;
            case 4:
                ImGui::SetCursorPos(ImVec2(30, 100));
                ImGui::Checkbox("God Mode", &bHealth);
                ImGui::SetCursorPos(ImVec2(30, 130)); 
                ImGui::Checkbox("Unlimited Ammo", &bAmmo);
                ImGui::SetCursorPos(ImVec2(30, 160));
                ImGui::SliderInt("Ammo Value", &iAmmo, 0, 1337);                
                break;
            case 5:
                ImGui::SetCursorPos(ImVec2((menuW / 2) - 100, (menuH / 2) - 25));
                if(ImGui::Button("Terminate Cheats", ImVec2(200, 50))) return 0;
                break;
            }
        }

        updateMenu(window);

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    CloseHandle(hProcess);
    return 0;
}