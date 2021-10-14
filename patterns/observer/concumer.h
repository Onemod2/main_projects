#pragma once

#include <set>

struct Listener;

struct Observer {
	void registerOne(Listener*);
	void removeOne(Listener*);
	void notifyAll();

private:
	std::set<Listener*> concumers;
};

struct Concumer : Observer {
	void setChanges();
	int get_day() const;
	int get_month() const;
	int get_year() const;

private:
	int day = 0;
	int month = 0;
	int year = 0;
};
