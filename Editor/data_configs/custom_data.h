/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2014 Vitaly Novichkov <admin@wohlnet.ru>
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

#ifndef CUSTOM_DATA_H
#define CUSTOM_DATA_H

#include <QPixmap>
#include <QBitmap>

struct UserBGOs
{
    QPixmap image;
    QPixmap mask;
    unsigned long id;
};

struct UserBlocks
{
    QPixmap image;
    QPixmap mask;
    unsigned long id;
};

struct UserNPCs
{
    bool withImg;
    QPixmap image;
    QPixmap mask;
    unsigned long id;

    bool withTxt;
    NPCConfigFile sets;
    obj_npc merged;
};

struct UserBGs
{
    QPixmap image;
    QPixmap second_image;
    unsigned long id;
    unsigned int q;//0 - only first; 1 - only second; 2 - fitst and seconf
};


#endif // CUSTOM_DATA_H
