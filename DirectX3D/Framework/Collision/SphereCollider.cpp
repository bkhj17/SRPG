#include "framework.h"

SphereCollider::SphereCollider(float radius, UINT stackCount, UINT sliceCount)
    : radius(radius), stackCount(stackCount), sliceCount(sliceCount)
{
	type = SPHERE;

    MakeMesh();
    mesh->CreateMesh();
}

bool SphereCollider::IsRayCollision(IN Ray ray, OUT Contact* contact)
{
	//중점으로부터 선의 길이
	UpdateWorld();

	Vector3 P = ray.pos;
	Vector3 D = ray.dir.GetNormalized();

	Vector3 C = GlobalPos();
	Vector3 A = P - C;

	float b = 2 * Dot(D, A);
	float c = Dot(A, A) - pow(Radius(), 2);

	float root = pow(b, 2) - 4 * c;
	if (root >= 0.0f) {
		if (contact != nullptr) {
			float t = (-b - sqrt(root)) * 0.5f;
			contact->tag = tag;
			contact->distance = t;
			contact->hitPoint = P + D * t;
		}

		return true;
	}
    return false;
}

bool SphereCollider::IsBoxCollision(BoxCollider* collider)
{
    return collider->IsSphereCollision(this);
}

bool SphereCollider::IsSphereCollision(SphereCollider* collider)
{
	float distance = (GlobalPos() - collider->GlobalPos()).Length();
	return distance <= Radius() + collider->Radius();
}

bool SphereCollider::IsCapsuleCollision(CapsuleCollider* collider)
{

    return false;
}

void SphereCollider::MakeMesh()
{
	float phiStep = XM_PI / stackCount;
	float thetaStep = XM_2PI / sliceCount;

	vector<Vertex>& vertices = mesh->GetVertices();
	vertices.reserve((stackCount + 1) * (sliceCount + 1));
	for (UINT i = 0; i <= stackCount; i++) {
		float phi = i * phiStep;
		for (UINT j = 0; j <= sliceCount; j++) {
			float theta = thetaStep * j;

			Vertex vertex;
			vertex.pos.x = sin(phi) * cos(theta) * radius;
			vertex.pos.y = cos(phi) * radius;
			vertex.pos.z = sin(phi) * sin(theta) * radius;

			vertices.push_back(vertex);
		}
	}

	vector<UINT>& indices = mesh->GetIndices();
	indices.reserve(stackCount * sliceCount * 6);
	for (int i = 0; i < stackCount; i++) {
		for (int j = 0; j < sliceCount; j++) {
			indices.push_back((sliceCount + 1) * i + j);
			indices.push_back((sliceCount + 1) * i + (j + 1));
			indices.push_back((sliceCount + 1) * i + j);
			indices.push_back((sliceCount + 1) * (i + 1) + j);
			/*
			indices.push_back((sliceCount + 1) * (i + 1) + j);
			indices.push_back((sliceCount + 1) * i + (j + 1));
			indices.push_back((sliceCount + 1) * (i + 1) + (j + 1));
			*/
		}
	}
}
