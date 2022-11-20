#include "Manager.h"
#include <string>

void Manager::run(const char* command)
{
	fin.open(command);
	flog.open("log.txt", ios::app); //log.txt open
	if(!fin)
	{
		flog << "File Open Error" << endl;
		cout << "File Open Error" << endl;
		return;
	}
	while (!fin.eof())
	{
		fin.getline(cmd, 64);
		char* tmp;
		tmp = strtok(cmd, " "); //string truncation

		// Load
		if (strcmp(tmp, "LOAD") == 0)
		{
			flog << "========" << cmd << "========" << endl;
			cout << "========" << cmd << "========" << endl;
			if (LOAD()) {
				printSuccessCode();
			}
			else {
				printErrorCode(100);
			}
			cout << "============================" << endl;
			flog << "============================" << endl;
		}

		// BTLOAD
		else if (strcmp(tmp, "BTLOAD") == 0)
		{
			flog << "========" << cmd << "========" << endl;
			cout << "========" << cmd << "========" << endl;
			if (BTLOAD()) {
				printSuccessCode();
			}
			else {
				printErrorCode(200);
			}
			cout << "============================" << endl;
			flog << "============================" << endl;
		}

		//PRINT_ITEMLIST
		else if (strcmp(tmp, "PRINT_ITEMLIST") == 0)
		{
			flog << "========" << cmd << "========" << endl;
			cout << "========" << cmd << "========" << endl;
			int start = 0, end = 0;
			if (!PRINT_ITEMLIST())
			{
				printErrorCode(300);
			}
			cout << "============================" << endl;
			flog << "============================" << endl;
		}

		//PRINT_FPTREE
		else if (strcmp(tmp, "PRINT_FPTREE") == 0)
		{
			flog << "========" << cmd << "========" << endl;
			cout << "========" << cmd << "========" << endl;
			if (!PRINT_FPTREE())
			{
				printErrorCode(400);
			}
			cout << "============================" << endl;
			flog << "============================" << endl;
		}

		//PRINT_BPTREE
		else if (strcmp(tmp, "PRINT_BPTREE") == 0)
		{
			flog << "========" << cmd << "========" << endl;
			cout << "========" << cmd << "========" << endl;
			auto item = strtok(NULL, " ");
			auto min_str = strtok(NULL, " ");
			bool error = false;
			if (!(item && min_str))
				error = true;
			auto min = atoi(min_str);

			if (error || !PRINT_BPTREE(item, min))
			{
				printErrorCode(500);
			}
			cout << "============================" << endl;
			flog << "============================" << endl;
		}

		//PRINT_CONFIDENCE
		else if (strcmp(tmp, "PRINT_CONFIDENCE") == 0)
		{
			flog << "========" << cmd << "========" << endl;
			cout << "========" << cmd << "========" << endl;
			auto item = strtok(NULL, " ");
			auto conf_str = strtok(NULL, " ");
			bool error = false;
			if (!(item && conf_str))
			{
				error = true;
			}
			auto conf = atof(conf_str);

			if (error || !PRINT_CONFIDENCE(item, conf))
			{
				printErrorCode(600);
			}
			cout << "============================" << endl;
			flog << "============================" << endl;
		}

		//PRINT_RANGE
		else if (strcmp(tmp, "PRINT_RANGE") == 0)
		{
			flog << "========" << cmd << "========" << endl;
			cout << "========" << cmd << "========" << endl;
			auto item = strtok(NULL, " ");
			auto min_str = strtok(NULL, " ");
			auto max_str = strtok(NULL, " ");
			bool error = false;
			if (!(item && min_str && max_str))
			{
				error = true;
			}
			auto min = atol(min_str);
			auto max = atol(max_str);

			if (!PRINT_RANGE(item, min, max))
			{
				printErrorCode(700);
			}
			cout << "============================" << endl;
			flog << "============================" << endl;
		}

		//SAVE
		else if (strcmp(tmp, "SAVE") == 0)
		{
			flog << "========" << cmd << "========" << endl;
			if (SAVE()) {
				printSuccessCode();
			}
			else 
			{
				printErrorCode(800);
			}
			flog << "============================" << endl;
		}

		//EXIT
		else if (strcmp(tmp, "EXIT") == 0)
		{
			flog << "========" << cmd << "========" << endl;
			cout << "========" << cmd << "========" << endl;
			printSuccessCode();
			cout << "============================" << endl;
			flog << "============================" << endl;
			break;
		}
		else {
			cout << "error command" << endl; //command error
		}
	}
	fin.close();
	return;
}

bool Manager::LOAD()
{
	ifstream fmarket;
	fmarket.open("market.txt");
	if (!fmarket)
	{
		flog << "File Open Error" << endl;
		cout << "File Open Error" << endl;
		return false;
	}

	//already exist
	if (fpgrowth->getHeaderTable()->getindexTable().size()) {
		return false;
	}

	while (!fmarket.eof()) {
		char items[256]; //on line read
		fmarket.getline(items, 256); //on line read
		char* item;
		item = strtok(items, "\t");
		while (item != NULL) {
			fpgrowth->createTable(item, 1);
			item = strtok(NULL, "\t"); //string truncation
		}
	}
	fmarket.clear();
	fmarket.seekg(0);

	//Sort Descending
	fpgrowth->getHeaderTable()->descendingIndexTable();

	//create data table
	fpgrowth->getHeaderTable()->insertTable_map(threshold);

	while (!fmarket.eof()) {
		char items[256];
		fmarket.getline(items, 256);
		char* item;
		list<string>* item_array = new list<string>;
		item = strtok(items, "\t");

		while (item != NULL) {
			int n = 0;
			n = fpgrowth->getHeaderTable()->find_frequency(item);
			if (n >= threshold) {
				int check = 0;
				list<string>::iterator iter = item_array->begin(); //iterator
				for (iter = (*item_array).begin(); iter != (*item_array).end(); iter++) {
					if (n > fpgrowth->getHeaderTable()->find_frequency(*iter)) {
						item_array->insert(iter, item); //insert, sort descending
						check = 1;
						break;
					}
				}
				if (check == 0) //final
					item_array->push_back(item);
			}
			item = strtok(NULL, "\t"); //tok
		}
		//create FPtree
		fpgrowth->createFPtree(fpgrowth->getTree(), fpgrowth->getHeaderTable(), *item_array, 1);
	}
	return true;
}




bool Manager::BTLOAD()
{
	ifstream fresult;
	fresult.open("result.txt");
	//file open error
	if (!fresult)
	{
		flog << "File Open Error" << endl;
		cout << "File Open Error" << endl;
		return false;
	}

	//already exist
	while (!fresult.eof()) {
		char items[256]; //one line read
		fresult.getline(items, 256);
		char* fre;
		int fre_i;
		char* item;
		fre = strtok(items, "\t");
		fre_i = atoi(fre);
		set<string> s;
		while (1) {
			item = strtok(NULL, "\t");
			if (item == NULL)
				break;
			s.insert(item);
		}
		bptree->Insert(fre_i, s);
	}
	return true;
}

bool Manager::PRINT_ITEMLIST() {
	if (fpgrowth->getHeaderTable() == NULL)
		return false;

	cout << "Item\tFrequency" << endl;

	list<pair<int, string>> li = fpgrowth->getHeaderTable()->getindexTable(); //list
	for (auto it = li.begin(); it != li.end(); it++) {
		cout << (*it).second << " " << (*it).first << endl; // print item
		flog << (*it).second << " " << (*it).first << endl; // print item
	}

	return true;
}

bool Manager::PRINT_FPTREE() {
	if (fpgrowth->getHeaderTable() == NULL)
		return false;

	cout << "{StandardItem,Frequency} {Path_Item,Frequency}" << endl;

	//Sort ascending
	fpgrowth->getHeaderTable()->ascendingIndexTable();

	//list
	list<pair<int, string>> li = fpgrowth->getHeaderTable()->getindexTable();
	for (auto it = li.begin(); it != li.end(); it++) {
		if ((*it).first >= threshold) {
			map<string, FPNode*> iter = fpgrowth->getHeaderTable()->getdataTable();
			auto it_print = iter.find((*it).second); //find item
			FPNode* temp_list = new FPNode;
			FPNode* temp_parent = new FPNode; //alloc
			temp_list = (*it_print).second;
			cout << "{" << (*it_print).first << "," << fpgrowth->getHeaderTable()->find_frequency((*it_print).first) << "}" << endl;
			flog << "{" << (*it_print).first << "," << fpgrowth->getHeaderTable()->find_frequency((*it_print).first) << "}" << endl;

			while (temp_list != NULL) { //print linked item
				temp_parent = temp_list;
				while (temp_parent->getParent() != NULL) { //print linked parent
					cout << "(" << temp_parent->getItem() << "," << temp_parent->getFrequency() << ") ";
					flog << "(" << temp_parent->getItem() << "," << temp_parent->getFrequency() << ") ";
					temp_parent = temp_parent->getParent();
				}
				temp_list = temp_list->getNext();
				cout << endl;
				flog << endl;
			}
			cout << endl;
		}
	}
	return true;
}

bool Manager::PRINT_BPTREE(char* item, int min_frequency) {
	return bptree -> printFrequency(item, min_frequency);
}

bool Manager::PRINT_CONFIDENCE(char* item, double rate) {
	return bptree -> printConfidence(item, rate);
}

bool Manager::PRINT_RANGE(char* item, int start, int end) {
	return bptree -> printRange(item, start, end);
}

//ERROR CODE PRINT
void Manager::printErrorCode(int n) {
	flog << "ERROR " << n << endl;
	flog << "=======================" << endl << endl;
	cout << "ERROR " << n << endl;
	cout << "=======================" << endl << endl;
}

//SUCCESS CODE PRINT 
void Manager::printSuccessCode() {
	flog << "Success" << endl;
	cout << "Success" << endl;
}

bool Manager::SAVE() {

}
