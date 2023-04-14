//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef SOUNDPLAYBACK_H
#define SOUNDPLAYBACK_H

#include <QHash>
#include <QString>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "CheatCardGui/gui_global.h"

namespace RC {

class CHEATCARD_GUI_EXPORT SoundPlayback
{
public:
    SoundPlayback();
    ~SoundPlayback();

    void playSound(const QString &sound);

private:

    void initPlaylist();

    QHash<QString, int> _positionSound;
    QMediaPlaylist *_playlist = nullptr;
    QMediaPlayer *_player = nullptr;
};

}

#endif // SOUNDPLAYBACK_H
