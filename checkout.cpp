// Shane Spangenberg
// Section #2
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "person.cpp"
#include "book.cpp"

using namespace std;

void printMenu() {
    cout << "----------Library Book Rental System----------" << endl;
    cout << "1.  Book checkout" << endl;
    cout << "2.  Book return" << endl;
    cout << "3.  View all available books" << endl;
    cout << "4.  View all outstanding rentals" << endl;
    cout << "5.  View outstanding rentals for a cardholder" << endl;
    cout << "6.  Open new library card" << endl;
    cout << "7.  Close library card" << endl;
    cout << "8.  Exit system" << endl;
    cout << "Please enter a choice: ";
}

 //You are not obligated to use these function declarations - they're just given as examples
void readBooks(vector<Book *> & myBooks) {
  int Id;
  string bookTitle, bookAuthor, bookCategory, line;
  ifstream inData;
  Book * ptr = NULL;

  inData.open("books.txt");
  while(inData>>Id)
  {
    getline(inData, line);
    getline(inData, bookTitle);
    getline(inData, bookAuthor);
    getline(inData, bookCategory);
    getline(inData, line);

    ptr = new Book(Id, bookTitle, bookAuthor, bookCategory);
    myBooks.push_back(ptr);
  }
inData.close();
cout<<myBooks.size();
//delete ptr;   Dont delete here because it will delete the last book object not the ptr!
//ptr=NULL;
    return;
}

int readPersons(vector<Person *> & myCardholders) {
  int largestID=0, cardID;
  bool active;
  string fName, lName;
  ifstream inData;
Person * ptr=NULL;

  inData.open("persons.txt");
  while(inData>>cardID)
  {
    inData>>active>>fName;
    cin.clear();
    if(largestID<cardID)
    {
      largestID=cardID;
    }
    inData>>lName;
    cin.clear();
    ptr=new Person(cardID, active, fName, lName);
    myCardholders.push_back(ptr);
  }
  inData.close();
//  cout<<myCardholders.size();
//delete ptr;
//ptr=NULL;
    return largestID+1;
}


void readRentals(vector<Book *> & myBooks, vector<Person *> myCardholders)
{
  vector<int> bookRentals;
  vector<int> cardRentals;
  ifstream inData;
int book, card;
Person * personptr=NULL;

  inData.open("rentals.txt");
  while(true)
  {
    inData>>book>>card;
    bookRentals.push_back(book);
    cardRentals.push_back(card);
    if(inData.eof()) break;
  }
inData.close();

for(int i=0; i<bookRentals.size(); i++)
{
  book=bookRentals[i];
  card=cardRentals[i];

  for(int i=0; i<myCardholders.size(); i++)
  {
    if(card==myCardholders[i]->getId())
    personptr=myCardholders[i];
  }

  for(int i=0; i<myBooks.size(); i++)
  {
    if(book==myBooks[i]->getId())
    myBooks[i]->setPersonPtr(personptr);
  }
}
//delete personptr;
//personptr=NULL;
    return;
}

bool validCardId(vector<Person* > &cardholders, int cardId)
{
  int count=0;
  for(int i=0; i<cardholders.size(); i++)
  {
    if(cardId==cardholders[i]->getId())
    return true;
    else
    count++;
  }
  if(count==cardholders.size())
  return false;
}

bool validBookId(vector<Book *> &myBooks, int bookId)
{
  int count=0;
  for(int i=0; i<myBooks.size(); i++)
  {
    if(bookId==myBooks[i]->getId())
    return true;
    else
    count++;
  }
  if(count==myBooks.size())
  return false;
}

unsigned int findCard(vector<Person* > &cardholders, int cardId)
{
  int count=0;
  for(unsigned int i=0; i<cardholders.size(); i++)
  {
    if(cardId==cardholders[i]->getId())
    {
    return i;
    }
    else
    count++;
  }
  if(count==cardholders.size())
  return (cardholders.size()+1);
}

int findBook(vector<Book* > &books, int bookId)
{
  int count=0;
  for(int i=0; i<books.size(); i++)
  {
    if(bookId==books[i]->getId())
    return i;
    else
    count++;
  }
  if(count==books.size())
  return (books.size()+1);
}

void openCard(vector<Person *> & myCardholders, int nextID) {
string fName, lName;
Person * ptr=nullptr;
    cout<<"Please enter the first name: ";
    cin>>fName;
    cin.clear();
    cout<<"Please enter the last name: ";
    cin>>lName;
    cin.clear();
    ptr=new Person(nextID, 1, fName, lName);
    myCardholders.push_back(ptr);
    cout<<"Card ID "<<nextID<<" active."<<endl;
    cout<<"Cardholder: "<<fName<<" "<<lName<<endl;
//delete ptr;
//ptr=NULL;
}
/*
Book * searchBook(vector<Book *> myBooks, int id) {

return nullptr;
}
*/

void bookCheckout(vector<Person *> &cardholders, vector<Book *> &books)
{
  int cardId, bookId, booknum, cardnum, count=0;
  Person * personptr=nullptr;

  cout<<"Please enter the card ID: ";
  cin>>cardId;

  if(!validCardId(cardholders, cardId))
  {
    cout<<"Invalid Card ID.\n";
    return;
  }
  else
  {
    for(int i=0; i<cardholders.size(); i++)
    {
      if(cardId==cardholders[i]->getId() && cardholders[i]->isActive())
      cout<<"Cardholder: "<<cardholders[i]->fullName()<<endl;
    }
  }
cout<<"Please enter the book ID: ";
cin>>bookId;
for(int i=0; i<books.size(); i++)
{
  if(bookId==books[i]->getId() && books[i]->getPersonPtr() != nullptr)
  {
  cout<<"Book already checked out.\n";
  return;
  }
  else if(bookId==books[i]->getId())
  {
  cout<<"Title: "<<books[i]->getTitle()<<endl;
  booknum=findBook(books, bookId);
  cardnum=findCard(cardholders, cardId);
  personptr=cardholders[cardnum];
  books[booknum]->setPersonPtr(personptr);
  cout<<"Book successfully checked out.\n";
  }
  else if(!validBookId(books, bookId))
  {
    count++;
  }
}
if(count==books.size())
cout<<"Invalid book ID."<<endl;
}

void viewBooks(vector<Book*> &myBooks)
{
  int booknum, count;

  for(int i=0; i<myBooks.size(); i++)
  {
    if(!myBooks[i]->getPersonPtr())
    {
      cout<<endl<<"Book ID: "<<myBooks[i]->getId()<<endl;
      cout<<"Title: "<<myBooks[i]->getTitle()<<endl;
      cout<<"Author: "<<myBooks[i]->getAuthor()<<endl;
      cout<<"Category: "<<myBooks[i]->getCategory()<<endl<<endl;
    }
    else
    {
      count++;
    }
  }
  if(count==myBooks.size())
  cout<<"No available books.\n";
}

void bookReturn(vector<Book* > &myBooks)
{
  Person * personptr=nullptr;
  int bookId, booknum;
  cout<<"\nPlease enter the book ID to return: ";
  cin>>bookId;
  if(validBookId(myBooks, bookId))
  {
    booknum=findBook(myBooks, bookId);
    if(!myBooks[booknum]->getPersonPtr())
    cout<<"Book has not been checked out yet.\n";
    else
    {
    myBooks[booknum]->setPersonPtr(personptr);
    cout<<"Return Completed.\n";
    return;
    }
  }
  else if(!validBookId(myBooks, bookId))
  {
    cout<<"Book ID not found.\n";
    return;
  }
}

void viewRentals(vector<Person *> &myCardholders, vector<Book *> &myBooks)
{
  int bookId, booknum;

  cout<<"\nBook ID: ";
  cin>>bookId;
  booknum=findBook(myBooks, bookId);
  if(validBookId(myBooks, bookId) && myBooks[booknum]->getPersonPtr())
  {
    cout<<"Title: "<<myBooks[booknum]->getTitle()<<endl;
    cout<<"Author: "<<myBooks[booknum]->getAuthor()<<endl;
    cout<<"Cardholder: "<<myBooks[booknum]->getPersonPtr()->getId()<<endl;
  }
  else
  {
    cout<<"No outstanding rentals.\n";
  }
}

void viewCardholderRentals(vector<Person* > &myCardholders, vector<Book *> &myBooks)
{
  int cardId;
  unsigned int cardnum;


  cout<<"Please enter the card ID: ";
  cin>>cardId;
  cardnum=findCard(myCardholders, cardId);

  if(!validCardId(myCardholders, cardId))
  {
    cout<<"Invalid Card ID.\n";
    return;
  }
  else if(myCardholders[cardnum]->isActive())
  {
    cout<<"Cardholder: "<<myCardholders.at(cardnum)->fullName()<<"hello"<<endl<<endl;
cout<<(*myBooks[3]).getTitle();
/*
    for(unsigned int i=0; i<myBooks.size(); i++)
    {
      Book * bookptr;  //I have a pointer that will point to a book object.
      bookptr=myBooks.at(i);  //The book pointer points to the same book object that the pointer at i points to
      Person * personptr; //I have a pointer that will point to a person object.
      personptr=(*bookptr).getPersonPtr();  //I am dereferencing bookptr so I can have access to the object directly and all of its member functions
      cout<<"Card ID: "<<(*personptr).getId();  //I am dereferencing person ptr so I can access the get id function

      if(myBooks[i]->getPersonPtr()->getId()==cardId)
      {
        cout<<"Book ID: "<<myBooks[i]->getId()<<endl;
        cout<<"Title: "<<myBooks[i]->getTitle()<<endl;
        cout<<"Author: "<<myBooks[i]->getAuthor()<<endl<<endl;

      }


    }
*/
  }
  else
  {
    cout<<"No books currently checked out."<<endl;
  }
}

void closeCard(vector<Person *> &myCardholders)
{
  int cardId, cardNo;
  string input;
  cout<<"Please enter the card ID: ";
  cin>>cardId;
  if(validCardId(myCardholders, cardId))
  {
    cardNo=findCard(myCardholders, cardId);
    cout<<myCardholders[cardNo]->fullName()<<endl;
    if(myCardholders[cardNo]->isActive())
    {
      cout<<"Are you sure you want to deactivate card (y/n)?"<<endl;
      cin>>input;
      if(input=="y")
      {
        myCardholders[cardNo]->setActive(0);
        cout<<"Card ID deactivated. "<<endl;
      }
      else if(input=="n")
      {
        return;
      }
      else
      {
        cout<<"invalid input (must be 'y' or 'n').\n";
      }
    }
    else
    {
      cout<<"Card ID is already inactive.\n";
    }
  }
  else
  {
    cout<<"Card ID not found.\n";
  }
}

void writeData(vector<Book *> &books, vector<Person *> &cardholders)
{
  ofstream outData;

  outData.open("persons.txt");
  for(int i=0; i<cardholders.size()-1; i++)
  {
    outData<<cardholders[i]->getId()<<" "<<cardholders[i]->isActive()<<" "<<cardholders[i]->fullName()<<endl;
  }
  outData.close();

outData.open("rentals.txt");
for(int i=0; i<books.size(); i++)
{
  if(books[i]->getPersonPtr())
  {
    outData<<books[i]->getId()<<" "<<books[i]->getPersonPtr()->getId()<<endl;
  }
}
outData.close();

}

int main()
{
    vector<Book *> books;
    vector<Person *> cardholders;

int largestID;
readBooks(books);
largestID=readPersons(cardholders);
readRentals(books, cardholders);
    int choice;
    do
    {
        // If you use cin anywhere, don't forget that you have to handle the <ENTER> key that
        // the user pressed when entering a menu option. This is still in the input stream.
        printMenu();
        cin >> choice;
        switch(choice)
        {
            case 1:
            bookCheckout(cardholders, books);
                // Book checkout
                break;

            case 2:
            bookReturn(books);
                // Book return
                break;

            case 3:
                // View all available books
                viewBooks(books);
                break;

            case 4:
                // View all outstanding rentals
                viewRentals(cardholders, books);
                break;

            case 5:
                // View outstanding rentals for a cardholder
                viewCardholderRentals(cardholders, books);
                break;

            case 6:
                // Open new library card
                openCard(cardholders, largestID);
                break;

            case 7:
                // Close library card
                closeCard(cardholders);
                break;

            case 8:
                // Must update records in files here before exiting the program
                writeData(books, cardholders);
                break;

            default:
                cout << "Invalid entry" << endl;
                break;
        }
        cout << endl;
   } while(choice != 8);


for(int i=0; i<books.size()-1; i++)
{
  delete books[i];
  books[i]=NULL;
}
for(int i=0; i<cardholders.size()-1; i++)
{
  delete cardholders[i];
  cardholders[i]=NULL;
}

      return 0;
}
