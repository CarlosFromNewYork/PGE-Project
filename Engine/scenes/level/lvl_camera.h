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

#ifndef LVL_CAMERA_H
#define LVL_CAMERA_H

#include "lvl_base_object.h"
#include <graphics/graphics.h>
#include <common_features/fader.h>
#include <common_features/rectf.h>

#include <QList>
#include <PGE_File_Formats/file_formats.h>

class   PGE_Phys_Object;
typedef QVector<PGE_Phys_Object *> PGE_RenderList;

class LVL_Background;
class LVL_Section;

class PGE_LevelCamera
{
    friend class LVL_Background;
    friend class LVL_Section;
    static bool _TreeSearchCallback(PGE_Phys_Object* item, void* arg);
    void queryItems(PGE_RectF &zone);
public:
    static const float _smbxTickTime;
    PGE_LevelCamera();
    PGE_LevelCamera(const PGE_LevelCamera &cam);
    ~PGE_LevelCamera();
    int w(); //!< Width
    int h(); //!< Height
    qreal posX(); //!< Position X
    qreal posY(); //!< Position Y

    void init(float x, float y, float w, float h);

    void setPos(float x, float y);
    void setCenterPos(float x, float y);
    void setSize(int w, int h);
    void setOffset(int x, int y);
    void update(float ticks);
    void drawBackground();
    void drawForeground();

    void changeSection(LVL_Section *sct, bool isInit=false);
    void changeSectionBorders(long left, long top, long right, long bottom);
    void resetLimits();

    //PGE_RenderList &renderObjects();
    PGE_Phys_Object**renderObjects_arr();
    int              renderObjects_max();
    int              renderObjects_count();
    void             setRenderObjects_count(int count);

    LevelSection* section;
    LVL_Section * cur_section;

    PGE_RectF posRect;

    float offset_x;
    float offset_y;

    /**************Fader**************/
    PGE_Fader fader;
    /**************Fader**************/

    /**************Autoscrool**************/
    void resetAutoscroll();
    void processAutoscroll(float tickTime);
    bool  isAutoscroll;
    float _autoscrollVelocityX_max;
    float _autoscrollVelocityY_max;
    float _autoscrollVelocityX;
    float _autoscrollVelocityY;
    PGE_RectF limitBox;
    /**************Autoscrool**************/
private:
    void _applyLimits();
    void sortElements();
    //PGE_RenderList     objects_to_render;

    PGE_Phys_Object** _objects_to_render;
    int               _objects_to_render_max;
    int               _objects_to_render_stored;
    int               _objects_to_render_recent;
};

#endif // LVL_CAMERA_H
