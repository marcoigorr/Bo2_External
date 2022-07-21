
#include <GL/glew.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include "proc.h"
#include "utils.h"
#include "Options.h"
using namespace Options;
#include "Offsets.h"
#include "Addr.h"
#include "Values.h"
using namespace Values;
using namespace Colors;
#include "DataTypes.h"

#define aModuleBase aProcess->aModuleBase
#define aLocalPlayer aProcess->aLocalPlayer
#define aEntList aProcess->aEntList
#define aClientGame aProcess->aClientGame

void SetStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowMinSize = ImVec2(1000.0f, 700.0f);       

    style.Colors[ImGuiCol_Border] = blue;
    style.Colors[ImGuiCol_WindowBg] = light_grey;
    style.Colors[ImGuiCol_ChildBg] = light_dark_grey;
    style.Colors[ImGuiCol_Text] = white;
    style.Colors[ImGuiCol_CheckMark] = white;
    style.Colors[ImGuiCol_ButtonHovered] = blue;
}

void NextItemPadding(int padding_x = 20, int padding_y = 0)
{
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + padding_x, ImGui::GetCursorPos().y + padding_y));
}

int main(int, char**)
{
    /* ------- Wait for game window ------- */
    HWND hGameWindow = NULL;
    std::cout << "Waiting for Call of Duty\xAE: Black Ops II - Zombies to be opened...\n";
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
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 20.0f);

    // Setup Dear ImGui style    
    SetStyle();
    ImVec2 tabBtnSize = ImVec2(150, 58);

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Get procId and handle
    if (!GetProcInfo())
        return 0;
        
    // Get Module base address of process
    aModuleBase = GetModuleBaseAddress64(procId); // 0x400000   

    /* --------- Main Loop --------- */
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();  

        std::vector<const char*> tabs = {
            "Info & Conf",
            "ESP",
            "Rapid Fire",
            "No Recoil",
            "Misc"            
        };
        static int tab = 0;

        // Menu overlay show/hide
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            bMenu = !bMenu; 
        }

        // Draw here
        if (bMenu)
        {
            ImGui::Begin("Bo2", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
            
            // --- Upper Section
            ImGui::BeginChild("##Upper_Section", ImVec2(ImGui::GetContentRegionAvail().x, 75), true);
            {    
                for (unsigned int i = 0; i < tabs.size(); i++)
                {
                    ImGui::SameLine();
                    ImGui::PushStyleColor(ImGuiCol_Button, tab == i ? active : inactive);
                    if (ImGui::Button(tabs[i], tabBtnSize))
                        tab = i;
                }
                ImGui::PopStyleColor(tabs.size());                
            }
            ImGui::EndChild();

            // --- Lower Section
            ImGui::BeginChild("##Lower_Section", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
            {
                switch (tab)
                {
                case 0: // Configuration
                    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize("Call of Duty\xAE: Black Ops II - Zombies").x / 2, 30));
                    ImGui::TextColored(ImVec4(0,255,0,255), "Call of Duty\xAE: Black Ops II - Zombies");

                    NextItemPadding(40, 40);
                    ImGui::Text("Zombie Count: ");
                    ImGui::SameLine();
                    ImGui::Text(std::to_string(iZombieCount).c_str());
                    NextItemPadding(40, 10);
                    ImGui::Checkbox("Instant kill", &bInstantKill);

                    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - 155, ImGui::GetContentRegionMax().y - 45));
                    if (ImGui::Button("Terminate Cheats", ImVec2(150, 35)))
                        return 0;
                    break;
                case 1: // ESP
                    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 20, ImGui::GetCursorPos().y + 15));
                    ImGui::Checkbox(" Active", &bESP);
                    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 20, ImGui::GetCursorPos().y + 20));
                    ImGui::Text("Options:");
                    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 35, ImGui::GetCursorPos().y + 8));
                    ImGui::Checkbox(" Snaplines", &bSnapLines);
                    break;
                case 2: // Rapid Fire
                    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 20, ImGui::GetCursorPos().y + 15));
                    ImGui::Checkbox(" Active", &bFireRate);
                    break;
                case 3: // No Recoil
                    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 20, ImGui::GetCursorPos().y + 15));
                    ImGui::Checkbox(" Active", &bRecoil);
                    break;
                case 4: // Misc    
                    ImGui::BeginChild("##LeftSide", ImVec2(480.0f, ImGui::GetContentRegionAvail().y), false);
                    {
                        NextItemPadding(30, 15);

                        // Health
                        ImGui::Text("Health Hack:");
                        NextItemPadding(40, 5);
                        ImGui::BeginChild("##Health Hack", ImVec2(300, 50), false);
                        {
                            ImGui::Checkbox(" God Mode", &bHealth);
                        }
                        ImGui::EndChild();

                        {
                            NextItemPadding(30, 5);
                        }

                        // Ammo
                        ImGui::Text("Ammo Hack:");
                        NextItemPadding(40, 5);
                        ImGui::BeginChild("##Ammo Hack", ImVec2(300, 50), false);
                        {
                            ImGui::Checkbox(" Unlimited Ammo", &bAmmo);
                        }
                        ImGui::EndChild();

                        {
                            NextItemPadding(30, 5);
                        }

                        // Points
                        ImGui::Text("Points Hack:");
                        NextItemPadding(40, 5);
                        ImGui::BeginChild("##Points Hack", ImVec2(358, 90), false);
                        {
                            ImGui::Checkbox(" Freeze Points", &bFreezePoints);
                            NextItemPadding(0, 10);
                            if (ImGui::Button(" Add", ImVec2(70, 26)))
                                bPoints = true;                                                       
                            ImGui::SameLine();
                            ImGui::SliderInt("Points", &iPoints, 10, 5000);
                        }
                        ImGui::EndChild();

                        {
                            NextItemPadding(30, 5);
                        }                       
                    }
                    ImGui::EndChild();

                    {
                        ImGui::SameLine();
                        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
                        ImGui::SameLine();
                    }

                    ImGui::BeginChild("##RightSide", ImVec2(500.0f, ImGui::GetContentRegionAvail().y), false);
                    {
                        NextItemPadding(30, 15);

                        // Gravity
                        ImGui::Text("Gravity Hack:");
                        NextItemPadding(40, 5);
                        ImGui::BeginChild("##Points Hack", ImVec2(358, 90), false);
                        {
                            ImGui::Checkbox(" Enable", &bGravity);
                            NextItemPadding(0, 5);
                            ImGui::SliderInt(" Gravity", &iGravity, 10, 2000);
                        }
                        ImGui::EndChild();

                        {
                            NextItemPadding(30, 5);
                        }

                        // Speed         
                        ImGui::Text("Speed Hack:");
                        NextItemPadding(40, 5);
                        ImGui::BeginChild("##Speed Hack", ImVec2(300, 100), false);
                        {
                            ImGui::Checkbox(" Enable", &bSpeedHack);
                            NextItemPadding(0, 5);
                            ImGui::SliderInt(" Speed", &iSpeed, 10, 1000);
                        }
                        ImGui::EndChild();
                    }
                    ImGui::EndChild();
                                        
                    break;                
                }
            }
            ImGui::EndChild();

            ImGui::End();
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
