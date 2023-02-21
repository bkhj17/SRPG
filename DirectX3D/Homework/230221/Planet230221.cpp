#include "framework.h"
#include "Planet230221.h"

Planet230221::Planet230221(wstring fileName, Vector3 size)
	: Cube230221(fileName, size)
{
}

void Planet230221::Update()
{
	Rot().y += DELTA * rotSpeedY;

	if (anchor) {
		Vector3 aPos = anchor->GlobalPos();

		float rot = rotSpeedA * DELTA;
		//r을 aPos를 중심으로 회전시켜야 한다
		Matrix MT = XMMatrixTranslation(farAnchor.x, farAnchor.y, farAnchor.z);
		Matrix MR = XMMatrixRotationY(rot);
		Matrix M = MT * MR;
		Vector4 outS, outR, outT;
		XMMatrixDecompose(&outS, &outR, &outT, M);

		Float3 ft;
		XMStoreFloat3(&ft, outT);
		
		farAnchor = ft;
		//공전 시의 위치
		Pos() = farAnchor + aPos;
	}
	
	UpdateWorld();
}