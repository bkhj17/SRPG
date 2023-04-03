#pragma once
class UIWindow : public Quad
{
private:
    const Vector2 DEFAULT_CURSOR_SIZE = { 20.0f, 20.0f };

    const Vector2 BORDER_WEIGHT = { 0.05f, 0.05f };
    const float INNER_ALPHA = 0.8f;

public:
    UIWindow(Vector2 size, Vector3 pos, bool renderCursor = false);
    UIWindow(float left, float right, float up, float down);
    virtual ~UIWindow();

    virtual void Init();
    virtual void Update();
    virtual void RenderCursor();
    virtual void Render() override;
    virtual void Close();

    bool IsMapControl() { return mapControl; }
    virtual void Control();

private:
    void SetShaderInfo();
    void CreateCursor(Vector2 size);

protected:
    virtual void ActiveFunc() {}
    virtual void CancelFunc() { Close(); }
protected:
    FloatValueBuffer* floatBuffer;

    Vector2 cursorSize;
    Quad* cQuad = nullptr;

    bool renderCursor = false;
    int cursor = 0;
    int maxCursor = 1;

    bool mapControl = false;        //맵 커서 움직이는 녀석인지 여부
};

