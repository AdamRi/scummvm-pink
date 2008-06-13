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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#ifndef THEME_PARSER_H
#define THEME_PARSER_H

#include "common/scummsys.h"
#include "graphics/surface.h"
#include "common/system.h"

#include "common/hashmap.h"
#include "common/hash-str.h"
#include "common/stack.h"

namespace GUI {

class ThemeParser {

	static const int PARSER_MAX_DEPTH = 4;
	typedef void (ThemeParser::*PARSER_CALLBACK)();

public:
	ThemeParser() {
		_callbacks["drawdata"] = &ThemeParser::parserCallback_DRAWDATA;
		_callbacks["draw"] = &ThemeParser::parserCallback_DRAW;
	}

	~ThemeParser() {}

	enum ParserState {
		kParserNeedKey,
		kParserNeedKeyName,
		kParserNeedKeyValues,
		kParserError
	};

	bool parse();
	void parseKeyValue(Common::String &key_name);
	void parseActiveKey(bool closed);

	void parserError(const char *error_string);

	void debug_testEval();
	
protected:
	void parserCallback_DRAW();
	void parserCallback_DRAWDATA();

	inline void skipSpaces() {
		while (isspace(_text[_pos]))
			_pos++;
	}

	inline void skipComments() {
		if (_text[_pos] == '/' && _text[_pos + 1] == '*') {
			_pos += 2;
			while (_text[_pos++]) {
				if (_text[_pos - 2] == '*' && _text[_pos - 1] == '/')
					break;
			}
			skipSpaces();
		}
	}

	int _pos;
	char *_text;

	ParserState _state;

	Common::String _error;
	Common::String _token;

	Common::FixedStack<Common::String, PARSER_MAX_DEPTH> _activeKey;
	Common::FixedStack<Common::StringMap, PARSER_MAX_DEPTH> _keyValues;

	Common::HashMap<Common::String, PARSER_CALLBACK, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> _callbacks;
};

}

#endif
