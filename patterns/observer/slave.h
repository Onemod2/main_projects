#pragma once

struct Concumer;

struct Listener {
	virtual void update() = 0;
};

struct User : Listener {
	User(Concumer *obs);

	void update() override;

private:
	int day;
	int month;
	int year;				

	Concumer *obs;
};
