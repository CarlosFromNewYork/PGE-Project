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

#include "../lvl_npc.h"
#include "../../scene_level.h"

void LVL_Npc::updateGenerator(float tickTime)
{
    if(!m_isGenerator) return;
    generatorTimeLeft-=tickTime;
    if(generatorTimeLeft<=0)
    {
        generatorTimeLeft += data.generator_period*100;
        if(!contacted_npc.isEmpty()) return;
        if(!contacted_players.isEmpty()) return;

        LevelNPC def = data;
        def.x=round(posX());
        def.y=round(posY());
        def.generator=false;
        def.layer="Spawned NPCs";
        _scene->spawnNPC(def,
                        (LevelScene::NpcSpawnType)generatorType,
                        (LevelScene::NpcSpawnDirection)generatorDirection, false);
    }

}

