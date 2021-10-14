#include <iostream>

#include "slave.h"
#include "concumer.h"

void User::update() {
		day = obs->get_day();	
		month = obs->get_month();	
		year = obs->get_year();	
	
		std::cout << "I'm update :)\n";
		std::cout << day << ' ' << month << ' ' << year << std::endl;
}	

User::User(Concumer *obs) : obs(obs) {
	obs->registerOne(this);
}
