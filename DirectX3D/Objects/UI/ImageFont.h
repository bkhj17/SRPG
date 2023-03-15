#pragma once
class ImageFont : public Transform
{
private:
	const UINT MAX_LENGTH = 1000;

public:
	enum AlignedType {
		LEFT, CENTER, RIGHT
	};

	
public:
	ImageFont(wstring textureFile, float interval = 1.0f);
	~ImageFont();

	void Update();
	void Render();

	void SetValue(UINT value) { this->value = value; }
	void SetAligned(AlignedType type) { alignedType = type; }
private:
	UINT value = 0;
	string strValue;

	vector<Quad*> quads;
	vector<Texture*> textures;
	IntValueBuffer* valueBuffer;

	float interval;
	AlignedType alignedType = LEFT;
	Vector2 fontSize;
};

