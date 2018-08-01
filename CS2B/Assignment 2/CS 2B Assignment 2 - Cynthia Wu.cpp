//=====================================================================================
// Name        : CS 2B Assignment #2 - Cynthia Wu.cpp
// Author      : Cynthia Wu
// Date        : October 20th, 2017
// Version     :
// Copyright   : Copyright (C). CS Department Foothill College 2017. All rights reserved.
// Program purpose:
// Revisions history: (date/programmer/what changed)
// 		10.20.17 - Declare all InventorySystem, InventoryItem, and Product classes
//		10.21.17 - Define all member data and methods for above classes
//		10.22.17 - Implement Extra Credit
//=====================================================================================
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

typedef enum { pcNew, pcUsed, pcDefective, pcRefurbished} T_ProductCondition;

class InventoryItem {
	protected:
		string name_;
		int quantity_;

	public:
		InventoryItem();
		InventoryItem(string name, int quantity);
		virtual ~InventoryItem();
		virtual void Display() const;
		string get_name() const;
		void set_name(string name);
		int get_quantity() const;
		void set_quantity(int quantity);
};

class Product: public InventoryItem {
	private:
		int product_id_;
		double price_;
		T_ProductCondition condition_;

	public:
		Product();
		Product(string name, int quantity, double price, T_ProductCondition condition);
		virtual ~Product();
		int get_product_id() const;
		void set_product_id(int product_id);
		double get_price() const;
		void set_price(double price);
		T_ProductCondition get_condition() const;
		void set_condition(T_ProductCondition condition);
		virtual void Display() const;
};

class InventorySystem {
	private:
		string store_name_;
		int store_id_;
		InventoryItem * item_list_[512];
		int item_count_;

	public:
		InventorySystem();
		InventorySystem(string store_name, int store_id);
		~InventorySystem();
		void BuildInventory();
		void ShowInventory() const;
		void ShowDefectInventory() const;
		void Terminate() const;
		string get_store_name() const;
		void set_store_name(string store_name);
		int get_store_id() const;
		void set_store_id(int store_id);
		int get_item_count() const;
		void set_item_count(int item_count);

		//EXTRA CREDIT: Discontinue(), SearchItem()
		InventoryItem * SearchItem(int product_id);
		void Discontinue();
		void SetPointers(InventoryItem * p_item);
};

InventoryItem::InventoryItem(): name_(""), quantity_(0) {}

InventoryItem::InventoryItem(string name, int quantity): name_(name), quantity_(quantity) {}

InventoryItem::~InventoryItem() {
	cout << "InventoryItem " << name_ << " with quantity " << quantity_ << " items destroyed..." << endl << endl;
}

void InventoryItem::Display() const{
	cout  << right << setw(20) << "InventoryItem name: " << left << setw(17) << name_ << right << setw(25) << "InventoryItem Quantity: " << left << setw(15) << quantity_ << endl;
}

string InventoryItem::get_name() const {return name_; }

void InventoryItem::set_name(string name) {name_ = name; }

int InventoryItem::get_quantity() const {return quantity_; }

void InventoryItem::set_quantity(int quantity) {quantity_ = quantity; }

//TODO: product class implementation here - check if doing variable() will set it to the default value, esp condition
Product::Product(): InventoryItem(), product_id_(0), price_(0.0), condition_() {}

Product::Product(string name, int quantity, double price, T_ProductCondition condition): InventoryItem(name, quantity), product_id_(rand() % 10000), price_(price), condition_(condition) {}

Product::~Product() {
	cout << "Product " << product_id_ << " priced $" << setprecision(2) << fixed << price_ << " destroyed..." << endl;
}

int Product::get_product_id() const {return product_id_; }

void Product::set_product_id(int product_id) { product_id_ = product_id; }

double Product::get_price() const { return price_; }

void Product::set_price(double price) {price_ = price; }

T_ProductCondition Product::get_condition() const {return condition_; }

void Product::set_condition(T_ProductCondition condition) {condition_ = condition; }

void Product::Display() const{
	InventoryItem::Display();
	char translated_condition = NULL;
	switch (condition_) {
		case pcNew:
			translated_condition = 'N';
			break;
		case pcRefurbished:
			translated_condition = 'R';
			break;
		case pcUsed:
			translated_condition = 'U';
			break;
		case pcDefective:
			translated_condition = 'D';
			break;
	}
	cout << right << setw(20) << "Product ID: " << left << setw(17) << product_id_ << right << setw(25) << setprecision(2) << fixed << "Price: " << "$" << left << setw(15) << price_ << setw(10) << "Condition: " << setw(10) << translated_condition << endl << endl;
}

InventorySystem::InventorySystem(): store_name_(""), store_id_(0), item_count_(0) {
	srand(time(NULL));
	for (int i = 0; i < 512; i++) {
		item_list_[i] = NULL;
	}
};

InventorySystem::InventorySystem(string store_name, int store_id): store_name_(store_name), store_id_(store_id), item_count_(0) {
	srand(time(NULL));
	for (int i = 0; i < 512; i++) {
			item_list_[i] = NULL;
	}
};

InventorySystem::~InventorySystem() {
	for (int i = 0; i < item_count_; i++) {
		delete item_list_[i];
	}
}

void InventorySystem::BuildInventory() {
	string buffer;
	string product_name;
	int product_quantity;
	double product_price;
	T_ProductCondition product_condition;

	ifstream fin("ProductsRead.txt");

	if (!fin) {
		cout << "ERROR: failed to open input file.\n";
		exit(-1);
	}

	while (getline(fin, buffer, ';') && item_count_ < 512) {
		product_name = buffer;
		getline(fin, buffer, ';');
		product_quantity = atoi(buffer.c_str());
		getline(fin, buffer, ';');
		product_price = atof(buffer.c_str());
		getline(fin, buffer, '\n');

		// determine the condition of the product
		if (buffer == "N") {
			product_condition = pcNew;
		} else if (buffer == "R") {
			product_condition = pcRefurbished;
		} else if (buffer == "U") {
			product_condition = pcUsed;
		} else if (buffer == "D") {
			product_condition = pcDefective;
		}
		item_list_[item_count_++] = new Product(product_name, product_quantity, product_price, product_condition);
	}
	fin.close();
}

void InventorySystem::ShowInventory() const {
	Product * p_product = NULL;
	for (int i = 0; i < item_count_; i++) {
		p_product = static_cast <Product *> (item_list_[i]);
		p_product -> Display();
	}
}

void InventorySystem::ShowDefectInventory() const {
	Product * p_product = NULL;
	for (int i = 0; i < item_count_; i++) {
		p_product = static_cast <Product *> (item_list_[i]);
		if (p_product -> get_condition() == pcDefective) {
			p_product -> Display();
		}
	}
}

void InventorySystem::Terminate() const{
	string product_name;
	int product_quantity;
	double product_price;
	string product_condition;
	T_ProductCondition raw_condition;
	Product * p_product = NULL;
	ofstream fout("ProductsWrite.txt");

	if (!fout) {
		cout << "ERROR: Unable to open output file.\n";
		exit(-2);
	}

	for (int i = 0; i < item_count_; i++) {
		p_product = static_cast <Product *> (item_list_[i]);
		product_name = p_product -> get_name();
		product_price = p_product -> get_price();
		product_quantity = p_product -> get_quantity();
		raw_condition = p_product -> get_condition();
		switch (raw_condition) {
				case pcNew:
					product_condition = 'N';
					break;
				case pcRefurbished:
					product_condition = 'R';
					break;
				case pcUsed:
					product_condition = 'U';
					break;
				case pcDefective:
					product_condition = 'D';
					break;
		}
		fout << product_name << ";" << product_quantity << ";" << product_price << ";" << product_condition << endl;
	}
	fout.close();
}

string InventorySystem::get_store_name() const {return store_name_; }

void InventorySystem::set_store_name(string store_name) {store_name_ = store_name; }

int InventorySystem::get_store_id() const {return store_id_; }

void InventorySystem::set_store_id(int store_id) {store_id_ = store_id; }

int InventorySystem::get_item_count() const {return item_count_; }

void InventorySystem::set_item_count(int item_count) {item_count_ = item_count; }

InventoryItem * InventorySystem::SearchItem(int product_id) {
	Product * p_product = NULL;
	for (int i = 0; i < item_count_; i++) {
		p_product = static_cast <Product *> (item_list_[i]);
		if (p_product -> get_product_id() == product_id) {
			return item_list_[i];
		}
	}
	return NULL;
}

void InventorySystem::Discontinue() {
	string buffer;
	int product_id;
	InventoryItem * p_item = NULL;

	cout << "Please enter a product id to discontinue" << endl;
	cin >> product_id;
	p_item = SearchItem(product_id);
	if (p_item == NULL) {
		cout << "ERROR: Could not find product matching product id " << product_id << endl;
	} else {
		SetPointers(p_item);
		delete p_item;
		item_count_ -= 1;
	}
}

void InventorySystem::SetPointers(InventoryItem * p_item) {
	Product * p_product = static_cast<Product *>(p_item);
	int discontinue_index;
	for (int i = 0; i < item_count_; i++) {
		Product * p_arr_index = static_cast<Product *> (item_list_[i]);
		if (p_product -> get_product_id() == p_arr_index -> get_product_id()) {
			discontinue_index = i;
		}
	}

	for (int j = discontinue_index + 1; j <= item_count_; j++) {
		item_list_[j-1] = item_list_[j];
	}
}

int main() {
	InventorySystem * p_inventory_sys = NULL;
	int user_input = 0;
	string buffer;
	p_inventory_sys = new InventorySystem("Barry's Electronics", 102017);
	p_inventory_sys -> BuildInventory();
	p_inventory_sys -> ShowInventory();
	p_inventory_sys -> ShowDefectInventory();

	while (user_input != 1) {
		p_inventory_sys -> Discontinue();
		cout << "Enter 1 if you would like to quit, otherwise to continue discontinuing products, select 2. " << endl;
		cin >> user_input;
	}

	p_inventory_sys -> Terminate();
	delete p_inventory_sys; // this will also call InventorySytem destructor which will deallocate InventoryItem (Product) objects
}


