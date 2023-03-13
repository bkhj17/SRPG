#include "framework.h"

ModelClip::~ModelClip()
{
    for (auto keyFrame : keyFrames)
        delete keyFrame.second;
    keyFrames.clear();
}

void ModelClip::Init()
{
    eventIter = events.begin();
    playTime = 0.0f;
    duration = frameCount / tickPerSecond;
}

void ModelClip::Execute()
{
    
    if (events.empty()) return;             //실행할 게 없다
    if (eventIter == events.end()) return;  //남은 이벤트가 없다

    float ratio = playTime / duration;

    if (eventIter->first > ratio) return;  //

    eventIter->second();
    eventIter++;
}

KeyFrame* ModelClip::GetKeyFrame(string boneName)
{
    if(keyFrames.count(boneName) == 0)
        return nullptr;
    return keyFrames[boneName];
}

void ModelClip::SetEvent(Event event, float timeRatio)
{
    if (events.count(timeRatio) > 0) return;
    events[timeRatio] = event;
}
