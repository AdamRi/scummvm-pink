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
 * $URL$
 * $Id$
 *
 */

#ifndef KYRA_TEXT_V2_H
#define KYRA_TEXT_V2_H

#include "kyra/text.h"

namespace Kyra {

class Screen_v2;
class KyraEngine_v2;

class TextDisplayer_v2 : public TextDisplayer {
friend class KyraEngine_v2;
public:
	TextDisplayer_v2(KyraEngine_v2 *vm, Screen_v2 *screen);

	void backupTalkTextMessageBkgd(int srcPage, int dstPage);
	void restoreScreen();

	char *preprocessString(const char *str);
	void calcWidestLineBounds(int &x1, int &x2, int w, int x);
private:
	KyraEngine_v2 *_vm;
};

} // end of namespace Kyra

#endif

