#include "HeaderTable.h"
#include <iostream>
using namespace std;

HeaderTable::~HeaderTable() {

}


void HeaderTable::insertTable(char* item, int frequency) {
	pair<int, string> p = make_pair(frequency, item);
	if (indexTable.size() == 0) {
		indexTable.push_front(p);
		return;
	}
	//iterator 
	list<pair<int, string>>::iterator iter = indexTable.begin();

	// Add frequency
	for (iter = indexTable.begin(); iter != indexTable.end(); iter++) {
		if ((*iter).second == item) {
			(*iter).first = (*iter).first + 1;
			return;
		}
	}

	indexTable.push_back(p);	
}
void HeaderTable::insertTable_map(int threshold) {
	if (indexTable.size() == 0) {
		printf("doesn't exist indexTable\n");
		return;
	}
	//iterator
	list<pair<int, string>>::iterator iter = indexTable.begin();
	// compare frequency threshold 
	for (iter = indexTable.begin(); iter != indexTable.end(); iter++) {
		if ((*iter).first >= threshold)
			dataTable.insert(map<string, FPNode*>::value_type((*iter).second, NULL)); //insert map
	}
}
int HeaderTable::find_frequency(string item){
	//iterator 
	list<pair<int, string>>::iterator iter = indexTable.begin();
	for (iter = indexTable.begin(); iter != indexTable.end(); iter++) {
		if ((*iter).second == item)
			return (*iter).first;
	}

	return 0;
}

void HeaderTable::setData(string item, FPNode* node) {
	FPNode* target_node = dataTable.find(item)->second;
	if (! (target_node == NULL))
	{
		target_node -> setItem(item.c_str());
	}
}
