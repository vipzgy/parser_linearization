#ifndef Instance_H_
#define Instance_H_

class Instance {
public:
	vector<string> words;
	vector<int> heads;
	vector<string> relations;

	void resize(const int size){
		words.resize(size);
		heads.resize(size);
		relations.resize(size);
	}

	void show(){
		int max_size = words.size();
		for(int idx = 0; idx < max_size; idx++) {
			cout << words[idx] << "\t" << heads[idx] << "\t" << relations[idx] << endl;
		}
		cout << endl;
	}

	bool checkRoot(){
		const int max_size = words.size();
		assert(max_size > 0);
		int min_head = heads[0], max_head = heads[0], tmpHead;
		for (int idx = 1; idx < max_size; idx++) {
			tmpHead = heads[idx];
			if (tmpHead < min_head)
				 min_head = tmpHead;
			if (tmpHead > max_head)
				max_head = tmpHead;
		}
		// start from 0 , root = -1
		if(min_head == 0) {
			for (int idx = 0; idx < max_size; idx++) {
				heads[idx]--;
			}
			min_head--;
			max_head--;
		}
		if (min_head == -1 && max_head < max_size)
			return true;
		else
			return false;
	}

	bool checkProjective() {
		int max_size = words.size();
		int head, tmpHead;
		int max, min;
		for(int idx = 0; idx < max_size; idx++) {
			head = heads[idx];
			min = idx > head ? head : idx;
			max = idx < head ? head : idx;
			for (int idy = min + 1; idy < max; idy++) {
				tmpHead = heads[idy];
				if(tmpHead < min || tmpHead > max) {
					return false;
				}
			}
		}
		return true;
	}
};

#endif