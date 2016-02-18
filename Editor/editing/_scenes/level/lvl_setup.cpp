/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2014-2015 Vitaly Novichkov <admin@wohlnet.ru>
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

#include <common_features/mainwinconnect.h>
#include <common_features/themes.h>
#include <main_window/global_settings.h>
#include <editing/edit_level/level_edit.h>
#include <main_window/dock/lvl_layers_box.h>
#include <main_window/dock/lvl_events_box.h>

#include "lvl_scene.h"
#include "items/item_block.h"
#include "items/item_bgo.h"
#include "items/item_npc.h"
#include "items/item_water.h"
#include "items/item_door.h"
#include "newlayerbox.h"

void LvlScene::SwitchEditingMode(int EdtMode)
{
    //int EditingMode; // 0 - selecting,  1 - erasing, 2 - placeNewObject
                     // 3 - drawing water/sand zone, 4 - placing from Buffer
    //bool EraserEnabled;
    //bool PasteFromBuffer;

    //bool DrawMode; //Placing/drawing on map, disable selecting and dragging items

    //bool disableMoveItems;

    //bool contextMenuOpened;
    EraserEnabled=false;
    PasteFromBuffer=false;
    DrawMode=false;
    disableMoveItems=false;

    switch(EdtMode)
    {
    case MODE_PlacingNew:
        switchMode("Placing");
        break;

    case MODE_DrawRect:
        switchMode("Square");
        break;

    case MODE_DrawCircle:
        switchMode("Circle");
        break;

    case MODE_Line:
        switchMode("Line");
        break;

    case MODE_Resizing:
        switchMode("Resize");
        break;

    case MODE_PasteFromClip:
        switchMode("Select");
        clearSelection();
        disableMoveItems=true;
        _viewPort->setInteractive(true);
        _viewPort->setCursor(Themes::Cursor(Themes::cursor_pasting));
        _viewPort->setDragMode(QGraphicsView::NoDrag);
        break;

    case MODE_Erasing:
        switchMode("Erase");
        break;

    case MODE_SelectingOnly:
        switchMode("Select");
        disableMoveItems=true;
        break;

    case MODE_HandScroll:
        switchMode("HandScroll");
        break;


    case MODE_Fill:
        switchMode("Fill");
        break;

    case MODE_Selecting:
    default:
        switchMode("Select");
        break;
    }
    EditingMode = EdtMode;

}

void LvlScene::switchMode(QString title)
{
    qDebug() << "Switching mode " << title;
    for(int i=0; i<EditModes.size(); i++)
    {
        if(EditModes[i]->name()==title)
        {
            CurrentMode = EditModes[i];
            CurrentMode->set();
            qDebug() << "mode " << title << "switched!";
            break;
        }
    }
}


void LvlScene::hideWarpsAndDoors(bool visible)
{
    QMap<QString, LevelLayer> localLayers;
    for(int i = 0; i < LvlData->layers.size(); ++i){
        localLayers[LvlData->layers[i].name] = LvlData->layers[i];
    }

    foreach (QGraphicsItem* i, items()) {
        if(i->data(ITEM_TYPE).toString()=="Water"){
            i->setVisible(!localLayers[((ItemWater*)i)->m_data.layer].hidden && visible);
        }else if(i->data(ITEM_TYPE).toString()=="Door_exit" || i->data(ITEM_TYPE).toString()=="Door_enter"){
            i->setVisible(!localLayers[((ItemDoor*)i)->m_data.layer].hidden && visible);
        }
    }
}


void LvlScene::setTiledBackground(bool forceTiled)
{
    ChangeSectionBG(LvlData->sections[LvlData->CurSection].background, -1, forceTiled);
}

void LvlScene::applyLayersVisible()
{
    QList<QGraphicsItem*> ItemList = items();
    QGraphicsItem *tmp;
    for (QList<QGraphicsItem*>::iterator it = ItemList.begin(); it != ItemList.end(); it++)
    {
        if((*it)->data(ITEM_TYPE)=="Block")
        {
            tmp = (*it);
            foreach(LevelLayer layer, LvlData->layers)
            {
                if( dynamic_cast<ItemBlock *>(tmp)->m_data.layer == layer.name)
                {
                    dynamic_cast<ItemBlock *>(tmp)->setVisible( !layer.hidden ); break;
                }
            }
        }
        else
        if((*it)->data(ITEM_TYPE)=="BGO")
        {
            tmp = (*it);
            foreach(LevelLayer layer, LvlData->layers)
            {
                if( dynamic_cast<ItemBGO *>(tmp)->m_data.layer == layer.name)
                {
                    dynamic_cast<ItemBGO *>(tmp)->setVisible( !layer.hidden ); break;
                }
            }
        }
        else
        if((*it)->data(ITEM_TYPE)=="NPC")
        {
            tmp = (*it);
            foreach(LevelLayer layer, LvlData->layers)
            {
                if( dynamic_cast<ItemNPC *>(tmp)->m_data.layer == layer.name)
                {
                    dynamic_cast<ItemNPC *>(tmp)->setVisible( !layer.hidden ); break;
                }
            }
        }
        else
        if((*it)->data(ITEM_TYPE)=="Water")
        {
            tmp = (*it);
            foreach(LevelLayer layer, LvlData->layers)
            {
                if( dynamic_cast<ItemWater *>(tmp)->m_data.layer == layer.name)
                {
                    dynamic_cast<ItemWater *>(tmp)->setVisible( !layer.hidden ); break;
                }
            }
        }
        else
        if(((*it)->data(ITEM_TYPE)=="Door_enter")||((*it)->data(ITEM_TYPE)=="Door_exit"))
        {
            tmp = (*it);
            foreach(LevelLayer layer, LvlData->layers)
            {
                if( dynamic_cast<ItemDoor *>(tmp)->m_data.layer == layer.name)
                {
                    dynamic_cast<ItemDoor *>(tmp)->setVisible( !layer.hidden ); break;
                }
            }
        }
    }
}


/////////////////////////////////////////////Locks////////////////////////////////
void LvlScene::setLocked(int type, bool lock)
{
    // setLock
    switch(type)
    {
    case 1://Block
        lock_block = lock;
        break;
    case 2://BGO
        lock_bgo = lock;
        break;
    case 3://NPC
        lock_npc = lock;
        break;
    case 4://Water
        lock_water = lock;
        break;
    case 5://Doors
        lock_door = lock;
        break;
    default: break;
    }

    QList<QGraphicsItem*> ItemList = items();
    for (QList<QGraphicsItem*>::iterator it = ItemList.begin(); it != ItemList.end(); it++)
    {
        if((*it)==NULL) continue;
        if((*it)->data(ITEM_IS_ITEM).isNull()) continue;

        switch(type)
        {
        case 1://Block
            if((*it)->data(ITEM_TYPE).toString()=="Block")
            {
                ItemBlock *item=qgraphicsitem_cast<ItemBlock *>(*it);
                if(item) item->setLocked(lock);
            }
            break;
        case 2://BGO
            if((*it)->data(ITEM_TYPE).toString()=="BGO")
            {
                ItemBGO *i=qgraphicsitem_cast<ItemBGO *>(*it);
                if(i) i->setLocked(lock);
            }
            break;
        case 3://NPC
            if((*it)->data(ITEM_TYPE).toString()=="NPC")
            {
                ItemNPC *i=qgraphicsitem_cast<ItemNPC *>(*it);
                if(i) i->setLocked(lock);
            }
            break;
        case 4://Water
            if((*it)->data(ITEM_TYPE).toString()=="Water")
            {
                ItemWater *i=qgraphicsitem_cast<ItemWater *>(*it);
                if(i) i->setLocked(lock);
            }
            break;
        case 5://Doors
            if(((*it)->data(ITEM_TYPE).toString()=="Door_enter")||((*it)->data(ITEM_TYPE).toString()=="Door_exit"))
            {
                ItemDoor *i=qgraphicsitem_cast<ItemDoor *>(*it);
                if(i) i->setLocked(lock);
            }
            break;
        default: break;
        }
    }

}


void LvlScene::setLayerToSelected()
{
    QString lName;
    ToNewLayerBox * layerBox = new ToNewLayerBox(LvlData);
    layerBox->setWindowFlags (Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    layerBox->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, layerBox->size(), qApp->desktop()->availableGeometry()));
    if(layerBox->exec()==QDialog::Accepted)
    {
        lName = layerBox->lName;

        //Store new layer into array
        LevelLayer nLayer;
        nLayer.name = lName;
        nLayer.hidden = layerBox->lHidden;
        LvlData->layers_array_id++;
        nLayer.array_id = LvlData->layers_array_id;
        LvlData->layers.push_back(nLayer);
        //scene->SyncLayerList=true; //Refresh layer list
        MainWinConnect::pMainWin->dock_LvlLayers->setLayerToolsLocked(true);
        MainWinConnect::pMainWin->dock_LvlLayers->setLayersBox();
        MainWinConnect::pMainWin->dock_LvlLayers->setLayerToolsLocked(false);
        MainWinConnect::pMainWin->LayerListsSync();
        MainWinConnect::pMainWin->dock_LvlEvents->setEventData();
        setLayerToSelected(lName, true);
    }
    delete layerBox;

}

void LvlScene::setLayerToSelected(QString lName, bool isNew)
{
    LevelData modData;
    foreach(LevelLayer lr, LvlData->layers)
    { //Find layer's settings
        if(lr.name==lName)
        {
            foreach(QGraphicsItem * SelItem, selectedItems() )
            {
                if(SelItem->data(ITEM_TYPE).toString()=="Block")
                {
                    modData.blocks.push_back(dynamic_cast<ItemBlock *>(SelItem)->m_data);
                    dynamic_cast<ItemBlock *>(SelItem)->m_data.layer = lr.name;
                    dynamic_cast<ItemBlock *>(SelItem)->setVisible(!lr.hidden);
                    dynamic_cast<ItemBlock *>(SelItem)->arrayApply();
                }
                else
                if(SelItem->data(ITEM_TYPE).toString()=="BGO")
                {
                    modData.bgo.push_back(dynamic_cast<ItemBGO *>(SelItem)->m_data);
                    dynamic_cast<ItemBGO *>(SelItem)->m_data.layer = lr.name;
                    dynamic_cast<ItemBGO *>(SelItem)->setVisible(!lr.hidden);
                    dynamic_cast<ItemBGO *>(SelItem)->arrayApply();
                }
                else
                if(SelItem->data(ITEM_TYPE).toString()=="NPC")
                {
                    modData.npc.push_back(dynamic_cast<ItemNPC *>(SelItem)->m_data);
                    dynamic_cast<ItemNPC *>(SelItem)->m_data.layer = lr.name;
                    dynamic_cast<ItemNPC *>(SelItem)->setVisible(!lr.hidden);
                    dynamic_cast<ItemNPC *>(SelItem)->arrayApply();
                }
                else
                if(SelItem->data(ITEM_TYPE).toString()=="Water")
                {
                    modData.physez.push_back(dynamic_cast<ItemWater *>(SelItem)->m_data);
                    dynamic_cast<ItemWater *>(SelItem)->m_data.layer = lr.name;
                    dynamic_cast<ItemWater *>(SelItem)->setVisible(!lr.hidden);
                    dynamic_cast<ItemWater *>(SelItem)->arrayApply();
                }
                else
                if((SelItem->data(ITEM_TYPE).toString()=="Door_exit")  ||
                        (SelItem->data(ITEM_TYPE).toString()=="Door_enter"))
                {
                    if(SelItem->data(ITEM_TYPE).toString()=="Door_exit"){
                        LevelDoor tDoor = dynamic_cast<ItemDoor *>(SelItem)->m_data;
                        tDoor.isSetOut = true;
                        tDoor.isSetIn = false;
                        modData.doors.push_back(tDoor);
                    }
                    else
                    if(SelItem->data(ITEM_TYPE).toString()=="Door_enter"){
                        LevelDoor tDoor = dynamic_cast<ItemDoor *>(SelItem)->m_data;
                        tDoor.isSetOut = false;
                        tDoor.isSetIn = true;
                        modData.doors.push_back(tDoor);
                    }
                    dynamic_cast<ItemDoor *>(SelItem)->m_data.layer = lr.name;
                    dynamic_cast<ItemDoor *>(SelItem)->setVisible(!lr.hidden);
                    dynamic_cast<ItemDoor *>(SelItem)->arrayApply();
                }
            }
            if(isNew)
            {
                addChangedNewLayerHistory(modData, lr);
            }
            break;
        }
    }//Find layer's settings

    if(!isNew)
    {
        addChangedLayerHistory(modData, lName);
    }
}
