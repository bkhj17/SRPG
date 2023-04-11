#include "Framework.h"

Environment::Environment()
{
    CreateProjection();
    CreateState();

    mainCamera = new Camera();
    mainCamera->Load();
    
    lightBuffer = new LightBuffer;

    uiViewBuffer = new ViewBuffer;
}

Environment::~Environment()
{
    delete projectionBuffer;

    mainCamera->Save();
    delete mainCamera;

    delete lightBuffer;

    delete samplerState;
    delete raterizerState[0];
    delete raterizerState[1];
    delete blendState[0];
    delete blendState[1];

    delete uiViewBuffer;
}

void Environment::Update()
{
    if (KEY_DOWN(VK_F1))
        isWireMode = !isWireMode;
    if (KEY_DOWN(VK_F2))
        Collider::RenderOnOff();

    mainCamera->Update();
}

void Environment::GUIRender()
{
    if (ImGui::TreeNode("Environment")) {
        mainCamera->GUIRender();
        ImGui::Text("LightOption");

        if (ImGui::Button("Add"))
            lightBuffer->Get().lightCount++;

        for (UINT i = 0; i < lightBuffer->Get().lightCount; i++) {
            string name = "Light_"+to_string(i);
            if (ImGui::TreeNode(name.c_str())) {
                LightBuffer::Light& light = lightBuffer->Get().lights[i];

                ImGui::Checkbox("Active", (bool*)&light.active);

                const char* list[] = { "Directional", "Point", "Spot", "Capsule"};
                ImGui::Combo("Type", &light.type, list, 4);

                ImGui::ColorEdit3("Color", (float*)&light.color, ImGuiColorEditFlags_PickerHueWheel);
                ImGui::SliderFloat3("Dir", (float*)&light.direction, -1, 1);
                ImGui::DragFloat3("Pos", (float*)&light.pos);
                ImGui::SliderFloat("Range", &light.range, 1, 1000);
                
                //각도 : 디그리로 받아 셰이더에서 변환
                ImGui::SliderFloat("Inner", &light.inner, 0, light.outer);
                ImGui::SliderFloat("Outer", &light.outer, light.inner, 180.0f);
                ImGui::SliderFloat("Length", &light.length, 1, 1000);

                ImGui::TreePop();
            }
        }

        ImGui::ColorEdit3("AmbientLight", (float*)&lightBuffer->Get().ambientLight, ImGuiColorEditFlags_PickerHueWheel);
        ImGui::ColorEdit3("AmbientCeil", (float*)&lightBuffer->Get().ambientCeil, ImGuiColorEditFlags_PickerHueWheel);
        ImGui::TreePop();
    }
}

void Environment::Set()
{
    SetViewport();
    SetPerspective();

    depthStencilState[0]->SetState();
    raterizerState[(int)isWireMode]->SetState();        

    lightBuffer->SetPS(0);
}

void Environment::PostSet()
{
    depthStencilState[1]->SetState();
    uiViewBuffer->SetVS(1);
    SetOrthographic();
}

void Environment::SetViewport(UINT width, UINT height)
{
    viewport.Width = (float)width;
    viewport.Height = (float)height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    DC->RSSetViewports(1, &viewport);
}

void Environment::SetPerspective()
{
    projectionBuffer->Set(perspective);
    projectionBuffer->SetVS(2);
    projectionBuffer->SetPS(2);
    projectionBuffer->SetDS(2);
}

void Environment::SetOrthographic()
{
    projectionBuffer->Set(orthographic);
    projectionBuffer->SetVS(2);
    projectionBuffer->SetPS(2);
}

LightBuffer::Light* Environment::AddLight()
{
    int index = lightBuffer->Get().lightCount++;

    return GetLight(index);
}

void Environment::CreateProjection()
{
    orthographic = XMMatrixOrthographicOffCenterLH(0.0f, (float)WIN_WIDTH, 0.0f, (float)WIN_HEIGHT, -1.0f, 1.0f);
    
    perspective = XMMatrixPerspectiveFovLH(XM_PIDIV4, 
       (float)WIN_WIDTH / WIN_HEIGHT, 0.1f, 5000.0f);

    projectionBuffer = new MatrixBuffer();
}

void Environment::CreateState()
{
    samplerState = new SamplerState;
    samplerState->SetState();

    raterizerState[0] = new RasterizerState();
    raterizerState[1] = new RasterizerState();
    raterizerState[1]->FillMode(D3D11_FILL_WIREFRAME);

    blendState[0] = new BlendState();
    blendState[1] = new BlendState();
    blendState[1]->SetState();

    depthStencilState[0] = new DepthStencilState();
    depthStencilState[1] = new DepthStencilState();
    depthStencilState[1]->DepthEnable(false);
}