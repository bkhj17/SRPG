#pragma once
class ModelClip {
private:
	friend class ModelAnimator;

	ModelClip() = default;
	~ModelClip();

	KeyFrame* GetKeyFrame(string boneName);

private:
	string name;

	UINT frameCount;
	float tickPerSecond;
	float duration;

	float playTime = 0.0f;

	unordered_map<string, KeyFrame*> keyFrames;
};