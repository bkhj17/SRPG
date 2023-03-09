#include "framework.h"

ModelClip::~ModelClip()
{
    for (auto keyFrame : keyFrames)
        delete keyFrame.second;
    keyFrames.clear();
}

KeyFrame* ModelClip::GetKeyFrame(string boneName)
{
    if(keyFrames.count(boneName) == 0)
        return nullptr;
    return keyFrames[boneName];
}
