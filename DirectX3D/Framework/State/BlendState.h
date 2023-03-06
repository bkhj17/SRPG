#pragma once
class BlendState
{
public:
	BlendState();
	~BlendState();

	void SetState();

	void Alpha(bool value);
	void Additive();
	void AlphaToCoverage(bool value);
	//3D에서는 연산 순서가 아닌 깊이를 통해 렌더링을 한다 => 컴퓨터가 알아서 하기 힘들다

	void Changed();

private:
	D3D11_BLEND_DESC desc = {};
	ID3D11BlendState* state = nullptr;
};

