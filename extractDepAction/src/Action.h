#ifndef CAction_H_
#define CAction_H_

class CAction {
public:
	enum CODE { ARC_LEFT = 0, ARC_RIGHT = 1, SHIFT = 2, POP_ROOT = 3, NO_ACTION = 4 };
	string _relation;
	unsigned long _code;

	inline bool isNone() const { return _code == NO_ACTION; }

	inline bool isFinish() const { return _code == POP_ROOT; }

	inline bool isShift() const { return _code == SHIFT; }

	inline bool isArcLeft() const { return _code == ARC_LEFT; }

	inline bool isArcRight() const { return _code == ARC_RIGHT; }

	bool operator == (const CAction &action) const {
		return _code == action._code && _relation == action._relation;
	}

	bool operator != (const CAction &action) const {
		return _code != action._code || _relation != action._relation;
	}

	inline void clear() {
		_code = NO_ACTION;
		_relation = "";
	}

	inline void set(const int &code, const string &relation) {
		_code = code;
		_relation = relation;
	}

	inline void set(const CAction &ac) {
		_code = ac._code;
		_relation = ac._relation;
	}
};

#endif