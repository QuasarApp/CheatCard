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

void SoundPlayback::initPlaylist() {

    _playlist = new QMediaPlaylist;
    _positionSound = {{"Bonus", 1},
                      {"Seal", 2}};

    _playlist->addMedia(QUrl("/resources/sounds/Bonus.mp3"));
    _playlist->addMedia(QUrl("/resources/sounds/Seal.mp3"));

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
