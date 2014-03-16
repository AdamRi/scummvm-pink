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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "common/scummsys.h"
#include "mads/mads.h"
#include "mads/scene.h"
#include "mads/nebular/nebular_scenes.h"
#include "mads/nebular/nebular_scenes2.h"

namespace MADS {

namespace Nebular {

void Scene2xx::setAAName() {
	int idx = (_scene->_nextSceneId == 216) ? 4 : 2;
	_game._aaName = Resources::formatAAName(idx);
}

void Scene2xx::setPlayerSpritesPrefix() {
	_vm->_sound->command(5);
	Common::String oldName = _game._player._spritesPrefix;

	switch(_scene->_nextSceneId) {
	case 213:
	case 216:
		_game._player._spritesPrefix = "";
		break;
	default:
		if (_globals[0] == SEX_MALE) {
			_game._player._spritesPrefix = "ROX";
		} else {
			_game._player._spritesPrefix = "RXM";
		}
		break;
	}

	if (_scene->_nextSceneId > 212)
		_game._player._unk4 = 0;
	else
		_game._player._unk4 = -1;

	if (oldName != _game._player._spritesPrefix)
		_game._player._spritesChanged = true;
	
	if ((_scene->_nextSceneId == 203 || _scene->_nextSceneId == 204) && _globals[34])
		_game._v3 = 0;

	_vm->_palette->setEntry(16, 10, 63, 63);
	_vm->_palette->setEntry(17, 10, 45, 45);
}

/*------------------------------------------------------------------------*/

void Scene201::setup() {
	setPlayerSpritesPrefix();
	setAAName();

	Scene &scene = _vm->_game->_scene;
	scene.addActiveVocab(NOUN_15F);
	scene.addActiveVocab(NOUN_487);
	scene.addActiveVocab(NOUN_D);
}

void Scene201::enter() {
	if ((_globals._frameTime) && (_vm->getRandomNumber(5000) == 9)) {
		_globals._spriteIndexes[20] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[5], false, 5, 1, 6, 0);
		int idx = _scene->_dynamicHotspots.add(351, 13, _globals._spriteIndexes[20], Common::Rect(0, 0, 0, 0));
		_scene->_dynamicHotspots.setPosition(idx, 270, 80, 6);
		_scene->_sequences.setDepth(_globals._spriteIndexes[20], 8);
		_vm->_sound->command(14);
		_globals._frameTime = 0;
	}
	
	if (_game._abortTimers == 70) {
		_globals._spriteIndexes[21] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[6], false, 9, 1, 0, 0);
		_game._player._visible = false;
		_scene->_sequences.setAnimRange(_globals._spriteIndexes[21], 12, 16);
		_globals._spriteIndexes[22] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[7], false, 9, 1, 0, 0);
		_vm->_sound->command(42);
		_scene->_sequences.setDepth(_globals._spriteIndexes[21], 1);
		_scene->_sequences.setDepth(_globals._spriteIndexes[22], 1);
		_scene->_sequences.addSubEntry(_globals._spriteIndexes[22], SM_FRAME_INDEX, 3, 81);
		_scene->_sequences.addSubEntry(_globals._spriteIndexes[22], SM_0, 0, 71);
		_scene->_sequences.addSubEntry(_globals._spriteIndexes[21], SM_0, 0, 73);
	}

	if (_game._abortTimers == 81) {
		_scene->_kernelMessages.reset();
	}

	if (_game._abortTimers == 71) {
		_globals._spriteIndexes[22] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[7], false, 9, 0, 0, 0);
		_scene->_sequences.setAnimRange(_globals._spriteIndexes[22], -2, -2);
		_scene->_sequences.setDepth(_globals._spriteIndexes[22], 1);
	}

	if (_game._abortTimers == 73) {
		_globals._spriteIndexes[21] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[6], false, 9, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._spriteIndexes[21], 17, -2);
		_scene->_sequences.addSubEntry(_globals._spriteIndexes[21], SM_0, 0, 74);
		_scene->_sequences.setDepth(_globals._spriteIndexes[21], 1);
	}

	if (_game._abortTimers == 74) {
		_vm->_sound->command(40);

		_scene->_kernelMessages.add(Common::Point(125, 56), 0xFDFC, 32, 82, 180, _game.getQuote(91));
		_globals._spriteIndexes[21] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[6], false, 9, 0, 0, 0);
		_scene->_sequences.setDepth(_globals._spriteIndexes[21], 1);
		_scene->_sequences.setAnimRange(_globals._spriteIndexes[21], -2, -2);
		_scene->_sequences.addTimer(180, 75);
	}

	if (_game._abortTimers == 75) {
		_globals[37] = 0;
		_scene->_nextSceneId = 202;
	}

	if (_game._abortTimers == 76) {
		_game._player._stepEnabled = true;
		_game._player._visible = true;
		_game._player._priorTimer = _scene->_frameStartTime - _game._player._ticksAmount;
	}

	if (_game._abortTimers == 77) {
		_globals[39] = 1;
		_scene->_nextSceneId = _globals[40];
		_scene->_reloadSceneFlag = true;
	}

	if (_game._abortTimers == 78) {
		_vm->_sound->command(40);
		Dialog::show(0x4E92);
		_scene->_reloadSceneFlag = true;
	}
}

void Scene201::step() {
}

void Scene201::actions() {
}

} // End of namespace Nebular
} // End of namespace MADS
