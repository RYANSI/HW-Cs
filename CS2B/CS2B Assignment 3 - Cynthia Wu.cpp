//=====================================================================================
// Name        : CS 2B Assignment #3 - Cynthia Wu.cpp
// Author      : Cynthia Wu
// Date        : November 04th, 2017
// Version     :
// Copyright   : Copyright (C). CS Department Foothill College 2017. All rights reserved.
// Program purpose: utilize dynamic casting, inheritance, abstract classes, operator
//					overloading, and polymorphism to create a transaction & report system
// Revisions history: (date/programmer/what changed)
//					** All changes/revisions made by Cynthia Wu **
// 		11.04.17 - Declare all Customer, Transaction and all derived classes
//		11.05.17 - Define Transaction & derived class member methods, create input file
//		11.11.17 - Implement Customer member methods & Extra Credit
//		11.12.17 - Code main() method
//=====================================================================================
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
using namespace std;

class Transaction {
	protected:
		string transaction_date_; // in format mm/dd/yy
		int transaction_id_;
		double transaction_amount_;

	public:
		Transaction();
		Transaction(string transaction_date, int transaction_id, double transaction_amount);
		virtual ~Transaction();

		//accessor and mutators
		string get_transaction_date() const;
		void set_transaction_date(string transaction_date);
		int get_transaction_id() const;
		void set_transaction_id(int transaction_id);
		double get_transaction_amount() const;
		void set_transaction_amount(double transaction_amount);

		// pure virtual functions - do not need to define
		virtual void Display() const = 0;
		virtual double EarnPoints() const = 0;

		// overloaded equality operator
		bool operator== (const Transaction &transaction) const;
};

class DepartmentStoreTransaction : public Transaction {
	private:
		string department_name_;
		int return_policy_;

	public:
		DepartmentStoreTransaction();
		DepartmentStoreTransaction(string transaction_date, int transaction_id, double transaction_amount, string department_name, int return_policy);
		virtual ~DepartmentStoreTransaction();

		// accessor + mutator methods
		string get_department_name() const;
		void set_department_name(string department_name);
		int get_return_policy() const;
		void set_return_policy(int return_policy);

		// redefined pure virtual methods
		virtual void Display() const;
		virtual double EarnPoints() const;

};

class BankingTransaction : public Transaction {
	private:
		string type_;
		double fee_charge_;

	public:
		BankingTransaction();
		BankingTransaction(string transaction_date, int transaction_id, double transaction_amount, string type, double fee_charge);
		virtual ~BankingTransaction();

		// accessor + mutator methods
		string get_type() const;
		void set_type(string type);
		double get_fee_charge() const;
		void set_fee_charge(double fee_charge);

		// redefined pure virtual methods
		virtual void Display() const;
		virtual double EarnPoints() const;
};

class GroceryTransaction : public Transaction {
	private:
		string store_name_;

	public:
		GroceryTransaction();
		GroceryTransaction(string transaction_date, int transaction_id, double transaction_amount, string store_name);
		virtual ~GroceryTransaction();

		// accessor + mutator functions
		string get_store_name() const;
		void set_store_name(string store_name);

		// redefined pure virtual methods
		virtual void Display() const;
		virtual double EarnPoints() const;

};

class Customer {
	private:
		string customer_name_;
		string credit_card_number_;
		double transaction_balance_;
		double total_rewards_;
		Transaction * transactions_[16]; // array of pointers pointing to dynamic Transaction objects

	public:
		Customer();
		Customer(string customer_name, string credit_card_number, double transaction_balance, double total_rewards);
		~Customer();

		string get_customer_name() const;
		void set_customer_name(string customer_name);
		string get_credit_card_number() const;
		void set_credit_card_number(string credit_card_number);
		double get_transaction_balance() const;
		void set_transaction_balance(double transaction_balance);
		double get_total_rewards() const;
		void set_total_rewards(double total_rewards);

		void ReadTransactions();
		void ReportAllTransactions() const;
		void ReportRewardsSummary() const;

		// EXTRA CREDIT: Create another report to show duplicate transactions
		void ReportDuplicateTransactions() const;
};

//Transaction base class methods
Transaction::Transaction(): transaction_date_(""), transaction_id_(0), transaction_amount_(0) {};
Transaction::Transaction(string transaction_date, int transaction_id, double transaction_amount):
		transaction_date_(transaction_date), transaction_id_(transaction_id), transaction_amount_(transaction_amount) {};
Transaction::~Transaction() {
	cout << "Transaction " << transaction_id_ << " destroyed..." << endl;
};
string Transaction::get_transaction_date() const { return transaction_date_; };
void Transaction::set_transaction_date(string transaction_date) { transaction_date_ = transaction_date; };
int Transaction::get_transaction_id() const { return transaction_id_; };
void Transaction::set_transaction_id(int transaction_id) { transaction_id_ = transaction_id; };
double Transaction::get_transaction_amount() const { return transaction_amount_; };
void Transaction::set_transaction_amount(double transaction_amount) {transaction_amount_ = transaction_amount; };

bool Transaction::operator==(const Transaction &transaction) const {
	int t1_day;
	int t1_month;
	int t1_year;
	int t2_day;
	int t2_month;
	int t2_year;

	sscanf(transaction.get_transaction_date().c_str(), "%2d/%2d/%2d", &t2_day, &t2_month, &t2_year);
	sscanf(get_transaction_date().c_str(), "%2d/%2d/%2d", &t1_day, &t1_month, &t1_year);
	return (get_transaction_amount() == transaction.get_transaction_amount() && t1_day == t2_day && t1_month == t2_month && t1_year == t2_year);
}

// Derived class DepartmentStoreTransaction methods
DepartmentStoreTransaction::DepartmentStoreTransaction() : Transaction(), department_name_(""), return_policy_(0) {};
DepartmentStoreTransaction::DepartmentStoreTransaction(string transaction_date, int transaction_id, double transaction_amount, string department_name, int return_policy):
		Transaction(transaction_date, transaction_id, transaction_amount), department_name_(department_name), return_policy_(return_policy) {};
DepartmentStoreTransaction::~DepartmentStoreTransaction() {
	cout << "Transaction Department Store destroyed..." << endl;
}
string DepartmentStoreTransaction::get_department_name() const { return department_name_; };
void DepartmentStoreTransaction::set_department_name(string department_name) { department_name_ = department_name; };
int DepartmentStoreTransaction::get_return_policy() const { return return_policy_; }
void DepartmentStoreTransaction::set_return_policy(int return_policy) { return_policy_ = return_policy; };
void DepartmentStoreTransaction::Display() const {
	cout << setw(15) << left << Transaction::get_transaction_date() << setw(25) << "Department Store" << setw(25) << department_name_ + ", return in " + to_string(return_policy_) + " days" << setw(15) << right << "$" << Transaction::get_transaction_amount() << endl;
}
double DepartmentStoreTransaction::EarnPoints() const { return Transaction::get_transaction_amount() * 1.5; };

// Derived class BankingTransaction methods
BankingTransaction::BankingTransaction(): Transaction(), type_(""), fee_charge_(0.00) {};
BankingTransaction::BankingTransaction(string transaction_date, int transaction_id, double transaction_amount, string type, double fee_charge):
		Transaction(transaction_date, transaction_id, transaction_amount), type_(type), fee_charge_(fee_charge) {};
BankingTransaction::~BankingTransaction() {
	cout << "Transaction banking destroyed..." << endl;
}
string BankingTransaction::get_type() const { return type_; }
void BankingTransaction::set_type(string type) { type_ = type; };
double BankingTransaction::get_fee_charge() const { return fee_charge_; };
void BankingTransaction::set_fee_charge(double fee_charge) { fee_charge_ = fee_charge; }
void BankingTransaction::Display() const {
	cout << setw(15) << left << Transaction::get_transaction_date() << setw(25) << "Banking" << setw(25) << type_ + " withdrawal" << setw(15) << right << "$" << Transaction::get_transaction_amount() << endl;
}
double BankingTransaction::EarnPoints() const { return 0; };

// Derived class GroceryTransaction methods
GroceryTransaction::GroceryTransaction(): Transaction(), store_name_("") {};
GroceryTransaction::GroceryTransaction(string transaction_date, int transaction_id, double transaction_amount, string store_name):
		Transaction(transaction_date, transaction_id, transaction_amount), store_name_(store_name) {};
GroceryTransaction::~GroceryTransaction() {
	cout << "Transaction Grocery destroyed..." << endl;
}
string GroceryTransaction::get_store_name() const { return store_name_; }
void GroceryTransaction::set_store_name(string store_name) { store_name_ = store_name; };
void GroceryTransaction::Display() const {
	cout << setw(15) << left << Transaction::get_transaction_date() << setw(25) << "Grocery" << setw(25) << store_name_ << setw(15) << right << "$" << Transaction::get_transaction_amount() << endl;
}
double GroceryTransaction::EarnPoints() const { return Transaction::get_transaction_amount() * 2.5; };

// Customer class method implementation
Customer::Customer(): customer_name_(""), credit_card_number_(""), transaction_balance_(0.00), total_rewards_(0.00) {
	for (int i = 0; i < 16; i++) {
		transactions_[i] = NULL;
	}
}

Customer::Customer(string customer_name, string credit_card_number, double transaction_balance, double total_rewards):
		customer_name_(customer_name), credit_card_number_(credit_card_number), transaction_balance_(transaction_balance), total_rewards_(total_rewards) {
	for (int i = 0; i < 16; i++) {
		transactions_[i] = NULL;
	}
}

Customer::~Customer() {
	for (int i = 0; i < 16; i++) {
		delete transactions_[i];
	}
}

string Customer::get_customer_name() const { return customer_name_; }
void Customer::set_customer_name(string customer_name) { customer_name_ = customer_name; }
string Customer::get_credit_card_number() const { return credit_card_number_; }
void Customer::set_credit_card_number(string credit_card_number) { credit_card_number_ = credit_card_number; }
double Customer::get_transaction_balance() const { return transaction_balance_; }
void Customer::set_transaction_balance(double transaction_balance) {transaction_balance_ = transaction_balance; }
double Customer::get_total_rewards() const { return total_rewards_; }
void Customer::set_total_rewards(double total_rewards) {total_rewards_ = total_rewards; }

void Customer::ReadTransactions() {
	ifstream fin("TransactionsRead.txt");
	string buffer;
	char transaction_type;
	string transaction_date;
	int transaction_id;
	double transaction_amount;
	string department_name;
	int return_policy;
	string banking_type;
	double fee_charge;
	string grocery_store_name;
	for (int i = 0; i < 16; i++ ) {
		getline(fin, buffer, '~');
		transaction_type = buffer[0];
		getline(fin, buffer, '~');
		transaction_date = buffer;
		getline(fin, buffer, '~');
		transaction_id = atoi(buffer.c_str());
		getline(fin, buffer, '~');
		transaction_amount = atof(buffer.c_str());
		transaction_balance_ += transaction_amount;

		switch (transaction_type) {
			case 'D':
				getline(fin, buffer, '~');
				department_name = buffer;
				getline(fin, buffer, '\n');
				return_policy = atoi(buffer.c_str());
				transactions_[i] = new DepartmentStoreTransaction::DepartmentStoreTransaction(transaction_date, transaction_id, transaction_amount, department_name, return_policy);
				break;

			case 'B':
				getline(fin, buffer, '~');
				banking_type = buffer;
				getline(fin, buffer, '\n');
				fee_charge = atof(buffer.c_str());
				transactions_[i] = new BankingTransaction(transaction_date, transaction_id, transaction_amount, banking_type, fee_charge);
				break;

			case 'G':
				getline(fin, buffer, '\n');
				grocery_store_name = buffer;
				transactions_[i] = new GroceryTransaction(transaction_date, transaction_id, transaction_amount, grocery_store_name);
				break;
		}
	}
}

void Customer::ReportAllTransactions() const {
	double fee_charge, department_balance, grocery_balance, banking_balance;
	int banking_count, grocery_count, department_store_count;
	DepartmentStoreTransaction * p_department = NULL;
	BankingTransaction * p_banking = NULL;
	GroceryTransaction * p_grocery = NULL;
	fee_charge = 0.00;
	department_balance = 0.00;
	grocery_balance = 0.00;
	banking_balance = 0.00;
	banking_count = 0;
	grocery_count = 0;
	department_store_count = 0;

	cout << setw(55) << right << "Transaction Listings" << endl;
	for (int i = 0; i < 16; i++) {
		transactions_[i] -> Display(); //correct method will be invoked due to polymorphism
		if ((p_department = dynamic_cast <DepartmentStoreTransaction *> (transactions_[i])) != NULL ) { //this is a department store object
			department_store_count++;
			department_balance += p_department -> Transaction::get_transaction_amount();
		} else if ((p_banking = dynamic_cast <BankingTransaction *> (transactions_[i])) != NULL) { //this is a banking object
			banking_count++;
			banking_balance += p_banking -> Transaction::get_transaction_amount();
			fee_charge += p_banking -> get_fee_charge();
		} else if ((p_grocery = dynamic_cast <GroceryTransaction *>(transactions_[i])) != NULL ){ //this is a grocery store object
			grocery_count++;
			grocery_balance += p_grocery -> Transaction::get_transaction_amount();
		}
	}
	cout << "---------------------------------------------------------------------------------------" << endl;
	cout << setw(18) << left << "Total fee charge: " << right << "$" << fee_charge << endl;
	cout << setw(18) << left << "Total Balance: " << right << "$" << department_balance + banking_balance + grocery_balance + fee_charge << endl;

	cout << setw(55) << right << "Transaction Summary" << endl;
	cout << setw(15) << "" << setw(25) << left << "Transaction Type" << setw(15) << right << "Transaction Count" << setw(25) << right << "Total Purchase" << endl;
	cout << setw(15) << "" << setw(33) << left << "Department Store" << setw(25) << department_store_count << setw(25) << department_balance << endl;
	cout << setw(15) << "" << setw(33) << left << "Banking" << setw(25) << banking_count << setw(25) << banking_balance << endl;
	cout << setw(15) << "" << setw(33) << left << "Grocery" << setw(25) << grocery_count << setw(22) << grocery_balance << endl;
	cout << setw(15) << "" << "--------------------------------------------------------------------" << endl;
	cout << setw(74) << right << "$" << department_balance + banking_balance + grocery_balance << endl << endl;
}

void Customer::ReportRewardsSummary() const {
	double department_points, banking_points, grocery_points;
	department_points = 0.00;
	banking_points = 0.00;
	grocery_points = 0.00;
	DepartmentStoreTransaction * p_department = NULL;
	BankingTransaction * p_banking = NULL;
	GroceryTransaction * p_grocery = NULL;

	for (int i = 0; i < 16; i++) {
		if ((p_department = dynamic_cast <DepartmentStoreTransaction *> (transactions_[i])) != NULL ) { // if this is a department store object
			department_points += p_department -> EarnPoints();
		} else if ((p_banking = dynamic_cast <BankingTransaction *> (transactions_[i])) != NULL) { // if this is a banking object
			banking_points += p_banking -> EarnPoints();
		} else if ((p_grocery = dynamic_cast <GroceryTransaction *>(transactions_[i])) != NULL ){ // if it is  a grocery store object
			grocery_points += p_grocery -> EarnPoints();
		}
	}
	cout << "Rewards Summary for " << customer_name_ << " " << credit_card_number_.substr(credit_card_number_.length() - 4) << endl;
	cout << setw(5) << "" << setw(35) << left << "Previous points balance" << setw(13) << right << total_rewards_ << endl;
	cout << setw(5) << "" << setw(35) << left << "+ Department Store Purchases:" << setw(13) << right << department_points << endl;
	cout << setw(5) << "" << setw(35) << left << "+ Banking Purchases:" << setw(13) << right << banking_points << endl;
	cout << setw(5) << "" << setw(35) << left << "+ Grocery Purchases:" << setw(13) << right << grocery_points << endl;
	cout << setw(5) << "" << "------------------------------------------------" << endl;
	cout << setw(5) << "" << setw(40) << left << "= Total points avaliable for redemption  " << department_points + banking_points + grocery_points + total_rewards_ << endl << endl;
}

// EXTRA CREDIT: uses overloaded operator (==) to see if transactions are duplicated
void Customer::ReportDuplicateTransactions() const {
	int duplicate_count;
	DepartmentStoreTransaction * p_department1 = NULL;
	DepartmentStoreTransaction * p_department2 = NULL;
	BankingTransaction * p_banking1 = NULL;
	BankingTransaction * p_banking2 = NULL;
	GroceryTransaction * p_grocery1 = NULL;
	GroceryTransaction * p_grocery2 = NULL;
	duplicate_count = 0;

	cout << setw(50) << right << "DUPLICATE TRANSACTION REPORT" << endl;
	for (int i = 0; i < 15; i++) {
		for (int j = i + 1; j < 16; j++) {
			if (transactions_[i] -> operator==(*transactions_[j])) { // == only checks for same date & amount, need to check for same types as well
				//check if both transaction types match
				if ((p_department1 = dynamic_cast <DepartmentStoreTransaction *> (transactions_[i])) != NULL
						&& (p_department2 = dynamic_cast<DepartmentStoreTransaction *>(transactions_[j])) != NULL) {
					if (duplicate_count == 0) {
						cout << setw(25) << left << "Transaction Type" << setw(15) << "Date" << setw(15) << "Amount" << setw(25) << "Transaction ID" << endl;
					}
					cout << setw(5) << "" << setw(20) << left << "Dept Store" << setw(15) << p_department1 -> Transaction::get_transaction_date() << setw(15) << p_department1 -> Transaction::get_transaction_amount() << setw(25) << p_department1 -> Transaction::get_transaction_id() << endl;
					cout << setw(5) << "" << setw(20) << left << "Dept Store" << setw(15) << p_department2 -> Transaction::get_transaction_date() << setw(15) << p_department2 -> Transaction::get_transaction_amount() << setw(25) << p_department2 -> Transaction::get_transaction_id() << endl;
					duplicate_count++;
					break; // at most 1 matching transaction, so can break after finding it

				} else if ((p_banking1 = dynamic_cast <BankingTransaction *> (transactions_[i])) != NULL
						&& (p_banking2 = dynamic_cast<BankingTransaction *> (transactions_[j])) != NULL) {
					if (duplicate_count == 0) {
						cout << setw(25) << left << "Transaction Type" << setw(15) << "Date" << setw(15) << "Amount" << setw(25) << "Transaction ID" << endl;
					}
					cout << setw(5) << "" << setw(20) << left << "Banking" << setw(15) << p_banking1 -> Transaction::get_transaction_date() << setw(15) << p_banking1 -> Transaction::get_transaction_amount() << setw(25) << p_banking1 -> Transaction::get_transaction_id() << endl;
					cout << setw(5) << "" << setw(20) << left << "Banking" << setw(15) << p_banking2 -> Transaction::get_transaction_date() << setw(15) << p_banking2 -> Transaction::get_transaction_amount() << setw(25) << p_banking2 -> Transaction::get_transaction_id() << endl;
					duplicate_count++;
					break;

				} else if ((p_grocery1 = dynamic_cast <GroceryTransaction *>(transactions_[i])) != NULL
						&& (p_grocery2 = dynamic_cast<GroceryTransaction *> (transactions_[j])) != NULL){
					if (duplicate_count == 0) {
						cout << setw(25) << left << "Transaction Type" << setw(15) << "Date" << setw(15) << "Amount" << setw(25) << "Transaction ID" << endl;
					}
					cout << setw(5) << "" << setw(20) << left << "Grocery" << setw(15) << p_grocery1 -> Transaction::get_transaction_date() << setw(15) << p_grocery1 -> Transaction::get_transaction_amount() << setw(25) << p_grocery1 -> Transaction::get_transaction_id() << endl;
					cout << setw(5) << "" << setw(20) << left << "Grocery" << setw(15) << p_grocery2 -> Transaction::get_transaction_date() << setw(15) << p_grocery2 -> Transaction::get_transaction_amount() << setw(25) << p_grocery2 -> Transaction::get_transaction_id() << endl;
					duplicate_count++;
					break;

				}
			}
		}
	}

	if (duplicate_count == 0) {
		cout << setw(15) << "" << "No duplicate transaction found." << endl;
	}
}

int main() {
	Transaction * p_transaction1 = NULL;
	Transaction * p_transaction2 = NULL;
	Transaction * p_transaction3 = NULL;
	bool is_same_transaction;
	Customer * p_customer = NULL;

	p_customer = new Customer("Cynthia Wu", "1111222233334444", 100.00, 250.50);
	p_customer -> ReadTransactions();
	p_customer -> ReportAllTransactions();
	p_customer -> ReportRewardsSummary();
	p_customer -> ReportDuplicateTransactions(); //EXTRA CREDIT
	delete p_customer;

	//EXTRA CREDIT: code showing overridden operator == is working
	p_transaction1 = new DepartmentStoreTransaction("11/10/17", 1234, 99.99, "Target", 30);
	p_transaction2 = new DepartmentStoreTransaction("11/10/17", 5678, 99.99, "Target", 30); //same transaction as p_transaction1
	p_transaction3 = new DepartmentStoreTransaction("11/10/17", 9101, 100.01, "Target", 30); // same transaction_date as p_transaction1 & p_transaction2 but different transaction_amount
	is_same_transaction = p_transaction1 -> operator==(*p_transaction2); // true
	is_same_transaction = p_transaction2 -> operator==(*p_transaction3); // false
	is_same_transaction = p_transaction3 -> operator==(*p_transaction1); // false
	delete p_transaction1;
	delete p_transaction2;
	delete p_transaction3;
}


