/* Copyright (C) 1994-1998 Revolution Software Ltd.
 * Copyright (C) 2003-2005 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 */

#include "common/stdafx.h"
#include "sword2/sword2.h"
#include "sword2/console.h"
#include "sword2/controls.h"
#include "sword2/defs.h"
#include "sword2/interpreter.h"
#include "sword2/logic.h"
#include "sword2/maketext.h"
#include "sword2/memory.h"
#include "sword2/mouse.h"
#include "sword2/resman.h"
#include "sword2/sound.h"

namespace Sword2 {

// Pointer resource id's

enum {
	CROSHAIR	= 18,
	EXIT0		= 788,
	EXIT1		= 789,
	EXIT2		= 790,
	EXIT3		= 791,
	EXIT4		= 792,
	EXIT5		= 793,
	EXIT6		= 794,
	EXIT7		= 795,
	EXITDOWN	= 796,
	EXITUP		= 797,
	MOUTH		= 787,
	NORMAL		= 17,
	PICKUP		= 3099,
	SCROLL_L	= 1440,
	SCROLL_R	= 1441,
	USE		= 3100
};

Mouse::Mouse(Sword2Engine *vm) {
	_vm = vm;

	setPos(0, 0);

	_mouseTouching = 0;
	_oldMouseTouching = 0;
	_menuSelectedPos = 0;
	_examiningMenuIcon = false;
	_mousePointerRes = 0;
	_mouseMode = 0;
	_mouseStatus = false;
	_mouseModeLocked = false;
	_currentLuggageResource = 0;
	_oldButton = 0;
	_buttonClick = 0;
	_pointerTextBlocNo = 0;
	_playerActivityDelay = 0;
	_realLuggageItem = 0;

	_mouseSprite = NULL;
	_mouseAnim = NULL;
	_luggageAnim = NULL;

	// For the menus
	_totalTemp = 0;
	memset(_tempList, 0, sizeof(_tempList));

	_totalMasters = 0;
	memset(_masterMenuList, 0, sizeof(_masterMenuList));
	memset(_mouseList, 0, sizeof(_mouseList));

	_iconCount = 0;

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < RDMENU_MAXPOCKETS; j++) {
			_icons[i][j] = NULL;
			_pocketStatus[i][j] = 0;
		}

		_menuStatus[i] = RDMENU_HIDDEN;
	}
}

Mouse::~Mouse() {
	free(_mouseAnim);
	free(_luggageAnim);
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < RDMENU_MAXPOCKETS; j++)
			free(_icons[i][j]);
}

void Mouse::getPos(int &x, int &y) {
	x = _pos.x;
	y = _pos.y;
}

void Mouse::setPos(int x, int y) {
	_pos.x = x;
	_pos.y = y;
}

/**
 * Call at beginning of game loop
 */

void Mouse::resetMouseList() {
	_curMouse = 0;
}

void Mouse::registerMouse(ObjectMouse *ob_mouse, BuildUnit *build_unit) {
	if (!ob_mouse->pointer)
		return;

	assert(_curMouse < TOTAL_mouse_list);

	if (build_unit) {
		_mouseList[_curMouse].x1 = build_unit->x;
		_mouseList[_curMouse].y1 = build_unit->y;
		_mouseList[_curMouse].x2 = build_unit->x + build_unit->scaled_width;
		_mouseList[_curMouse].y2 = build_unit->y + build_unit->scaled_height;
	} else {
		_mouseList[_curMouse].x1 = ob_mouse->x1;
		_mouseList[_curMouse].y1 = ob_mouse->y1;
		_mouseList[_curMouse].x2 = ob_mouse->x2;
		_mouseList[_curMouse].y2 = ob_mouse->y2;
	}

	_mouseList[_curMouse].priority = ob_mouse->priority;
	_mouseList[_curMouse].pointer = ob_mouse->pointer;

	// Check if pointer text field is set due to previous object using this
	// slot (ie. not correct for this one)

	// If 'pointer_text' field is set, but the 'id' field isn't same is
	// current id then we don't want this "left over" pointer text

	if (_mouseList[_curMouse].pointer_text && _mouseList[_curMouse].id != (int32) Logic::_scriptVars[ID])
		_mouseList[_curMouse].pointer_text = 0;

	// Get id from system variable 'id' which is correct for current object
	_mouseList[_curMouse].id = Logic::_scriptVars[ID];

	// Not using sprite as detection mask - this is only done from
	// fnRegisterFrame()
	_mouseList[_curMouse].anim_resource = 0;
	_mouseList[_curMouse].anim_pc = 0;

	_curMouse++;
}

void Mouse::registerPointerText(int32 text_id) {
	assert(_curMouse < TOTAL_mouse_list);

	// current object id - used for checking pointer_text when mouse area
	// registered (in fnRegisterMouse and fnRegisterFrame)

	_mouseList[_curMouse].id = Logic::_scriptVars[ID];
	_mouseList[_curMouse].pointer_text = text_id;
}

/**
 * This function is called every game cycle.
 */

void Mouse::mouseEngine() {
	monitorPlayerActivity();
	clearPointerText();

	// If George is dead, the system menu is visible all the time, and is
	// the only thing that can be used.

	if (Logic::_scriptVars[DEAD]) {
		if (_mouseMode != MOUSE_system_menu) {
			_mouseMode = MOUSE_system_menu;

			if (_mouseTouching) {
				_oldMouseTouching = 0;
				_mouseTouching = 0;
			}

			setMouse(NORMAL_MOUSE_ID);
			buildSystemMenu();
		}
		systemMenuMouse();
		return;
	}

	// If the mouse is not visible, do nothing

	if (_mouseStatus)
		return;

	switch (_mouseMode) {
	case MOUSE_normal:
		normalMouse();
		break;
	case MOUSE_menu:
		menuMouse();
		break;
	case MOUSE_drag:
		dragMouse();
		break;
	case MOUSE_system_menu:
		systemMenuMouse();
		break;
	case MOUSE_holding:
		if (_pos.y < 400) {
			_mouseMode = MOUSE_normal;
			debug(5, "   releasing");
		}
		break;
	default:
		break;
	}
}

#if RIGHT_CLICK_CLEARS_LUGGAGE
bool Mouse::heldIsInInventory() {
	for (uint i = 0; i < _totalMasters; i++) {
		if ((uint32) _masterMenuList[i].icon_resource == Logic::_scriptVars[OBJECT_HELD])
			return true;
	}
	return false;
}
#endif

int Mouse::menuClick(int menu_items) {
	if (_pos.x < RDMENU_ICONSTART)
		return -1;

	if (_pos.x > RDMENU_ICONSTART + menu_items * (RDMENU_ICONWIDE + RDMENU_ICONSPACING) - RDMENU_ICONSPACING)
		return -1;

	return (_pos.x - RDMENU_ICONSTART) / (RDMENU_ICONWIDE + RDMENU_ICONSPACING);
}

void Mouse::systemMenuMouse(void) {
	uint32 safe_looping_music_id;
	MouseEvent *me;
	int hit;
	byte *icon;
	int32 pars[2];
	uint32 icon_list[5] = {
		OPTIONS_ICON,
		QUIT_ICON,
		SAVE_ICON,
		RESTORE_ICON,
		RESTART_ICON
	};

	// If the mouse is moved off the menu, close it. Unless the player is
	// dead, in which case the menu should always be visible.

	if (_pos.y > 0 && !Logic::_scriptVars[DEAD]) {
		_mouseMode = MOUSE_normal;
		hideMenu(RDMENU_TOP);
		return;
	}

	// Check if the user left-clicks anywhere in the menu area.

	me = _vm->mouseEvent();

	if (!me || !(me->buttons & RD_LEFTBUTTONDOWN))
		return;

	if (_pos.y > 0)
		return;

	hit = menuClick(ARRAYSIZE(icon_list));

	if (hit < 0)
		return;

	// No save when dead

	if (icon_list[hit] == SAVE_ICON && Logic::_scriptVars[DEAD])
		return;

	// Gray out all he icons, except the one that was clicked

	for (int i = 0; i < ARRAYSIZE(icon_list); i++) {
		if (i != hit) {
			icon = _vm->_resman->openResource(icon_list[i]) + sizeof(StandardHeader);
			setMenuIcon(RDMENU_TOP, i, icon);
			_vm->_resman->closeResource(icon_list[i]);
		}
	}

	_vm->_sound->pauseFx();

	// NB. Need to keep a safe copy of '_loopingMusicId' for savegame & for
	// playing when returning from control panels because control panel
	// music will overwrite it!

	safe_looping_music_id = _vm->_sound->getLoopingMusicId();

	pars[0] = 221;
	pars[1] = FX_LOOP;
	_vm->_logic->fnPlayMusic(pars);

	// HACK: Restore proper looping_music_id
	_vm->_sound->setLoopingMusicId(safe_looping_music_id);

	processMenu();

	// call the relevant screen

	switch (hit) {
	case 0:
		{
			OptionsDialog dialog(_vm);
			dialog.runModal();
		}
		break;
	case 1:
		{
			QuitDialog dialog(_vm);
			dialog.runModal();
		}
		break;
	case 2:
		{
			SaveDialog dialog(_vm);
			dialog.runModal();
		}
		break;
	case 3:
		{
			RestoreDialog dialog(_vm);
			dialog.runModal();
		}
		break;
	case 4:
		{
			RestartDialog dialog(_vm);
			dialog.runModal();
		}
		break;
	}

	// Menu stays open on death screen. Otherwise it's closed.

	if (!Logic::_scriptVars[DEAD]) {
		_mouseMode = MOUSE_normal;
		hideMenu(RDMENU_TOP);
	} else {
		setMouse(NORMAL_MOUSE_ID);
		buildSystemMenu();
	}

	// Back to the game again

	processMenu();

	// Reset game palette, but not after a successful restore or restart!
	// See RestoreFromBuffer() in save_rest.cpp

	ScreenInfo *screenInfo = _vm->_screen->getScreenInfo();

	if (screenInfo->new_palette != 99) {
		// 0 means put back game screen palette; see build_display.cpp
		_vm->_screen->setFullPalette(0);

		// Stop the engine fading in the restored screens palette
		screenInfo->new_palette = 0;
	} else
		screenInfo->new_palette = 1;

	_vm->_sound->unpauseFx();

	// If there was looping music before coming into the control panels
	// then restart it! NB. If a game has been restored the music will be
	// restarted twice, but this shouldn't cause any harm.

	if (_vm->_sound->getLoopingMusicId()) {
		pars[0] = _vm->_sound->getLoopingMusicId();
		pars[1] = FX_LOOP;
		_vm->_logic->fnPlayMusic(pars);
	} else
		_vm->_logic->fnStopMusic(NULL);
}

void Mouse::dragMouse(void) {
	byte buf1[NAME_LEN], buf2[NAME_LEN];
	MouseEvent *me;
	int hit;

	// We can use dragged object both on other inventory objects, or on
	// objects in the scene, so if the mouse moves off the inventory menu,
	// then close it.

	if (_pos.y < 400) {
		_mouseMode = MOUSE_normal;
		hideMenu(RDMENU_BOTTOM);
		return;
	}

	// Handles cursors and the luggage on/off according to type

	mouseOnOff();

	// Now do the normal click stuff

	me = _vm->mouseEvent();

	if (!me)
		return;

#if RIGHT_CLICK_CLEARS_LUGGAGE
	if ((me->buttons & RD_RIGHTBUTTONDOWN) && heldIsInInventory()) {
		Logic::_scriptVars[OBJECT_HELD] = 0;
		_menuSelectedPos = 0;
		_mouseMode = MOUSE_menu;
		setLuggage(0);
		buildMenu();
		return;
	}
#endif

	if (!(me->buttons & RD_LEFTBUTTONDOWN))
		return;

	// there's a mouse event to be processed

	// could be clicking on an on screen object or on the menu
	// which is currently displayed

	if (_mouseTouching) {
		// mouse is over an on screen object - and we have luggage

		// Depending on type we'll maybe kill the object_held - like
		// for exits

		// Set global script variable 'button'. We know that it was the
		// left button, not the right one.

		Logic::_scriptVars[LEFT_BUTTON]  = 1;
		Logic::_scriptVars[RIGHT_BUTTON] = 0;

		// These might be required by the action script about to be run
		ScreenInfo *screenInfo = _vm->_screen->getScreenInfo();

		Logic::_scriptVars[MOUSE_X] = _pos.x + screenInfo->scroll_offset_x;
		Logic::_scriptVars[MOUSE_Y] = _pos.y + screenInfo->scroll_offset_y;

		// For scripts to know what's been clicked. First used for
		// 'room_13_turning_script' in object 'biscuits_13'

		Logic::_scriptVars[CLICKED_ID] = _mouseTouching;

		_vm->_logic->setPlayerActionEvent(CUR_PLAYER_ID, _mouseTouching);

		debug(2, "Used \"%s\" on \"%s\"",
			_vm->fetchObjectName(Logic::_scriptVars[OBJECT_HELD], buf1),
			_vm->fetchObjectName(Logic::_scriptVars[CLICKED_ID], buf2));

		// Hide menu - back to normal menu mode

		hideMenu(RDMENU_BOTTOM);
		_mouseMode = MOUSE_normal;

		return;
	}

	// Better check for combine/cancel. Cancel puts us back in MOUSE_menu
	// mode

	hit = menuClick(TOTAL_engine_pockets);

	if (hit < 0 || !_masterMenuList[hit].icon_resource)
		return;

	// Always back into menu mode. Remove the luggage as well.

	_mouseMode = MOUSE_menu;
	setLuggage(0);

	if ((uint) hit == _menuSelectedPos) {
		// If we clicked on the same icon again, reset the first icon

		Logic::_scriptVars[OBJECT_HELD] = 0;
		_menuSelectedPos = 0;
	} else {
		// Otherwise, combine the two icons

		Logic::_scriptVars[COMBINE_BASE] = _masterMenuList[hit].icon_resource;
		_vm->_logic->setPlayerActionEvent(CUR_PLAYER_ID, MENU_MASTER_OBJECT);

		// Turn off mouse now, to prevent player trying to click
		// elsewhere BUT leave the bottom menu open

		hideMouse();

		debug(2, "Used \"%s\" on \"%s\"",
			_vm->fetchObjectName(Logic::_scriptVars[OBJECT_HELD], buf1),
			_vm->fetchObjectName(Logic::_scriptVars[COMBINE_BASE], buf2));
	}

	// Refresh the menu

	buildMenu();
}

void Mouse::menuMouse() {
	byte buf[NAME_LEN];
	MouseEvent *me;
	int hit;

	// If the mouse is moved off the menu, close it.

	if (_pos.y < 400) {
		_mouseMode = MOUSE_normal;
		hideMenu(RDMENU_BOTTOM);
		return;
	}

	me = _vm->mouseEvent();

	if (!me)
		return;

	hit = menuClick(TOTAL_engine_pockets);

	// Check if we clicked on an actual icon.

	if (hit < 0 || !_masterMenuList[hit].icon_resource)
		return;

	if (me->buttons & RD_RIGHTBUTTONDOWN) {
		// Right button - examine an object, identified by its icon
		// resource id.

		_examiningMenuIcon = true;
		Logic::_scriptVars[OBJECT_HELD] = _masterMenuList[hit].icon_resource;

		// Must clear this so next click on exit becomes 1st click
		// again

		Logic::_scriptVars[EXIT_CLICK_ID] = 0;

		_vm->_logic->setPlayerActionEvent(CUR_PLAYER_ID, MENU_MASTER_OBJECT);

		// Refresh the menu

		buildMenu();

		// Turn off mouse now, to prevent player trying to click
		// elsewhere BUT leave the bottom menu open

		hideMouse();

		debug(2, "Right-click on \"%s\" icon",
			_vm->fetchObjectName(Logic::_scriptVars[OBJECT_HELD], buf));

		return;
	}

	if (me->buttons & RD_LEFTBUTTONDOWN) {
		// Left button - bung us into drag luggage mode. The object is
		// identified by its icon resource id. We need the luggage
		// resource id for mouseOnOff

		_mouseMode = MOUSE_drag;

		_menuSelectedPos = hit;
		Logic::_scriptVars[OBJECT_HELD] = _masterMenuList[hit].icon_resource;
		_currentLuggageResource = _masterMenuList[hit].luggage_resource;

		// Must clear this so next click on exit becomes 1st click
		// again

		Logic::_scriptVars[EXIT_CLICK_ID] = 0;

		// Refresh the menu

		buildMenu();

		setLuggage(_masterMenuList[hit].luggage_resource);

		debug(2, "Left-clicked on \"%s\" icon - switch to drag mode",
			_vm->fetchObjectName(Logic::_scriptVars[OBJECT_HELD], buf));
	}
}

void Mouse::normalMouse(void) {
	// The gane is playing and none of the menus are activated - but, we
	// need to check if a menu is to start. Note, won't have luggage

	MouseEvent *me;

	// Check if the cursor has moved onto the system menu area. No save in
	// big-object menu lock situation, of if the player is dragging an
	// object.

	if (_pos.y < 0 && !_mouseModeLocked && !Logic::_scriptVars[OBJECT_HELD]) {
		_mouseMode = MOUSE_system_menu;

		if (_mouseTouching) {
			// We were on something, but not anymore
			_oldMouseTouching = 0;
			_mouseTouching = 0;
		}

		// Reset mouse cursor - in case we're between mice

		setMouse(NORMAL_MOUSE_ID);
		buildSystemMenu();
		return;
	}

	// Check if the cursor has moved onto the inventory menu area. No
	// inventory in big-object menu lock situation,

	if (_pos.y > 399 && !_mouseModeLocked) {
		// If an object is being held, i.e. if the mouse cursor has a
		// luggage, go to drag mode instead of menu mode, but the menu
		// is still opened.
		//
		// That way, we can still use an object on another inventory
		// object, even if the inventory menu was closed after the
		// first object was selected.

		if (!Logic::_scriptVars[OBJECT_HELD])
			_mouseMode = MOUSE_menu;
		else
			_mouseMode = MOUSE_drag;

		// If mouse is moving off an object and onto the menu then do a
		// standard get-off

		if (_mouseTouching) {
			_oldMouseTouching = 0;
			_mouseTouching = 0;
		}

		// Reset mouse cursor

		setMouse(NORMAL_MOUSE_ID);
		buildMenu();
		return;
	}

	// Check for moving the mouse on or off things

	mouseOnOff();

	me = _vm->mouseEvent();

	if (!me)
		return;

	bool button_down = (me->buttons & (RD_LEFTBUTTONDOWN | RD_RIGHTBUTTONDOWN)) != 0;

	// For debugging. We can draw a rectangle on the screen and see its
	// coordinates. This was probably used to help defining hit areas.

	if (_vm->_debugger->_definingRectangles) {
		ScreenInfo *screenInfo = _vm->_screen->getScreenInfo();

		if (_vm->_debugger->_draggingRectangle == 0) {
			// Not yet dragging a rectangle, so need click to start

			if (button_down) {
				// set both (x1,y1) and (x2,y2) to this point
				_vm->_debugger->_rectX1 = _vm->_debugger->_rectX2 = (uint32) _pos.x + screenInfo->scroll_offset_x;
				_vm->_debugger->_rectY1 = _vm->_debugger->_rectY2 = (uint32) _pos.y + screenInfo->scroll_offset_y;
				_vm->_debugger->_draggingRectangle = 1;
			}
		} else if (_vm->_debugger->_draggingRectangle == 1) {
			// currently dragging a rectangle - click means reset

			if (button_down) {
				// lock rectangle, so you can let go of mouse
				// to type in the coords
				_vm->_debugger->_draggingRectangle = 2;
			} else {
				// drag rectangle
				_vm->_debugger->_rectX2 = (uint32) _pos.x + screenInfo->scroll_offset_x;
				_vm->_debugger->_rectY2 = (uint32) _pos.y + screenInfo->scroll_offset_y;
			}
		} else {
			// currently locked to avoid knocking out of place
			// while reading off the coords

			if (button_down) {
				// click means reset - back to start again
				_vm->_debugger->_draggingRectangle = 0;
			}
		}

		return;
	}

#if RIGHT_CLICK_CLEARS_LUGGAGE
	if (Logic::_scriptVars[OBJECT_HELD] && (me->buttons & RD_RIGHTBUTTONDOWN) && heldIsInInventory()) {
		Logic::_scriptVars[OBJECT_HELD] = 0;
		_menuSelectedPos = 0;
		setLuggage(0);
		return;
	}
#endif

	// Now do the normal click stuff

	// We only care about down clicks when the mouse is over an object.

	if (!_mouseTouching || !button_down)
		return;

	// There's a mouse event to be processed and the mouse is on something.
	// Notice that the floor itself is considered an object.

	// There are no menus about so its nice and simple. This is as close to
	// the old advisor_188 script as we get, I'm sorry to say.

	// If player is walking or relaxing then those need to terminate
	// correctly. Otherwise set player run the targets action script or, do
	// a special walk if clicking on the scroll-more icon

	// PLAYER_ACTION script variable - whatever catches this must reset to
	// 0 again
	// Logic::_scriptVars[PLAYER_ACTION] = _mouseTouching;

	// Idle or router-anim will catch it

	// Set global script variable 'button'

	if (me->buttons & RD_LEFTBUTTONDOWN) {
		Logic::_scriptVars[LEFT_BUTTON]  = 1;
		Logic::_scriptVars[RIGHT_BUTTON] = 0;
		_buttonClick = 0;	// for re-click
	} else {
		Logic::_scriptVars[LEFT_BUTTON]  = 0;
		Logic::_scriptVars[RIGHT_BUTTON] = 1;
		_buttonClick = 1;	// for re-click
	}

	// These might be required by the action script about to be run
	ScreenInfo *screenInfo = _vm->_screen->getScreenInfo();

	Logic::_scriptVars[MOUSE_X] = _pos.x + screenInfo->scroll_offset_x;
	Logic::_scriptVars[MOUSE_Y] = _pos.y + screenInfo->scroll_offset_y;

	if (_mouseTouching == Logic::_scriptVars[EXIT_CLICK_ID] && (me->buttons & RD_LEFTBUTTONDOWN)) {
		// It's the exit double click situation. Let the existing
		// interaction continue and start fading down. Switch the human
		// off too

		noHuman();
		_vm->_logic->fnFadeDown(NULL);

		// Tell the walker

		Logic::_scriptVars[EXIT_FADING] = 1;
	} else if (_oldButton == _buttonClick && _mouseTouching == Logic::_scriptVars[CLICKED_ID] && _mousePointerRes != NORMAL_MOUSE_ID) {
		// Re-click. Do nothing, except on floors
	} else {
		// For re-click

		_oldButton = _buttonClick;

		// For scripts to know what's been clicked. First used for
		// 'room_13_turning_script' in object 'biscuits_13'

		Logic::_scriptVars[CLICKED_ID] = _mouseTouching;

		// Must clear these two double-click control flags - do it here
		// so reclicks after exit clicks are cleared up

		Logic::_scriptVars[EXIT_CLICK_ID] = 0;
		Logic::_scriptVars[EXIT_FADING] = 0;

		// WORKAROUND: Examining the lift while at the top of the
		// pyramid causes the game to hang.
		//
		// Actually, what happens is that the elevator's click handler
		// (action script 2) disables the mouse cursor. Then it checks
		// if the user clicked the left button, in which case it
		// triggers the "going down" animation.
		//
		// If the user didn't click the left button, the script will
		// terminate. With the mouse cursor still disabled. Ouch!

		if (_mouseTouching == 2773 && !Logic::_scriptVars[LEFT_BUTTON]) {
			warning("Working around elevator script bug");
		} else
			_vm->_logic->setPlayerActionEvent(CUR_PLAYER_ID, _mouseTouching);

		byte buf1[NAME_LEN], buf2[NAME_LEN];

		if (Logic::_scriptVars[OBJECT_HELD])
			debug(2, "Used \"%s\" on \"%s\"",
				_vm->fetchObjectName(Logic::_scriptVars[OBJECT_HELD], buf1),
				_vm->fetchObjectName(Logic::_scriptVars[CLICKED_ID], buf2));
		else if (Logic::_scriptVars[LEFT_BUTTON])
			debug(2, "Left-clicked on \"%s\"",
				_vm->fetchObjectName(Logic::_scriptVars[CLICKED_ID], buf1));
		else	// RIGHT BUTTON
			debug(2, "Right-clicked on \"%s\"",
				_vm->fetchObjectName(Logic::_scriptVars[CLICKED_ID], buf1));
	}
}

void Mouse::mouseOnOff() {
	// this handles the cursor graphic when moving on and off mouse areas
	// it also handles the luggage thingy

	uint32 pointer_type;
	static uint8 mouse_flicked_off = 0;

	_oldMouseTouching = _mouseTouching;

	// don't detect objects that are hidden behind the menu bars (ie. in
	// the scrolled-off areas of the screen)

	if (_pos.y < 0 || _pos.y > 399) {
		pointer_type = 0;
		_mouseTouching = 0;
	} else {
		// set '_mouseTouching' & return pointer_type
		pointer_type = checkMouseList();
	}

	// same as previous cycle?
	if (!mouse_flicked_off && _oldMouseTouching == _mouseTouching) {
		// yes, so nothing to do
		// BUT CARRY ON IF MOUSE WAS FLICKED OFF!
		return;
	}

	// can reset this now
	mouse_flicked_off = 0;

	//the cursor has moved onto something
	if (!_oldMouseTouching && _mouseTouching) {
		// make a copy of the object we've moved onto because one day
		// we'll move back off again! (but the list positioning could
		// theoretically have changed)

		// we can only move onto something from being on nothing - we
		// stop the system going from one to another when objects
		// overlap

		_oldMouseTouching = _mouseTouching;

		// run get on

		if (pointer_type) {
			// 'pointer_type' holds the resource id of the
			// pointer anim

			setMouse(pointer_type);

			// setup luggage icon
			if (Logic::_scriptVars[OBJECT_HELD]) {
				setLuggage(_currentLuggageResource);
			}
		} else {
			byte buf[NAME_LEN];

			error("ERROR: mouse.pointer==0 for object %d (%s) - update logic script!", _mouseTouching, _vm->fetchObjectName(_mouseTouching, buf));
		}
	} else if (_oldMouseTouching && !_mouseTouching) {
		// the cursor has moved off something - reset cursor to
		// normal pointer

		_oldMouseTouching = 0;
		setMouse(NORMAL_MOUSE_ID);

		// reset luggage only when necessary
	} else if (_oldMouseTouching && _mouseTouching) {
		// The cursor has moved off something and onto something
		// else. Flip to a blank cursor for a cycle.

		// ignore the new id this cycle - should hit next cycle
		_mouseTouching = 0;
		_oldMouseTouching = 0;
		setMouse(0);

		// so we know to set the mouse pointer back to normal if 2nd
		// hot-spot doesn't register because mouse pulled away 
		// quickly (onto nothing)

		mouse_flicked_off = 1;
		
		// reset luggage only when necessary
	} else {
		// Mouse was flicked off for one cycle, but then moved onto
		// nothing before 2nd hot-spot registered

		// both '_oldMouseTouching' & '_mouseTouching' will be zero
		// reset cursor to normal pointer

		setMouse(NORMAL_MOUSE_ID);
	}

	// possible check for edge of screen more-to-scroll here on large
	// screens
}

void Mouse::setMouse(uint32 res) {
	// high level - whats the mouse - for the engine
	_mousePointerRes = res;

	if (res) {
		byte *icon = _vm->_resman->openResource(res) + sizeof(StandardHeader);
		uint32 len = _vm->_resman->fetchLen(res) - sizeof(StandardHeader);

		// don't pulse the normal pointer - just do the regular anim
		// loop

		if (res == NORMAL_MOUSE_ID)
			setMouseAnim(icon, len, RDMOUSE_NOFLASH);
		else
 			setMouseAnim(icon, len, RDMOUSE_FLASH);

		_vm->_resman->closeResource(res);
	} else {
		// blank cursor
		setMouseAnim(NULL, 0, 0);
	}
}

void Mouse::setLuggage(uint32 res) {
	_realLuggageItem = res;

	if (res) {
		byte *icon = _vm->_resman->openResource(res) + sizeof(StandardHeader);
		uint32 len = _vm->_resman->fetchLen(res) - sizeof(StandardHeader);

		setLuggageAnim(icon, len);
		_vm->_resman->closeResource(res);
	} else
		setLuggageAnim(NULL, 0);
}

void Mouse::setObjectHeld(uint32 res) {
	setLuggage(res);

	Logic::_scriptVars[OBJECT_HELD] = res;
	_currentLuggageResource = res;

	// mode locked - no menu available
	_mouseModeLocked = true;
}

uint32 Mouse::checkMouseList() {
	ScreenInfo *screenInfo = _vm->_screen->getScreenInfo();

	// Number of priorities subject to implementation needs
	for (int priority = 0; priority < 10; priority++) {
		for (uint i = 0; i < _curMouse; i++) {
			// If the mouse pointer is over this
			// mouse-detection-box

			if (_mouseList[i].priority == priority &&
			    _pos.x + screenInfo->scroll_offset_x >= _mouseList[i].x1 &&
			    _pos.x + screenInfo->scroll_offset_x <= _mouseList[i].x2 &&
			    _pos.y + screenInfo->scroll_offset_y >= _mouseList[i].y1 &&
			    _pos.y + screenInfo->scroll_offset_y <= _mouseList[i].y2) {
				// Record id
				_mouseTouching = _mouseList[i].id;

				// Change all COGS pointers to CROSHAIR
				if (_mouseList[i].pointer == USE)
					_mouseList[i].pointer = CROSHAIR;

				createPointerText(_mouseList[i].pointer_text, _mouseList[i].pointer);

				// Return pointer type
				return _mouseList[i].pointer;
			}
		}
	}

	// Touching nothing; no pointer to return
	_mouseTouching = 0;
	return 0;
}

#define POINTER_TEXT_WIDTH	640		// just in case!
#define POINTER_TEXT_PEN	184		// white

void Mouse::createPointerText(uint32 text_id, uint32 pointer_res) {
	uint32 local_text;
	uint32 text_res;
	byte *text;
	// offsets for pointer text sprite from pointer position
	int16 xOffset, yOffset;
	uint8 justification;

	if (!_objectLabels || !text_id)
		return;

	// Check what the pointer is, to set offsets correctly for text
	// position

	switch (pointer_res) {
	case CROSHAIR:
		yOffset = -7;
		xOffset = +10;
		break;
	case EXIT0:
		yOffset = +15;
		xOffset = +20;
		break;
	case EXIT1:
		yOffset = +16;
		xOffset = -10;
		break;
	case EXIT2:
		yOffset = +10;
		xOffset = -22;
		break;
	case EXIT3:
		yOffset = -16;
		xOffset = -10;
		break;
	case EXIT4:
		yOffset = -15;
		xOffset = +15;
		break;
	case EXIT5:
		yOffset = -12;
		xOffset = +10;
		break;
	case EXIT6:
		yOffset = +10;
		xOffset = +25;
		break;
	case EXIT7:
		yOffset = +16;
		xOffset = +20;
		break;
	case EXITDOWN:
		yOffset = -20;
		xOffset = -10;
		break;
	case EXITUP:
		yOffset = +20;
		xOffset = +20;
		break;
	case MOUTH:
		yOffset = -10;
		xOffset = +15;
		break;
	case NORMAL:
		yOffset = -10;
		xOffset = +15;
		break;
	case PICKUP:
		yOffset = -40;
		xOffset = +10;
		break;
	case SCROLL_L:
		yOffset = -20;
		xOffset = +20;
		break;
	case SCROLL_R:
		yOffset = -20;
		xOffset = -20;
		break;
	case USE:
		yOffset = -8;
		xOffset = +20;
		break;
	default:
		// Shouldn't happen if we cover all the different mouse
		// pointers above
		yOffset = -10;
		xOffset = +10;
		break;
	}

	// Set up justification for text sprite, based on its offsets from the
	// pointer position

	if (yOffset < 0) {
		// Above pointer
		if (xOffset < 0) {
			// Above left
			justification = POSITION_AT_RIGHT_OF_BASE;
		} else if (xOffset > 0) {
			// Above right
			justification = POSITION_AT_LEFT_OF_BASE;
		} else {
			// Above centre
			justification = POSITION_AT_CENTRE_OF_BASE;
		}
	} else if (yOffset > 0) {
		// Below pointer
		if (xOffset < 0) {
			// Below left
			justification = POSITION_AT_RIGHT_OF_TOP;
		} else if (xOffset > 0) {
			// Below right
			justification = POSITION_AT_LEFT_OF_TOP;
		} else {
			// Below centre
			justification = POSITION_AT_CENTRE_OF_TOP;
		}
	} else {
		// Same y-coord as pointer
		if (xOffset < 0) {
			// Centre left
			justification = POSITION_AT_RIGHT_OF_CENTRE;
		} else if (xOffset > 0) {
			// Centre right
			justification = POSITION_AT_LEFT_OF_CENTRE;
		} else {
			// Centre centre - shouldn't happen anyway!
			justification = POSITION_AT_LEFT_OF_CENTRE;
		}
	}

	// Text resource number, and line number within the resource

	text_res = text_id / SIZE;
	local_text = text_id & 0xffff;

	// open text file & get the line
	text = _vm->fetchTextLine(_vm->_resman->openResource(text_res), local_text);

	// 'text+2' to skip the first 2 bytes which form the
	// line reference number

	_pointerTextBlocNo = _vm->_fontRenderer->buildNewBloc(
		text + 2, _pos.x + xOffset,
		_pos.y + yOffset,
		POINTER_TEXT_WIDTH, POINTER_TEXT_PEN,
		RDSPR_TRANS | RDSPR_DISPLAYALIGN,
		_vm->_speechFontId, justification);

	// now ok to close the text file
	_vm->_resman->closeResource(text_res);
}

void Mouse::clearPointerText() {
	if (_pointerTextBlocNo) {
		_vm->_fontRenderer->killTextBloc(_pointerTextBlocNo);
		_pointerTextBlocNo = 0;
	}
}

void Mouse::hideMouse() {
	// leaves the menus open
	// used by the system when clicking right on a menu item to examine
	// it and when combining objects

	// for logic scripts
	Logic::_scriptVars[MOUSE_AVAILABLE] = 0;

	// human/mouse off
	_mouseStatus = true;

	setMouse(0);
	setLuggage(0);
}

void Mouse::noHuman() {
	hideMouse();
	clearPointerText();

	// Must be normal mouse situation or a largely neutral situation -
	// special menus use hideMouse()

	// Don't hide menu in conversations
	if (Logic::_scriptVars[TALK_FLAG] == 0)
		hideMenu(RDMENU_BOTTOM);

	if (_mouseMode == MOUSE_system_menu) {
		// Close menu
		_mouseMode = MOUSE_normal;
		hideMenu(RDMENU_TOP);
	}
}

void Mouse::addHuman() {
	// For logic scripts
	Logic::_scriptVars[MOUSE_AVAILABLE] = 1;

	if (_mouseStatus) {
		// Force engine to choose a cursor
		_mouseStatus = false;
		_mouseTouching = 1;
	}

	// Clear this to reset no-second-click system
	Logic::_scriptVars[CLICKED_ID] = 0;

	// This is now done outside the OBJECT_HELD check in case it's set to
	// zero before now!

	// Unlock the mouse from possible large object lock situtations - see
	// syphon in rm 3

	_mouseModeLocked = false;

	if (Logic::_scriptVars[OBJECT_HELD]) {
		// Was dragging something around - need to clear this again
		Logic::_scriptVars[OBJECT_HELD] = 0;

		// And these may also need clearing, just in case
		_examiningMenuIcon = false;
		Logic::_scriptVars[COMBINE_BASE] = 0;

		setLuggage(0);
	}

	// If mouse is over menu area
	if (_pos.y > 399) {
		if (_mouseMode != MOUSE_holding) {
			// VITAL - reset things & rebuild the menu
			_mouseMode = MOUSE_normal;
		}
		setMouse(NORMAL_MOUSE_ID);
	}

	// Enabled/disabled from console; status printed with on-screen debug
	// info

	if (_vm->_debugger->_testingSnR) {
		uint8 black[4] = {   0,  0,    0,   0 };
		uint8 white[4] = { 255, 255, 255,   0 };

		// Testing logic scripts by simulating instant Save & Restore

		_vm->_screen->setPalette(0, 1, white, RDPAL_INSTANT);

		// Stops all fx & clears the queue - eg. when leaving a room
		_vm->_sound->clearFxQueue();

		// Trash all object resources so they load in fresh & restart
		// their logic scripts

		_vm->_resman->killAllObjects(false);

		_vm->_screen->setPalette(0, 1, black, RDPAL_INSTANT);
	}
}

void Mouse::refreshInventory() {
	// Can reset this now
	Logic::_scriptVars[COMBINE_BASE] = 0;

	// Cause 'object_held' icon to be greyed. The rest are coloured.
	_examiningMenuIcon = true;
	buildMenu();
	_examiningMenuIcon = false;
}

void Mouse::startConversation() {
	if (Logic::_scriptVars[TALK_FLAG] == 0) {
		// See fnChooser & speech scripts
		Logic::_scriptVars[CHOOSER_COUNT_FLAG] = 0;
	}

	noHuman();
}

void Mouse::endConversation() {
	hideMenu(RDMENU_BOTTOM);

	if (_pos.y > 399) {
		// Will wait for cursor to move off the bottom menu
		_mouseMode = MOUSE_holding;
	}

	// In case DC forgets
	Logic::_scriptVars[TALK_FLAG] = 0;
}

void Mouse::monitorPlayerActivity() {
	// if there is at least one mouse event outstanding
	if (_vm->checkForMouseEvents()) {
		// reset activity delay counter
		_playerActivityDelay = 0;
	} else {
		// no. of game cycles since mouse event queue last empty
		_playerActivityDelay++;
	}
}

void Mouse::checkPlayerActivity(uint32 seconds) {
	// Convert seconds to game cycles
	uint32 threshold = seconds * 12;

	// If the actual delay is at or above the given threshold, reset the
	// activity delay counter now that we've got a positive check.

	if (_playerActivityDelay >= threshold) {
		_playerActivityDelay = 0;
		Logic::_scriptVars[RESULT] = 1;
	} else
		Logic::_scriptVars[RESULT] = 0;
}

void Mouse::pauseGame() {
	// Make the mouse cursor normal. This is the only place where we are
	// allowed to clear the luggage this way.

	clearPointerText();
	setLuggageAnim(NULL, 0);
	setMouse(0);
	setMouseTouching(1);
}

void Mouse::unpauseGame() {
	if (Logic::_scriptVars[OBJECT_HELD] && _realLuggageItem)
		setLuggage(_realLuggageItem);
}

} // End of namespace Sword2
