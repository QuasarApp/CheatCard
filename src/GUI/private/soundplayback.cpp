//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "soundplayback.h"

namespace RC {

SoundPlayback::SoundPlayback() {

}

SoundPlayback::~SoundPlayback() {
    delete _player;
    delete _playlist;
}

void SoundPlayback::initPlaylist() {

    if (!_playlist)
        return;

    auto add = [this](const QString& name, const QString& path) {

        _positionSound[name] = _playlist->mediaCount();
        _playlist->addMedia(QUrl::fromLocalFile(path));;
    };

    _playlist = new QMediaPlaylist;
    add("Bonus", "qrc:/private/resources/sounds/Bonus.mp3");
    add("Seal", "qrc:/private/resources/sounds/Seal.mp3");

    _player = new QMediaPlayer;
    _player->setPlaylist(_playlist);
}

void SoundPlayback::playSound(const QString &name) {

    if (!_player)
        initPlaylist();

    _playlist->setCurrentIndex(_positionSound[name]);
    _player->play();

}

}
