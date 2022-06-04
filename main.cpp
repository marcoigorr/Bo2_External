
#include "GL/glew.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include "MemMan.h"
#include "proc.h"
#include "Options.h"
#include "Offsets.h"
#include "Menu.h"


int main(int, char**)
{
    // Setup window
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

    DWORD procId = GetProcId(L"plutonium - bootstrapper - win32.exe");
    if (!procId)
    {
        std::cout << "\nProcess not found, press any key to exit";
        std::cin;
        return 0;
    }
    
    // Open Handle to process
    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    // Get process's module base address 
    uintptr_t aModuleBase = GetModuleBaseAddress64(procId);

    uintptr_t aLocalPlayer = aModuleBase + p_process->local_player;
    uintptr_t aEntList = aModuleBase + p_process->entity_list;
    uintptr_t aClientGame = aModuleBase + p_process->client_game;

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