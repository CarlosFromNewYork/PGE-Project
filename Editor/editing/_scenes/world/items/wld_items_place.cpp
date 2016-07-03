/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2014-2016 Vitaly Novichkov <admin@wohlnet.ru>
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

#include <common_features/grid.h>
#include <common_features/main_window_ptr.h>
#include <editing/edit_world/world_edit.h>
#include <PGE_File_Formats/file_formats.h>

#include "../wld_scene.h"
#include "item_tile.h"
#include "item_scene.h"
#include "item_path.h"
#include "item_level.h"
#include "item_music.h"
#include "item_point.h"




void WldScene::placeTile(WorldTiles &tile, bool toGrid)
{
    obj_w_tile &mergedSet = uTiles[tile.id];
    long animator = mergedSet.animator_id;
    if(!mergedSet.isValid)
    {
        animator = 0;
        mergedSet = pConfigs->main_wtiles[1];
        mergedSet.image = uTileImg;
    }

    QPoint newPos = QPoint(tile.x, tile.y);
    if(toGrid)
    {
        newPos = applyGrid(QPoint(tile.x, tile.y), mergedSet.grid);
        tile.x = newPos.x();
        tile.y = newPos.y();
    }

    ItemTile *TileItem = new ItemTile(this);
    TileItem->setTileData(tile, &mergedSet, &animator);

    TileItem->setFlag(QGraphicsItem::ItemIsSelectable, (!lock_tile));
    TileItem->setFlag(QGraphicsItem::ItemIsMovable, (!lock_tile));

    if(PasteFromBuffer) TileItem->setSelected(true);
}





void WldScene::placeScenery(WorldScenery &scenery, bool toGrid)
{
    obj_w_scenery &mergedSet = uScenes[scenery.id];
    long animator = mergedSet.animator_id;
    if(!mergedSet.isValid)
    {
        animator = 0;
        mergedSet = pConfigs->main_wscene[1];
        mergedSet.image = uSceneImg;
    }

    QPoint newPos = QPoint(scenery.x, scenery.y);
    if(toGrid)
    {
        newPos = applyGrid(QPoint(scenery.x, scenery.y), mergedSet.grid);
        scenery.x = newPos.x();
        scenery.y = newPos.y();
    }

    ItemScene *SceneItem = new ItemScene(this);
    SceneItem->setSceneData(scenery, &mergedSet, &animator);

    SceneItem->setFlag(QGraphicsItem::ItemIsSelectable, (!lock_scene));
    SceneItem->setFlag(QGraphicsItem::ItemIsMovable, (!lock_scene));

    if(PasteFromBuffer) SceneItem->setSelected(true);
}





void WldScene::placePath(WorldPaths &path, bool toGrid)
{
    obj_w_path &mergedSet = uPaths[path.id];
    long animator = mergedSet.animator_id;
    if(!mergedSet.isValid)
    {
        animator = 0;
        mergedSet = pConfigs->main_wpaths[1];
        mergedSet.image = uPathImg;
    }

    QPoint newPos = QPoint(path.x, path.y);
    if(toGrid)
    {
        newPos = applyGrid(QPoint(path.x, path.y), mergedSet.grid);
        path.x = newPos.x();
        path.y = newPos.y();
    }

    ItemPath *PathItem = new ItemPath(this);
    PathItem->setPathData(path, &mergedSet, &animator);

    PathItem->setOpacity(opts.semiTransparentPaths ? 0.5 : 1);

    PathItem->setFlag(QGraphicsItem::ItemIsSelectable, (!lock_path));
    PathItem->setFlag(QGraphicsItem::ItemIsMovable, (!lock_path));

    if(PasteFromBuffer) PathItem->setSelected(true);
}





void WldScene::placeLevel(WorldLevels &level, bool toGrid)
{
    long animator=0, pathAnimator=0, bPathAnimator=0;
    obj_w_level &mergedSet = uLevels[level.id];
    animator =      mergedSet.animator_id;
    if(!mergedSet.isValid)
    {
        animator = 0;
        mergedSet = pConfigs->main_wlevels[0];
        mergedSet.image = uLevelImg;
    }
    pathAnimator =  uLevels[pConfigs->marker_wlvl.path].animator_id;
    bPathAnimator = uLevels[pConfigs->marker_wlvl.bigpath].animator_id;

    QPoint newPos = QPoint(level.x, level.y);
    if(toGrid)
    {
        newPos = applyGrid(QPoint(level.x, level.y), mergedSet.grid);
        level.x = newPos.x();
        level.y = newPos.y();
    }

    ItemLevel *LevelItem = new ItemLevel(this);

    LevelItem->setLevelData(level, &mergedSet, &animator, &pathAnimator, &bPathAnimator);

    LevelItem->setFlag(QGraphicsItem::ItemIsSelectable, (!lock_level));
    LevelItem->setFlag(QGraphicsItem::ItemIsMovable, (!lock_level));

    if(PasteFromBuffer) LevelItem->setSelected(true);
}





void WldScene::placeMusicbox(WorldMusic &musicbox, bool toGrid)
{
    int j = pConfigs->getMusWldI(musicbox.id);

    ItemMusic *MusicBoxItem = new ItemMusic(this);

    QPoint newPos = QPoint(musicbox.x, musicbox.y);
    if(toGrid)
    {
        newPos = applyGrid(QPoint(musicbox.x, musicbox.y), MusicBoxItem->getGridSize());
        musicbox.x = newPos.x();
        musicbox.y = newPos.y();
    }

    MusicBoxItem->setMusicData(musicbox);
    if(j>=0)
    {
        MusicBoxItem->m_musicTitle =
                (pConfigs->music_w_custom_id==musicbox.id) ?
                    musicbox.music_file:
                    pConfigs->main_music_wld[j].name;
    }

    MusicBoxItem->setFlag(QGraphicsItem::ItemIsSelectable, (!lock_musbox));
    MusicBoxItem->setFlag(QGraphicsItem::ItemIsMovable, (!lock_musbox));

    if(PasteFromBuffer) MusicBoxItem->setSelected(true);
}



