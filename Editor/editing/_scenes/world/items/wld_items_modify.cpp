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
#include <common_features/item_rectangles.h>
#include <editing/edit_world/world_edit.h>
#include <editing/_scenes/world/wld_item_placing.h>

#include "../wld_scene.h"
#include "item_tile.h"
#include "item_scene.h"
#include "item_path.h"
#include "item_level.h"
#include "item_music.h"
#include "item_point.h"

void WldScene::applyArrayForItemGroup(QList<QGraphicsItem * >items)
{
    foreach(QGraphicsItem * it, items)
    {
        if(it) applyArrayForItem(it);
    }
}

///
/// \brief WldScene::applyArrayForItem
/// \param item
/// Register all changes of item into the main data arrays
///
void WldScene::applyArrayForItem(QGraphicsItem * item)
{
    if(!item) return;
    if(!item->data(ITEM_IS_ITEM).isNull())
    {
        dynamic_cast<WldBaseItem *>(item)->arrayApply();
    }
}

void WldScene::collectDataFromItem(WorldData &dataToStore, QGraphicsItem *item)
{
    if(!item) return;

    QString ObjType = item->data(ITEM_TYPE).toString();
    if( ObjType == "TILE")
    {
        dataToStore.tiles << dynamic_cast<ItemTile *>(item)->m_data;
    }
    else
    if( ObjType == "SCENERY")
    {
        dataToStore.scenery << dynamic_cast<ItemScene *>(item)->m_data;
    }
    else
    if( ObjType == "PATH")
    {
        dataToStore.paths << dynamic_cast<ItemPath *>(item)->m_data;
    }
    else
    if( ObjType == "LEVEL")
    {
        dataToStore.levels << dynamic_cast<ItemLevel *>(item)->m_data;
    }
    else
    if( ObjType == "MUSICBOX" )
    {
        dataToStore.music << dynamic_cast<ItemMusic *>(item)->m_data;
    }
}

void WldScene::collectDataFromItems(WorldData &dataToStore, QList<QGraphicsItem *> items)
{
    foreach (QGraphicsItem* item, items) {
        collectDataFromItem(dataToStore, item);
    }
}


void WldScene::returnItemBackGroup(QList<QGraphicsItem * >items)
{
    foreach(QGraphicsItem * it, items)
    {
        if(it) returnItemBack(it);
    }
}

void WldScene::returnItemBack(QGraphicsItem * item)
{
    if(!item) return;

    if(!item->data(ITEM_IS_ITEM).isNull())
    {
        dynamic_cast<WldBaseItem *>(item)->returnBack();
    }
}





void WldScene::placeItemsByRectArray()
{
    //using namespace wld_control;
    //This function placing items by yellow rectangles
    if(item_rectangles::rectArray.isEmpty()) return;

    QGraphicsItem * backup = cursor;
    while(!item_rectangles::rectArray.isEmpty())
    {
        cursor = item_rectangles::rectArray.first();
        item_rectangles::rectArray.pop_front();

        foreach(dataFlag_w flag, WldPlacingItems::flags)
            cursor->setData(flag.first, flag.second);

        placeItemUnderCursor();

        if(cursor) delete cursor;
    }
    cursor = backup;
    cursor->hide();

    if(!overwritedItems.tiles.isEmpty()||
        !overwritedItems.scenery.isEmpty()||
        !overwritedItems.paths.isEmpty()||
        !overwritedItems.levels.isEmpty()||
        !overwritedItems.music.isEmpty() )
    {
        addOverwriteHistory(overwritedItems, placingItems);
        overwritedItems.tiles.clear();
        overwritedItems.scenery.clear();
        overwritedItems.paths.clear();
        overwritedItems.levels.clear();
        overwritedItems.music.clear();
        placingItems.tiles.clear();
        placingItems.paths.clear();
        placingItems.scenery.clear();
        placingItems.levels.clear();
        placingItems.music.clear();
    }
    else
    if(!placingItems.tiles.isEmpty()||
            !placingItems.paths.isEmpty()||
            !placingItems.scenery.isEmpty()||
            !placingItems.levels.isEmpty()||
            !placingItems.music.isEmpty()){
        addPlaceHistory(placingItems);
        placingItems.tiles.clear();
        placingItems.paths.clear();
        placingItems.scenery.clear();
        placingItems.levels.clear();
        placingItems.music.clear();
    }

}

void WldScene::placeItemUnderCursor()
{
    //using namespace wld_control;
    bool wasPlaced=false;

    if(WldPlacingItems::overwriteMode)
    {   //remove all colliaded items before placing
        QGraphicsItem * xxx;
        while( (xxx=itemCollidesWith(cursor)) != NULL )
        {
            bool removed=false;
            if(xxx->data(ITEM_TYPE).toString()=="TILE")
            {
                if(xxx->data(ITEM_ARRAY_ID).toLongLong()>last_tile_arrayID) break;
                overwritedItems.tiles.push_back( ((ItemTile *)xxx)->m_data );
                ((ItemTile *)xxx)->removeFromArray();
                delete xxx; removed=true;
            }
            else
            if(xxx->data(ITEM_TYPE).toString()=="SCENERY")
            {
                if(xxx->data(ITEM_ARRAY_ID).toLongLong()>last_scene_arrayID) break;
                overwritedItems.scenery.push_back( ((ItemScene *)xxx)->m_data );
                ((ItemScene *)xxx)->removeFromArray();
                delete xxx; removed=true;
            }
            else
            if(xxx->data(ITEM_TYPE).toString()=="PATH")
            {
                if(xxx->data(ITEM_ARRAY_ID).toLongLong()>last_path_arrayID) break;
                overwritedItems.paths.push_back( ((ItemPath *)xxx)->m_data );
                ((ItemPath *)xxx)->removeFromArray();
                delete xxx; removed=true;
            }
            else
            if(xxx->data(ITEM_TYPE).toString()=="LEVEL")
            {
                if(xxx->data(ITEM_ARRAY_ID).toLongLong()>last_level_arrayID) break;
                overwritedItems.levels.push_back( ((ItemLevel *)xxx)->m_data );
                ((ItemLevel *)xxx)->removeFromArray();
                delete xxx; removed=true;
            }
            else
            if(xxx->data(ITEM_TYPE).toString()=="MUSICBOX")
            {
                if(xxx->data(ITEM_ARRAY_ID).toLongLong()>last_musicbox_arrayID) break;
                overwritedItems.music.push_back( ((ItemMusic *)xxx)->m_data );
                ((ItemMusic *)xxx)->removeFromArray();
                delete xxx; removed=true;
            }

            if(removed) //Remove pointer of deleted item
            {
                if(collisionCheckBuffer.contains(xxx))
                    collisionCheckBuffer.removeAt(collisionCheckBuffer.indexOf(xxx));
            }
        }
    }

//    QList<QGraphicsItem *> * checkZone;
//    if(collisionCheckBuffer.isEmpty())
//        checkZone = NULL;
//    else
//        checkZone = &collisionCheckBuffer;

    if( itemCollidesWith(cursor) )
    {
        return;
    }
    else
    {
        if(placingItem == PLC_Tile)
        {
            WldPlacingItems::TileSet.x = cursor->scenePos().x();
            WldPlacingItems::TileSet.y = cursor->scenePos().y();

            WldData->tile_array_id++;
            WldPlacingItems::TileSet.meta.array_id = WldData->tile_array_id;

            WldData->tiles.push_back(WldPlacingItems::TileSet);
            placeTile(WldPlacingItems::TileSet, true);
            placingItems.tiles.push_back(WldPlacingItems::TileSet);
            wasPlaced=true;
        }
        else
        if(placingItem == PLC_Scene)
        {
            WldPlacingItems::SceneSet.x = cursor->scenePos().x();
            WldPlacingItems::SceneSet.y = cursor->scenePos().y();

            WldData->scene_array_id++;
            WldPlacingItems::SceneSet.meta.array_id = WldData->scene_array_id;

            WldData->scenery.push_back(WldPlacingItems::SceneSet);
            placeScenery(WldPlacingItems::SceneSet, true);
            placingItems.scenery.push_back(WldPlacingItems::SceneSet);
            wasPlaced=true;
        }
        else
        if(placingItem == PLC_Path)
        {
            WldPlacingItems::PathSet.x = cursor->scenePos().x();
            WldPlacingItems::PathSet.y = cursor->scenePos().y();

            WldData->path_array_id++;
            WldPlacingItems::PathSet.meta.array_id = WldData->path_array_id;

            WldData->paths.push_back(WldPlacingItems::PathSet);
            placePath(WldPlacingItems::PathSet, true);
            placingItems.paths.push_back(WldPlacingItems::PathSet);
            wasPlaced=true;
        }
        else
        if(placingItem == PLC_Level)
        {
            WldPlacingItems::LevelSet.x = cursor->scenePos().x();
            WldPlacingItems::LevelSet.y = cursor->scenePos().y();

            WldData->level_array_id++;
            WldPlacingItems::LevelSet.meta.array_id = WldData->level_array_id;

            WldData->levels.push_back(WldPlacingItems::LevelSet);
            placeLevel(WldPlacingItems::LevelSet, true);
            placingItems.levels.push_back(WldPlacingItems::LevelSet);
            wasPlaced=true;
        }
        else
        if(placingItem == PLC_Musicbox)
        {
            WldPlacingItems::MusicSet.x = cursor->scenePos().x();
            WldPlacingItems::MusicSet.y = cursor->scenePos().y();

            WldData->musicbox_array_id++;
            WldPlacingItems::MusicSet.meta.array_id = WldData->musicbox_array_id;

            WldData->music.push_back(WldPlacingItems::MusicSet);
            placeMusicbox(WldPlacingItems::MusicSet, true);
            placingItems.music.push_back(WldPlacingItems::MusicSet);
            wasPlaced=true;
        }

    }
    if(wasPlaced)
    {
        WldData->meta.modified = true;
    }
}












void WldScene::removeItemUnderCursor()
{
    if(contextMenuOpened) return;

    QGraphicsItem * findItem;
    findItem = itemCollidesCursor(cursor);
    removeWldItem(findItem, true);
}


void WldScene::removeSelectedWldItems()
{
    QList<QGraphicsItem*> selectedList = selectedItems();
    if(selectedList.isEmpty()) return;
    removeWldItems(selectedList);
    Debugger_updateItemList();
}

void WldScene::removeWldItem(QGraphicsItem * item, bool globalHistory)
{
    if(!item) return;
    QList<QGraphicsItem * > items;
    items.push_back(item);
    removeWldItems(items, globalHistory);
}

void WldScene::removeWldItems(QList<QGraphicsItem * > items, bool globalHistory)
{
    WorldData historyBuffer;
    bool deleted=false;
    QString objType;

    for (QList<QGraphicsItem*>::iterator it = items.begin(); it != items.end(); it++)
    {
            objType=(*it)->data(ITEM_TYPE).toString();

            if(!(*it)->isVisible()) continue;  //Invisible items can't be deleted

            //remove data from main array before deletion item from scene
            if( objType=="TILE" )
            {
                historyBuffer.tiles.push_back(((ItemTile *)(*it))->m_data);
                ((ItemTile *)(*it))->removeFromArray();
                if((*it)) delete (*it);
                deleted=true;
            }
            else
            if( objType=="SCENERY" )
            {
                historyBuffer.scenery.push_back(((ItemScene*)(*it))->m_data);
                ((ItemScene *)(*it))->removeFromArray();
                if((*it)) delete (*it);
                deleted=true;
            }
            else
            if( objType=="PATH" )
            {
                historyBuffer.paths.push_back(((ItemPath*)(*it))->m_data);
                ((ItemPath *)(*it))->removeFromArray();
                if((*it)) delete (*it);
                deleted=true;
            }
            else
            if( objType=="LEVEL" )
            {
                historyBuffer.levels.push_back(((ItemLevel*)(*it))->m_data);
                ((ItemLevel *)(*it))->removeFromArray();
                if((*it)) delete (*it);
                deleted=true;
            }
            else
            if( objType=="MUSICBOX" )
            {
                historyBuffer.music.push_back(((ItemMusic*)(*it))->m_data);
                ((ItemMusic *)(*it))->removeFromArray();
                if((*it)) delete (*it);
                deleted=true;
            }

    }

    if(deleted)
    {
        if(globalHistory)
        {
            overwritedItems.tiles << historyBuffer.tiles;
            overwritedItems.scenery << historyBuffer.scenery;
            overwritedItems.paths << historyBuffer.paths;
            overwritedItems.levels << historyBuffer.levels;
            overwritedItems.music << historyBuffer.music;
        }
        else
            addRemoveHistory(historyBuffer);
    }
}

void WldScene::placeAll(const WorldData &data){
    foreach (WorldTerrainTile tile, data.tiles)
    {
        //place them back
        WldData->tiles.push_back(tile);
        placeTile(tile);
    }
    foreach (WorldPathTile path, data.paths)
    {
        //place them back
        WldData->paths.push_back(path);
        placePath(path);
    }
    foreach (WorldScenery scenery, data.scenery)
    {
        //place them back
        WldData->scenery.push_back(scenery);
        placeScenery(scenery);
    }
    foreach (WorldLevelTile level, data.levels)
    {
        //place them back
        WldData->levels.push_back(level);
        placeLevel(level);
    }
    foreach (WorldMusicBox music, data.music)
    {
        WldData->music.push_back(music);
        placeMusicbox(music);
    }

    //refresh Animation control
    if(opts.animationEnabled) stopAnimation();
    if(opts.animationEnabled) startAnimation();
}



