#ifndef PINK_CGAMEPAGE_H
#define PINK_CGAMEPAGE_H

#include "common/scummsys.h"
#include "cpage.h"

namespace Common {
	template<class T> class Array;
}

namespace Pink {
	class CModule;
	class CCursorMgr;
	class CWalkMgr;
	class CSequencer;
}

namespace Pink {

	class CGamePage : public CPage {
	public:
		CGamePage();
		virtual ~CGamePage();

		static CObject *createObject();
		static CRuntimeClass *RuntimeClass();
		virtual CRuntimeClass *getRuntimeClass();

		virtual void deserialize(CArchive &archive);
		virtual void readFromOrb(CArchive &archive);

		virtual void init(PinkEngine *pink);
		virtual void execute(PinkEngine *pink);

		CModule *_module;
		CCursorMgr *cursor_manager;
		CWalkMgr *walk_manager;
		CSequencer *sequencer;
		Common::Array<CObject *>*handlers;
	};

};

#endif //PINK_CGAMEPAGE_H
