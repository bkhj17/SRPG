#include "Framework.h"
#include "Scenes/GridScene.h"
#include "Scenes/TerrainEditorScene.h"
#include "Scenes/ModelExportScene.h"
#include "Scenes/ModelRenderScene.h"
#include "Scenes/ModelAnimationScene.h"
#include "Scenes/ParticleToolScene.h"

#include "_SRPG/Scene/SRPGScene.h"

GameManager::GameManager()
{
    Create();

    //SceneManager::Get()->Create("Grid", new GridScene());

    //SceneManager::Get()->Create("ModelExport", new ModelExportScene);
    //SceneManager::Get()->Create("Start", new ModelRenderScene);
    //SceneManager::Get()->Create("Start", new ModelAnimationScene);
        
    SceneManager::Get()->Create("Start", new SRPGScene);
    
    //SceneManager::Get()->Add("Grid");
    SceneManager::Get()->Add("Start");
}

GameManager::~GameManager()
{
    Delete();
}

void GameManager::Update()
{
    Keyboard::Get()->Update();
    Timer::Get()->Update();

    SceneManager::Get()->Update();

    Environment::Get()->Update();

    Audio::Get()->Update();
}

void GameManager::Render()
{
    SceneManager::Get()->PreRender();
    
    Device::Get()->Clear();
    
    Environment::Get()->Set();
    
    CAM->SetView();
    SceneManager::Get()->Render();

    Environment::Get()->PostSet();
    Font::Get()->GetDC()->BeginDraw();
    SceneManager::Get()->PostRender();

#if DEBUG_MODE
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    
    //CAM->GUIRender();
    static bool isActive = true;
    if (isActive) {
        ImGui::Begin("Inspector", &isActive);
        Environment::Get()->GUIRender();
        SceneManager::Get()->GUIRender();
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    string fps = "FPS : " + to_string(Timer::Get()->GetFPS());
    Font::Get()->RenderText(fps, { 100, WIN_HEIGHT - 10 });

#endif // !DEBUG_MODE

    Font::Get()->GetDC()->EndDraw();
    Device::Get()->Present();
}

void GameManager::Create()
{
    Device::Get();
    Keyboard::Get();
    Timer::Get();
    Environment::Get();
    Observer::Get();
    Audio::Get();
    
    Font::Get()->AddColor("White", 1, 1, 1);
    Font::Get()->AddStyle("Default", L"배달의민족 주아");

    Font::Get()->SetColor("White");
    Font::Get()->SetStyle("Default");
    
    Texture::Add(L"Textures/Color/White.png");

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(DEVICE, DC);
}

void GameManager::Delete()
{
    SceneManager::Delete();
    Font::Delete();

    Observer::Delete();
    Audio::Delete();
    Keyboard::Delete();
    Timer::Delete();

    Environment::Delete();
    Shader::Delete();
    Texture::Delete();

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    Device::Delete();
}
