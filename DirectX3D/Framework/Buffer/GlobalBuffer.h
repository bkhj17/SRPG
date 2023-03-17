#pragma once

class MatrixBuffer : public ConstBuffer
{
public:
    MatrixBuffer() : ConstBuffer(&matrix, sizeof(Matrix))
    {
        matrix = XMMatrixIdentity();
    }

    void Set(Matrix value)
    {
        matrix = XMMatrixTranspose(value);
    }

private:
    Matrix matrix;
};

class WorldBuffer : public ConstBuffer
{
private:
    struct Data {
        Matrix world = XMMatrixIdentity();

        int type = 0;

        float padding[3];
    };
public:
    WorldBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }

    void Set(Matrix value)
    {
        data.world = XMMatrixTranspose(value);
    }

    void SetType(int type) {
        data.type = type;
    }
private:
    Data data;
};

class ViewBuffer : public ConstBuffer
{
private:
    struct Data {
        Matrix view;
        Matrix invView;
    };
public:
    ViewBuffer() : ConstBuffer(&data, sizeof(Data))
    {
        data.view = XMMatrixIdentity();
        data.invView = XMMatrixIdentity();
    }

    void Set(Matrix view, Matrix invView)
    {
        data.view = XMMatrixTranspose(view);
        data.invView = XMMatrixTranspose(invView);
    }
private:
    Data data;
};

class ColorBuffer : public ConstBuffer
{
public:
    ColorBuffer() : ConstBuffer(&color, sizeof(Float4))
    {
    }

    Float4& Get() { return color; }

private:
    Float4 color = { 1, 1, 1, 1 };
};

class IntValueBuffer : public ConstBuffer
{
public:
    IntValueBuffer() : ConstBuffer(values, sizeof(int) * 4)
    {
    }

    int* Get() { return values; }

private:
    int values[4] = {};
};

class FloatValueBuffer : public ConstBuffer
{
public:
    FloatValueBuffer() : ConstBuffer(values, sizeof(float) * 4)
    {
    }

    float* Get() { return values; }

private:
    float values[4] = {};
};

class LightBuffer : public ConstBuffer
{
public:
    struct Light {
        Float4 color = { 1, 1, 1, 1 };
        Float3 direction = { 0, -1, 1 };
        int type = 0;

        Float3 pos = { 0,0,0 };
        float range = 100.0f;

        float inner = 55.0f;    //최소각도
        float outer = 70.0f;    //최대각도
        float length = 50.0f;   //
        int active = 1;         //가동여부
    };

    struct Data {
        Light lights[MAX_LIGHT];
        UINT lightCount = 1;
        Float3 ambientLight = { 0.3f, 0.3f, 0.3f };
        Float3 ambientCeil = { 0.3f, 0.3f, 0.3f };
        float padding = 0.0f;
    };
public:
    LightBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }

    Data& Get() { return data; }
private:
    Data data;
};