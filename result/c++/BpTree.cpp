#include "BpTree.h"
#include <set>
#include <string>
#include <vector>

bool BpTree::Insert(int key, set<string> set) {

	if (root == NULL) {
		BpTreeDataNode* newDataNode = new BpTreeDataNode;
		FrequentPatternNode* newFrePat = new FrequentPatternNode;
		newFrePat->InsertList(set);
		newFrePat->setFrequency(1); //frequentpatternnode setting
		newDataNode->insertDataMap(key, newFrePat);

		//cout << newFrePat->getList().begin()->second.begin()->c_str() << endl;
		root = newDataNode;
		return true;
	}
	BpTreeDataNode* newDataNode = dynamic_cast<BpTreeDataNode*>(searchDataNode(key)); // search datanode
	FrequentPatternNode* newFrePat = new FrequentPatternNode;
	newFrePat->InsertList(set);
	newFrePat->setFrequency(1); //frequentpatternnode setting
	map<int, FrequentPatternNode*>* m_data_it = newDataNode->getDataMap();
	map<int, FrequentPatternNode*>* m_data = newDataNode->getDataMap();
	for (auto it = m_data_it->begin(); it != m_data_it->end(); it++) {
		if ((*it).first == key) {
			it->second->InsertList(set);
			return true;
		}
	}//same key, insert frequentpattern item set

	m_data->insert(pair<int, FrequentPatternNode*>(key, newFrePat)); // insert map data 
	map<int, FrequentPatternNode*>::iterator iter = m_data->begin();

	if (excessDataNode(newDataNode)) { //split, when exceed
		splitDataNode(newDataNode);
	}
	return true;
}

BpTreeNode* BpTree::searchDataNode(int n) {
	BpTreeNode* pCur = root;
	map<int, BpTreeNode*>::iterator iter;

	while (pCur->getMostLeftChild() != NULL) { // exist leftchild 

		iter = dynamic_cast<BpTreeIndexNode*> (pCur)->getIndexMap()->begin();

		if (n < iter->first) {  // indexnode
			pCur = pCur->getMostLeftChild();
		}
		else { // first > n
			while (true) {
				iter++;
				if (iter == dynamic_cast<BpTreeIndexNode*>(pCur)->getIndexMap()->end() || n < iter->first) {
					iter--;
					pCur = iter->second;
					break;
				}
			}
		}
	}
	return pCur;
}

void BpTree::splitDataNode(BpTreeNode* pDataNode) {
	if (pDataNode == root) { //split root
		BpTreeIndexNode* newIndexNode = new BpTreeIndexNode();
		BpTreeDataNode* newDataNode = new BpTreeDataNode(); //alloc

		map<int, FrequentPatternNode*>::iterator iter = dynamic_cast<BpTreeDataNode*> (pDataNode)->getDataMap()->begin();
		map<int, FrequentPatternNode*>* m_data = dynamic_cast<BpTreeDataNode*> (pDataNode)->getDataMap();
		newDataNode->insertDataMap(m_data->begin()->first, iter->second); //setting
		m_data->erase(iter); //remove iter
		iter = dynamic_cast<BpTreeDataNode*> (pDataNode)->getDataMap()->begin();

		pDataNode->setParent(newIndexNode);
		newDataNode->setParent(newIndexNode);
		newIndexNode->getIndexMap()->insert(pair<int, BpTreeNode*>(iter->first, pDataNode));
		newIndexNode->setMostLeftChild(newDataNode);
		//setting indexnode, new tree
		root = newIndexNode; // when pDataNode = root

		newDataNode->setNext(pDataNode);
		dynamic_cast<BpTreeDataNode*> (pDataNode)->setPrev(newDataNode);
		//pre, next linked
		return;
	}
	else {
		BpTreeIndexNode* newIndexNode = dynamic_cast<BpTreeIndexNode*> ((pDataNode)->getParent());
		BpTreeDataNode* newDataNode = new BpTreeDataNode();
		map<int, FrequentPatternNode*>::iterator iter = dynamic_cast<BpTreeDataNode*> (pDataNode)->getDataMap()->begin();
		map<int, FrequentPatternNode*>* m_data = dynamic_cast<BpTreeDataNode*> (pDataNode)->getDataMap();
		iter++;
		newDataNode->insertDataMap(iter->first, iter->second);
		iter++;
		newDataNode->insertDataMap(iter->first, iter->second);
		iter = dynamic_cast<BpTreeDataNode*> (pDataNode)->getDataMap()->begin();
		m_data->erase(++iter); //earase
		iter = dynamic_cast<BpTreeDataNode*> (pDataNode)->getDataMap()->begin();
		m_data->erase(++iter); // erase
		iter = newDataNode->getDataMap()->begin();
		newIndexNode->getIndexMap()->insert(pair<double, BpTreeNode*>(iter->first, newDataNode));
		newDataNode->setParent(newIndexNode); // link parent
		if (dynamic_cast<BpTreeDataNode*>(pDataNode)->getNext() != NULL)
		{
			newDataNode->setNext(dynamic_cast<BpTreeDataNode*> (pDataNode)->getNext()); // next and prev node set
			dynamic_cast<BpTreeDataNode*>(dynamic_cast<BpTreeDataNode*>(pDataNode)->getNext())->setPrev(newDataNode); // next and prev node set
		}
		newDataNode->setPrev(pDataNode); dynamic_cast<BpTreeDataNode*>(pDataNode)->setNext(newDataNode); // next and prev node set
		if (excessIndexNode(newIndexNode)) // indexnode splite
			splitIndexNode(newIndexNode); // splint indexnode
		else // not splinte
			return;
	}
	
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
	if (pIndexNode == root) { //when pIndexNode = root
		BpTreeIndexNode* newIndexNode_left = new BpTreeIndexNode();
		BpTreeIndexNode* newIndexNode_right = new BpTreeIndexNode();
		//alloc new, newIndexNode 

		map<int, BpTreeNode*>* m_data = (pIndexNode)->getIndexMap();
		map<int, BpTreeNode*>::iterator iter = (pIndexNode)->getIndexMap()->begin();

		iter++;
		newIndexNode_left->insertIndexMap(iter->first, newIndexNode_right);
		newIndexNode_left->setMostLeftChild(pIndexNode);
		root = newIndexNode_left;
		newIndexNode_right->setMostLeftChild(iter->second);
		iter->second->setParent(newIndexNode_right);
		//set, connect chind parent, root Index

		iter++;
		newIndexNode_right->insertIndexMap(iter->first, iter->second);
		pIndexNode->setParent(newIndexNode_left);
		newIndexNode_right->setParent(newIndexNode_left);

		map<int, BpTreeNode*>::iterator iter_temp = (newIndexNode_right)->getIndexMap()->begin();
		iter_temp->second->setParent(newIndexNode_right);
		iter = (pIndexNode)->getIndexMap()->begin();
		m_data->erase(++iter); // erase index
		iter = (pIndexNode)->getIndexMap()->begin();
		m_data->erase(++iter); // erase index
		return;
	}
	else { //not root
		BpTreeIndexNode* newIndexNode_left = dynamic_cast<BpTreeIndexNode*> (pIndexNode->getParent());
		BpTreeIndexNode* newIndexNode_right = new BpTreeIndexNode();
		//alloc new, newIndexNode 

		map<int, BpTreeNode*>::iterator iter = dynamic_cast<BpTreeDataNode*> (pIndexNode)->getIndexMap()->begin();
		map<int, BpTreeNode*>* m_data = dynamic_cast<BpTreeDataNode*> (pIndexNode)->getIndexMap();

		iter++;
		newIndexNode_left->insertIndexMap(iter->first, newIndexNode_right);
		newIndexNode_left->setMostLeftChild(iter->second);
		iter->second->setParent(newIndexNode_right);
		iter++; //next
		newIndexNode_right->insertIndexMap(iter->first, iter->second);
		newIndexNode_right->setParent(newIndexNode_left);
		//set, parent child

		map<int, BpTreeNode*>::iterator iter_temp = dynamic_cast<BpTreeDataNode*> (newIndexNode_right)->getIndexMap()->begin();
		iter_temp->second->setParent(newIndexNode_right);
		iter = dynamic_cast<BpTreeIndexNode*> (pIndexNode)->getIndexMap()->begin();
		m_data->erase(++iter); // erase
		iter = dynamic_cast<BpTreeIndexNode*> (pIndexNode)->getIndexMap()->begin();
		m_data->erase(++iter); // erase


		if (excessIndexNode(newIndexNode_left))
			splitIndexNode(newIndexNode_left); // splite indexNode 
		else // not splite
			return;
	}
}
bool BpTree::excessDataNode(BpTreeNode* pDataNode) {
	if (pDataNode->getDataMap()->size() > order - 1) return true;//order is equal to the number of elements 
	else return false;
}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) {
	if (pIndexNode->getIndexMap()->size() > order - 1)return true;//order is equal to the number of elements 
	else return false;
}
bool BpTree::printFrequency(string item, int min_frequency)//print winratio in ascending order
{
	bool has_pattern = false;
	BpTreeNode* node = root;
	if (root == NULL) return false;
	while(!dynamic_cast<BpTreeDataNode*>(node)) node = node->getMostLeftChild();
	while(node != NULL)
	{
		map <int, FrequentPatternNode*>* mapData = node->getDataMap();;
		for (auto it = mapData->begin(); it != mapData->end(); it++)
		{
			multimap<int, set<string> > fplist = it->second->getList();
			for(auto iter = fplist.begin(); iter != fplist.end(); iter++)
			{
				if (iter->first < min_frequency)
					continue;
				set<string> fqlist = iter-> second;
				auto it = fqlist.find(item);
				if (it != fqlist.end()) {
					has_pattern = true;
					cout << '{';
					for (auto i=fqlist.begin(); i != fqlist.end();)
					{
						cout << *i;
						if (++i != fqlist.end())
							cout << ", ";
					}
					cout << "} " << iter->first << endl;
				}  	
			}
		}
		node = node->getNext();
	}
	return has_pattern;
}
int BpTree::getFreq(set<string> items) {
	int freq = 0;
	BpTreeNode* node = root;
	if (root == NULL) return false;
	while(!dynamic_cast<BpTreeDataNode*>(node)) node = node->getMostLeftChild();
	while(node != NULL)
	{
		map <int, FrequentPatternNode*>* mapData = node->getDataMap();;
		for (auto it = mapData->begin(); it != mapData->end(); it++)
		{
			multimap<int, set<string> > fplist = it->second->getList();
			for(auto iter = fplist.begin(); iter != fplist.end(); iter++)
			{
				set<string> fqlist = iter-> second;

				int item_include_count = 0;
				for(auto item : items) {
					auto it = fqlist.find(item);
					if (it != fqlist.end()) {
						item_include_count++;
					}  	
				}
				if (item_include_count == items.size()) freq++;
			}
		}
		node = node->getNext();
	}
	return freq;
}
bool BpTree::printConfidence(string item, double min_confidence)
{
	vector<pair<set<string>, int> > freq_list;
	int freq = 0;
	//Find item's frequency
	BpTreeNode* node = root;
	if (root == NULL) return false;
	while(!dynamic_cast<BpTreeDataNode*>(node)) node = node->getMostLeftChild();
	while(node != NULL)
	{
		map <int, FrequentPatternNode*>* mapData = node->getDataMap();;
		for (auto it = mapData->begin(); it != mapData->end(); it++)
		{
			multimap<int, set<string> > fplist = it->second->getList();
			for(auto iter = fplist.begin(); iter != fplist.end(); iter++)
			{
				set<string> fqlist = iter-> second;
				auto it = fqlist.find(item);
				if (it != fqlist.end()) {
					freq++;
					freq_list.push_back(pair<set<string>, int>(fqlist, getFreq(fqlist)));
				}  	
			}
		}
		node = node->getNext();
	}
	double threshold = min_confidence * freq;
	for(auto i : freq_list)
	{
		if (threshold < i.second)
		{
			set<string> fqlist = i.first;
			cout << '{';
			for (auto i=fqlist.begin(); i != fqlist.end();)
			{
				cout << *i;
				if (++i != fqlist.end())
					cout << ", ";
			}
			cout << "} " << i.second << ' ' << (double)i.second / freq << endl;
		}
	}
	return true;
}
bool BpTree::printRange(string item, int min, int max) {
	bool has_pattern = false;
	BpTreeNode* node = root;
	if (root == NULL) return false;
	cout << "FrequentPattern      Frequency" << endl;
	while(!dynamic_cast<BpTreeDataNode*>(node)) node = node->getMostLeftChild();
	while(node != NULL)
	{
		map <int, FrequentPatternNode*>* mapData = node->getDataMap();;
		for (auto it = mapData->begin(); it != mapData->end(); it++)
		{
			multimap<int, set<string> > fplist = it->second->getList();
			for(auto iter = fplist.begin(); iter != fplist.end(); iter++)
			{
				if (iter->first < min || iter->first > max)
					continue;
				set<string> fqlist = iter-> second;
				auto it = fqlist.find(item);
				if (it != fqlist.end()) {
					has_pattern = true;
					cout << '{';
					for (auto i=fqlist.begin(); i != fqlist.end();)
					{
						cout << *i;
						if (++i != fqlist.end())
							cout << ", ";
					}
					cout << "} " << iter->first << endl;
				}  	
			}
		}
		node = node->getNext();
	}
	return has_pattern;
}
void BpTree::printFrequentPatterns(set<string> pFrequentPattern) {
	*fout << "{";
	set<string> curPattern = pFrequentPattern;
	for (set<string>::iterator it = curPattern.begin(); it != curPattern.end();) {
		string temp = *it++;
		*fout << temp;
		if (it == curPattern.end()) {
			*fout << "} ";
			break;
		}
		*fout << ", ";
	}
}
