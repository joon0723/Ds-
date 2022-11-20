#pragma once
#include "FPGrowth.h"
#include "BpTree.h"

class Manager
{
private:
	char* cmd;
	FPGrowth* fpgrowth;
	BpTree* bptree;
	int threshold;
	int bpOrder;
public:
	Manager(int threshold, int bpOrder)	//constructor
	{
		/* You must fill here */
		cmd = (char *) malloc(100);
		this->threshold = threshold;
		this->bpOrder = bpOrder;

		FPGrowth* newfpgrowth = new FPGrowth(&flog, threshold);
		BpTree* newbptree = new BpTree(&flog, bpOrder);
		fpgrowth = newfpgrowth;
		bptree = newbptree;
	}


	~Manager()//destructor
	{
		/* You must fill here */
	}

	ifstream fin;
	ofstream flog;
	

	void run(const char* command);
	bool SAVE();
	bool LOAD();
	bool BTLOAD();
	
	bool PRINT_ITEMLIST();
	bool PRINT_FPTREE();

	bool PRINT_RANGE(char* item, int start, int end);
	bool PRINT_CONFIDENCE(char* item, double rate);
	bool PRINT_BPTREE(char* item, int min_frequency);


	void printErrorCode(int n);
	void printSuccessCode();

};

