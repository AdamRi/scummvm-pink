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

#include "tsage/blue_force/blueforce_scenes8.h"
#include "tsage/blue_force/blueforce_dialogs.h"
#include "tsage/scenes.h"
#include "tsage/tsage.h"
#include "tsage/staticres.h"

namespace TsAGE {

namespace BlueForce {

/*--------------------------------------------------------------------------
 * Scene 800 - Jamison & Ryan
 *
 *--------------------------------------------------------------------------*/

bool Scene800::Item1::startAction(CursorType action, Event &event) {
	if (action == CURSOR_LOOK) {
		SceneItem::display2(800, 11);
		return true;
	} else {
		return SceneHotspot::startAction(action, event);
	}
}

bool Scene800::Item2::startAction(CursorType action, Event &event) {
	if (action == CURSOR_LOOK) {
		SceneItem::display2(800, 1);
		return true;
	} else {
		return SceneHotspot::startAction(action, event);
	}
}

/*--------------------------------------------------------------------------*/

bool Scene800::Doorway::startAction(CursorType action, Event &event) {
	Scene800 *scene = (Scene800 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(800, 4);
		return true;
	case CURSOR_USE:
		if (BF_GLOBALS._dayNumber == 5)
			SceneItem::display2(800, 14);
		else if (BF_GLOBALS._dayNumber < 2)
			SceneItem::display2(800, BF_GLOBALS.getFlag(onDuty) ? 6 : 15);
		else if (((BF_INVENTORY.getObjectScene(INV_SCHEDULE) == 1) && (BF_GLOBALS._dayNumber == 3)) ||
				(BF_GLOBALS._bookmark == bDoneWithIsland))
			SceneItem::display(800, 5);
		else {
			if (BF_GLOBALS.getFlag(fWithLyle)) {
				ADD_PLAYER_MOVER_NULL(scene->_lyle, 277, 145);
			}

			BF_GLOBALS._player.disableControl();
			scene->_sceneMode = 8000;
			setAction(&scene->_sequenceManager, scene, 8000, &BF_GLOBALS._player, this, NULL);
		}
		return true;
	default:
		return NamedObject::startAction(action, event);
	}
}

bool Scene800::Car1::startAction(CursorType action, Event &event) {
	Scene800 *scene = (Scene800 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(800, 2);
		return true;
	case CURSOR_USE:
		SceneItem::display2(800, 3);
		return true;
	case INV_TICKET_BOOK:
		if (BF_GLOBALS.getFlag(ticketVW))
			SceneItem::display2(800, 12);
		else if (!BF_GLOBALS.getFlag(onDuty))
			SceneItem::display2(800, 13);
		else {
			BF_GLOBALS.setFlag(ticketVW);
			BF_GLOBALS._player.disableControl();
			BF_GLOBALS._uiElements.addScore(30);
			scene->_sceneMode = 8005;
			scene->setAction(&scene->_action1);
		}
		return true;
	default:
		return NamedObject::startAction(action, event);
	}
}

bool Scene800::Motorcycle::startAction(CursorType action, Event &event) {
	Scene800 *scene = (Scene800 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(800, 0);
		return true;
	case CURSOR_USE:
		if (BF_GLOBALS.getFlag(fWithLyle))
			SceneItem::display2(800, 8);
		else {
			BF_GLOBALS._player.disableControl();
			scene->_sceneMode = 8002;
			setAction(&scene->_sequenceManager, scene, 8002, &BF_GLOBALS._player, this, NULL);
		}
		return true;
	default:
		return NamedObject::startAction(action, event);
	}
}

bool Scene800::Lyle::startAction(CursorType action, Event &event) {
	Scene800 *scene = (Scene800 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(800, 7);
		return true;
	case CURSOR_TALK:
		BF_GLOBALS._player.disableControl();
		scene->_sceneMode = 8001;
		scene->_stripManager.start(8003, scene);
		return true;
	default:
		return NamedObject::startAction(action, event);
	}
}

bool Scene800::Car2::startAction(CursorType action, Event &event) {
	Scene800 *scene = (Scene800 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(800, 9);
		return true;
	case CURSOR_USE:
		if (!BF_GLOBALS.getFlag(fWithLyle))
			SceneItem::display2(800, 10);
		else {
			BF_GLOBALS._player.disableControl();
			scene->_sceneMode = 8003;
			setAction(&scene->_sequenceManager, scene, 8003, &BF_GLOBALS._player, &scene->_lyle, NULL);
		}
		return true;
	default:
		return NamedObject::startAction(action, event);
	}
}

/*--------------------------------------------------------------------------*/

void Scene800::Action1::signal() {
	Scene800 *scene = (Scene800 *)BF_GLOBALS._sceneManager._scene;

	switch (_actionIndex++) {
	case 0: {
		BF_GLOBALS._player.disableControl();
		ADD_PLAYER_MOVER_THIS(BF_GLOBALS._player, 95, 153);
		break;
	}
	case 1: {
		ADD_MOVER(BF_GLOBALS._player, 70, 157);
		break;
	}
	case 2:
		BF_GLOBALS._player.setVisage(800);
		BF_GLOBALS._player.setStrip(1);
		BF_GLOBALS._player.setFrame(1);
		BF_GLOBALS._player.fixPriority(200);
		BF_GLOBALS._player.animate(ANIM_MODE_4, 16, 1, this);
		break;
	case 3:
		scene->_object6.postInit();
		scene->_object6.setVisage(800);
		scene->_object6.setStrip(2);
		scene->_object6.setFrame(2);
		scene->_object6.setPosition(Common::Point(58, 135));
		scene->_object6.fixPriority(170);
		BF_GLOBALS._player.animate(ANIM_MODE_5, this);
		break;
	case 4:
		BF_GLOBALS._player.setVisage(352);
		BF_GLOBALS._player.setStrip(2);
		BF_GLOBALS._player.setFrame(1);
		BF_GLOBALS._player.fixPriority(-1);
		BF_GLOBALS._player.animate(ANIM_MODE_1, NULL);
		ADD_MOVER(BF_GLOBALS._player, 89, 154);
		break;
	case 5:
		BF_GLOBALS._player.enableControl();
		remove();
		break;
	}

}

/*--------------------------------------------------------------------------*/

void Scene800::postInit(SceneObjectList *OwnerList) {
	loadScene(800);
	SceneExt::postInit();
	setZoomPercents(130, 50, 155, 100);

	_stripManager.addSpeaker(&_gameTextSpeaker);
	_stripManager.addSpeaker(&_jakeJacketSpeaker);
	_stripManager.addSpeaker(&_lyleHatSpeaker);

	BF_GLOBALS._player.postInit();
	BF_GLOBALS._player.setVisage(1358);
	BF_GLOBALS._player.animate(ANIM_MODE_1, NULL);
	BF_GLOBALS._player.setObjectWrapper(new SceneObjectWrapper());
	BF_GLOBALS._player.setPosition(Common::Point(319, 144));
	BF_GLOBALS._player._moveDiff = Common::Point(3, 2);
	BF_GLOBALS._player.changeZoom(-1);

	_motorcycle.postInit();
	_motorcycle.setVisage(301);
	_motorcycle.setPosition(Common::Point(319, 162));

	if ((BF_GLOBALS._dayNumber >= 2) && (BF_GLOBALS._dayNumber < 5)) {
		_car2.postInit();
		_car2.setVisage(444);
		_car2.setStrip(3);
		_car2.setFrame(2);
		_car2.setPosition(Common::Point(237, 161));
		_car2.changeZoom(110);
		_car2.fixPriority(158);
		BF_GLOBALS._sceneItems.push_back(&_car2);

		BF_GLOBALS._walkRegions.proc1(8);
	}

	if ((BF_GLOBALS._dayNumber == 4) && (BF_GLOBALS._bookmark < bEndDayThree)) {
		_car2.remove();
		BF_GLOBALS._walkRegions.proc2(8);
	}

	if (BF_GLOBALS.getFlag(fWithLyle)) {
		_lyle.postInit();
		_lyle.setVisage(1359);
		_lyle.setObjectWrapper(new SceneObjectWrapper());
		_lyle.animate(ANIM_MODE_1, NULL);
		_lyle.setPosition(Common::Point(210, 160));
		_lyle.changeZoom(-1);
		BF_GLOBALS._sceneItems.push_back(&_lyle);

		_motorcycle.setVisage(580);
		_motorcycle.setStrip(2);
		_motorcycle.setFrame(2);
	} else if (BF_GLOBALS.getFlag(onDuty)) {
		BF_GLOBALS._player.setVisage(352);
	} else {
		_motorcycle.setVisage(580);
		_motorcycle.setStrip(2);
		_motorcycle.setFrame(2);
	}
	BF_GLOBALS._sceneItems.push_back(&_motorcycle);

	_doorway.postInit();
	_doorway.setVisage(800);
	_doorway.setStrip(3);
	_doorway.setPosition(Common::Point(287, 140));
	BF_GLOBALS._sceneItems.push_back(&_doorway);

	_car1.postInit();
	_car1.setVisage(800);
	_car1.setStrip(2);
	_car1.setFrame(1);
	_car1.setPosition(Common::Point(58, 163));
	BF_GLOBALS._sceneItems.push_back(&_car1);

	if (BF_GLOBALS._sceneManager._previousScene == 810) {
		if (BF_GLOBALS.getFlag(fWithLyle)) {
			BF_GLOBALS._player.setPosition(Common::Point(271, 148));
			BF_GLOBALS._player.enableControl();
		} else {
			BF_GLOBALS._player.changeZoom(75);
			BF_GLOBALS._player.setPosition(Common::Point(275, 135));
			BF_GLOBALS._player.disableControl();
			_sceneMode = 8001;
			setAction(&_sequenceManager, this, 8001, &BF_GLOBALS._player, &_doorway, NULL);
		}
	} else if ((BF_INVENTORY.getObjectScene(INV_SCHEDULE) == 1) && (BF_GLOBALS._bookmark == bFlashBackThree)) {
		BF_GLOBALS._bookmark = bFlashBackThree;
		BF_GLOBALS._player.disableControl();
		BF_GLOBALS._player.setPosition(Common::Point(231, 150));
		BF_GLOBALS._player.setStrip(3);

		_lyle.setPosition(Common::Point(244, 162));
		_lyle.setStrip(4);
		_sceneMode = 8004;
		setAction(&_sequenceManager, this, 8001, &_lyle, &_doorway, NULL);
	} else {
		BF_GLOBALS._player.updateAngle(_motorcycle._position);
		BF_GLOBALS._player.enableControl();
	}

	_item2.setBounds(Rect(47, 26, 319, 143));
	BF_GLOBALS._sceneItems.push_back(&_item2);
	_item1.setBounds(Rect(0, 0, 320, 165));
	BF_GLOBALS._sceneItems.push_back(&_item1);

	BF_GLOBALS._sound1.fadeSound(33);
}

void Scene800::signal() {
	switch (_sceneMode) {
	case 8000:
		BF_GLOBALS._sceneManager.changeScene(810);
		break;
	case 8001:
	case 8005:
		BF_GLOBALS._player.enableControl();
		break;
	case 8002:
	case 8003:
		BF_GLOBALS._sceneManager.changeScene(60);
		break;
	case 8004:
		BF_GLOBALS.clearFlag(fWithLyle);
		_lyle.remove();
	}
}

void Scene800::dispatch() {
	SceneExt::dispatch();
	if (BF_GLOBALS._player.getRegionIndex() == 7) {
		BF_GLOBALS._player.updateZoom();
	} else {
		BF_GLOBALS._player.changeZoom(-1);
	}
}

/*--------------------------------------------------------------------------
 * Scene 810 - Lyle's Office
 *
 *--------------------------------------------------------------------------*/

void Scene810::Action1::signal() {
	Scene810 *scene = (Scene810 *)BF_GLOBALS._sceneManager._scene;

	switch (_actionIndex++) {
	case 0:
		if (scene->_lyle._position.x == 115) {
			ADD_PLAYER_MOVER(174, 142);
		} else if (scene->_lyle._position.x < 160) {
			ADD_PLAYER_MOVER(scene->_lyle._position.x + 20, scene->_lyle._position.y + 15);
		} else {
			ADD_PLAYER_MOVER(scene->_lyle._position.x - 20, scene->_lyle._position.y + 15);
		}
		break;
	case 1:
		BF_GLOBALS._player.updateAngle(scene->_lyle._position);
		scene->_stripManager.start(scene->_sceneMode, this);
		break;
	case 2:
		if (BF_GLOBALS.getFlag(shownFax) && (BF_GLOBALS._dayNumber == 3) && !BF_GLOBALS.getFlag(fWithLyle))
			BF_GLOBALS.setFlag(showMugAround);

		BF_GLOBALS._player.enableControl();
		remove();
		break;
	default:
		break;
	}
}

void Scene810::Action2::signal() {
	Scene810 *scene = (Scene810 *)BF_GLOBALS._sceneManager._scene;

	switch (_actionIndex++) {
	case 0:
		if (!BF_GLOBALS.getFlag(shownLyleCrate1Day1))
			BF_GLOBALS._uiElements.addScore(30);

		if (scene->_lyle._position.x == 115) {
			ADD_PLAYER_MOVER(174, 142);
		} else {
			ADD_PLAYER_MOVER(193, 105);
		}
		break;
	case 1:
		BF_GLOBALS._player.setStrip(8);
		if (scene->_lyle._position.x != 115)
			_actionIndex = 3;

		if (BF_GLOBALS.getFlag(shownLyleCrate1Day1)) {
			if (BF_GLOBALS.getFlag(onDuty)) {
				scene->_stripManager.start(8138, this);
			} else {
				scene->_stripManager.start((BF_GLOBALS._dayNumber == 3) ? 8110 : 8126, this);
			}
		} else if (BF_GLOBALS._dayNumber >= 3) {
			scene->_stripManager.start(8110, this);
		} else {
			scene->_stripManager.start(BF_GLOBALS.getFlag(onDuty) ? 8140 : 8128, this);
		}
		break;
	case 2:
		scene->setAction(&scene->_sequenceManager1, this, 8117, &scene->_lyle, &scene->_chair, NULL);
		break;
	case 3:
		BF_GLOBALS._walkRegions.proc2(4);
		ADD_PLAYER_MOVER_THIS(scene->_lyle, 27, 124);
		break;
	case 4:
		scene->_lyle.setVisage(813);
		scene->_lyle.setStrip(2);
		scene->_lyle.setFrame(1);
		
		ADD_PLAYER_MOVER(84, 113);
		break;
	case 5:
		BF_GLOBALS._player.setStrip(8);
		scene->_lyle.animate(ANIM_MODE_4, 5, 1, this);
		break;
	case 6:
		scene->_lyle.animate(ANIM_MODE_5, NULL);
		scene->_stripManager.start(8111, this);
		break;
	case 7:
		scene->_lyle.setVisage(845);
		scene->_lyle.setStrip(1);
		scene->_lyle.setFrame(1);
		scene->_lyle.animate(ANIM_MODE_1, NULL);

		scene->_stripManager.start(BF_GLOBALS.getFlag(onDuty) ? 8137 : 8112, this);
		break;
	case 8:
		BF_GLOBALS._walkRegions.proc1(13);
		BF_GLOBALS._player.enableControl();
		remove();
		break;
	default:
		break;
	}
}

/*--------------------------------------------------------------------------*/

bool Scene810::Lyle::startAction(CursorType action, Event &event) {
	Scene810 *scene = (Scene810 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(810, 26);
		return true;

	case CURSOR_TALK:
		BF_GLOBALS._player.disableControl();
		BF_GLOBALS._player.updateAngle(_position);

		switch (BF_GLOBALS._dayNumber) {
		case 4:
			scene->_sceneMode = (BF_INVENTORY.getObjectScene(INV_AUTO_RIFLE) == 810) ? 8001 : 8123;
			break;
		case 2:
			if (BF_GLOBALS.getFlag(shownFax))
				scene->_sceneMode = 8151;
			else if (BF_GLOBALS.getFlag(onDuty)) {
				if (BF_GLOBALS.getFlag(shownLyleCrate1)) {
					scene->_sceneMode = BF_GLOBALS.getFlag(shownLyleCrate1Day1) ? 8145 : 8154;
				} else if (BF_GLOBALS.getFlag(shownLyleRapsheet) || BF_GLOBALS.getFlag(shownLylePO)) {
					scene->_sceneMode = 8145;
				} else if (!_flag) {
					++_flag;
					scene->_sceneMode = 8139;
				} else {
					scene->_sceneMode = 8152;
				}
			} else {
				if (BF_GLOBALS.getFlag(shownLyleCrate1)) {
					scene->_sceneMode = BF_GLOBALS.getFlag(shownLyleCrate1Day1) ? 8133 : 8153;
				} else if (BF_GLOBALS.getFlag(shownLyleRapsheet) || BF_GLOBALS.getFlag(shownLylePO)) {
					scene->_sceneMode = 8133;
				} else if (!_flag) {
					++_flag;
					scene->_sceneMode = 8127;
				} else {
					scene->_sceneMode = 8152;
				}
			}
			break;
		default:
			if (BF_GLOBALS.getFlag(shownFax))
				scene->_sceneMode = 8146;
			else if (BF_GLOBALS.getFlag(shownLylePO) || BF_GLOBALS.getFlag(shownLyleRapsheet) || BF_GLOBALS.getFlag(shownLyleCrate1))
				scene->_sceneMode = 8108;
			else if (BF_INVENTORY.getObjectScene(INV_COBB_RAP) == 1)
				scene->_sceneMode = 8107;
			else
				scene->_sceneMode = 8155;
			break;
		}

		scene->setAction(&scene->_action1);
		return true;

	case INV_FOREST_RAP:
		if (BF_GLOBALS.getFlag(shownLyleRapsheet))
			scene->_sceneMode = 8148;
		else {
			BF_GLOBALS.setFlag(shownLyleRapsheet);
			if (BF_GLOBALS._dayNumber != 2) {
				scene->_sceneMode = BF_GLOBALS.getFlag(shownLylePO) ? 8122 : 8101;
			} else if (BF_GLOBALS.getFlag(onDuty)) {
				scene->_sceneMode = BF_GLOBALS.getFlag(shownLylePO) ? 8142 : 8143;
			} else {
				scene->_sceneMode = BF_GLOBALS.getFlag(shownLylePO) ? 8130 : 8131;
			}
		}
		BF_GLOBALS._player.disableControl();
		scene->setAction(&scene->_action1);
		return true;

	case INV_COBB_RAP:
		if (BF_GLOBALS.getFlag(shownFax)) {
			scene->_sceneMode = 8151;
		} else {
			BF_GLOBALS.setFlag(shownFax);
			scene->_sceneMode = 8118;
		}

		BF_GLOBALS._player.disableControl();
		scene->setAction(&scene->_action1);
		return true;

	case INV_AUTO_RIFLE:
		BF_INVENTORY.setObjectScene(INV_AUTO_RIFLE, 810);
		BF_GLOBALS._player.disableControl();
		scene->_sceneMode = 8116;
		scene->setAction(&scene->_sequenceManager1, scene, 8116, &BF_GLOBALS._player, NULL);
		return true;

	case INV_PRINT_OUT:
		if (BF_GLOBALS.getFlag(shownLylePO)) {
			scene->_sceneMode = 8149;
		} else if (!BF_GLOBALS.getFlag(shownLylePO)) {
			if (!BF_GLOBALS.getFlag(shownFax)) {
				// WORKAROUND: Original did not do a 'not', but I think this is correct
				BF_GLOBALS.setFlag(shownFax);
				scene->_sceneMode = 8125;
			} else if (BF_GLOBALS.getFlag(shownLyleRapsheet)) {
				scene->_sceneMode = 8104;
			} else {
				scene->_sceneMode = 8121;
			}
		} else if (BF_GLOBALS.getFlag(onDuty)) {
			scene->_sceneMode = 8141;
		} else {
			if (BF_GLOBALS.getFlag(shownLyleRapsheet) || BF_GLOBALS.getFlag(shownLyleCrate1))
				scene->_sceneMode = 8129;
			else
				scene->_sceneMode = 8121;
		}
		BF_GLOBALS._player.disableControl();
		scene->setAction(&scene->_action1);
		return true;

	case INV_CRATE1:
		if (BF_GLOBALS.getFlag(shownLyleCrate1)) {
			BF_GLOBALS._player.disableControl();
			scene->_sceneMode = 8147;
			scene->setAction(&scene->_action1);
		} else {
			BF_GLOBALS.setFlag(shownLyleCrate1);
			BF_GLOBALS._player.disableControl();
			scene->setAction(&scene->_action2);
		}
		return true;

	default:
		return NamedObjectExt::startAction(action, event);
	}
}		

bool Scene810::Chair::startAction(CursorType action, Event &event) {
	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(810, 28);
		return true;
	case CURSOR_USE:
		SceneItem::display2(810, 29);
		return true;
	default:
		return NamedObject::startAction(action, event);
	}
}

bool Scene810::Object3::startAction(CursorType action, Event &event) {
	Scene810 *scene = (Scene810 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(810, 30);
		return true;
	case CURSOR_USE:
		if (!BF_GLOBALS.getFlag(seenFolder)) {
			BF_GLOBALS.setFlag(seenFolder);
			BF_GLOBALS._player.disableControl();
			scene->_sceneMode = 8104;
			scene->setAction(&scene->_sequenceManager1, scene, 8104, &BF_GLOBALS._player, this, NULL);
		} else if (BF_INVENTORY.getObjectScene(INV_MICROFILM) == 810) {
			BF_GLOBALS._player.disableControl();
			scene->_sceneMode = 8114;
			scene->setAction(&scene->_sequenceManager1, scene, 8114, &BF_GLOBALS._player, NULL);
		} else {
			SceneItem::display2(810, 38);
		}
		return true;
	default:
		return NamedObject::startAction(action, event);
	}
}

bool Scene810::Object4::startAction(CursorType action, Event &event) {
	Scene810 *scene = (Scene810 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_WALK:
		return true;
	case CURSOR_LOOK:
		SceneItem::display2(810, 32);
		return true;
	case CURSOR_USE:
		if (scene->_rect3.contains(event.mousePos)) {
			if (BF_INVENTORY.getObjectScene(INV_PRINT_OUT) == 811) {
				BF_GLOBALS._uiElements.addScore(50);
				scene->_sound1.play(77);
				BF_GLOBALS._player.disableControl();

				scene->_sceneMode = 8109;
				scene->setAction(&scene->_sequenceManager1, scene, 8109, &BF_GLOBALS._player,
					&scene->_object6, &scene->_object5, NULL);
				scene->_fieldA70 = 1;
				scene->_fieldA74 = 1;
				remove();
			} else {
				SceneItem::display2(810, 39);
			}
		}

		if (scene->_rect1.contains(event.mousePos) || scene->_rect2.contains(event.mousePos)) {
			if (BF_INVENTORY.getObjectScene(INV_PRINT_OUT) == 811) {
				scene->_sound1.play(77);
				BF_GLOBALS._player.disableControl();

				scene->_sceneMode = 8109;
				scene->setAction(&scene->_sequenceManager1, scene, 8109, &BF_GLOBALS._player,
					&scene->_object6, &scene->_object5, NULL);
				scene->_fieldA74 = 1;
				remove();
			} else {
				SceneItem::display2(810, 39);
			}
		}
		return true;
	default:
		return NamedObject::startAction(action, event);
	}
}

bool Scene810::Object5::startAction(CursorType action, Event &event) {
	Scene810 *scene = (Scene810 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(810, 33);
		return true;
	case CURSOR_USE: {
		scene->_sceneMode = 8195;
		BF_GLOBALS._player.disableControl();
		
		PlayerMover *mover = new PlayerMover();
		Common::Point destPos(67, 111);
		BF_GLOBALS._player.addMover(mover, &destPos, scene);
		return true;
	}
	default:
		return NamedObject::startAction(action, event);
	}
}

bool Scene810::Object7::startAction(CursorType action, Event &event) {
	Scene810 *scene = (Scene810 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_LOOK:
	case CURSOR_USE:
		BF_GLOBALS._player.disableControl();
		scene->_sceneMode = 8113;
		scene->setAction(&scene->_sequenceManager1, scene, 8113, &BF_GLOBALS._player, NULL);
		return true;
	default:
		return NamedObject::startAction(action, event);
	}
}

/*--------------------------------------------------------------------------*/

bool Scene810::Map::startAction(CursorType action, Event &event) {
	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(810, 0);
		return true;
	case CURSOR_USE:
		SceneItem::display2(810, 1);
		return true;
	default:
		return SceneHotspot::startAction(action, event);
	}
}

bool Scene810::Window::startAction(CursorType action, Event &event) {
	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(810, 2);
		return true;
	case CURSOR_USE:
		SceneItem::display2(810, 3);
		return true;
	default:
		return SceneHotspot::startAction(action, event);
	}
}

bool Scene810::Bookcase::startAction(CursorType action, Event &event) {
	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(810, 4);
		return true;
	case CURSOR_USE:
		SceneItem::display2(810, 5);
		return true;
	default:
		return SceneHotspot::startAction(action, event);
	}
}

bool Scene810::FaxMachine::startAction(CursorType action, Event &event) {
	Scene810 *scene = (Scene810 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(810, 6);
		return true;
	case CURSOR_USE:
		if (scene->_fieldA74 == 1) {
			scene->_object5.startAction(action, event);
		} else {
			BF_GLOBALS._player.disableControl();
			scene->_sceneMode = 8105;
			ADD_PLAYER_MOVER(67, 111);
		}
		return true;
	case INV_PRINT_OUT:
		BF_INVENTORY.setObjectScene(INV_PRINT_OUT, 811);
		BF_GLOBALS._player.disableControl();
		scene->_sceneMode = 811;

		if (BF_GLOBALS._sceneObjects->contains(&scene->_lyle)) {
			scene->setAction(&scene->_sequenceManager1, scene, BF_GLOBALS.getFlag(onDuty) ? 8108 : 8105,
				&scene->_object6, NULL);
		} else {
			scene->setAction(&scene->_sequenceManager1, scene, 8111, &BF_GLOBALS._player, &scene->_object6, NULL);
		}
		return true;
	default:
		return SceneHotspot::startAction(action, event);
	}
}

bool Scene810::GarbageCan::startAction(CursorType action, Event &event) {
	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(810, 8);
		return true;
	case CURSOR_USE:
		SceneItem::display2(810, 9);
		return true;
	default:
		return SceneHotspot::startAction(action, event);
	}
}

bool Scene810::FileCabinets::startAction(CursorType action, Event &event) {
	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(810, 10);
		return true;
	case CURSOR_USE:
		SceneItem::display2(810, 11);
		return true;
	default:
		return SceneHotspot::startAction(action, event);
	}
}

bool Scene810::CoffeeMaker::startAction(CursorType action, Event &event) {
	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(810, 12);
		return true;
	case CURSOR_USE:
		SceneItem::display2(810, 13);
		return true;
	default:
		return SceneHotspot::startAction(action, event);
	}
}

bool Scene810::Shelves::startAction(CursorType action, Event &event) {
	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(810, 14);
		return true;
	case CURSOR_USE:
		SceneItem::display2(810, 15);
		return true;
	default:
		return SceneHotspot::startAction(action, event);
	}
}

bool Scene810::MicroficheReader::startAction(CursorType action, Event &event) {
	Scene810 *scene = (Scene810 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(810, 16);
		return true;
	case CURSOR_USE:
		SceneItem::display2(810, 17);
		return true;
	case INV_MICROFILM:
		BF_GLOBALS._player.disableControl();
		scene->_sceneMode = 8106;
		scene->setAction(&scene->_sequenceManager1, scene, 8106, &BF_GLOBALS._player, NULL);
		return true;
	default:
		return SceneHotspot::startAction(action, event);
	}
}

bool Scene810::Item10::startAction(CursorType action, Event &event) {
	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(810, 18);
		return true;
	case CURSOR_USE:
		SceneItem::display2(810, 19);
		return true;
	default:
		return SceneHotspot::startAction(action, event);
	}
}

bool Scene810::Pictures::startAction(CursorType action, Event &event) {
	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(810, 20);
		return true;
	case CURSOR_USE:
		SceneItem::display2(810, 21);
		return true;
	default:
		return SceneHotspot::startAction(action, event);
	}
}

bool Scene810::Item12::startAction(CursorType action, Event &event) {
	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(810, 22);
		return true;
	case CURSOR_USE:
		SceneItem::display2(810, 23);
		return true;
	default:
		return SceneHotspot::startAction(action, event);
	}
}

bool Scene810::Background::startAction(CursorType action, Event &event) {
	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(810, 24);
		return true;
	default:
		return SceneHotspot::startAction(action, event);
	}
}

bool Scene810::Desk::startAction(CursorType action, Event &event) {
	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(810, 36);
		return true;
	case CURSOR_USE:
		SceneItem::display2(810, 37);
		return true;
	default:
		return SceneHotspot::startAction(action, event);
	}
}

bool Scene810::Exit::startAction(CursorType action, Event &event) {
	ADD_PLAYER_MOVER(event.mousePos.x, event.mousePos.y);
	return true;
}

/*--------------------------------------------------------------------------*/

Scene810::Scene810(): SceneExt() {
	_fieldA70 = _fieldA74 = 0;
	_rect1 = Rect(68, 12, 120, 22);
	_rect2 = Rect(59, 27, 117, 37);
	_rect3 = Rect(49, 43, 112, 54);
}

void Scene810::synchronize(Serializer &s) {
	SceneExt::synchronize(s);
	s.syncAsSint16LE(_fieldA70);
	s.syncAsSint16LE(_fieldA72);
	s.syncAsSint16LE(_fieldA74);
}

void Scene810::postInit(SceneObjectList *OwnerList) {
	SceneExt::postInit();
	loadScene(810);
	setZoomPercents(90, 80, 135, 100);
	if (BF_GLOBALS._sceneManager._previousScene != 820)
		BF_GLOBALS._sound1.fadeSound(76);

	_stripManager.addSpeaker(&_gameTextSpeaker);
	_stripManager.addSpeaker(&_jakeJacketSpeaker);
	_stripManager.addSpeaker(&_jakeUniformSpeaker);
	_stripManager.addSpeaker(&_lyleHatSpeaker);

	BF_GLOBALS._player.postInit();
	BF_GLOBALS._player.setVisage(BF_GLOBALS.getFlag(onDuty) ? 361 : 368);
	BF_GLOBALS._player.animate(ANIM_MODE_1, NULL);
	BF_GLOBALS._player.setObjectWrapper(new SceneObjectWrapper());
	BF_GLOBALS._player.changeZoom(-1);
	BF_GLOBALS._player._moveDiff = Common::Point(5, 3);
	BF_GLOBALS._player.enableControl();

	if (BF_GLOBALS._dayNumber == 2)
		BF_GLOBALS.setFlag(beenToJRDay2);

	if (BF_GLOBALS._dayNumber == 3) {
		_object3.postInit();
		_object3.setVisage(810);
		_object3.setStrip(3);
		_object3.setPosition(Common::Point(154, 97));
		_object3.fixPriority(128);
		BF_GLOBALS._sceneItems.push_back(&_object3);
	}

	if (BF_GLOBALS._dayNumber == 4) {
		BF_INVENTORY.setObjectScene(INV_FOREST_RAP, 0);
		BF_INVENTORY.setObjectScene(INV_COBB_RAP, 0);
		BF_INVENTORY.setObjectScene(INV_PRINT_OUT, 0);
		BF_INVENTORY.setObjectScene(INV_CRATE1, 0);
	}

	_desk._sceneRegionId = 12;
	BF_GLOBALS._sceneItems.push_back(&_desk);

	_lyle.postInit();
	_lyle.setVisage(812);
	_lyle.setPosition(Common::Point(115, 112));
	_lyle._moveDiff = Common::Point(4, 2);
	_lyle.changeZoom(-1);
	_lyle._flag = 0;
	BF_GLOBALS._sceneItems.push_back(&_lyle);

	_chair.postInit();
	_chair.setVisage(810);
	_chair.setStrip(2);
	_chair.setPosition(Common::Point(113, 126));
	_chair.hide();
	BF_GLOBALS._sceneItems.push_back(&_chair);

	_object6.postInit();
	_object6.setVisage(810);
	_object6.setStrip(6);
	_object6.setPosition(Common::Point(51, 65));
	_object6._numFrames = 3;
	_object6.hide();

	_object5.postInit();
	_object5.setVisage(810);
	_object5.setStrip(5);
	_object5.setPosition(Common::Point(58, 82));
	_object5._numFrames = 3;
	_object5.fixPriority(108);
	_object5.hide();

	if (BF_INVENTORY.getObjectScene(INV_PRINT_OUT) == 811) {
		_object5.show();
		BF_GLOBALS._sceneItems.push_back(&_object5);
	}

	if ((BF_GLOBALS._dayNumber == 4) && (BF_GLOBALS._bookmark < bEndDayThree)) {
		_lyle.remove();
		_chair.show();
	}

	switch (BF_GLOBALS._sceneManager._previousScene) {
	case 820:
		BF_GLOBALS._player.setStrip(7);
		BF_GLOBALS._player.setPosition(Common::Point(278, 116));
		
		_lyle.setVisage(845);
		_lyle.setPosition(Common::Point(340, 175));
		_lyle.setObjectWrapper(new SceneObjectWrapper());
		_lyle.animate(ANIM_MODE_1, NULL);
		
		_chair.show();

		BF_GLOBALS._player.disableControl();
		_sceneMode = 8107;
		setAction(&_sequenceManager1, this, 8107, &BF_GLOBALS._player, &_lyle, NULL);
		break;
	case 935:
		BF_GLOBALS._v51C44 = 1;
		BF_GLOBALS._scenePalette.loadPalette(2);
		_lyle.remove();

		BF_GLOBALS._player.setPosition(Common::Point(174, 142));
		BF_GLOBALS._player.setStrip(8);
		BF_GLOBALS._player.enableControl();

		_chair.remove();
		break;
	default:
		BF_GLOBALS._player.setPosition(Common::Point(340, 180));
		BF_GLOBALS._player.disableControl();
		_sceneMode = 8100;

		if (BF_GLOBALS.getFlag(fWithLyle)) {
			_lyle.setVisage(845);
			_lyle.setPosition(Common::Point(340, 175));
			_lyle.setObjectWrapper(new SceneObjectWrapper());
			_lyle.animate(ANIM_MODE_1, NULL);

			_chair.show();
			_sceneMode = 8196;
			setAction(&_sequenceManager1, NULL, 8100, &BF_GLOBALS._player, NULL);
			_lyle.setAction(&_sequenceManager2, this, 8107, &BF_GLOBALS._player, &_lyle, NULL);
		} else {
			setAction(&_sequenceManager1, this, 8100, &BF_GLOBALS._player, NULL);
		}
		break;
	}

	_exit.setDetails(Rect(315, 117, 320, 154), 810, -1, -1, -1, 1, NULL);
	_map.setBounds(Rect(10, 10, 81, 52));
	_window.setBounds(Rect(96, 10, 155, 49));
	_bookcase.setBounds(Rect(5, 70, 74, 105));
	_garbageCan.setBounds(Rect(84, 118, 101, 142));
	_fileCabinets.setBounds(Rect(203, 41, 255, 100));
	_coffeeMaker.setBounds(Rect(182, 54, 202, 89));
	_shelves.setBounds(Rect(265, 10, 319, 41));
	_microficheReader.setBounds(Rect(283, 47, 314, 73));

	_faxMachine._sceneRegionId = 8;
	BF_GLOBALS._sceneItems.push_back(&_faxMachine);
	_item10._sceneRegionId = 9;
	BF_GLOBALS._sceneItems.push_back(&_item10);
	_pictures._sceneRegionId = 10;
	BF_GLOBALS._sceneItems.push_back(&_pictures);
	_item12._sceneRegionId = 8;
	BF_GLOBALS._sceneItems.push_back(&_item12);

	BF_GLOBALS._sceneItems.addItems(&_microficheReader, &_map, &_window, &_bookcase, &_garbageCan, 
		&_fileCabinets, &_coffeeMaker, &_shelves, &_background, NULL);
	_background.setBounds(Rect(0, 0, SCREEN_WIDTH, BF_INTERFACE_Y));
}

void Scene810::signal() {
	switch (_sceneMode) {
	case 811:
	case 8105:
		_object4.postInit();
		_object4.setVisage(810);
		_object4.setPosition(Common::Point(77, 94));
		_object4.setStrip(8);
		_object4.fixPriority(250);
		BF_GLOBALS._sceneItems.push_back(&_object4);
		break;
	case 8100:
		if (BF_GLOBALS.getFlag(examinedFile810)) {
			if ((BF_GLOBALS._dayNumber == 4) && BF_GLOBALS._sceneObjects->contains(&_lyle)) {
				_sceneMode = 8115;
				setAction(&_sequenceManager1, this, 8115, &BF_GLOBALS._player, NULL);
			} else {
				BF_GLOBALS._player.enableControl();
			}
		} else {
			if ((BF_GLOBALS._dayNumber == 3) && BF_GLOBALS._sceneObjects->contains(&_lyle)) {
				_sceneMode = 8103;
				setAction(&_sequenceManager1, this, 8103, &BF_GLOBALS._player, &_lyle, &_chair, NULL);
			} else if (BF_GLOBALS.getFlag(shownLyleCrate1Day1) && !BF_GLOBALS.getFlag(shownLyleCrate1)) {
				BF_GLOBALS.setFlag(shownLyleCrate1);
				setAction(&_action2);
			} else {
				BF_GLOBALS._player.enableControl();
			}
		}
		break;
	case 8101:
		BF_GLOBALS._sound1.fadeOut2(NULL);
		BF_GLOBALS._sceneManager.changeScene(800);
		break;
	case 8103:
		_lyle.remove();
		BF_GLOBALS._player.enableControl();
		break;
	case 8104:
		BF_GLOBALS.setFlag(examinedFile810);
		_object7.postInit();
		_object7.setVisage(810);
		_object7.setPosition(Common::Point(54, 101));
		_object7.fixPriority(200);
		BF_GLOBALS._sceneItems.push_front(&_object7);
		BF_GLOBALS._player.enableControl();
		break;
	case 8106:
		BF_GLOBALS._uiElements.addScore(30);
		BF_INVENTORY.setObjectScene(INV_MICROFILM, 820);
		BF_GLOBALS._sceneManager.changeScene(820);
		break;
	case 8107:
		if (BF_GLOBALS.getFlag(shownFax)) {
			BF_GLOBALS.setFlag(showMugAround);
		} else {
			BF_GLOBALS._walkRegions.proc1(4);
			BF_GLOBALS._player.enableControl();
		}
		break;
	case 8109:
		_object6.setFrame(1);
		BF_GLOBALS._sceneItems.push_front(&_object5);
		BF_GLOBALS._player.enableControl();
		break;
	case 8110:
	case 8115:
		BF_GLOBALS._player.enableControl();
		break;
	case 8112:
		BF_GLOBALS.setFlag(fWithLyle);
		BF_GLOBALS._sceneManager.changeScene(800);
		break;
	case 8113:
		BF_GLOBALS._sound1.fadeOut2(NULL);
		BF_GLOBALS._sceneManager.changeScene(935);
		break;
	case 8114:
		BF_GLOBALS._uiElements.addScore(10);
		BF_INVENTORY.setObjectScene(INV_MICROFILM, 1);
		BF_GLOBALS._player.enableControl();
		break;
	case 8116:
		BF_GLOBALS._bookmark = bDoneWithIsland;
		BF_GLOBALS._player.enableControl();
		break;
	case 8195:
		BF_GLOBALS._player.setStrip(8);
		BF_INVENTORY.setObjectScene(INV_PRINT_OUT, 1);
		if (_fieldA70 == 1) {
			BF_INVENTORY.setObjectScene(INV_COBB_RAP, 1);
			_sceneMode = 8110;
			if (BF_GLOBALS._sceneObjects->contains(&_lyle)) {
				_sceneMode = 8198;
				BF_GLOBALS.setFlag(shownFax);
				_stripManager.start(BF_GLOBALS.getFlag(onDuty) ? 8135 : 8106, this);
			} else {
				_stripManager.start(8117, this);
			}
		} else {
			SceneItem::display2(810, 34);
			BF_GLOBALS._player.enableControl();
		}

		_fieldA74 = 0;
		_object5.hide();
		_object5.setFrame(1);
		break;
	case 8196:
		BF_GLOBALS._walkRegions.proc1(4);
		BF_GLOBALS._player.enableControl();
		break;
	case 8198:
		if (BF_GLOBALS._dayNumber == 3) {
			BF_GLOBALS.setFlag(showMugAround);
		} else {
			BF_GLOBALS._player.enableControl();
		}
		break;
	default:
		break;
	}
}

void Scene810::process(Event &event) {
	if (BF_GLOBALS._player._enabled && (event.mousePos.y < (BF_INTERFACE_Y - 1))) {
		// Check if the cursor is on an exit
		if (_exit.contains(event.mousePos)) {
			GfxSurface surface = _cursorVisage.getFrame(EXITFRAME_E);
			BF_GLOBALS._events.setCursor(surface);
		} else {
			// In case an exit cursor was being shown, restore the previously selected cursor
			CursorType cursorId = BF_GLOBALS._events.getCursor();
			BF_GLOBALS._events.setCursor(cursorId);
		}
	}

	if (!event.handled)
		SceneExt::process(event);
}

void Scene810::dispatch() {
	SceneExt::dispatch();

	if (BF_GLOBALS._sceneObjects->contains(&_lyle) && (BF_GLOBALS._player._position.x != 115) && !_lyle._mover) {
		_lyle.updateAngle(BF_GLOBALS._player._position);
	}

	if (BF_GLOBALS._sceneObjects->contains(&_object4) && (BF_GLOBALS._player._position.x != 67) && 
			(BF_GLOBALS._player._position.y != 111)) {
		_object4.remove();
	}

	if (!_action) {
		if (BF_GLOBALS.getFlag(showMugAround)) {
			if (_lyle._position.y == 115) {
				BF_GLOBALS._player.disableControl();

				_sceneMode = 8110;
				setAction(&_sequenceManager1, this, 8117, &_lyle, &_chair, NULL);
			} else {
				BF_GLOBALS.clearFlag(showMugAround);
				BF_GLOBALS._player.disableControl();
				BF_GLOBALS._walkRegions.proc2(4);
				BF_GLOBALS._walkRegions.proc2(13);

				_sceneMode = 8112;
				setAction(&_sequenceManager1, this, 8112, &BF_GLOBALS._player,  &_lyle, NULL);
			}
		}

		if (BF_GLOBALS._player._position.x >= 318) {
			BF_GLOBALS._player.disableControl();

			if ((BF_GLOBALS._dayNumber == 3) && !BF_GLOBALS.getFlag(examinedFile810)) {
				SceneItem::display2(810, 35);
				_sceneMode = 8100;
				setAction(&_sequenceManager1, this, 8100, &BF_GLOBALS._player, NULL);
			} else if (BF_GLOBALS.getFlag(fWithLyle)) {
				BF_GLOBALS._walkRegions.proc2(4);
				BF_GLOBALS._walkRegions.proc2(13);

				ADD_MOVER_NULL(_lyle, 320, 155);

				_sceneMode = 8101;
				setAction(&_sequenceManager1, this, 8101, &BF_GLOBALS._player, NULL);
			}
		}
	}
}

/*--------------------------------------------------------------------------
 * Scene 820 - Microfiche Reader
 *
 *--------------------------------------------------------------------------*/

bool Scene820::PowerButton::startAction(CursorType action, Event &event) {
	Scene820 *scene = (Scene820 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(820, 5);
		return true;
	case CURSOR_USE:
		scene->_sound1.play(69);
		if (_flags & OBJFLAG_HIDING) {
			scene->_pageNumber = 0;
			show();
			BF_GLOBALS._scenePalette.loadPalette(821);
			BF_GLOBALS._scenePalette.refresh();

			SceneItem::display(820, scene->_pageNumber, SET_WIDTH, 240, SET_X, 41, SET_Y, 0, 
				SET_FONT, 50, SET_FG_COLOR, 18, SET_EXT_BGCOLOR, 12, SET_KEEP_ONSCREEN, true, LIST_END);
		} else {
			BF_GLOBALS._scenePalette.loadPalette(820);
			BF_GLOBALS._scenePalette.refresh();

			scene->_object4.remove();
			scene->_object5.remove();

			SceneItem::display(0, 0);
			hide();

			BF_GLOBALS._sceneManager.changeScene(810);
		}
		return true;
	default:
		return NamedObject::startAction(action, event);
	}
}

bool Scene820::BackButton::startAction(CursorType action, Event &event) {
	Scene820 *scene = (Scene820 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(820, 7);
		return true;
	case CURSOR_USE:
		// WORKAROUND: The original game had a bug where you could see the microfiche text by directly
		// using the paging buttons, but then you had to use the power button twice to 'turn on' the
		// reader and then off again. This check prevents the paging buttons being used until the
		// reader is properly turned on.
		if (scene->_powerButton._flags & OBJFLAG_HIDING)
			return true;

		scene->_sound1.play(72);
		show();
		scene->_sceneMode = 8200;
		scene->setAction(&scene->_sequenceManager, scene, 8200, NULL);

		if (scene->_pageNumber)
			--scene->_pageNumber;
		if (scene->_pageNumber == 3) {
			scene->_object4.hide();
			scene->_object5.hide();
		}

		SceneItem::display(820, scene->_pageNumber, SET_WIDTH, 240, SET_X, 41, SET_Y, 0, 
			SET_FONT, 50, SET_FG_COLOR, 18, SET_EXT_BGCOLOR, 12, SET_KEEP_ONSCREEN, true, LIST_END);
		return true;
	default:
		return NamedObject::startAction(action, event);
	}
}

bool Scene820::ForwardButton::startAction(CursorType action, Event &event) {
	Scene820 *scene = (Scene820 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_LOOK:
		SceneItem::display2(820, 6);
		return true;
	case CURSOR_USE:
		// WORKAROUND: The original game had a bug where you could see the microfiche text by directly
		// using the paging buttons, but then you had to use the power button twice to 'turn on' the
		// reader and then off again. This check prevents the paging buttons being used until the
		// reader is properly turned on.
		if (scene->_powerButton._flags & OBJFLAG_HIDING)
			return true;

		scene->_sound1.play(72);
		show();
		scene->_sceneMode = 8200;
		scene->setAction(&scene->_sequenceManager, scene, 8200, NULL);

		if (scene->_pageNumber < 4)
			++scene->_pageNumber;

		SceneItem::display(820, scene->_pageNumber, SET_WIDTH, 240, SET_X, 41, SET_Y, 0, 
			SET_FONT, 50, SET_FG_COLOR, 18, SET_EXT_BGCOLOR, 12, SET_KEEP_ONSCREEN, true, LIST_END);

		if (scene->_pageNumber == 4) {
			scene->_object4.show();
			scene->_object5.show();
		}

		return true;
	default:
		return NamedObject::startAction(action, event);
	}
}

/*--------------------------------------------------------------------------*/

Scene820::Scene820(): SceneExt() {
	_pageNumber = 0;
}

void Scene820::synchronize(Serializer &s) {
	SceneExt::synchronize(s);
	s.syncAsSint16LE(_pageNumber);
}

void Scene820::postInit(SceneObjectList *OwnerList) {
	SceneExt::postInit();
	loadScene(820);
	
	_stripManager.addSpeaker(&_gameTextSpeaker);

	_powerButton.postInit();
	_powerButton.setVisage(820);
	_powerButton.setPosition(Common::Point(42, 163));
	_powerButton.hide();
	BF_GLOBALS._sceneItems.push_back(&_powerButton);

	_backButton.postInit();
	_backButton.setVisage(820);
	_backButton.setStrip(2);
	_backButton.setPosition(Common::Point(278, 155));
	_backButton.hide();
	BF_GLOBALS._sceneItems.push_back(&_backButton);

	_forwardButton.postInit();
	_forwardButton.setVisage(820);
	_forwardButton.setStrip(3);
	_forwardButton.setPosition(Common::Point(278, 164));
	_forwardButton.hide();
	BF_GLOBALS._sceneItems.push_back(&_forwardButton);

	_object4.postInit();
	_object4.setVisage(821);
	_object4.setPosition(Common::Point(96, 130));
	_object4.hide();

	_object5.postInit();
	_object5.setVisage(821);
	_object5.setStrip(2);
	_object5.setPosition(Common::Point(223, 130));
	_object5.hide();

	BF_GLOBALS._player.enableControl();
	BF_GLOBALS._player._canWalk = false;

	_item1.setDetails(Rect(0, 0, SCREEN_WIDTH, BF_INTERFACE_Y), 820, -1, -1, -1, 1, NULL);
}

void Scene820::signal() {
	if (_sceneMode == 8200) {
		_forwardButton.hide();
		_backButton.hide();
	}
}

/*--------------------------------------------------------------------------
 * Scene 830 - Outside Boat Rentals
 *
 *--------------------------------------------------------------------------*/

bool Scene830::Lyle::startAction(CursorType action, Event &event) {
	Scene830 *scene = (Scene830 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_TALK:
		if (scene->_field18AA) {
			scene->_stripManager.start(8304, &BF_GLOBALS._stripProxy);
		} else {
			scene->_stripManager.start(8303, &BF_GLOBALS._stripProxy);
			++scene->_field18AA;
		}
		return true;
	default:
		return NamedObject::startAction(action, event);
	}
}

bool Scene830::Door::startAction(CursorType action, Event &event) {
	Scene830 *scene = (Scene830 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_LOOK:
		if (BF_GLOBALS._dayNumber == 2) {
			scene->_sceneMode = 14;
			Common::Point pt(186, 102);
			PlayerMover *mover = new PlayerMover();
			BF_GLOBALS._player.addMover(mover, &pt, NULL);
			return true;
		}
		break;
	case CURSOR_USE:
		if (BF_GLOBALS._dayNumber == 2) {
			scene->_sceneMode = 14;
			Common::Point pt(186, 102);
			PlayerMover *mover = new PlayerMover();
			BF_GLOBALS._player.addMover(mover, &pt, NULL);
		} else if (scene->_field18A4) {
			BF_GLOBALS._player.disableControl();
			scene->_sceneMode = 832;

			if (BF_GLOBALS.getFlag(onDuty)) {
				scene->setAction(&scene->_sequenceManager, scene, 8302, &BF_GLOBALS._player, &scene->_door, NULL);
			} else if (BF_GLOBALS.getFlag(fWithLyle)) {
				scene->_sceneMode = 11;
				scene->setAction(&scene->_sequenceManager, scene, 8308, &BF_GLOBALS._player, &scene->_door, &scene->_lyle, NULL);
			} else {
				scene->setAction(&scene->_sequenceManager, scene, 832, &BF_GLOBALS._player, &scene->_door, NULL);
			}
		} else {
			scene->_stripManager.start(8305, &BF_GLOBALS._stripProxy);
			ADD_PLAYER_MOVER_NULL(BF_GLOBALS._player, 192, 145);
		}
		return true;
	default:
		break;
	}

	return NamedObject::startAction(action, event);
}

bool Scene830::Object4::startAction(CursorType action, Event &event) {
	Scene830 *scene = (Scene830 *)BF_GLOBALS._sceneManager._scene;

	if (action == INV_RENTAL_KEYS) {
		BF_GLOBALS._player.disableControl();
		if (BF_GLOBALS.getFlag(fWithLyle)) {
			scene->_sceneMode = 8300;
			scene->setAction(&scene->_sequenceManager, scene, 8300, &BF_GLOBALS._player, &scene->_lyle, NULL);
		} else {
			scene->_sceneMode = 834;
			scene->setAction(&scene->_sequenceManager, scene, 8300, &BF_GLOBALS._player, &scene->_object4, NULL);
		}
		return true;
	} else {
		return NamedObject::startAction(action, event);
	}
}

bool Scene830::Object5::startAction(CursorType action, Event &event) {
	Scene830 *scene = (Scene830 *)BF_GLOBALS._sceneManager._scene;

	if (action == CURSOR_USE) {
		BF_GLOBALS._player.disableControl();
		scene->_sceneMode = 8307;
		scene->setAction(&scene->_sequenceManager, scene, 8307, &BF_GLOBALS._player, &scene->_object5, NULL);
		return true;
	} else {
		return NamedObject::startAction(action, event);
	}
}

/*--------------------------------------------------------------------------*/

bool Scene830::SouthEastExit::startAction(CursorType action, Event &event) {
	ADD_PLAYER_MOVER(BF_GLOBALS._player._position.x + 80, BF_GLOBALS._player._position.y + 100);
	return true;
}

bool Scene830::NoteBoard::startAction(CursorType action, Event &event) {
	Scene830 *scene = (Scene830 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_LOOK:
		if (BF_GLOBALS._sceneObjects->contains(&scene->_object6)) {
			BF_GLOBALS._player.disableControl();
			scene->_sceneMode = 8309;
			scene->setAction(&scene->_sequenceManager, scene, 8309, &BF_GLOBALS._player, NULL);
			return true;
		}
		break;
	case CURSOR_USE:
		if (BF_GLOBALS._sceneObjects->contains(&scene->_object6)) {
			BF_GLOBALS._player.disableControl();
			scene->_sceneMode = 8305;
			scene->setAction(&scene->_sequenceManager, scene, 8305, &BF_GLOBALS._player, &scene->_object6, NULL);
			return true;
		}
		break;
	default:
		break;
	}

	return NamedHotspot::startAction(action, event);
}

/*--------------------------------------------------------------------------*/

Scene830::Scene830(): PalettedScene() {
	_field18A4 = _field18A6 = _field18A8 = _field18AA = _field18AC = 0;
}

void Scene830::synchronize(Serializer &s) {
	PalettedScene::synchronize(s);
	s.syncAsByte(_field18A4);
	s.syncAsByte(_field18A6);
	s.syncAsByte(_field18A8);
	s.syncAsByte(_field18AA);
	s.syncAsByte(_field18AC);
}

void Scene830::postInit(SceneObjectList *OwnerList) {
	PalettedScene::postInit();
	loadScene(830);
	_sound1.fadeSound(35);

	_stripManager.addSpeaker(&_gameTextSpeaker);
	_stripManager.addSpeaker(&_jakeJacketSpeaker);
	_stripManager.addSpeaker(&_lyleHatSpeaker);

	if (BF_GLOBALS._dayNumber == 0) {
		BF_GLOBALS._sceneManager._previousScene = 340;
		BF_GLOBALS._dayNumber = 1;
		BF_GLOBALS.setFlag(fWithLyle);
	}

	_field18A8 = (BF_GLOBALS._dayNumber <= 3) ? 0 : 1;
	_field18AC = 0;
	BF_GLOBALS.clearFlag(fCanDrawGun);

	_seExit.setDetails(Rect(183, 161, 320, 167), 830, -1, -1, -1, 1, NULL);

	PaletteRotation *rot = BF_GLOBALS._scenePalette.addRotation(184, 189, 1);
	rot->setDelay(30);

	BF_GLOBALS._player.postInit();
	BF_GLOBALS._player.disableControl();

	if (_field18A8) {
		_object4.postInit();
		_object4.setVisage(830);
		_object4.setStrip(1);
		_object4.setPosition(Common::Point(271, 146));
		_object4.fixPriority(90);
		_object4.setDetails(830, 0, 1, 2, 1, NULL);
	}

	_door.postInit();
	_door.setVisage(830);
	_door.setDetails(830, 3, 4, -1, 1, NULL);
	_door.setStrip((BF_GLOBALS._dayNumber == 2) ? 4 : 3);
	_door.setPosition(Common::Point(182, 97));

	_field18A4 = 1;
	_noteBoard.setDetails(1, 830, 30, 31, 32, 1);

	switch (BF_GLOBALS._dayNumber) {
	case 1:
		if ((BF_GLOBALS._bookmark >= bEndOfWorkDayOne) && (BF_INVENTORY.getObjectScene(INV_CARTER_NOTE) != 1) &&
				(BF_INVENTORY.getObjectScene(INV_CARTER_NOTE) != 3)) {
			_object6.postInit();
			_object6.setVisage(830);
			_object6.setStrip(4);
			_object6.setFrame(2);
			_object6.setPosition(Common::Point(302, 124));
		}
		break;
	case 4:
		if (BF_INVENTORY.getObjectScene(INV_FISHING_NET) == 830) {
			_object5.postInit();
			_object5.setVisage(830);
			_object5.setStrip(2);
			_object5.setFrame(2);
			_object5.setPosition(Common::Point(126, 133));
			_object5.setDetails(830, 7, 8, -1, 1, NULL);
		}
		break;
	case 5:
		if ((BF_GLOBALS._sceneManager._previousScene == 840) && (BF_INVENTORY.getObjectScene(INV_RENTAL_KEYS) == 1) &&
					!BF_GLOBALS.getFlag(fWithLyle)) {
			_field18A4 = 0;

			_lyle.postInit();
			_lyle._flags = OBJFLAG_CHECK_REGION;
			_lyle.setVisage(835);
			_lyle.setObjectWrapper(new SceneObjectWrapper());
			_lyle.animate(ANIM_MODE_1, NULL);
			_lyle.setStrip(4);
			_lyle.setPosition(Common::Point(180, 154));
			_lyle._moveDiff = Common::Point(2, 0);
			_lyle.setDetails(830, 28, -1, 29, 1, NULL);

			_field18AC = 1;
		}
		break;
	}

	_item2.setDetails(7, 830, 9, 10, 11, 1);
	_item4.setDetails(2, 830, 12, 13, 14, 1);
	_item5.setDetails(3, 830, 15, 16, 17, 1);
	_item6.setDetails(4, 830, 18, 19, 20, 1);
	_item7.setDetails(5, 830, 21, 22, 23, 1);
	_item8.setDetails(6, 830, 24, 25, 26, 1);

	switch (BF_GLOBALS._sceneManager._previousScene) {
	case 840:
		_sceneMode = 833;
		if (BF_GLOBALS.getFlag(onDuty)) {
			setAction(&_sequenceManager, this, 8304, &BF_GLOBALS._player, &_door, NULL);
		} else if (BF_GLOBALS.getFlag(fWithLyle)) {
			_lyle.postInit();
			_lyle._moveDiff = Common::Point(2, 0);
			_sceneMode = 837;
			setAction(&_sequenceManager, this, 837, &_lyle, &_door, &BF_GLOBALS._player, NULL);
		} else {
			setAction(&_sequenceManager, this, 833, &BF_GLOBALS._player, &_door, NULL);
		}
		break;
	case 850:
		BF_GLOBALS._player.setVisage(831);
		BF_GLOBALS._player.setObjectWrapper(new SceneObjectWrapper());
		BF_GLOBALS._player.animate(ANIM_MODE_1, NULL);
		BF_GLOBALS._player.setStrip(3);
		BF_GLOBALS._player._moveDiff = Common::Point(2, 0);
		BF_GLOBALS._player.setPosition(Common::Point(220, 122));
		BF_GLOBALS._player.enableControl();

		if (BF_GLOBALS.getFlag(fWithLyle)) {
			_lyle.postInit();
			_lyle.setVisage(835);
			_lyle.setObjectWrapper(new SceneObjectWrapper());
			_lyle.animate(ANIM_MODE_1, NULL);
			_lyle.setStrip(3);
			_lyle._moveDiff = Common::Point(2, 0);
			_lyle.setPosition(Common::Point(224, 126));
		}

		if ((BF_GLOBALS._bookmark == bEndDayThree) && (BF_INVENTORY.getObjectScene(INV_FISHING_NET) == 1)) {
			BF_INVENTORY.setObjectScene(INV_FISHING_NET, 0);
			BF_INVENTORY.setObjectScene(INV_HOOK, 0);
		}
		break;
	case 340:
	case 830:
	default:
		_sceneMode = 830;
		setAction(&_sequenceManager, this, BF_GLOBALS.getFlag(onDuty) ? 8303 : 830, &BF_GLOBALS._player, NULL);

		if (BF_GLOBALS.getFlag(fWithLyle)) {
			_lyle.postInit();
			_lyle.setVisage(835);
			_lyle.setObjectWrapper(new SceneObjectWrapper());
			_lyle.animate(ANIM_MODE_1, NULL);
			_lyle.setPosition(Common::Point(SCREEN_WIDTH, SCREEN_HEIGHT));
			_lyle._moveDiff = Common::Point(2, 0);
			ADD_MOVER(_lyle, 280, 160);
		}
		break;
	}

	_field18A6 = 1;
}

void Scene830::remove() {
	BF_GLOBALS._scenePalette.clearListeners();
	PalettedScene::remove();
}

void Scene830::signal() {
	static uint32 black = 0;

	switch (_sceneMode) {
	case 11:
		_sceneMode = 832;
		BF_GLOBALS._scenePalette.clearListeners();
		addFader((const byte *)&black, 5, this);
		BF_GLOBALS._v51C44 = 0;
		break;
	case 12:
		_sceneMode = 831;
		BF_GLOBALS._scenePalette.clearListeners();
		addFader((const byte *)&black, 5, this);
		BF_GLOBALS._v51C44 = 0;
		break;
	case 13:
		BF_GLOBALS._sceneManager.changeScene(850);
		break;
	case 14:
		BF_GLOBALS._player.disableControl();
		_sceneMode = 8312;

		_object1.postInit();
		setAction(&_sequenceManager, this, 8312, &_object1, NULL);
		break;
	case 830:
		_field18A6 = 1;
		BF_GLOBALS._player.enableControl();
		break;
	case 831:
	case 8306:
		BF_GLOBALS._sceneManager.changeScene((BF_GLOBALS._bookmark < bStartOfGame) ||
			(BF_GLOBALS._bookmark >= bCalledToDomesticViolence) ? 342 : 340);
		break;
	case 832:
		BF_GLOBALS._sceneManager.changeScene(840);
		break;
	case 833:
		if (_field18AC) {
			BF_GLOBALS.setFlag(fWithLyle);
			_sceneMode = 8311;
			setAction(&_sequenceManager, this, 8311, &BF_GLOBALS._player, &_lyle, NULL);
		} else {
			BF_GLOBALS._player.enableControl();
			BF_GLOBALS._player.setObjectWrapper(new SceneObjectWrapper());
			BF_GLOBALS._player.animate(ANIM_MODE_1, NULL);
			BF_GLOBALS._player._strip = 3;
			BF_GLOBALS._player._frame = 1;
		}
		break;
	case 834:
		BF_GLOBALS._sceneManager.changeScene(850);
		break;
	case 837:
		BF_GLOBALS._player.enableControl();
		break;
	case 8300:
		_sceneMode = 13;
		BF_GLOBALS._scenePalette.clearListeners();
		addFader((const byte *)&black, 5, this);
		break;
	case 8305:
		_object6.remove();
		BF_GLOBALS._player.enableControl();
		BF_GLOBALS._player.setObjectWrapper(new SceneObjectWrapper());
		BF_GLOBALS._player.animate(ANIM_MODE_1, NULL);
		BF_GLOBALS._player._strip = 7;

		BF_GLOBALS._uiElements.addScore(30);
		BF_INVENTORY.setObjectScene(INV_CARTER_NOTE, 1);
		break;
	case 8307:
		BF_GLOBALS._player.enableControl();
		_object5.remove();
		BF_GLOBALS._uiElements.addScore(30);
		BF_INVENTORY.setObjectScene(INV_FISHING_NET, 1);
		break;
	case 8309:
	case 8311:
		BF_GLOBALS._player.enableControl();
		break;
	case 8312:
		_object1.remove();
		BF_GLOBALS._player.enableControl();
		break;
	}
}

void Scene830::process(Event &event) {
	if (BF_GLOBALS._player._enabled && (event.eventType == EVENT_BUTTON_DOWN) &&
			(BF_GLOBALS._events.getCursor() == CURSOR_WALK) && !_field18A4) {
		_stripManager.start(8306, &BF_GLOBALS._stripProxy);
		event.handled = true;
	}

	if (BF_GLOBALS._player._enabled && (event.mousePos.y < (BF_INTERFACE_Y - 1))) {
		// Check if the cursor is on an exit
		if (_seExit.contains(event.mousePos)) {
			GfxSurface surface = _cursorVisage.getFrame(EXITFRAME_SE);
			BF_GLOBALS._events.setCursor(surface);
		} else {
			// In case an exit cursor was being shown, restore the previously selected cursor
			CursorType cursorId = BF_GLOBALS._events.getCursor();
			BF_GLOBALS._events.setCursor(cursorId);
		}
	}
}

void Scene830::dispatch() {
	SceneExt::dispatch();

	if (!_action && _field18A6 && (BF_GLOBALS._player._position.y > 160)) {
		if (!_field18A4) {
			_stripManager.start(8305, &BF_GLOBALS._player);
			ADD_PLAYER_MOVER_NULL(BF_GLOBALS._player, 192, 145);
		} else if ((BF_INVENTORY.getObjectScene(INV_RENTAL_KEYS) == 1) && (BF_GLOBALS._bookmark == bEndDayThree)) {
			BF_GLOBALS._player.disableControl();
			SceneItem::display2(830, 27);
			_field18A6 = 0;
			_sceneMode = 830;
			ADD_PLAYER_MOVER(BF_GLOBALS._player._position.x - 10, BF_GLOBALS._player._position.y - 10);
		} else {
			BF_GLOBALS._player.disableControl();
			_field18A6 = 0;
			_sceneMode = 831;
			ADD_PLAYER_MOVER(BF_GLOBALS._player._position.x + 40, BF_GLOBALS._player._position.y + 40);

			if (BF_GLOBALS.getFlag(fWithLyle)) {
				ADD_MOVER_NULL(_lyle, SCREEN_WIDTH, SCREEN_HEIGHT);
				if (_lyle._position.y < 160)
					_sceneMode = 12;
			}
		}
	}
}

/*--------------------------------------------------------------------------
 * Scene 840 - Boat Rentals
 *
 *--------------------------------------------------------------------------*/

void Scene840::Object2::postInit(SceneObjectList *OwnerList) {
	FocusObject::postInit(OwnerList);

	if (BF_INVENTORY.getObjectScene(INV_RENTAL_KEYS) != 1) {
		_rentalKeys.postInit();
		_rentalKeys.setVisage(840);
		_rentalKeys.setStrip(2);
		_rentalKeys.setFrame(3);
		_rentalKeys.setPosition(Common::Point(120, 132));
		_rentalKeys.fixPriority(255);
		_rentalKeys.setDetails(840, 52, 8, -1, 2, NULL);
	}

	if (BF_INVENTORY.getObjectScene(INV_WAVE_KEYS) != 1) {
		_waveKeys.postInit();
		_waveKeys.setVisage(840);
		_waveKeys.setStrip(2);
		_waveKeys.setFrame(2);
		_waveKeys.setPosition(Common::Point(201, 91));
		_waveKeys.fixPriority(255);
		_waveKeys.setDetails(840, 53, 8, -1, 2, NULL);
	}

	_v1B4 = _v1B6 = 0;
}

void Scene840::Object2::remove() {
	Scene840 *scene = (Scene840 *)BF_GLOBALS._sceneManager._scene;

	_rentalKeys.remove();
	_waveKeys.remove();
	BF_GLOBALS._player.disableControl();

	scene->_sceneMode = 8412;
	scene->setAction(&scene->_sequenceManager1, scene, 8412, &BF_GLOBALS._player, NULL);
}

void Scene840::Object2::process(Event &event) {
	if (BF_GLOBALS._player._enabled) {
		if (_bounds.contains(event.mousePos)) {
			CursorType cursorId = BF_GLOBALS._events.getCursor();
			BF_GLOBALS._events.setCursor(cursorId);

			if ((event.eventType == EVENT_BUTTON_DOWN) && (cursorId == CURSOR_WALK) && (event.btnState == 3)) {
				BF_GLOBALS._events.setCursor(CURSOR_USE);
				event.handled = true;
			}
		} else if (event.mousePos.y < 168) {
			BF_GLOBALS._events.setCursor(_img);
			if (event.eventType == EVENT_BUTTON_DOWN) {
				event.handled = true;
				remove();
			}
		}
	}

	FocusObject::process(event);
}

bool Scene840::Object2::startAction(CursorType action, Event &event) {
	switch (action) {
	case CURSOR_LOOK:
		if ((event.mousePos.y > 43) && (event.mousePos.y < 92)) {
			if (event.mousePos.x < 135) {
				SceneItem::display2(840, 54);
				return true;
			} else if (event.mousePos.x < 183) {
				SceneItem::display2(840, 57);
				return true;
			}
		} else if ((event.mousePos.y > 91) && (event.mousePos.y < 132)) {
			if (event.mousePos.x > 182) {
				SceneItem::display2(840, 58);
				return true;
			} else if (event.mousePos.x > 134) {
				SceneItem::display2(840, 57);
				return true;
			}
		}
		break;
	case INV_WAVE_KEYS:
		if ((BF_GLOBALS._dayNumber != 4) || (BF_GLOBALS._bookmark != bEndDayThree))
			SceneItem::display(840, 47);
		else {
			BF_GLOBALS._uiElements.addScore(50);

			if (BF_INVENTORY.getObjectScene(INV_RENTAL_KEYS) == 1) {
				// Replace rental keys
				_rentalKeys.postInit();
				_rentalKeys.setVisage(840);
				_rentalKeys.setStrip(2);
				_rentalKeys.setFrame(3);
				_rentalKeys.setPosition(Common::Point(120, 132));
				_rentalKeys.fixPriority(255);
				_rentalKeys.setDetails(840, 52, 8, -1, 2, NULL);
			}

			if (BF_INVENTORY.getObjectScene(INV_WAVE_KEYS) == 1) {
				// Replace wave keys
				_waveKeys.postInit();
				_waveKeys.setVisage(840);
				_waveKeys.setStrip(2);
				_waveKeys.setFrame(2);
				_waveKeys.setPosition(Common::Point(201, 91));
				_waveKeys.fixPriority(255);
				_waveKeys.setDetails(840, 53, 8, -1, 2, NULL);
			}

			BF_INVENTORY.setObjectScene(INV_WAVE_KEYS, 0);
			BF_INVENTORY.setObjectScene(INV_RENTAL_KEYS, 0);
		}
		return true;
	default:
		break;
	}

	return FocusObject::startAction(action, event);
}

bool Scene840::Object2::RentalKeys::startAction(CursorType action, Event &event) {
	Scene840 *scene = (Scene840 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_USE:
		if ((BF_GLOBALS._dayNumber == 4) && (BF_GLOBALS._bookmark >= bEndDayThree)) {
			SceneItem::display2(840, 9);
		} else {
			SceneItem::display2(840, 55);
			BF_INVENTORY.setObjectScene(INV_RENTAL_KEYS, 1);
			BF_GLOBALS._uiElements.addScore(30);

			scene->_object2._v1B4 = 1;
			remove();
		}
		return true;
	default:
		return NamedObject::startAction(action, event);
	}
}

bool Scene840::Object2::WaveKeys::startAction(CursorType action, Event &event) {
	Scene840 *scene = (Scene840 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_USE:
		if (scene->_field1AC2) {
			SceneItem::display(840, 56);
			BF_INVENTORY.setObjectScene(INV_WAVE_KEYS, 1);
			BF_GLOBALS._uiElements.addScore(50);
			scene->_object2._v1B6 = 1;
			remove();
		} else {
			SceneItem::display2(840, 9);
		}
		return true;
	default:
		return NamedObject::startAction(action, event);
	}
}

bool Scene840::Object6::startAction(CursorType action, Event &event) {
	Scene840 *scene = (Scene840 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_LOOK:
		scene->_object1.postInit();
		BF_GLOBALS._player.disableControl();
		scene->_sceneMode = 8410;
		scene->setAction(&scene->_sequenceManager1, scene, 8410, &scene->_object1, &BF_GLOBALS._player, NULL);
		return true;
	case CURSOR_USE:
		BF_GLOBALS._player.disableControl();
		scene->_sceneMode = 8411;
		scene->setAction(&scene->_sequenceManager1, scene, 8411, &BF_GLOBALS._player, NULL);
		return true;
	case INV_WAVE_KEYS:
	case INV_RENTAL_KEYS:
		if ((BF_GLOBALS._dayNumber == 4) && (BF_GLOBALS._bookmark == bEndDayThree)) {
			BF_GLOBALS._player.disableControl();
			scene->_sceneMode = 8417;
			scene->setAction(&scene->_sequenceManager1, scene, 8417, &BF_GLOBALS._player, NULL);
		} else {
			SceneItem::display2(840, 47);
		}
		return true;
	default:
		return NamedObject::startAction(action, event);
	}
}

bool Scene840::Carter::startAction(CursorType action, Event &event) {
	Scene840 *scene = (Scene840 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_TALK:
		BF_GLOBALS._player.disableControl();
		scene->_sceneMode = 1;

		if ((BF_GLOBALS._player._position.x != 219) || (BF_GLOBALS._player._position.y != 129)) {
			Common::Point pt(219, 129);
			PlayerMover *mover = new PlayerMover();
			BF_GLOBALS._player.addMover(mover, &pt, scene);
		} else {
			scene->signal();
		}
		return true;
	case INV_RENTAL_COUPON: {
		BF_GLOBALS._player.disableControl();
		scene->_sceneMode = 6;

		Common::Point pt(219, 129);
		PlayerMover *mover = new PlayerMover();
		BF_GLOBALS._player.addMover(mover, &pt, scene);
		return true;
	}
	case INV_NICKEL:
		++BF_GLOBALS._v4CEB6;
		BF_GLOBALS._player.disableControl();

		if (BF_INVENTORY.getObjectScene(INV_WAVE_KEYS) == 1) {
			if (!BF_GLOBALS.getFlag(fGotPointsForCombo)) {
				BF_GLOBALS._uiElements.addScore(50);
				BF_GLOBALS.setFlag(fGotPointsForCombo);
			}
		}

		scene->_sceneMode = 8407;
		scene->setAction(&scene->_sequenceManager1, scene, 8407, &BF_GLOBALS._player, NULL);
		return true;
	case INV_CARTER_NOTE:
		BF_GLOBALS._player.disableControl();
		scene->_sceneMode = 8402;

		if (!BF_GLOBALS._v4CEB6) {
			scene->setAction(&scene->_sequenceManager1, scene, 8402, &BF_GLOBALS._player, &scene->_carter, NULL);
		} else {
			scene->setAction(&scene->_sequenceManager1, scene, 8414, &BF_GLOBALS._player, &scene->_carter, NULL);
		}
		return true;
	default:
		return NamedObject::startAction(action, event);
	}
}

/*--------------------------------------------------------------------------*/

bool Scene840::Coins::startAction(CursorType action, Event &event) {
	Scene840 *scene = (Scene840 *)BF_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_LOOK:
		BF_GLOBALS._player.disableControl();
		scene->_sceneMode = 8404;
		scene->setAction(&scene->_sequenceManager1, scene, BF_GLOBALS.getFlag(onDuty) ? 8415 : 8404,
			&BF_GLOBALS._player, NULL);
		return true;
	default:
		return NamedHotspot::startAction(action, event);
	}
}

bool Scene840::Exit::startAction(CursorType action, Event &event) {
	ADD_PLAYER_MOVER_NULL(BF_GLOBALS._player, 315, 150);
	return true;
}

/*--------------------------------------------------------------------------*/

Scene840::Scene840(): PalettedScene() {
	_field1ABC = 1;
	_field1ABE = 0;
	_field1AC0 = 0;
	_field1AC2 = 0;
	_field1AC4 = 0;
	_field1AC6 = (BF_GLOBALS._dayNumber > 3) ? 1 : 0;
}

void Scene840::synchronize(Serializer &s) {
	PalettedScene::synchronize(s);

	s.syncAsSint16LE(_field1ABA);
	s.syncAsSint16LE(_field1ABC);
	s.syncAsSint16LE(_field1ABE);
	s.syncAsSint16LE(_field1AC0);
	s.syncAsSint16LE(_field1AC2);
	s.syncAsSint16LE(_field1AC4);
	s.syncAsSint16LE(_field1AC6);
}

void Scene840::postInit(SceneObjectList *OwnerList) {
	PalettedScene::postInit(OwnerList);
	BF_GLOBALS._sound1.changeSound(41);
	loadScene(840);
	_field1ABA = 0;

	if (BF_GLOBALS._dayNumber == 0) {
		// Debugging mode setup
		BF_GLOBALS._dayNumber = 4;
		BF_INVENTORY.setObjectScene(INV_NICKEL, 1);
		BF_INVENTORY.setObjectScene(INV_RENTAL_COUPON, 1);
	}

	setZoomPercents(0, 90, 110, 100);
	_exit.setDetails(Rect(310, 133, 320, 167), 840, -1, -1, -1, 1, NULL);

	_stripManager.addSpeaker(&_gameTextSpeaker);
	_stripManager.addSpeaker(&_jakeJacketSpeaker);
	_stripManager.addSpeaker(&_jakeUniformSpeaker);
	_stripManager.addSpeaker(&_carterSpeaker);

	BF_GLOBALS._player.postInit();
	BF_GLOBALS._player.changeZoom(-1);
	BF_GLOBALS._player._moveDiff.x = BF_GLOBALS.getFlag(onDuty) ? 8 : 7;

	_doors.postInit();
	_doors.setVisage(840);
	_doors.setStrip(3);
	_doors.setPosition(Common::Point(157, 81));
	_doors.setDetails(840, 0, 1, 2, 1, NULL);

	_carter.postInit();
	_carter.setVisage(843);
	_carter.setStrip(6);
	_carter.fixPriority(110);
	_carter.setPosition(Common::Point(262, 128));
	_carter.changeZoom(-1);

	_item14.setDetails(1, 840, 29, 30, 31, 1);
	_item2.setDetails(Rect(6, 5, 85, 31), 840, 16, 14, 12, 1, NULL);
	_item3.setDetails(Rect(91, 7, 102, 26), 840, 17, 14, 12, 1, NULL);
	_item4.setDetails(Rect(6, 49, 80, 67), 840, 18, 15, 13, 1, NULL);
	_item5.setDetails(Rect(80, 29, 100, 65), 840, 19, 15, 13, 1, NULL);
	_item6.setDetails(Rect(104, 32, 127, 78), 840, 20, 15, 13, 1, NULL);
	_item7.setDetails(Rect(1, 106, 35, 166), 840, 21, 15, 13, 1, NULL);
	_item9.setDetails(Rect(205, 17, 251, 60), 840, 23, 15, 13, 1, NULL);
	_item10.setDetails(Rect(199, 56, 236, 80), 840, 24, 14, 12, 1, NULL);
	_item11.setDetails(Rect(256, 94, 319, 118), 840, 25, 15, 13, 1, NULL);
	_item18.setDetails(6, 840, 38, 39, 40, 1);
	_carter.setDetails(840, 3, 4, 5, 1, NULL);
	_item8.setDetails(Rect(259, 4, 319, 87), 840, 22, 15, 13, 1, NULL);
	_item15.setDetails(2, 840, 32, 33, 34, 1);
	_coins.setDetails(3, 840, -1, 6, 7, 1);
	_item16.setDetails(4, 840, 44, 45, 46, 1);
	_item17.setDetails(5, 840, 26, 27, 28, 1);
	_item12.setDetails(7, 840, 35, 36, 37, 1);
	_item13.setDetails(Rect(0, 0, SCREEN_WIDTH - 1, BF_INTERFACE_Y), 840, 41, 42, 43, 1, NULL);

	if (BF_INVENTORY.getObjectScene(INV_RENTAL_KEYS) == 1) {
		_object6.postInit();
		_object6.setVisage(840);
		_object6.setStrip(4);
		_object6.setFrame(1);
		_object6.setPosition(Common::Point(250, 83));
		_object6.fixPriority(120);
		_object6.setDetails(840, -1, 8, 9, 2, NULL);
		_field1AC0 = 1;
	}

	_sceneMode = BF_GLOBALS.getFlag(onDuty) ? 8401 : 8400;
	setAction(&_sequenceManager1, this, _sceneMode, &BF_GLOBALS._player, NULL);

	if (BF_GLOBALS.getFlag(fWithLyle)) {
		_lyle.postInit();
		_lyle.setAction(&_sequenceManager2, NULL, 8405, &_lyle, NULL);
	}
}

void Scene840::signal() {
	switch (_sceneMode) {
	case 1:
		// Talking to Carter
		BF_GLOBALS._player._strip = 7;
		BF_GLOBALS._player._frame = 1;
		_sceneMode = 3;

		switch (BF_GLOBALS._dayNumber) {
		case 1:
			if (_field1ABA == 1) {
				_stripManager.start(BF_GLOBALS.getFlag(onDuty) ? 8430 : 8406, this);
			} else if (!BF_GLOBALS.getFlag(onDuty)) {
				_stripManager.start(!BF_GLOBALS._v4CEB6 ? 8400 : 8406, this);
			} else if (BF_GLOBALS._bookmark < bStartOfGame) {
				_stripManager.start(!BF_GLOBALS._v4CEB6 ? 8429 : 8430, this);
			} else if (BF_GLOBALS._bookmark < bCalledToDomesticViolence) {
				_stripManager.start(!BF_GLOBALS._v4CEB6 ? 8402 : 8401, this);
			} else {
				_stripManager.start(!BF_GLOBALS._v4CEB6 ? 8404 : 8403, this);
			}
			break;
		case 2:
			_stripManager.start(!BF_GLOBALS._v4CEB6 ? 8405 : 8430, this);
			break;
		case 3:
			if (!BF_GLOBALS._v4CEB6) {
				_stripManager.start(BF_GLOBALS.getFlag(fWithLyle) ? 8409 : 8411, this);
			} else if (_field1ABA == 1) {
				_stripManager.start(8406, this);
			} else if (BF_GLOBALS.getFlag(fWithLyle)) {
				_stripManager.start(BF_GLOBALS.getFlag(fCarterMetLyle) ? 8415 : 8414, this);
			} else {
				_stripManager.start(8413, this);
			}

			BF_GLOBALS.setFlag(fTalkedCarterDay3);
			break;
		case 4:
			if (!BF_GLOBALS._v4CEB6) {
				if (BF_GLOBALS.getFlag(fTalkedCarterDay3)) {
					if (BF_GLOBALS.getFlag(fWithLyle))
						_stripManager.start(BF_GLOBALS.getFlag(fCarterMetLyle) ? 8415 : 8414, this);
					else
						_stripManager.start(8413, this);
				} else if (BF_GLOBALS.getFlag(fWithLyle)) {
					_stripManager.start(BF_GLOBALS.getFlag(fCarterMetLyle) ? 8431 : 8409, this);
				} else {
					_stripManager.start(8411, this);
				}
			} else if (_field1ABA == 1) {
				_stripManager.start(8406, this);
			} else if (!BF_GLOBALS.getFlag(fWithLyle)) {
				_stripManager.start(8413, this);
			} else {
				_stripManager.start(BF_GLOBALS.getFlag(fCarterMetLyle) ? 8415 : 8414, this);
			}
			break;
		case 5:
			_stripManager.start((_field1ABA == 1) ? 8406 : 8413, this);
			break;
		default:
			break;
		}

		_field1ABA = 1;
		break;
	case 2:
		BF_GLOBALS._sceneManager.changeScene(830);
		BF_GLOBALS._sound1.changeSound(10);
		break;
	case 3:
	case 8404:
	case 8416:
		BF_GLOBALS._player.enableControl();
		break;
	case 4:
		_sceneMode = 8403;
		_object6.postInit();
		_object6.setDetails(840, -1, 8, 9, 2, NULL);
		setAction(&_sequenceManager1, this, 8403, &_carter, &_object6, NULL);
		break;
	case 5:
		_sceneMode = 8408;
		setAction(&_sequenceManager1, this, 8408, &BF_GLOBALS._player, &_carter, &_doors, NULL);
		_field1AC2 = 1;
		break;
	case 6:
		BF_GLOBALS._player._strip = 7;
		BF_GLOBALS._player._frame = 1;

		if ((BF_GLOBALS._dayNumber == 4) && (BF_GLOBALS._bookmark >= bEndDayThree)) {
			_stripManager.start(8440, this);
			_sceneMode = 3;
		} else if (BF_GLOBALS._sceneObjects->contains(&_object6)) {
			_stripManager.start(8442, this);
			_sceneMode = 3;
		} else if (_field1AC6) {
			_sceneMode = 4;
			_stripManager.start(BF_GLOBALS.getFlag(fWithLyle) ? 8424 : 8410, this);
		} else {
			_sceneMode = 3;
			_stripManager.start(8436, this);
		}
		break;
	case 8400:
	case 8401:
		BF_GLOBALS._player._strip = 7;
		BF_GLOBALS._player._frame = 1;
		BF_GLOBALS._player.enableControl();
		break;
	case 8402:
		BF_GLOBALS._player.enableControl();
		BF_GLOBALS._uiElements.addScore(30);
		BF_INVENTORY.setObjectScene(INV_CARTER_NOTE, 3);
		BF_INVENTORY.setObjectScene(INV_BASEBALL_CARD, 1);
		BF_INVENTORY.setObjectScene(INV_RENTAL_COUPON, 1);
		break;
	case 8403:
		BF_GLOBALS._uiElements.addScore(30);
		_sceneMode = 3;
		_field1AC0 = 1;
		_stripManager.start(8441, this);
		break;
	case 8407:
		if (((BF_GLOBALS._dayNumber == 4) && (BF_GLOBALS._bookmark >= bEndDayThree)) ||
				(BF_INVENTORY.getObjectScene(INV_WAVE_KEYS) == 1)) {
			_sceneMode = 8416;
			setAction(&_sequenceManager1, this, 8416, &BF_GLOBALS._player, NULL);
		} else {
			_stripManager.start(_field1ABE + 8419, this);
			_sceneMode = 5;

			if (++_field1ABE == 4)
				_field1ABE = 1;
		}
		break;
	case 8408:
		if (_field1AC0) {
			BF_GLOBALS._player.enableControl();
		} else {
			_sceneMode = 8409;
			setAction(&_sequenceManager1, this, 8409, &BF_GLOBALS._player, &_carter, &_doors, NULL);
		}
		break;
	case 8409:
		_field1AC2 = 0;
		_sceneMode = 3;
		_stripManager.start(8423, this);
		break;
	case 8410:
		BF_GLOBALS._player.enableControl();
		_object1.remove();
		break;
	case 8411:
		BF_GLOBALS._player.enableControl();
		_object2.postInit();
		_object2.setVisage(840);
		_object2.setStrip(2);
		_object2.setPosition(Common::Point(160, 140));
		_object2.fixPriority(254);
		_object2.setDetails(840, 50, 8, 51);
		break;
	case 8412:
		if (_object2._v1B6) {
			_sceneMode = 8409;
			setAction(&_sequenceManager1, this, 8409, &BF_GLOBALS._player, &_carter, &_doors, NULL);
		} else if (!_object2._v1B4) {
			BF_GLOBALS._player.enableControl();
		} else {
			_sceneMode = 3;
			_field1AC0 = 1;

			if (BF_GLOBALS._dayNumber == 5) {
				_stripManager.start(8425, this);
			} else {
				_stripManager.start(BF_GLOBALS.getFlag(INV_NICKEL) ? 8417 : 8416, this);
			}
		}
		break;
	case 8413:
		BF_GLOBALS._uiElements.addScore(50);
		_sceneMode = 8409;
		setAction(&_sequenceManager1, this, 8409, &BF_GLOBALS._player, &_carter, &_doors, NULL);
		break;
	case 8417:
		_field1ABA = 1;
		BF_GLOBALS._uiElements.addScore(50);
		BF_INVENTORY.setObjectScene(INV_WAVE_KEYS, 0);
		BF_INVENTORY.setObjectScene(INV_RENTAL_KEYS, 0);
		BF_GLOBALS._player.enableControl();
		break;
	}
}

void Scene840::process(Event &event) {
	SceneExt::process(event);

	if (BF_GLOBALS._player._enabled && !_focusObject && (event.mousePos.y < (BF_INTERFACE_Y - 1))) {
		if (_exit.contains(event.mousePos)) {
			GfxSurface surface = _cursorVisage.getFrame(EXITFRAME_E);
			BF_GLOBALS._events.setCursor(surface);
		} else {
			// In case an exit cursor was being shown, restore the previously selected cursor
			CursorType cursorId = BF_GLOBALS._events.getCursor();
			BF_GLOBALS._events.setCursor(cursorId);
		}
	}
}

void Scene840::dispatch() {
	SceneExt::dispatch();

	if (_field1ABC && (BF_GLOBALS._player._position.x > 310) && !_action) {
		_field1ABC = 0;
		BF_GLOBALS._player.disableControl();
		_sceneMode = 2;
		ADD_PLAYER_MOVER(350, 150);

		if (BF_GLOBALS.getFlag(fWithLyle)) {
			_lyle.setAction(&_sequenceManager2, NULL, 8406, &_lyle, NULL);
		}
	}
}

} // End of namespace BlueForce

} // End of namespace TsAGE
