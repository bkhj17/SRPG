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
private:
    struct Light {
        Float4 color = { 1, 1, 1, 1 };
        Float3 direction = { 0, -1, 1 };
        float shininess = 24.0f;
    };

    struct Data {
        Light lights[MAX_LIGHT];
        UINT lightCount = 1;
        Float3 ambientLight = { 0.1f, 0.1f, 0.1f };
        Float3 ambientCeil = { 0.1f, 0.1f, 0.1f };
        float padding;
    };
public:
    LightBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }

    Data& Get() { return data; }
private:
    Data data;
};