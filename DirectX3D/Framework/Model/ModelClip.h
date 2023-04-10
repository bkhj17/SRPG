#pragma once
class ModelClip {
private:
	friend class ModelAnimator;
	friend class ModelAnimatorInstancing;

	ModelClip() = default;
	~ModelClip();

	void Init();
	void Execute();

	KeyFrame* GetKeyFrame(string boneName);

public:
	void SetEvent(Event event, float timeRatio);
private:
	string name;
	string lockBone;

	UINT frameCount = 0;
	float tickPerSecond = 0.0f;
	float duration = 0.0f;

	float playTime = 0.0f;

	unordered_map<string, KeyFrame*> keyFrames;

	map<float, Event>::iterator eventIter;
	map<float, Event> events;	//0부터 1사이에 정규화된 시간
};