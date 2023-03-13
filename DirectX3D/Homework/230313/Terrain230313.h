#pragma once

class Terrain230313 : public Terrain
{
protected:
	class RayBuffer : public ConstBuffer {
	private:
		struct Data {
			Float3 pos;
			UINT triangleSize; //Æú¸®°ï °³¼ö
			Float3 dir;

			float padding;
		};
	public:
		RayBuffer() : ConstBuffer(&data, sizeof(Data)) {}

		Data& Get() { return data; }
	private:
		Data data;
	};

	struct InputDesc {
		Float3 v0, v1, v2;
	};

	struct OutputDesc {
		int picked;
		float distance;
	};
public:
	Terrain230313();
	~Terrain230313();

	bool ComputePicking(Vector3& pos);
private:
	void MakeComputeData();
private:

	UINT triangleSize;

	RayBuffer* rayBuffer;
	StructuredBuffer* structuredBuffer;
	vector<InputDesc> inputs;
	vector<OutputDesc> outputs;

	ComputeShader* computeShader;
};

