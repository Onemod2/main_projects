#include <iostream>
#include <cstdlib>
#include <ctime>
#include <set>

#include "concumer.h"
#include "slave.h"



int main() {
	srand(time(0));

	Concumer obs;

	User u(&obs), u2(&obs);
	obs.setChanges();

	return 0;
}
