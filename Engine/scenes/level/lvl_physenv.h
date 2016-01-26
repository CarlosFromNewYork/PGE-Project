/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2015 Vitaly Novichkov <admin@wohlnet.ru>
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

#ifndef LVL_PHYSENV_H
#define LVL_PHYSENV_H

#include "lvl_base_object.h"
#include <PGE_File_Formats/file_formats.h>

class LVL_PhysEnv : public PGE_Phys_Object
{
public:
    LVL_PhysEnv(LevelScene *_parent=NULL);
    ~LVL_PhysEnv();

    void init();
    static const int numOfEnvironments;
    enum EnvType
    {
        Env_SameAsAround = -1,
        Env_Air = 0,
        Env_Water,
        Env_Quicksand
    };
    int env_type;

    LevelPhysEnv data;
};

#endif // LVL_PHYSENV_H
