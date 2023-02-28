#include "framework.h"
#include "RawBuffer.h"

RawBuffer::RawBuffer(UINT byteWidth)
	: byteWidth(byteWidth)
{
	CreateOutput(); 
	CreateUAV(); 
	CreateResult();
}

RawBuffer::~RawBuffer()
{
	output->Release();
	uav->Release();
	result->Release();
}

void RawBuffer::Copy(void* data, UINT size)
{
	DC->CopyResource(result, output);

	D3D11_MAPPED_SUBRESOURCE subResource;

	DC->Map(result, 0, D3D11_MAP_READ, 0, &subResource);	//잠시 동안 GPU 메모리에 접근 허용 -> GPU는 작업을 못 하게 된다(잠김. 본사시찰)
	memcpy(data, subResource.pData, size);					
	DC->Unmap(result, 0);									//Map 해제. 필수! 안 하면 GPU 동작 못 함
}

void RawBuffer::CreateOutput()
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;	//uordered_view에 접근 가능
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	DEVICE->CreateBuffer(&bufferDesc, nullptr, &output);
}

void RawBuffer::CreateUAV()
{
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
	uavDesc.Buffer.NumElements = byteWidth / 4;

	DEVICE->CreateUnorderedAccessView(output, &uavDesc, &uav);
}

void RawBuffer::CreateResult()
{
	D3D11_BUFFER_DESC bufferDesc = {};
	output->GetDesc(&bufferDesc);	//output desc 복사

	bufferDesc.Usage = D3D11_USAGE_STAGING;
	bufferDesc.BindFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	DEVICE->CreateBuffer(&bufferDesc, nullptr, &result);
}