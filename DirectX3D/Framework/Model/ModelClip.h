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

	unordered_map<string, KeyFrame*> keyFrames;
};