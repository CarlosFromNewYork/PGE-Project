/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2016 Vitaly Novichkov <admin@wohlnet.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LVL_LAYER_H
#define LVL_LAYER_H

#include <QHash>
#include "lvl_base_object.h"

class LevelScene;
class LVL_LayerEngine
{
    friend class LevelScene;
    LevelScene * _scene;
public:
    LVL_LayerEngine(LevelScene *_parent=NULL);
    void hide(QString layer, bool smoke=true);
    void show(QString layer, bool smoke=true);
    void toggle(QString layer, bool smoke=true);
    void registerItem(QString layer, PGE_Phys_Object* item);
    void removeRegItem(QString layer, PGE_Phys_Object* item);
    bool isEmpty(QString layer);
    QHash<QString, QList<PGE_Phys_Object* > > members;
};

#endif // LVL_LAYER_H
