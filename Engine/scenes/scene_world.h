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

#ifndef SCENE_WORLD_H
#define SCENE_WORLD_H

#include <QString>
#include <QList>
#include "scene.h"
#include <controls/controller.h>
#include <PGE_File_Formats/wld_filedata.h>
#include <common_features/pge_texture.h>
#include <common_features/episode_state.h>
#include <common_features/rect.h>
#include <common_features/point.h>
#include <data_configs/config_manager.h>

#include "world/wld_player_portrait.h"

struct WorldScene_misc_img
{
    int x;
    int y;
    PGE_Texture t;
    SimpleAnimator a;
    int frmH;
};


class WorldScene : public Scene
{
public:
    WorldScene();
    ~WorldScene();

    bool init();
    bool isInit;

    bool loadConfigs();

    int exitWorldDelay;
    int exitWorldCode;
    QString targetLevel;

    Controller *player1Controller;
    controller_keys controls_1;

    bool worldIsContinues;
    bool isPauseMenu;

    void onKeyboardPressedSDL(SDL_Keycode sdl_key, Uint16 modifier);

    void update();
    void render();
    int exec();

    void tickAnimations(int ticks);

    bool isExit();
    void setExiting(int delay, int reason);

    bool loadFile(QString filePath);

    QString getLastError();

    WorldData data;

    PGE_Rect viewportRect;

    double posX;
    double posY;
    QString currentMusicFile;

    QString levelTitle;
    long health;
    long stars;
    long points;
    long coins;

    void jump();
    bool jumpTo;
    PGE_Point jumpToXY;


    int i;
    int delayToEnter;
    bool debug_player_jumping;
    bool debug_player_onground;
    int  debug_player_foots;
    int  debug_render_delay;
    int  debug_phys_delay;
    int  debug_event_delay;

    int dir;
    bool lock_controls;
    bool ignore_paths;
    bool allow_left;
    bool allow_up;
    bool allow_right;
    bool allow_down;
    bool _playStopSnd;
    bool _playDenySnd;

    void updateAvailablePaths();//!< Checks paths by sides arround player and sets walking permission
    void updateCenter();
    void setGameState(EpisodeState *_state);

    void playMusic(long musicID, QString customMusicFile, bool fade=false, int fadeLen=300);
    void stopMusic(bool fade=false, int fadeLen=300);

private:
    float move_speed;//!< Calculated movement step dependent to physical step
    float move_steps_count;//!< Speps counterm, used to correct inter-cell position
    EpisodeState *gameState;
    QString errorMsg;
    WorldMapSetup common_setup;

    int numOfPlayers;
    QList<PlayerState > players;

    PGE_Texture backgroundTex;
    QList<PGE_Texture > textures_bank;

    obj_player     mapwalker_setup;
    PGE_Texture    mapwalker_texture;
    float          mapwalker_img_h;
    SimpleAnimator mapwalker_ani;
    int            mapwalker_offset_x;
    int            mapwalker_offset_y;
    void           mapwalker_refreshDir();

    QVector<WorldScene_misc_img > imgs;
    QVector<WorldScene_Portrait > portraits;
};

#endif // SCENE_WORLD_H
