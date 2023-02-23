#include "Framework.h"

GameObject::GameObject(wstring shaderFile)
{
    material = new Material(shaderFile);
    worldBuffer = new MatrixBuffer();
}

GameObject::~GameObject()
{ 
    delete worldBuffer;
}

void GameObject::SetRender()
{   
    worldBuffer->Set(world);
    worldBuffer->SetVS(0);    

    material->Set();
}

void GameObject::RenderUI()
{
    if (ImGui::TreeNode(tag.c_str())) {

        __super::RenderUI();
        material->RenderUI();

        ImGui::TreePop();
    }

}
