//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include <QString>
#include <QStringList>
#include "usersnames.h"

UsersNames::UsersNames() {

}

QString UsersNames::randomUserName() {
    static QStringList list = {
        QObject::tr("Forget bag"),
        QObject::tr("Warty Warthog"),
        QObject::tr("Hoary Hedgehog"),
        QObject::tr("Breezy Badger"),
        QObject::tr("Dapper Drake"),
        QObject::tr("Edgy Eft"),
        QObject::tr("Feisty Fawn"),
        QObject::tr("Gutsy Gibbon"),
        QObject::tr("Hardy Heron"),
        QObject::tr("Intrepid Ibex"),
        QObject::tr("Jaunty Jackalope"),
        QObject::tr("Karmic Koala"),
        QObject::tr("Lucid Lynx"),
        QObject::tr("Maverick Meerkat"),
        QObject::tr("Natty Narwhal"),
        QObject::tr("Oneiric Ocelot"),
        QObject::tr("Precise Pangolin"),
        QObject::tr("Quantal Quetzal"),
        QObject::tr("Raring Ringtail"),
        QObject::tr("Saucy Salamander"),
        QObject::tr("Trusty Tahr"),
        QObject::tr("Utopic Unicorn"),
        QObject::tr("Vivid Vervet"),
        QObject::tr("Wily Werewolf"),
        QObject::tr("Xenial Xerus"),
        QObject::tr("Yakkety Yak"),
        QObject::tr("Zesty Zapus"),
        QObject::tr("Artful Aardvark"),
        QObject::tr("Bionic Beaver"),
        QObject::tr("Cosmic Cuttlefish"),
        QObject::tr("Disco Dingo"),
        QObject::tr("Eoan Ermine"),
        QObject::tr("Focal Fossa"),
        QObject::tr("Groovy Gorilla"),
        QObject::tr("Hirsute Hippo"),
        QObject::tr("Impish Indri"),
        QObject::tr("Wardog"),
        QObject::tr("Blender"),
        QObject::tr("Grinder"),
        QObject::tr("Coffe Hiller"),
        QObject::tr("Hardy Hero"),
        QObject::tr("El Presidente"),
        QObject::tr("Defolt User"),
        QObject::tr("Eater Lord"),
        QObject::tr("Coffe User"),
        QObject::tr("Abstract Somethinc"),
        QObject::tr("Guitar hero"),
        QObject::tr("Coffe cat"),
        QObject::tr("Simple Dimple"),
        QObject::tr("Sushi Introvert"),
        QObject::tr("Crazy Hiller"),
        QObject::tr("Tower Boss"),
        QObject::tr("Crazy Coder"),
        QObject::tr("Loli Pop"),
        QObject::tr("Drifter Anarchist"),
        QObject::tr("Legendary Gamer"),
        QObject::tr("Just Try"),
        QObject::tr("Newbie Licker"),
        QObject::tr("Good Man"),
        QObject::tr("Cyber Punck"),
        QObject::tr("Cosmic Disaster"),
        QObject::tr("Super Power"),
        QObject::tr("Driver Taxy"),
        QObject::tr("Like Gorillaz"),
        QObject::tr("Horror Master"),
        QObject::tr("Cheat Giver"),
        QObject::tr("Lacky Client"),
        QObject::tr("Grate Boss"),
        QObject::tr("Hard Code"),
        QObject::tr("Coffe Giver"),
        QObject::tr("Common Worker")
    };

    return list[rand() % list.size()];
}
