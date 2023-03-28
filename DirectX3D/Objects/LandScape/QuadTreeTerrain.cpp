#include "framework.h"
#include "QuadTreeTerrain.h"

QuadTreeTerrain::QuadTreeTerrain(wstring heightFile)
{
    terrainData = new TerrainData(heightFile);
    width = (UINT)terrainData->GetSize().x;
    height = (UINT)terrainData->GetSize().y;

    material->SetDiffuseMap(L"Textures/LandScape/Dirt2.png");

    vertices = terrainData->GetVertices();
    UINT vertexCount = (UINT)vertices.size();
    triangleCount = vertexCount / 3;

    float centerX = 0.0f;
    float centerZ = 0.0f;
    float size = 0.0f;

    CalcMeshDimensions(vertexCount, centerX, centerZ, size);

    root = new Node();
    CreateTreeNode(root, centerX, centerZ, size);
}

QuadTreeTerrain::~QuadTreeTerrain()
{
    DeleteNode(root);
    delete root;
}

void QuadTreeTerrain::Render()
{
    drawCount = 0;
    SetRender();
    RenderNode(root);
}

void QuadTreeTerrain::GUIRender()
{
    ImGui::Text("DrawCount : %d", drawCount);
}

void QuadTreeTerrain::RenderNode(Node* node)
{
    Vector3 center(node->x, 0.0f, node->z);
    float radius = node->size * 0.5f;

    if (!CAM->ContainSphere(center, radius))
        return;
        
    UINT count = 0;
    for (int i = 0; i < 4; i++) {
        if (node->children[i]) {
            count++;
            RenderNode(node->children[i]);
        }
    }

    if (count != 0)
        return;

    //Leaf
    node->mesh->Draw();
    drawCount += node->triangleCount;
}

void QuadTreeTerrain::DeleteNode(Node* node)
{
    for (int i = 0; i < 4; i++) {
        if (node->children[i]) {
            DeleteNode(node->children[i]);
            delete node->children[i];
        }
    }
    delete node->mesh;
}

void QuadTreeTerrain::CalcMeshDimensions(UINT vertexCount, float& centerX, float& centerZ, float& size)
{
    for (UINT i = 0; i < vertexCount; i++) {
        centerX += vertices[i].pos.x;
        centerZ += vertices[i].pos.z;
    }

    centerX /= (float) vertexCount;
    centerZ /= (float) vertexCount;

    float maxX = 0.0f, maxZ = 0.0f;
    for (UINT i = 0; i < vertexCount; i++) {
        float width = abs(vertices[i].pos.x - centerX);
        float depth = abs(vertices[i].pos.z - centerZ);

        if (width > maxX) maxX = width;
        if (depth > maxZ) maxZ = width;
    }

    size = max(maxX, maxZ) * 2.0f;
}

bool QuadTreeTerrain::IsTriangleContained(UINT index, float x, float z, float size)
{
    UINT vertexIndex = index * 3;
    float halfSize = size * 0.5f;

    float x1 = vertices[vertexIndex].pos.x;
    float z1 = vertices[vertexIndex].pos.z;
    vertexIndex++;

    float x2 = vertices[vertexIndex].pos.x;
    float z2 = vertices[vertexIndex].pos.z;
    vertexIndex++;

    float x3 = vertices[vertexIndex].pos.x;
    float z3 = vertices[vertexIndex].pos.z;
    
    float minX = min(x1, min(x2, x3));
    if(minX > (x + halfSize))
        return false;
    
    float minZ = min(z1, min(z2, z3));
    if (minZ > (z + halfSize))
        return false;

    float maxX = max(x1, max(x2, x3));
    if (maxX < (x - halfSize))
        return false;

    float maxZ = max(z1, max(z2, z3));
    if (maxZ < (z - halfSize))
        return false;

    return true;
}

UINT QuadTreeTerrain::ContainTriangleCount(float x, float z, float size)
{
    UINT count = 0;
    for (int i = 0; i < triangleCount; i++) {
        if (IsTriangleContained(i, x, z, size))
            count++;
    }

    return count;
}

void QuadTreeTerrain::CreateTreeNode(Node* node, float x, float z, float size)
{
    node->x = x;
    node->z = z;
    node->size = size;

    UINT triangles = ContainTriangleCount(x, z, size);
    if (triangles == 0)
        return;

    if (triangles > MIN_TRIANGLE) { 
        //쪼개야 하는 노드
        for (int i = 0; i < 4; i++) {
            float offsetX = (((i % 2) == 0) ? -1.0f : 1.0f) * (size / 4.0f);
            float offsetZ = ((i < 2) ? -1.0f : 1.0f) * (size / 4.0f);

            node->children[i] = new Node();
            CreateTreeNode(node->children[i], x + offsetX, z + offsetZ, size * 0.5f);
        }

        return;
    }

    //안 쪼개졌다:말단
    node->triangleCount = triangles;
    UINT vertexCount = triangles * 3;
    node->mesh = new Mesh<VertexType>();
    vector<VertexType>& vertices = node->mesh->GetVertices();
    vertices.resize(vertexCount);

    UINT index = 0, vertexIndex = 0;
    for (int i = 0; i < triangleCount; i++) {
        if (IsTriangleContained(i, x, z, size)) {
            vertexIndex = i * 3;
            vertices[index] = this->vertices[vertexIndex];
            index++;

            vertexIndex++;
            vertices[index] = this->vertices[vertexIndex];
            index++;

            vertexIndex++;
            vertices[index] = this->vertices[vertexIndex];
            index++;
        }
    }

    node->mesh->CreateMesh();
}