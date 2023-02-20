#include "Framework.h"
#include "Scenes/TutorialScene.h"

GameManager::GameManager()
{
    Create();

    SceneManager::Get()->Create("Start", new TutorialScene());
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
}

void GameManager::Render()
{
    SceneManager::Get()->PreRender();
    
    Device::Get()->Clear();
    //Font::Get()->GetDC()->BeginDraw();
    
    Environment::Get()->SetViewport();
    Environment::Get()->SetProjection();
    
    SceneManager::Get()->Render();

    //uiViewBuffer->SetVS(1);
    
    SceneManager::Get()->PostRender();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    /*
    string fps = "FPS : " + to_string(Timer::Get()->GetFPS());
    Font::Get()->RenderText(fps, { 100, WIN_HEIGHT - 10 });

    CAM->RenderUI();
    */
    SceneManager::Get()->GUIRender();
    
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    /*
    Font::Get()->GetDC()->EndDraw();
    */
    Device::Get()->Present();
}

void GameManager::Create()
{
    Keyboard::Get();
    Timer::Get();
    Device::Get();
    
    Environment::Get();
    //Observer::Get();
    /*
    Font::Get()->AddColor("White", 1, 1, 1);
    Font::Get()->AddStyle("Default", L"배달의민족 주아");

    Font::Get()->SetColor("White");
    Font::Get()->SetStyle("Default");
    */
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(DEVICE, DC);
}

void GameManager::Delete()
{
    SceneManager::Delete();

    Keyboard::Delete();
    Timer::Delete();
    Device::Delete();
    
    Shader::Delete();
    Environment::Delete();    
    /*
    Texture::Delete();
    Observer::Delete();
    Font::Delete();
    */
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();

    ImGui::DestroyContext();
}
