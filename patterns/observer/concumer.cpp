#include "concumer.h"
#include "slave.h"

#include <cstdlib>

void Observer::registerOne(Listener *current) {
	concumers.insert(current);	
}

void Observer::removeOne(Listener *current) {
	concumers.erase(current);
}

void Observer::notifyAll() {
	for (const auto& concumer_ptr : concumers) {
		concumer_ptr->update();
	}
} 

void Concumer::setChanges() {	
	day = rand() % 30;
	month = rand() % 12;
	year = rand() % 2021;

	notifyAll();
}

int Concumer::get_day() const {
	return day;
}

int Concumer::get_month() const {
	return month;
}

int Concumer::get_year() const {
	return year;
}


