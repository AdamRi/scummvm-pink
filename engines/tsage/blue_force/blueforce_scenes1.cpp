/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "common/config-manager.h"
#include "tsage/blue_force/blueforce_scenes1.h"
#include "tsage/scenes.h"
#include "tsage/tsage.h"
#include "tsage/staticres.h"
#include "tsage/globals.h"

namespace TsAGE {

namespace BlueForce {

/*--------------------------------------------------------------------------
 * Scene 100 - Tsunami Title Screen #2
 *
 *--------------------------------------------------------------------------*/

void Scene100::Text::dispatch() {
	SceneText::dispatch();

	// Keep the second text string below the first one
	Scene100 *scene = (Scene100 *)BF_GLOBALS._sceneManager._scene;
	Common::Point &pt = scene->_action1._sceneText1._position;
	scene->_action1._sceneText2.setPosition(Common::Point(pt.x,
		pt.y + scene->_action1._textHeight));
}


void Scene100::Action1::signal() {
	static byte black[3] = { 0, 0, 0 };

	switch (_actionIndex++) {
	case 0:
		_state = 0;
		setDelay(6);
		break;
	case 1: {
		Common::String msg1 = g_resourceManager->getMessage(100, _state++);
		if (msg1.compareTo("LASTCREDIT")) {
			Common::String msg2 = g_resourceManager->getMessage(100, _state++);
			setTextStrings(msg1, msg2, this);
			--_actionIndex;
		} else {
			setTextStrings(BF_NAME, BF_ALL_RIGHTS_RESERVED, this);

			Common::Point pt(_sceneText1._position.x, 80);
			NpcMover *mover = new NpcMover();
			_sceneText1.addMover(mover, &pt, this);
		}
		break;
	}
	case 2:
		setDelay(600);
		break;
	case 3:
		BF_GLOBALS._sound1.fade(0, 10, 10, 1, this);
		GLOBALS._scenePalette.addFader(black, 1, 2, NULL);
		break;
	case 4:
		error("??exit");
		break;
	}
}

void Scene100::Action1::setTextStrings(const Common::String &msg1, const Common::String &msg2, Action *action) {
	// Set data for first text control
	_sceneText1._fontNumber = 10;
	_sceneText1._width = 160;
	_sceneText1._textMode = ALIGN_RIGHT;
	_sceneText1._color1 = BF_GLOBALS._scenePalette._colors.foreground;
	_sceneText1._color2 = BF_GLOBALS._scenePalette._colors.background;
	_sceneText1._color3 = BF_GLOBALS._scenePalette._colors.foreground;
	_sceneText1.setup(msg1);
	_sceneText1.fixPriority(255);
	_sceneText1.setPosition(Common::Point(
		(SCREEN_WIDTH - _sceneText1.getFrame().getBounds().width()) / 2, 202));
	_sceneText1._moveRate = 30;
	_sceneText1._moveDiff.y = 1;

	// Set data for second text control
	_sceneText2._fontNumber = 10;
	_sceneText2._width = _sceneText1._width;
	_sceneText2._textMode = _sceneText1._textMode;
	_sceneText2._color1 = _sceneText1._color1;
	_sceneText2._color2 = 31;
	_sceneText2._color3 = _sceneText1._color3;
	_sceneText2.setup(msg2);
	_sceneText2.fixPriority(255);
	GfxSurface textSurface = _sceneText2.getFrame();
	_sceneText2.setPosition(Common::Point((SCREEN_WIDTH - textSurface.getBounds().width()) / 2, 202));
	_sceneText2._moveRate = 30;
	_sceneText2._moveDiff.y = 1;

	_textHeight = textSurface.getBounds().height();
	int yp = -(_textHeight * 2);

	Common::Point pt(_sceneText1._position.x, yp);
	NpcMover *mover = new NpcMover();
	_sceneText1.addMover(mover, &pt, action);
}

void Scene100::Action2::signal() {
	Scene100 *scene = (Scene100 *)g_globals->_sceneManager._scene;
	static byte black[3] = {0, 0, 0};

	switch (_actionIndex++) {
	case 0:
		BF_GLOBALS._scenePalette.addFader(black, 1, -2, this);
		break;
	case 1:
		setDelay(180);
		break;
	case 2: {
		const char *SEEN_INTRO = "seen_intro";
		if (!ConfMan.hasKey(SEEN_INTRO) || !ConfMan.getBool(SEEN_INTRO)) {
			// First time being played, so will need to show the intro
			ConfMan.setBool(SEEN_INTRO, true);
			ConfMan.flushToDisk();
		} else {
			// Prompt user for whether to start play or watch introduction
			g_globals->_player.enableControl();

			if (MessageDialog::show2(WATCH_INTRO_MSG, START_PLAY_BTN_STRING, INTRODUCTION_BTN_STRING) == 0) {
				// Signal to start the game
				scene->_index = 190;
				remove();
				return;
			}
		}

		// At this point the introduction needs to start
		g_globals->_scenePalette.addFader(black, 1, 2, this);
		break;
	}
	case 3:
		remove();
		break;
	}
}

/*--------------------------------------------------------------------------*/

Scene100::Scene100(): SceneExt() {
	_index = 0;
}

void Scene100::postInit(SceneObjectList *OwnerList) {
	SceneExt::postInit();
	if (BF_GLOBALS._dayNumber < 6) {
		// Title
		loadScene(100);
	} else {
		// Credits
		loadScene(101);
	}
	BF_GLOBALS._scenePalette.loadPalette(2);
	BF_GLOBALS._v51C44 = 1;
	BF_GLOBALS._interfaceY = SCREEN_HEIGHT;

	g_globals->_player.postInit();
	g_globals->_player.hide();
	g_globals->_player.disableControl();
	_index = 109;

	if (BF_GLOBALS._dayNumber < 6) {
		// Title
		BF_GLOBALS._sound1.play(2);
		setAction(&_action2, this);
	} else {
		// Credits
		BF_GLOBALS._sound1.play(118);
		setAction(&_action1, this);
	}
}

void Scene100::signal() {
	++_sceneMode;
	if (BF_GLOBALS._dayNumber < 6) {
		BF_GLOBALS._scenePalette.clearListeners();
		BF_GLOBALS._scenePalette.loadPalette(100);
		BF_GLOBALS._sceneManager.changeScene(_index);
	} else {
		if (_sceneMode > 1)
			BF_GLOBALS._events.setCursor(CURSOR_ARROW);

		setAction(this, &_action1, this);
	}
}

/*--------------------------------------------------------------------------
 * Scene 109 - Introduction Bar Room
 *
 *--------------------------------------------------------------------------*/

void Scene109::Action1::signal() {
	Scene109 *scene = (Scene109 *)BF_GLOBALS._sceneManager._scene;

	switch (_actionIndex++) {
	case 0:
		setDelay(30);
		break;
	case 1:
		BF_GLOBALS._sound1.play(12);
		BF_GLOBALS._sceneObjects->draw();
		BF_GLOBALS._scenePalette.loadPalette(2);
		BF_GLOBALS._scenePalette.refresh();
		setDelay(10);
		break;
	case 2:
		scene->_text.setup(BF_19840515, this);
		break;
	case 3:
		BF_GLOBALS._v51C44 = 1;
		scene->loadScene(115);

		scene->_protaginist2.show();
		scene->_protaginist2.setPriority(133);
		scene->_protaginist1.show();
		scene->_bartender.show();
		scene->_object1.show();
		scene->_drunk.show();
		scene->_drunk.setAction(&scene->_action3);
		scene->_object2.show();
		scene->_object9.show();
		scene->_object9.setAction(&scene->_action2);

		BF_GLOBALS._v501FC = 170;
		setDelay(60);
		break;
	case 4:
		// Start drinking
		scene->_bartender.setAction(&scene->_sequenceManager4, NULL, 109, &scene->_bartender, &scene->_object2, NULL);
		scene->_protaginist1.setAction(&scene->_sequenceManager5, NULL, 107, &scene->_protaginist1, NULL);
		scene->_protaginist2.setAction(&scene->_sequenceManager6, this, 106, &scene->_protaginist2, NULL);
		break;
	case 5:
		// Open briefcase and pass over disk
		setAction(&scene->_sequenceManager6, this, 105, &scene->_object10, NULL);
		break;
	case 6:
		// Protaginist 2 walk to the bar
		scene->_object10.remove();
		setAction(&scene->_sequenceManager6, this, 100, &scene->_protaginist2, NULL);
		break;
	case 7:
		// Two thugs enter and walk to table
		scene->_object7.setAction(&scene->_sequenceManager7, NULL, 103, &scene->_object7, NULL);
		scene->_object5.setAction(&scene->_sequenceManager8, this, 102, &scene->_object5, NULL);
		scene->_protaginist2.setAction(&scene->_sequenceManager6, NULL, 104, &scene->_protaginist2, &scene->_bartender, NULL);
		break;
	case 8:
		// Protaginist 1 leaves, protaginist 2 stands up
		setAction(&scene->_sequenceManager8, this, 101, &scene->_object5, &scene->_protaginist1, NULL);
		break;
	case 9:
		// Shots fired!
		scene->_protaginist1.setAction(&scene->_sequenceManager5, this, 98, &scene->_protaginist1, NULL);
		scene->_object7.setAction(&scene->_sequenceManager7, NULL, 99, &scene->_object7, NULL);
		break;
	case 10:
		// End scene
		scene->_sceneMode = 1;
		remove();
		break;
	}
}

void Scene109::Action2::signal() {
	Scene109 *scene = (Scene109 *)BF_GLOBALS._sceneManager._scene;
	scene->setAction(&scene->_sequenceManager2, this, 3117, &scene->_object9, NULL);
}

void Scene109::Action3::signal() {
	Scene109 *scene = (Scene109 *)BF_GLOBALS._sceneManager._scene;
	scene->setAction(&scene->_sequenceManager3, this, 108, &scene->_drunk, NULL);
}

/*--------------------------------------------------------------------------*/

Scene109::Text::Text(): SceneText() {
	_action = NULL;
	_frameNumber = 0;
	_diff = 0;
}

void Scene109::Text::setup(const Common::String &msg, Action *action) {
	_frameNumber = BF_GLOBALS._events.getFrameNumber();
	_diff = 180;
	_action = action;
	_fontNumber = 4;
	_width = 300;
	_textMode = ALIGN_CENTER;
	_color1 = BF_GLOBALS._scenePalette._colors.background;
	_color2 = _color3 = 0;

	SceneText::setup(msg);

	// Center the text on-screen
	reposition();
	_bounds.center(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	// Set the new position
	_position.x = _bounds.left;
	_position.y = _bounds.top;
}

void Scene109::Text::synchronize(Serializer &s) {
	SceneText::synchronize(s);
	SYNC_POINTER(_action);
	s.syncAsUint32LE(_frameNumber);
	s.syncAsSint16LE(_diff);
}

void Scene109::Text::dispatch() {
	if (_diff) {
		uint32 frameNumber = BF_GLOBALS._events.getFrameNumber();
		if (_frameNumber < frameNumber) {
			_diff -= frameNumber - _frameNumber;
			_frameNumber = frameNumber;

			if (_diff <= 0) {
				// Time has expired, so remove the text and signal the designated action
				remove();
				if (_action)
					_action->signal();
			}
		}
	}
}

/*--------------------------------------------------------------------------*/

Scene109::Scene109(): PalettedScene() {
}

void Scene109::postInit(SceneObjectList *OwnerList) {
	PalettedScene::postInit(OwnerList);
	loadScene(999);

	_protaginist2.postInit();
	_protaginist2.setVisage(119);
	_protaginist2.setFrame(11);
	_protaginist2.fixPriority(133);
	_protaginist2.setPosition(Common::Point(165, 124));
	_protaginist2.hide();

	_protaginist1.postInit();
	_protaginist1.setVisage(118);
	_protaginist1.setStrip(1);
	_protaginist1.setFrame(8);
	_protaginist1.fixPriority(132);
	_protaginist1.setPosition(Common::Point(143, 125));
	_protaginist1.hide();

	_bartender.postInit();
	_bartender.setVisage(121);
	_bartender.setStrip(2);
	_bartender.setFrame(1);
	_bartender.setPriority(-1);
	_bartender.setPosition(Common::Point(92, 64));
	_bartender.hide();

	_object1.postInit();
	_object1.setVisage(121);
	_object1.setStrip(6);
	_object1.setFrame(1);
	_object1.setPriority(-1);
	_object1.setPosition(Common::Point(110, 64));
	_object1.hide();

	_drunk.postInit();
	_drunk.setVisage(120);
	_drunk.setStrip(2);
	_drunk.setFrame(5);
	_drunk.setPriority(-1);
	_drunk.setPosition(Common::Point(127, 97));
	_drunk.hide();

	_object2.postInit();
	_object2.setVisage(121);
	_object2.setStrip(5);
	_object2.setFrame(1);
	_object2.setPriority(-1);
	_object2.setPosition(Common::Point(104, 64));
	_object2.hide();

	_object9.postInit();
	_object9.setVisage(115);
	_object9.setStrip(4);
	_object9.setFrame(1);
	_object9.setPosition(Common::Point(262, 29));
	_object9.hide();

	_object5.postInit();
	_object5.hide();

	_object7.postInit();
	_object7.hide();

	_object10.postInit();
	_object10.hide();

	BF_GLOBALS._player.disableControl();
	setAction(&_action1, this);
}

void Scene109::signal() {
	if (_sceneMode == 1) {
		BF_GLOBALS._scenePalette.clearListeners();
		BF_GLOBALS._sceneManager.changeScene(110);
	}
}

/*--------------------------------------------------------------------------
 * Scene 190 - Front of Police Station
 *
 *--------------------------------------------------------------------------*/

bool Scene190::Object4::startAction(CursorType action, Event &event) {
	Scene190 *scene = (Scene190 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_USE: {
		BF_GLOBALS._player.disableControl();
		scene->_sceneMode = 13;
		Common::Point pt(62, 96);
		PlayerMover *mover = new PlayerMover();
		BF_GLOBALS._player.addMover(mover, &pt, scene);
		return true;
	}
	default:
		return NamedObject::startAction(action, event);
	}
}

/*--------------------------------------------------------------------------*/

bool Scene190::Item1::startAction(CursorType action, Event &event) {
	Scene190 *scene = (Scene190 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_USE:
		scene->setAction(&scene->_action1);
		return true;
	default:
		return NamedHotspot::startAction(action, event);
	}
}

bool Scene190::Item2::startAction(CursorType action, Event &event) {
	Scene190 *scene = (Scene190 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_USE:
		scene->_stripManager.start(1900, scene);
		return true;
	default:
		return NamedHotspot::startAction(action, event);
	}
}

bool Scene190::Exit::startAction(CursorType action, Event &event) {
	Scene190 *scene = (Scene190 *)BF_GLOBALS._sceneManager._scene;

	Common::Point pt(316, 91);
	PlayerMover *mover = new PlayerMover();
	BF_GLOBALS._player.addMover(mover, &pt, scene);
	return true;
}

/*--------------------------------------------------------------------------*/

void Scene190::Action1::signal() {
	Scene190 *scene = (Scene190 *)BF_GLOBALS._sceneManager._scene;

	switch (_actionIndex++) {
	case 0:
		BF_GLOBALS._player.disableControl();
		setDelay(2);
		break;
	case 1: {
		ADD_MOVER(BF_GLOBALS._player, 165, 91);
		break;
	}
	case 2:
		scene->_sound.play(82);
		scene->_object2.animate(ANIM_MODE_5, this);
		break;
	case 3:
		ADD_MOVER(BF_GLOBALS._player, 180, 86);
		break;
	case 4:
		scene->_sound.play(82);
		scene->_object2.animate(ANIM_MODE_6, this);
		break;
	case 5:
		BF_GLOBALS._sound1.fadeOut2(NULL);
		BF_GLOBALS._sceneManager.changeScene(315);
		break;
	}
}

/*--------------------------------------------------------------------------*/

Scene190::Scene190(): SceneExt() {
	_fieldB52 = true;
	_cursorVisage.setVisage(1, 8);
}

void Scene190::postInit(SceneObjectList *OwnerList) {
	BF_GLOBALS._dialogCenter.y = 100;
	if ((BF_GLOBALS._sceneManager._previousScene == 100) ||
			(BF_GLOBALS._sceneManager._previousScene == 20)) {
//		clearScreen();
	}
	if (BF_GLOBALS._dayNumber == 0)
		// If at start of game, change to first day
		BF_GLOBALS._dayNumber = 1;

	// Load the scene data
	loadScene(190);
	BF_GLOBALS._scenePalette.loadPalette(2);

	_stripManager.addSpeaker(&_speaker);
	BF_GLOBALS._player.postInit();
	BF_GLOBALS._player.disableControl();

	// Initialise objects
	_object2.postInit();
	_object2.setVisage(190);
	_object2.setStrip(1);
	_object2.setPosition(Common::Point(179, 88));

	_object3.postInit();
	_object3.setVisage(190);
	_object3.setStrip(2);
	_object3.fixPriority(200);
	_object3.setPosition(Common::Point(170, 31));
	_object3.animate(ANIM_MODE_7, 0, NULL);
	_object3.setDetails(190, 8, 26, 19, 1, NULL);

	if (BF_GLOBALS.getFlag(fWithLyle)) {
		BF_GLOBALS._player.setVisage(303);
		BF_GLOBALS._player.setObjectWrapper(new SceneObjectWrapper());
		BF_GLOBALS._player.animate(ANIM_MODE_1, NULL);
		BF_GLOBALS._player._moveDiff = Common::Point(3, 1);

		_object4.postInit();
		_object4.setVisage(444);
		_object4.setFrame(2);
		_object4.setPosition(Common::Point(54, 114));
		_object4.setDetails(190, -1, -1, -1, 1, NULL);

		switch (BF_GLOBALS._sceneManager._previousScene) {
		case 300: {
			_sceneMode = 12;
			BF_GLOBALS._player.setPosition(Common::Point(316, 91));
			ADD_MOVER(BF_GLOBALS._player, 305, 91);
			break;
		}
		case 315:
			_sceneMode = 1901;
			setAction(&_sequenceManager, this, 1901, &BF_GLOBALS._player, &_object2, NULL);
			break;
		case 50:
		case 60:
		default:
			_fieldB52 = false;
			BF_GLOBALS._player.setPosition(Common::Point(62, 96));
			BF_GLOBALS._player._strip = 3;
			BF_GLOBALS._player.enableControl();
			break;
		}
	} else {
		BF_GLOBALS._player.setVisage(BF_GLOBALS._player._visage);
		BF_GLOBALS._player.setObjectWrapper(new SceneObjectWrapper());
		BF_GLOBALS._player.animate(ANIM_MODE_1, NULL);

		switch (BF_GLOBALS._sceneManager._previousScene) {
		case 300: {
			if (!BF_GLOBALS.getFlag(onBike)) {
				BF_GLOBALS._player._moveDiff = Common::Point(3, 1);
				_sceneMode = BF_GLOBALS.getFlag(onDuty) ? 11 : 12;
				BF_GLOBALS._player.setVisage(BF_GLOBALS.getFlag(onDuty) ? 1304 : 303);
				BF_GLOBALS._player.setPosition(Common::Point(316, 91));
				ADD_MOVER(BF_GLOBALS._player, 305, 91);
			} else {
				BF_GLOBALS._player.disableControl();
				_sceneMode = BF_GLOBALS.getFlag(onDuty) ? 193 : 191;
				setAction(&_sequenceManager, this, 193, &BF_GLOBALS._player, NULL);
			}
			break;
		}
		case 315:
			BF_GLOBALS._player._moveDiff = Common::Point(3, 1);
			_sceneMode = BF_GLOBALS.getFlag(onDuty) ? 1900 : 1901;
			setAction(&_sequenceManager, this, _sceneMode, &BF_GLOBALS._player, &_object2, NULL);
			break;
		case 50:
		case 60:
		default:
			BF_GLOBALS.setFlag(onBike);
			BF_GLOBALS._player.disableControl();
			_sceneMode = BF_GLOBALS.getFlag(onDuty) ? 192 : 190;
			setAction(&_sequenceManager, this, _sceneMode, &BF_GLOBALS._player, NULL);
			break;
		}
	}

	if (BF_GLOBALS.getFlag(onBike)) {
		BF_GLOBALS._sound1.play(BF_GLOBALS.getFlag(onDuty) ? 37 : 29);
	} else if (BF_GLOBALS._sceneManager._previousScene != 300) {
		BF_GLOBALS._sound1.play(33);
	}

	_exit.setDetails(Rect(310, 50, 320, 125), 190, -1, -1, -1, 1, NULL);
	_item2.setDetails(Rect(108, 1, 111, 94), 190, 7, 11, 18, 1, NULL);
	_item4.setDetails(2, 190, 5, 10, 16, 1);
	_item3.setDetails(1, 190, 4, 10, 15, 1);
	_item8.setDetails(6, 190, 20, 21, 22, 1);
	_item1.setDetails(7, 190, 1, 10, -1, 1);
	_item7.setDetails(5, 190, 0, 10, 12, 1);
	_item6.setDetails(4, 190, 2, 10, 13, 1);
	_item5.setDetails(3, 190, 3, 10, 14, 1);
	_item9.setDetails(Rect(0, 0, 89, 68), 190, 6, 10, 17, 1, NULL);
	_item10.setDetails(Rect(0, 0, SCREEN_WIDTH, BF_INTERFACE_Y), 190, 23, -1, -1, 1, NULL);
}

void Scene190::signal() {
	switch (_sceneMode) {
	case 10:
		if ((BF_GLOBALS._dayNumber == 2) && (BF_GLOBALS._bookmark < bEndDayOne))
			BF_GLOBALS._sound1.changeSound(49);
		BF_GLOBALS._sceneManager.changeScene(300);
		break;
	case 11:
	case 12:
	case 1900:
	case 1901:
		BF_GLOBALS._player.enableControl();
		_fieldB52 = false;
		break;
	case 13:
	case 191:
	case 193:
		BF_GLOBALS._sceneManager.changeScene(60);
		break;
	case 190:
	case 192:
		BF_GLOBALS._sceneManager.changeScene(300);
		break;
	case 0:
	default:
		BF_GLOBALS._player.enableControl();
		break;
	}
}

void Scene190::process(Event &event) {
	SceneExt::process(event);

	if (BF_GLOBALS._player._enabled && !_focusObject && (event.mousePos.y < (BF_INTERFACE_Y - 1))) {
		// Check if the cursor is on an exit
		if (_exit.contains(event.mousePos)) {
			GfxSurface surface = _cursorVisage.getFrame(3);
			BF_GLOBALS._events.setCursor(surface);
		} else {
			// In case an exit cursor was being shown, restore the previously selected cursor
			CursorType cursorId = BF_GLOBALS._events.getCursor();
			BF_GLOBALS._events.setCursor(cursorId);
		}
	}
}

void Scene190::dispatch() {
	SceneExt::dispatch();

	if (!_action && !_fieldB52 && (BF_GLOBALS._player._position.x >= 310)
			&& !BF_GLOBALS.getFlag(onBike)) {
		// Handle walking off to the right side of the screen
		BF_GLOBALS._player.disableControl();
		_fieldB52 = true;
		_sceneMode = 10;

		ADD_MOVER(BF_GLOBALS._player, 330, BF_GLOBALS._player._position.y);
	}
}

} // End of namespace BlueForce

} // End of namespace TsAGE
