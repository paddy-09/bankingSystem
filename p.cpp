#include<iostream>
#include<fstream>
#include<map>
using namespace std;
#define MIN_BALANCE 500
class InsufficientFunds{};
class Account
{
private:
 long accountNumber;
 string firstName;
 string lastName;
 float balance;
 static long NextAccountNumber;
public:
 Account(){}
 Account(string fname,string lname,float balance)
{
 NextAccountNumber++;
 accountNumber=NextAccountNumber;
 firstName=fname;
 lastName=lname;
 this->balance=balance;
}
 long getAccNo()
 {return accountNumber;}
 string getFirstName(){return firstName;}
 string getLastName(){return lastName;}
 float getBalance(){return balance;}
 void Deposit(float amount)
 {
      balance+=amount;
 }
 void Withdraw(float amount)
 {
      if(balance-amount<MIN_BALANCE)
 throw InsufficientFunds();
 balance-=amount;
 }
 static void setLastAccountNumber(long accountNumber)
 {
      NextAccountNumber=accountNumber;
 }
 static long getLastAccountNumber()
 {
      return NextAccountNumber;

 }
 friend ofstream & operator<<(ofstream &ofs,Account &acc);
 friend ifstream & operator>>(ifstream &ifs,Account &acc);
 friend ostream & operator<<(ostream &os,Account &acc);
};
long Account::NextAccountNumber=0;

ofstream & operator<<(ofstream &ofs,Account &acc)
{
 ofs<<acc.accountNumber<<endl;
 ofs<<acc.firstName<<endl;
 ofs<<acc.lastName<<endl;
 ofs<<acc.balance<<endl;
 return ofs;
}
 
//  reading data from infile and putting it in accounts
//  This reads data from the file into the account instance.
ifstream & operator>>(ifstream &ifs,Account &acc)
{
 ifs>>acc.accountNumber;
 ifs>>acc.firstName;
 ifs>>acc.lastName;
 ifs>>acc.balance;
 return ifs;

}


ostream & operator<<(ostream &os,Account &acc)
{
 os<<"First Name:"<<acc.getFirstName()<<endl;
 os<<"Last Name:"<<acc.getLastName()<<endl;
 os<<"Account Number:"<<acc.getAccNo()<<endl;
 os<<"Balance:"<<acc.getBalance()<<endl;
 return os;
}

class Bank
{
private:
 map<long,Account> accounts;
public:
 Bank()
 {
 Account account;
 ifstream infile; //will be used to read data from the file.
 infile.open("Bank.data");
 if(!infile)
 {
 return;
 }
 while(!infile.eof())
 {
 infile>>account;
 accounts.insert(pair<long,Account>(account.getAccNo(),account));
 }
 Account::setLastAccountNumber(account.getAccNo());

 infile.close();
 }


 Account OpenAccount(string fname,string lname,float balance)
 {
      ofstream outfile; // will be used to write data to the file
 Account account(fname,lname,balance);
 accounts.insert(pair<long,Account>(account.getAccNo(),account));

 outfile.open("Bank.data", ios::trunc);

 map<long,Account>::iterator itr;
 for(itr=accounts.begin();itr!=accounts.end();itr++)
 {
 outfile<<itr->second;
 }
 outfile.close();
 return account;

 }
 Account BalanceEnquiry(long accountNumber)
 {
      map<long,Account>::iterator itr=accounts.find(accountNumber);
 return itr->second;
 }
 Account Deposit(long accountNumber,float amount)
 {
      map<long,Account>::iterator itr=accounts.find(accountNumber);
 itr->second.Deposit(amount);
 return itr->second;
 }
 Account Withdraw(long accountNumber,float amount)
 {
      map<long,Account>::iterator itr=accounts.find(accountNumber);
 itr->second.Withdraw(amount);
 return itr->second;
 }
 void CloseAccount(long accountNumber)
 {
      map<long,Account>::iterator itr=accounts.find(accountNumber);
 cout<<"Account Deleted"<<itr->second;
 accounts.erase(accountNumber);

 }
 void ShowAllAccounts()
 {
      map<long,Account>::iterator itr;
 for(itr=accounts.begin();itr!=accounts.end();itr++)
 {
 cout<<"Account "<<itr->first<<endl<<itr->second<<endl;
 }
 }
 ~Bank()
 {
      ofstream outfile;
 outfile.open("Bank.data", ios::trunc);

 map<long,Account>::iterator itr;
 for(itr=accounts.begin();itr!=accounts.end();itr++)
 {
 outfile<<itr->second;
 }
 outfile.close();
 }
};
int main()
{
 Bank b;
 Account acc;

 int choice;
 string fname,lname;
 long accountNumber;
 float balance;
 float amount;

 cout<<"**Banking System**"<<endl;
 do
 {
 cout<<"\n\tSelect one option below ";
 cout<<"\n\t1 Open an Account";
 cout<<"\n\t2 Balance Enquiry";
 cout<<"\n\t3 Deposit";
 cout<<"\n\t4 Withdrawal";
 cout<<"\n\t5 Close an Account";
 cout<<"\n\t6 Show All Accounts";
 cout<<"\n\t7 Quit";
 cout<<"\nEnter your choice: ";
 cin>>choice;
 switch(choice)
 {
 case 1:
 cout<<"Enter First Name: ";
cin>>fname;
cout<<"Enter Last Name: ";
cin>>lname;
cout<<"Enter initil Balance: ";
cin>>balance;
 acc=b.OpenAccount(fname,lname,balance);
 cout<<endl<<"Congradulation Account is Created"<<endl;
 cout<<acc;
break;
 case 2:
 cout<<"Enter Account Number:";
cin>>accountNumber;
 acc=b.BalanceEnquiry(accountNumber);
 cout<<endl<<"Your Account Details"<<endl;
 cout<<acc;
break;
 case 3:
 cout<<"Enter Account Number:";
cin>>accountNumber;
cout<<"Enter Balance:";
cin>>amount;
 acc=b.Deposit(accountNumber, amount);
 cout<<endl<<"Amount is Deposited"<<endl;
 cout<<acc;
break;
 case 4:
 cout<<"Enter Account Number:";
cin>>accountNumber;
cout<<"Enter Balance:";
cin>>amount;
 acc=b.Withdraw(accountNumber, amount);
 cout<<endl<<"Amount Withdrawn"<<endl;
 cout<<acc;
break;
 case 5:
 cout<<"Enter Account Number:";
cin>>accountNumber;
 b.CloseAccount(accountNumber);
 cout<<endl<<"Account is Closed"<<endl;
 cout<<acc;
 case 6:
 b.ShowAllAccounts();
 break;
 case 7: break;
 default:
 cout<<"\nEnter corret choice";
exit(0);
 }
 }while(choice!=7);

 return 0;
}