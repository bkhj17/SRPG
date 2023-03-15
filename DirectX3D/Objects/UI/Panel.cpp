#include "Framework.h"
#include "Panel.h"

Panel::Panel(wstring bgTextureFile, wstring cancelBtnTextureFile)
	: Quad(Vector2(500.0f, 400.0f))
{
	SetTexture(Texture::Add(bgTextureFile));

	cancelButton = new Button(cancelBtnTextureFile);
	cancelButton->SetParent(this);
	cancelButton->Pos() = Half() - cancelButton->GetSize() * 0.5f;

	cancelButton->SetEvent(bind(&Panel::Close, this));

	collider = new RectCollider(size);
	collider->SetParent(this);

	isActive = false;
}

Panel::~Panel()
{
	delete cancelButton;
	delete collider;
}

void Panel::Update()
{
	if (!isActive)
		return;

	UpdateWorld();
	collider->UpdateWorld();
	cancelButton->UpdateWorld();
}

void Panel::Render()
{
	if (!isActive)
		return;

	__super::Render();
	cancelButton->Render();
	collider->Render();
}

void Panel::Show(Vector2 pos)
{
	Pos() = pos;
	isActive = true;
}

void Panel::Drag()
{
	cancelButton->Update();

	if (cancelButton->GetCollider()->IsPointCollision(mousePos)) return;
	if (!collider->IsPointCollision(mousePos)) return;
	//if (!isDrag) return;

	if (KEY_DOWN(VK_LBUTTON)) {
		offset = mousePos - Pos();
		isDrag = true;
	}

	if (isDrag && KEY_PRESS(VK_LBUTTON)) {
		Pos() = mousePos - offset;
	}

	if (KEY_UP(VK_LBUTTON)) {
		isDrag = false;
	}

}

void Panel::Close()
{
	isActive = false;
	UIManager::Get()->Hide(tag);
}
