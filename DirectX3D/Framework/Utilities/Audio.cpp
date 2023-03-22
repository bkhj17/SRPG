#include "Framework.h"

Audio::Audio()
{
    System_Create(&soundSystem);
    soundSystem->init(MAX_CHANNEL, FMOD_INIT_NORMAL, nullptr);

    soundSystem->set3DSettings(1.0f, 100.0f, 0.01f);
}

Audio::~Audio()
{
    for (pair<string, SoundInfo*> sound : sounds)
        delete sound.second;

    soundSystem->release();
}

void Audio::Update()
{
    FMOD_VECTOR listenerPos = { CAM->Pos().x, CAM->Pos().y, CAM->Pos().z };

    soundSystem->set3DListenerAttributes(0, &listenerPos, nullptr, nullptr, nullptr);

    soundSystem->update();
}

void Audio::Add(string key, string file, bool bgm, bool loop)
{
    if (sounds.count(key) > 0) return;

    SoundInfo* info = new SoundInfo();

    if (bgm)
        soundSystem->createStream(file.c_str(),
            FMOD_LOOP_NORMAL, nullptr, &info->sound);
    else
    {
        if (loop) 
            soundSystem->createSound(file.c_str(), FMOD_LOOP_NORMAL, nullptr, &info->sound);
        else 
            soundSystem->createSound(file.c_str(), FMOD_DEFAULT, nullptr, &info->sound);
    }

    sounds[key] = info;
}

void Audio::Play(string key, float volume)
{
    if (sounds.count(key) == 0) return;

    soundSystem->playSound(sounds[key]->sound, nullptr, false, &sounds[key]->channel);
    sounds[key]->channel->setVolume(volume);
}

void Audio::Play(string key, Float3 position, float volume) 
{
    if (sounds.count(key) == 0) return;

    soundSystem->playSound(sounds[key]->sound, nullptr, false, &sounds[key]->channel);
    sounds[key]->channel->setVolume(volume);
    
    FMOD_VECTOR audioPos = { position.x, position.y, position.z };
    
    sounds[key]->channel->set3DAttributes(&audioPos, nullptr);
    sounds[key]->channel->set3DMinMaxDistance(1.0f, 10000.0f);
}

void Audio::Stop(string key)
{
    if (sounds.count(key) == 0) return;

    sounds[key]->channel->stop();
}

void Audio::Pause(string key)
{
    if (sounds.count(key) == 0) return;

    sounds[key]->channel->setPaused(true);
}

void Audio::Resume(string key)
{
    if (sounds.count(key) == 0) return;

    sounds[key]->channel->setPaused(false);
}

bool Audio::IsPlaySound(string key)
{
    if (sounds.count(key) == 0) return false;

    bool isPlay = false;
    sounds[key]->channel->isPlaying(&isPlay);

    return isPlay;
}
