#include "Framework.h"
#include "UIWindow.h"

UIWindow::UIWindow(Vector2 size, Vector3 pos)
	: Quad(size)
{
	localPosition = pos;

	isActive = false;
	UpdateWorld();
	SetShaderInfo();

	CreateCursor(DEFAULT_CURSOR_SIZE);
}

UIWindow::UIWindow(float left, float right, float up, float down)
	: Quad(Vector2(right - left, up - down))
{
	localPosition = { (right + left) / 2, (up + down) / 2 };

	isActive = false;
	UpdateWorld();
	SetShaderInfo();

	CreateCursor(DEFAULT_CURSOR_SIZE);
}

UIWindow::~UIWindow()
{
	delete floatBuffer;
	delete cQuad;
}

void UIWindow::Init()
{
	isActive = true;
	cursor = 0;
}

void UIWindow::Update()
{
	if (!Active() || !renderCursor)
		return;

	if (KEY_DOWN(VK_DOWN))
		cursor = (++cursor) % maxCursor;

	if (KEY_DOWN(VK_UP))
		cursor = cursor == 0 ? maxCursor - 1 : cursor-1;

	if (KEY_DOWN('X'))
		Close();

	UpdateWorld();
}

void UIWindow::RenderCursor()
{
	if (!Active() || !renderCursor)
		return;

	cQuad->Render();
}

void UIWindow::Render()
{
	floatBuffer->SetPS(10);

	__super::Render();
}

void UIWindow::Close()
{
	isActive = false;
}

void UIWindow::SetShaderInfo()
{
	material->SetDiffuseMap(Texture::Add(L"Textures/SRPG/Blueframe.png"));
	material->SetShader(L"SRPG/WindowFrame.hlsl");

	floatBuffer = new FloatValueBuffer;
	floatBuffer->Get()[0] = BORDER_WEIGHT.x;
	floatBuffer->Get()[1] = BORDER_WEIGHT.y;
	floatBuffer->Get()[2] = INNER_ALPHA;
}

void UIWindow::CreateCursor(Vector2 size)
{
	cursorSize = size;
	cQuad = new Quad(size);
	cQuad->GetMaterial()->SetDiffuseMap(L"Textures/SRPG/Cursor.png");
	cQuad->SetParent(this);
}
