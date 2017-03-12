/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2017 Vitaly Novichkov <admin@wohlnet.ru>
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

#ifndef PGE_MENU_H
#define PGE_MENU_H

#include <common_features/pge_texture.h>
#include <common_features/rect.h>
#include <common_features/size.h>
#include <common_features/point.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <functional>
#include <Utils/vptrlist.h>

#include "menu/_pge_menuitem.h"

#include "menu/pge_bool_menuitem.h"
#include "menu/pge_int_menuitem.h"
#include "menu/pge_int_named_menuitem.h"
#include "menu/pge_keygrab_menuitem.h"

struct KM_Key;

class PGE_Menu
{
friend class PGE_KeyGrabMenuItem;
public:
    enum menuAlignment { HORIZONTAL, VERTICLE };

    PGE_Menu(menuAlignment align=VERTICLE, int itemGap = 0);
    PGE_Menu(const PGE_Menu&menu);
    ~PGE_Menu();

    void addMenuItem(std::string value, std::string title="",
                     std::function<void()> _extAction=([]()->void{}), bool enabled=true);
    void addBoolMenuItem(bool *flag, std::string value, std::string title="",
                         std::function<void()> _extAction=([]()->void{}), bool enabled=true);
    void addIntMenuItem(int *intvalue, int min, int max, std::string value, std::string title, bool rotate=false,
                        std::function<void()> _extAction=([]()->void{}), bool enabled=true );
    void addNamedIntMenuItem(int *intvalue, std::vector<NamedIntItem > _items, std::string value, std::string title, bool rotate=false,
                        std::function<void()> _extAction=([]()->void{}), bool enabled=true );
    void addKeyGrabMenuItem(KM_Key *key, std::string item_key, std::string title, SDL_Joystick *joystick_device=NULL, bool enabled=true);

    /*!
     * \brief Change enable state of element by value key
     * \param value Unique value key to identify menu item
     * \param enabled enabled state
     */
    void setEnabled(std::string menuitem_key, bool enabled);

    void setValueOffset(int offset);
    void setItemWidth(int width);

    void clear(); //!< Clean all menuitems

    void selectUp();   //!< move selection cursor up
    void selectDown(); //!< move selection cursor down
    void scrollUp();   //!< Scroll by mousewheel
    void scrollDown(); //!< Scroll by mousewheel
    void selectLeft(); //!< switch to left  (for lists)
    void selectRight();//!< switch to right (for lists)
    void acceptItem(); //!< Accept currently selected item
    void rejectItem(); //!< Reject menu
    void resetState(); //!< Reset state after acception or rejection
    void setMouseHoverPos(int x, int y);/*!< Set hovering mouse position.
                                          Will be selected item which under mouse*/

    void setMouseClickPos(int x, int y);
    int findItem(int x, int y);

    void reset();   //!< Reset menu to initial state

    void setItemsNumber(int q); //!< Set number of item which will show on screen

    void sort();   //!< Sort menu items in alphabetic order
    void render(); //!< Draw menu on screen

    bool isSelected();  //!< Is menu was accepted or rejected
    bool isAccepted();  //!< Is menu was accepted, else rejected
    bool isKeyGrabbing();         //!< Is a key grabbing mode
    bool processJoystickBinder(); //!< Is a joystick key grabbing mode
    void storeKey(int scancode);
    menuAlignment getAlignment();
    const PGE_Menuitem currentItem(); //!< Returns current menu item entry
    int currentItemI();       //!< Returns index of current menu item
    void setCurrentItem(int i); //!< Sets current index of menuitem
    int line();  //!< Returns number of current line where cursor is located on screen
    void setLine(int ln); //!< Sets current line number
    int offset();          //!< Returns scrolling offset from begin of menu list
    void setOffset(int of);//!< Sets scrolling offset from begin of menu list

    //Position and size Rectangle
    PGE_Rect rect(); //!< Returns rectangle of menu box
    PGE_Rect rectFull(); //!< Returns rectangle of menu box include sliders
    int  topOffset();//!< Offset
    void setPos(int x, int y); //!< Sets current position of menu box
    void setPos(PGE_Point p);     //!< Sets current position of menu box
    void setSize(int w, int h); //!< Sets size of menu box
    void setSize(PGE_Size s);      //!< Sets size of menu box
    void setTextLenLimit(uint32_t maxlen, bool strict=false);
    int  getMenuItemGap();

    bool isKeygrabViaKey() const;
    void setKeygrabViaKey(bool value);

private:
    void refreshRect();
    PGE_Rect menuRect;

    /*******Key grabbing********/
    PGE_KeyGrabMenuItem *m_item;
    bool is_keygrab;
    //This is needed, because the first inital keypress should not be prossesed.
    bool is_keygrabViaKey;
    /*******Key grabbing********/

    menuAlignment alignment;
    int _itemsOnScreen;
    int _currentItem;
    int _line;
    int _offset;
    bool arrowUpViz;
    bool arrowDownViz;
    bool _EndSelection;
    bool _accept;
    VPtrList<PGE_BoolMenuItem >     _items_bool;
    VPtrList<PGE_IntMenuItem >      _items_int;
    VPtrList<PGE_NamedIntMenuItem > _items_named_int;
    VPtrList<PGE_Menuitem >         _items_normal;
    VPtrList<PGE_KeyGrabMenuItem >  _items_keygrabs;

    std::vector<PGE_Menuitem *> _items;
    typedef std::unordered_map<std::string, PGE_Menuitem *> MenuIndex;
    MenuIndex _items_index;
    bool namefileLessThan(const PGE_Menuitem *d1, const PGE_Menuitem *d2);
    bool namefileMoreThan(const PGE_Menuitem *d1, const PGE_Menuitem *d2);
    void autoOffset();
    PGE_Texture _selector;
    PGE_Texture _scroll_up;
    PGE_Texture _scroll_down;
    int _item_height;
    int _width_limit;
    uint32_t _text_len_limit;
    bool _text_len_limit_strict;
    int menuItemGap;

    int _font_id;
    int _font_offset;
};

#endif // PGE_MENU_H
