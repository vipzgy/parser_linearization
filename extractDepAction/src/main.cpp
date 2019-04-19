#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <assert.h>
#include <set>
#include <cstring>


using namespace std;

#include "State.h"
#include "Action.h"
#include "Instance.h"

const int max_state_size = 40960;
vector<CState> all_states(max_state_size);

void split_bychar(const string& str, vector<string>& vec, const char separator);
void readInstance(const char *path, const char *out_path);
void getInst(const vector<string> &vecInfo, Instance &inst);
void extractActions(Instance &inst, ofstream &outf);

void getGoldAction(const Instance &inst, vector<string> &actions_str);
void writeActions(const vector<string> &actions_str, ofstream &outf);

int main(int argc, char* argv[]) {
	vector<Instance> insts;
	vector<string> actions_str;
	readInstance(argv[1], argv[2]);
	cout << "extract ok" << endl;
	return 0;
}

void readInstance(const char *in_path, const char *out_path){
	ifstream inf(in_path);
	ofstream outf(out_path);
	if(inf.is_open() && outf.is_open()){
		string line;
		vector<string> vecLines;
		while(getline(inf, line)) { 
			if (line == "") {
				Instance inst;
				getInst(vecLines, inst);
				extractActions(inst, outf);
				vecLines.clear();
			} 
			else
				vecLines.push_back(line);
		}
		if(vecLines.size() > 0) {
			Instance inst;
			getInst(vecLines, inst);
			extractActions(inst, outf);
		}
		inf.close();
		outf.close();
	}
	else
		cout << "open file error. " << endl;
}

void getInst(const vector<string> &vecInfo, Instance &inst) {
	int max_size = vecInfo.size();
	inst.resize(max_size);
	vector<string> fields;
	for(int idx = 0; idx < max_size; idx++) {
		split_bychar(vecInfo[idx], fields, '\t');
		assert(fields.size() == 10);
		inst.words[idx] = fields[1];
		inst.heads[idx] = atoi(fields[6].c_str());
		inst.relations[idx] = fields[7];
	}
}

void split_bychar(const string& str, vector<string>& vec, const char separator) {
	//assert(vec.empty());
	vec.clear();
	string::size_type pos1 = 0, pos2 = 0;
	string word;
	while ((pos2 = str.find_first_of(separator, pos1)) != string::npos) {
		word = str.substr(pos1, pos2 - pos1);
		pos1 = pos2 + 1;
		if (!word.empty())
			vec.push_back(word);
	}
	word = str.substr(pos1);
	if (!word.empty())
		vec.push_back(word);
}

void getGoldAction(const Instance &inst, vector<string> &actions_str) {
	actions_str.clear();
	CAction ac;
	int step = 0;
	all_states[0].clear(); // clear first state;
	all_states[0].ready(&inst); //  the state of instance
	step = 0;
	while (!all_states[step].isEnd()) {
		all_states[step].getGoldAction(ac);
		all_states[step].move(&all_states[step + 1], ac);
		actions_str.push_back(all_states[step]._cur_action_str);
		step++;
	}
}

void extractActions(Instance &inst, ofstream &outf) {
	if(!inst.checkRoot()) { 
		cout << "No root id or Head out of range, skip" << endl;
		inst.show();
		return;
	}
	if (!inst.checkProjective()) {
		cout << "No Projective find, skip" << endl;
		inst.show();
		return;
	}

	vector<string> actions_str;
	getGoldAction(inst, actions_str);
	assert(inst.words.size() * 2 == actions_str.size());
	writeActions(actions_str, outf);
}

void writeActions(const vector<string> &actions_str, ofstream &outf) {
	int max_size = actions_str.size();
	for(int idx = 0; idx < max_size; idx++){
		outf << actions_str[idx] << " ";
	}
	outf << endl;
}
