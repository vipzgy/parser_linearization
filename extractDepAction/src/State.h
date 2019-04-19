#ifndef CState_H_
#define CState_H_

#include "Instance.h"
#include "Action.h"

const int max_length = 1024;

class CState {
public:
	short _stack[max_length]; // the stack
	short _stack_size; // the size  of stack
	short _head[max_length]; // head
	short _have_parent[max_length]; // recored head assigned

	short _next_index; // index of top element of buffer
	int _word_size; // word num of sentence

	const Instance* inst; // Instance 

	CState* _pre_state; //  previous action

	CAction _pre_action; //previous action
	bool _is_start; // first state or not
	bool _is_gold; // moving to gold action and being gold state

	string _cur_action_str;

								 // prepare instance
	inline void ready(const Instance *pInst) {
		this->inst = pInst;
		_word_size = inst->words.size();
	}

	void clear() {
		_stack_size = 0;
		_next_index = 0;
		_word_size = 0;

		_is_start = true;
		_is_gold = true;
		_pre_state = NULL;
		doneMark();
	}

	// temp mark
	inline void doneMark() {
		_stack[_stack_size] = -2;
		_head[_next_index] = -2;
		_have_parent[_next_index] = -2;
	}

	// copy data to next state	
	void copyState(CState *next) {
		memcpy(next->_stack, _stack, sizeof(short) * _stack_size);
		memcpy(next->_head, _head, sizeof(short) * _next_index);
		memcpy(next->_have_parent, _have_parent, sizeof(short) * _next_index);
		next->_word_size = _word_size;
		next->inst = inst;
		next->_pre_state = this;
	}

	void getGoldAction(CAction &ac) {
		if (_stack_size == 0) {
			ac.set(CAction::SHIFT, "");
		}
		else if (_stack_size == 1) {
			const int& top0 = _stack[_stack_size - 1];
			if (_next_index == _word_size)
				ac.set(CAction::POP_ROOT, "");
			else
				ac.set(CAction::SHIFT, "");
		}
		else if (_stack_size > 1) {
			const int& top0 = _stack[_stack_size - 1];
			const int& top1 = _stack[_stack_size - 2];
			if (inst->heads[top1] == top0) {
				ac.set(CAction::ARC_LEFT, inst->relations[top1]);
			}
			else if (inst->heads[top0] == top1) {
				// check the top of stack have right children or not.
				bool have_right_children = false;
				for (int idx = _next_index; idx < _word_size; idx++) {
					if (inst->heads[idx] == top0) {
						have_right_children = true;
						break;
					}
				}
				if (have_right_children) {
					ac.set(CAction::SHIFT, "");
				}
				else {
					ac.set(CAction::ARC_RIGHT, inst->relations[top0]);
				}
			}
			else {
				ac.set(CAction::SHIFT, "");
			}
		}
	}

	bool isEnd() const {
		if (_pre_action.isFinish())
			return true;
		else
			return false;
	}

	// shift action, top of buffer -> stack
	void shift(CState *next) {
		assert(_next_index <= _word_size && _stack_size >= 0 && _word_size >= 0);
		next->_next_index = _next_index + 1;
		next->_stack_size = _stack_size + 1;
		copyState(next);

		next->_stack[next->_stack_size - 1] = _next_index;
		next->_have_parent[_next_index] = 0;
		next->doneMark();
		next->_pre_action.set(CAction::SHIFT, "");
		_cur_action_str = inst->words[_next_index];
	}
	// left action
	void arcLeft(CState *next, const string &relation) {
		assert(_next_index <= _word_size && _stack_size >= 0 && _word_size >= 0);
		next->_next_index = _next_index;
		next->_stack_size = _stack_size - 1;
		copyState(next);

		int top0 = _stack[_stack_size - 1];
		int top1 = _stack[_stack_size - 2];
		next->_stack[next->_stack_size - 1] = top0;
		next->_head[top1] = top0;
		next->_have_parent[top1] = 1;

		next->doneMark();
		next->_pre_action.set(CAction::ARC_LEFT, relation);
		_cur_action_str = "arc##left##" + relation;
	}
	//right action	
	void arcRight(CState *next, const string &relation) {
		assert(_next_index <= _word_size && _stack_size >= 0 && _word_size >= 0);
		next->_next_index = _next_index;
		next->_stack_size = _stack_size - 1;
		copyState(next);

		int top0 = _stack[_stack_size - 1];
		int top1 = _stack[_stack_size - 2];
		next->_head[top0] = top1;
		next->_have_parent[top0] = 1;

		next->doneMark();
		next->_pre_action.set(CAction::ARC_RIGHT, relation);
		_cur_action_str = "arc##right##" + relation;
	}
	// pop root action
	void popRoot(CState *next, const string &relation) {
		assert(_next_index <= _word_size && _stack_size >= 0 && _word_size >= 0);
		next->_next_index = _word_size;
		next->_stack_size = 0;
		copyState(next);

		int top0 = _stack[_stack_size - 1];
		next->_head[top0] = -1;
		next->_have_parent[top0] = 1;

		next->doneMark();
		next->_pre_action.set(CAction::POP_ROOT, relation);
		_cur_action_str = "pop##NoDirection##root";
	}

	//move to next state 
	void move(CState *next, const CAction &ac) {
		next->_is_start = false; // if a state move to next, next state can't be start 
		next->_is_gold = false; // here we don't know the action is gold or not
		if (ac.isShift()) {
			shift(next);
		}
		else if (ac.isArcLeft()) {
			arcLeft(next, ac._relation);
		}
		else if (ac.isArcRight()) {
			arcRight(next, ac._relation);
		}
		else if (ac.isFinish()) {
			popRoot(next, ""); // pop root
		}
		else {
			cout << "error action" << endl;
			exit(0);
		}
	}
};

#endif
