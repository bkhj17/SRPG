#include "Framework.h"
#include "Scenes/TutorialScene.h"
#include "Scenes/CubeScene.h"
#include "Scenes/GridScene.h"
#include "Scenes/TerrainScene.h"
#include "Scenes/SphereScene.h"
#include "Scenes/TerrainEditorScene.h"
#include "Scenes/LightScene.h"
#include "Scenes/CollisionScene.h"
#include "Scenes/MineCraftScene.h"
#include "Scenes/ModelExportScene.h"
#include "Scenes/ModelRenderScene.h"
#include "Scenes/ModelAnimationScene.h"
#include "Scenes/ActionScene.h"
#include "Scenes/InstancingScene.h"
#include "Scenes/GameScene.h"
//#include "Homework/230221/Scene230221.h"
//#include "Homework/230222/GridScene230222.h"
//#include "Homework/230222/TerrainScene230222.h"
//#include "Homework/230223/Scene230223.h"
//#include "Homework/230224/Scene230224.h"
//#include "Homework/230227/Scene230227.h"
//#include "Homework/230302/Scene230302.h"
//#include "Homework/230303/Scene230303.h"
//#include "Homework/230306/Scene230306.h"
//#include "Homework/230308/Scene230308.h"
#include "Homework/230313/Scene230313.h"
#include "Homework/230308/TerrainScene230308.h"
#include "Homework/230313/TerrainScene230313.h"
#include "Homework/230310/TestScene230310.h"

GameManager::GameManager()
{
    Create();

    SceneManager::Get()->Create("Grid", new GridScene());

    //SceneManager::Get()->Create("ModelExport", new ModelExportScene);
    //SceneManager::Get()->Create("Start", new ModelRenderScene);
    //SceneManager::Get()->Create("Start", new ModelAnimationScene);

    //SceneManager::Get()->Create("Start", new TutorialScene());
    //SceneManager::Get()->Create("Start", new CubeScene());
    //SceneManager::Get()->Create("Start", new TerrainScene);
    //SceneManager::Get()->Create("Start", new TerrainEditorScene);
    //SceneManager::Get()->Create("Start", new LightScene);
    //SceneManager::Get()->Create("Start", new CollisionScene);
    //SceneManager::Get()->Create("Start", new MineCraftScene);
    //SceneManager::Get()->Create("Start", new InstancingScene);
    //SceneManager::Get()->Create("Start", new ActionScene);
    SceneManager::Get()->Create("Start", new GameScene);
    
    //SceneManager::Get()->Create("Grid", new GridScene230222());
    //SceneManager::Get()->Create("Start", new SphereScene);
    //SceneManager::Get()->Create("Start", new Scene230221());
    //SceneManager::Get()->Create("Start", new Scene230223);
    //SceneManager::Get()->Create("Start", new Scene230224);
    //SceneManager::Get()->Create("Start", new Scene230227);
    //SceneManager::Get()->Create("Start", new Scene230302);
    //SceneManager::Get()->Create("Start", new Scene230303);
    //SceneManager::Get()->Create("Start", new Scene230306);
    //SceneManager::Get()->Create("Terrain", new TerrainScene230308);
    //SceneManager::Get()->Create("Start", new Scene230308);
    //SceneManager::Get()->Create("Start", new TestScene230310);
    //SceneManager::Get()->Create("Terrain", new TerrainScene230313);
    //SceneManager::Get()->Create("Start", new Scene230313);
    
    SceneManager::Get()->Add("Grid");
    //SceneManager::Get()->Add("Terrain");
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

    Font::Get()->GetDC()->EndDraw();    
    
    Device::Get()->Present();
}

void GameManager::Create()
{
    Keyboard::Get();
    Timer::Get();
    Device::Get();
    
    Environment::Get();
    Observer::Get();
    
    Font::Get()->AddColor("White", 1, 1, 1);
    Font::Get()->AddStyle("Default", L"배달의민족 주아");

    Font::Get()->SetColor("White");
    Font::Get()->SetStyle("Default");
    
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
    
    Texture::Delete();
    Observer::Delete();
    Font::Delete();
    
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();

    ImGui::DestroyContext();
}
