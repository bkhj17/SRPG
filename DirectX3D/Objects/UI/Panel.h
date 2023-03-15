#pragma once

class Panel : public Quad
{
private:
	friend class UIManager;
public:
	Panel(wstring bgTextureFile, wstring cancelBtnTextureFile);
	~Panel();

	void Update();
	void Render();

	void Show(Vector2 pos);

	RectCollider* GetCollider() { return collider; }

	void SetIsDrag(bool value) { isDrag = value; }

	void Drag();
private:
	void Close();

private:
	Button* cancelButton;
	RectCollider* collider;

	bool isDrag = false;
	Vector2 offset;
};

