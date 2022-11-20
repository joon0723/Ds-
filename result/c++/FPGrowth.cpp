#include "FPGrowth.h"


FPGrowth::~FPGrowth() {

}

void FPGrowth::createFPtree(FPNode* root, HeaderTable* table, list<string> item_array, int frequency) {
	FPNode* parentfpnode = new FPNode;
	parentfpnode = root; //start root

	list<string>::iterator it = item_array.begin();//iterator
	for (it = item_array.begin(); it != item_array.end(); it++) {

		FPNode* newfpnode = new FPNode; //alloc
		newfpnode->setItem((char*)(*it).c_str()); // string-> char change
		newfpnode->updateFrequency(frequency); //frequency
		newfpnode->setParent(parentfpnode);
		//set

		if (parentfpnode->getChildrenNode(*it) == NULL) {
			parentfpnode->pushchildren((*it), newfpnode);
			connectNode(table, newfpnode->getItem(), newfpnode);
		} // find same item, dosn't exist
		else {
			parentfpnode->getChildrenNode(*it)->updateFrequency(1); //update
		}// find same item, exist
		parentfpnode = parentfpnode->getChildrenNode(*it); //change
	}

}

void FPGrowth::connectNode(HeaderTable* table, string item, FPNode* node) {
	map<string, FPNode*> map_find;
	map_find = table->getdataTable();
	auto it = map_find.find(item);

	FPNode* temp = new FPNode;
	FPNode* p_temp = new FPNode;

	temp = (*it).second;
	if (temp == NULL) {
		table->setData(item, node);
		(*it).second = node;
		return;
	} //first string table 
	while (temp != NULL) {
		p_temp = temp;
		temp = temp->getNext(); //next node
	}
	p_temp->setNext(node); // link same string
}

bool FPGrowth::contains_single_path(FPNode* pNode) {
	if (pNode->getChildren().size() == 0) return true;
	else if (pNode->getChildren().size() > 1) return false;
	return contains_single_path(pNode->getChildren().begin()->second);
}

map<set<string>, int> FPGrowth::getFrequentPatterns(HeaderTable* pTable, FPNode* pTree) {

	return {};
}



void FPGrowth::powerSet(map<set<string>, int>* FrequentPattern, vector<string> data, string item, int frequency, int* ptr, int depth) {
	if (data.size() == depth) {
		set<string> set; set.insert(item);
		for (int i = 0; i < data.size(); i++) { if (ptr[i] == 1) set.insert(data[i]); }
		FrequentPattern->insert(make_pair(set, frequency)); return;
	}
	ptr[depth] = 1;
	powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1);
	ptr[depth] = 0;
	powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1);
}

bool FPGrowth::printList() {

	return true;
}
bool FPGrowth::printTree() {
	return true;
}
void FPGrowth::saveFrequentPatterns(){

}
