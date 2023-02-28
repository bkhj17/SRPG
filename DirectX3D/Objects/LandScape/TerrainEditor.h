#pragma once
class TerrainEditor : public GameObject
{
private:
	typedef VertexUVNormalTangent VertexType;
	const float MAX_HEIGHT = 20.0f;

	const UINT MIN_SIZE = 2;
	const UINT MAX_SIZE = 100;


	class BrushBuffer : public ConstBuffer {
	private:
		struct Data {
			int type = 0;
			Float3 pickingPos;

			float range = 10.0f;
			Float3 color = { 0, 1, 0 };
		};
	public:
		BrushBuffer() : ConstBuffer(&data, sizeof(Data)) {}

		Data& Get() { return data; }
	private:
		Data data;
	};

	class RayBuffer : public ConstBuffer {
	private:
		struct Data {
			Float3 pos;
			float triangleSize; //Æú¸®°ï °³¼ö

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
	TerrainEditor();
	virtual ~TerrainEditor();

	virtual void Update();
	void Render() override;
	void RenderUI() override;

	Vector3 Picking();
	Vector3 ComputePicking();
protected:
	void MakeMesh();
	void MakeNormal();
	void MakeTangent();

	void MakeComputeData();

	void Resize();
protected:
	UINT width, height;
	UINT triangleSize;

	Mesh<VertexType>* mesh;
	BrushBuffer* brushBuffer;

	RayBuffer* rayBuffer;
	StructuredBuffer* structuredBuffer;
	vector<InputDesc> inputs;
	vector<OutputDesc> outputs;


	Texture* heightMap;

	ComputeShader* computeShader;

};

