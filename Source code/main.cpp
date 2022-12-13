#include "Manager.h"
#include <iomanip>

int main()
{
	freopen("log.txt","w",stdout);
	Manager ds;
	ds.run("command.txt");

	return 0;
}
