//begin person.h
#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>
using namespace std;

class Person
{
private:
    string firstName;
    string lastName;
    int    cardID;
    bool   active;

public:
    Person (int cardNo, bool act, string fName, string lName);
    string getFirstName();
    string getLastName();
    int    getId();
	void   setActive(bool act);
	bool   isActive();
    string fullName();
};
#endif // end person.h
