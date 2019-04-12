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

    return largestID+1;
}


void readRentals(vector<Book *> & myBooks, vector<Person *> myCardholders)
{
  ifstream inData;
int book, card;
Person * personptr=NULL;

  inData.open("rentals.txt");
  while(inData>>book)
  {
    inData>>card;
    for(int i=0; i<myCardholders.size(); i++)
    {
      if(card==myCardholders.at(i)->getId())
      personptr=myCardholders.at(i);
    }
    for(int i=0; i<myBooks.size(); i++)
    {
      if(book==myBooks.at(i)->getId())
      myBooks.at(i)->setPersonPtr(personptr);
    }
  }
inData.close();
    return;
}

bool validCardId(vector<Person* > &cardholders, int cardId)
{
  for(int i=0; i<cardholders.size(); i++)
  {
    if(cardId==cardholders.at(i)->getId())
    return true;
  }
  return false;
}

bool validBookId(vector<Book *> &myBooks, int bookId)
{
  for(int i=0; i<myBooks.size(); i++)
  {
    if(bookId==myBooks.at(i)->getId())
    return true;
  }
  return false;
}

int findCard(vector<Person* > &cardholders, int cardId)
{
  for(int i=0; i<cardholders.size(); i++)
  {
    if(cardId==cardholders.at(i)->getId())
    {
    return i;
    }
  }
  return -1;
}

int findBook(vector<Book* > &books, int bookId)
{
  for(int i=0; i<books.size(); i++)
  {
    if(bookId==books.at(i)->getId())
    return i;
  }
  return -1;
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
}

void bookCheckout(vector<Person *> &cardholders, vector<Book *> &books)
{
  int cardId, bookId, bookIndex, cardIndex, count=0;
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
      if(cardId==cardholders.at(i)->getId() && cardholders.at(i)->isActive())
      cout<<"Cardholder: "<<cardholders.at(i)->fullName()<<endl;
      if(cardId==cardholders.at(i)->getId() && !cardholders.at(i)->isActive())
      {
      cout<<"Cardholder is inactive."<<endl;
      return;
      }
    }
  }

cout<<"Please enter the book ID: ";
cin>>bookId;

for(int i=0; i<books.size(); i++)
{
  if(bookId==books.at(i)->getId() && books.at(i)->getPersonPtr() != nullptr)
  {
  cout<<"Book already checked out.\n";
  return;
  }
  else if(bookId==books.at(i)->getId())
  {
  cout<<"Title: "<<books.at(i)->getTitle()<<endl;
  bookIndex=findBook(books, bookId);
  cardIndex=findCard(cardholders, cardId);
  personptr=cardholders.at(cardIndex);
  books.at(bookIndex)->setPersonPtr(personptr);
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
  int bookIndex, count;

  for(int i=0; i<myBooks.size(); i++)
  {
    if(!myBooks.at(i)->getPersonPtr())
    {
      cout<<endl<<"Book ID: "<<myBooks.at(i)->getId()<<endl;
      cout<<"Title: "<<myBooks.at(i)->getTitle()<<endl;
      cout<<"Author: "<<myBooks.at(i)->getAuthor()<<endl;
      cout<<"Category: "<<myBooks.at(i)->getCategory()<<endl<<endl;
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
  int bookId, bookIndex;

  cout<<"\nPlease enter the book ID to return: ";
  cin>>bookId;

  if(validBookId(myBooks, bookId))
  {
    bookIndex=findBook(myBooks, bookId);
    if(!myBooks.at(bookIndex)->getPersonPtr())
    cout<<"Book has not been checked out yet.\n";
    else
    {
    myBooks.at(bookIndex)->setPersonPtr(personptr);
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
  int bookId, bookIndex;

  cout<<"\nBook ID: ";
  cin>>bookId;
  bookIndex=findBook(myBooks, bookId);
  if(validBookId(myBooks, bookId) && myBooks[bookIndex]->getPersonPtr())
  {
    cout<<"Title: "<<myBooks.at(bookIndex)->getTitle()<<endl;
    cout<<"Author: "<<myBooks.at(bookIndex)->getAuthor()<<endl;
    cout<<"Cardholder: "<<myBooks.at(bookIndex)->getPersonPtr()->getId()<<endl;
  }
  else
  {
    cout<<"No outstanding rentals.\n";
  }
}

void viewCardholderRentals(vector<Person* > &myCardholders, vector<Book *> &myBooks)
{
  int cardId;
  int cardIndex;
  int count=0;

  cout<<"Please enter the card ID: ";
  cin>>cardId;
  cardIndex=findCard(myCardholders, cardId);

  if(!validCardId(myCardholders, cardId))
  {
    cout<<"Invalid Card ID.\n";
    return;
  }

  if(myCardholders.at(cardIndex)->isActive())
  {
    cout<<"Cardholder: "<<myCardholders.at(cardIndex)->fullName()<<endl<<endl;

    for(int i=0; i<myBooks.size(); i++)
    {
    Book * bookptr; //I have a pointer that will point to a book object.
    bookptr=myBooks.at(i); //The book pointer points to the same book object that the pointer at i points to
    Person * personptr; //I have a pointer that will point to a person object.
    personptr=(*bookptr).getPersonPtr(); //I am dereferencing bookptr so I can have access to the object directly and all of its member functions
      if(myBooks.at(i)->getPersonPtr()!=nullptr && myBooks.at(i)->getPersonPtr()->getId()==cardId)
      {   //If the personptr is not nullptr then the person has a book checked out
        cout<<"Book ID: "<<myBooks.at(i)->getId()<<endl;
        cout<<"Title: "<<myBooks.at(i)->getTitle()<<endl;
        cout<<"Author: "<<myBooks.at(i)->getAuthor()<<endl<<endl;
      }
      else
      {
        count++;
      }
    }
    if(count==myBooks.size())
    cout<<"No outstanding rentals.\n";
  }
  else
  {
    cout<<"Card ID is inactive."<<endl;
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
    cout<<myCardholders.at(cardNo)->fullName()<<endl;
    if(myCardholders.at(cardNo)->isActive())
    {
      cout<<"Are you sure you want to deactivate card (y/n)?"<<endl;
      cin>>input;
      if(input=="y")
      {
        myCardholders.at(cardNo)->setActive(0);
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
  for(int i=0; i<cardholders.size(); i++)
  {
    outData<<cardholders.at(i)->getId()<<" "<<cardholders.at(i)->isActive()<<" "<<cardholders.at(i)->fullName()<<endl;
  }
  outData.close();

outData.open("rentals.txt");
for(int i=0; i<books.size(); i++)
{
  if(books.at(i)->getPersonPtr())
  {
    outData<<books.at(i)->getId()<<" "<<books.at(i)->getPersonPtr()->getId()<<endl;
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

//Handling memory cleanup
for(int i=0; i<books.size()-1; i++)
{
  delete books.at(i);
  books.at(i)=NULL;
}
for(int i=0; i<cardholders.size()-1; i++)
{
  delete cardholders.at(i);
  cardholders.at(i)=NULL;
}
      return 0;
}
