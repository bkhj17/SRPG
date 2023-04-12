#pragma once

class IndexBuffer
{
public:
    IndexBuffer(void* data, UINT count);
    ~IndexBuffer();

    void Set();

    void Update(void* data, UINT count);

    void SetDebugName(string name = "");
private:
    ID3D11Buffer* buffer;
};