﻿
#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

#define MAX_LIGHT 10
#define MAX_BONE 256	//적으면 추후 수정
#define MAX_FRAME 512	//적으면 추후 수정
#define MAX_INSTANCE 128

#define CENTER_X (WIN_WIDTH * 0.5f)
#define CENTER_Y (WIN_HEIGHT * 0.5f)

#define DEVICE Device::Get()->GetDevice()
#define DC Device::Get()->GetDeviceContext()

#define DELTA Timer::Get()->GetElapsedTime()

#define KEY_DOWN(k) Keyboard::Get()->Down(k)
#define KEY_UP(k) Keyboard::Get()->Up(k)
#define KEY_PRESS(k) Keyboard::Get()->Press(k)

#define CAM Environment::Get()->GetMainCamera()
#define DIALOG ImGuiFileDialog::Instance()

#define FOR(n) for(int i = 0; i < n; i++)

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <stack>
#include <queue>
#include <functional>
#include <bitset>
#include <algorithm>
#include <assert.h>

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
//DirectXTex
#include <DirectXTex.h>
#include <DirectXCollision.h>
//ImGui
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <ImGuiFileDialog.h>
//Font
#include <d2d1_2.h>
#include <dwrite.h>
//assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//FMOD
#include <FMOD/fmod.hpp>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "DirectXTex.lib")
#pragma comment(lib, "ImGui.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "assimp-vc143-mtd.lib")
#pragma comment(lib, "fmod_vc.lib")

using namespace std;
using namespace DirectX;
using namespace DirectX::TriangleTests;

typedef XMFLOAT2 Float2;
typedef XMFLOAT3 Float3;
typedef XMFLOAT4 Float4;
typedef XMVECTOR Vector4;
typedef XMMATRIX Matrix;
typedef XMFLOAT4X4 Float4x4;

typedef function<void()> Event;
typedef function<void(void*)> ParamEvent;
typedef function<void(int)> IntParamEvent;

#include "Framework/Utilities/Singleton.h"
#include "Framework/Utilities/BinaryReader.h"
#include "Framework/Utilities/BinaryWriter.h"
#include "Framework/Utilities/Keyboard.h"
#include "Framework/Utilities/Timer.h"
#include "Framework/Utilities/tinyxml2.h"
#include "Framework/Utilities/Font.h"
#include "Framework/Utilities/Observer.h"
#include "Framework/Utilities/Audio.h"
#include "Framework/Utilities/Utility.h"

using namespace Utility;

#include "Framework/System/Device.h"

#include "Framework/Buffer/VertexBuffer.h"
#include "Framework/Buffer/IndexBuffer.h"
#include "Framework/Buffer/ConstBuffer.h"
#include "Framework/Buffer/VertexLayouts.h"
#include "Framework/Buffer/GlobalBuffer.h"
#include "Framework/Buffer/RawBuffer.h"
#include "Framework/Buffer/StructuredBuffer.h"

#include "Framework/Shader/Shader.h"
#include "Framework/Shader/VertexShader.h"
#include "Framework/Shader/PixelShader.h"
#include "Framework/Shader/ComputeShader.h"
#include "Framework/Shader/GeometryShader.h"
#include "Framework/Shader/HullShader.h"
#include "Framework/Shader/DomainShader.h"

#include "Framework/State/RasterizerState.h"
#include "Framework/State/SamplerState.h"
#include "Framework/State/BlendState.h"
#include "Framework/State/DepthStencilState.h"

#include "Framework/Math/Vector2.h"
#include "Framework/Math/Vector3.h"
#include "Framework/Math/Transform.h"
#include "Framework/Math/GameMath.h"

using namespace GameMath;

#include "Framework/Render/Texture.h"
#include "Framework/Render/Material.h"
#include "Framework/Render/Mesh.h"
#include "Framework/Render/DepthStencil.h"
#include "Framework/Render/RenderTarget.h"
#include "Framework/Render/GBuffer.h"

#include "Framework/Environment/Camera.h"
#include "Framework/Environment/Environment.h"
#include "Framework/Environment/Shadow.h"
#include "Framework/Environment/Reflection.h"
#include "Framework/Environment/Refraction.h"

#include "Objects/Basic/GameObject.h"
#include "Objects/Basic/Cube.h"
#include "Objects/Basic/Sphere.h"
#include "Objects/Basic/Cylinder.h"
#include "Objects/Basic/Quad.h"

#include "Objects/UI/ProgressBar.h"

#include "Framework/Collision/Collider.h"
#include "Framework/Collision/BoxCollider.h"
#include "Framework/Collision/SphereCollider.h"
#include "Framework/Collision/CapsuleCollider.h"

#include "Objects/Algorithm/Node.h"
#include "Objects/Algorithm/Heap.h"
#include "Objects/Algorithm/AStar.h"

#include "Framework/Model/ModelData.h"
#include "Framework/Model/ModelExporter.h"
#include "Framework/Model/ModelMesh.h"
#include "Framework/Model/Model.h"
#include "Framework/Model/ModelClip.h"
#include "Framework/Model/ModelAnimator.h"
#include "Framework/Model/ModelInstancing.h"
#include "Framework/Model/ModelAnimatorInstancing.h"

#include "Objects/Particle/Particle.h"
#include "Objects/Particle/Sprite.h"
#include "Objects/Particle/Spark.h"
#include "Objects/Particle/Rain.h"
#include "Objects/Particle/Snow.h"
#include "Objects/Particle/ParticleSystem.h"
#include "Objects/Particle/Trail.h"

#include "Objects/LandScape/Terrain.h"
#include "Objects/LandScape/TerrainEditor.h"
#include "Objects/LandScape/SkyBox.h"
#include "Objects/LandScape/Water.h"
#include "Objects/LandScape/TerrainData.h"
#include "Objects/LandScape/QuadTreeTerrain.h"
#include "Objects/LandScape/TerrainLOD.h"
#include "Objects/LandScape/Scattering.h"

#include "Objects/Manager/ParticleManager.h"

#include "Objects/UI/ImageFont.h"

//포트폴리오
#include "_SRPG/Field/GridedTerrain.h"
#include "_SRPG/Field/MapCursor.h"

#include "_SRPG/Character/SRPGObject.h"
#include "_SRPG/Character/Weapon.h"
#include "_SRPG/Character/WeaponManager.h"
#include "_SRPG/Character/Character.h"
#include "_SRPG/Character/Obstacle.h"
#include "_SRPG/Character/CharacterManager.h"

#include "_SRPG/UI/UIWindow.h"
#include "_SRPG/UI/SelectUI.h"
#include "_SRPG/UI/ActionSelectUI.h"
#include "_SRPG/UI/MainSelectUI.h"
#include "_SRPG/UI/MapSelectUI.h"
#include "_SRPG/UI/AttackSelectUI.h"
#include "_SRPG/UI/InfoUI.h"
#include "_SRPG/UI/TurnUI.h"
#include "_SRPG/UI/FloatingDamage.h"
#include "_SRPG/UI/TurnChangeUI.h"
#include "_SRPG/UI/MessageUI.h"
#include "_SRPG/UI/SRPGUIManager.h"

#include "_SRPG/Controller/Controller.h"
#include "_SRPG/Controller/PlayerController.h"
#include "_SRPG/Controller/EnemyController.h"
#include "_SRPG/Controller/TurnManager.h"

#include "_SRPG/Cam/BattleCameraMan.h"

#include "Scenes/Scene.h"
#include "Manager/SceneManager.h"
#include "Manager/GameManager.h"

extern HWND hWnd;
extern Vector3 mousePos;