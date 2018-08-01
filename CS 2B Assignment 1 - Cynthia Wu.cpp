//=====================================================================================
// Name        : CS 2B Assignment#1 - Cynthia Wu.cpp
// Author      : Cynthia Wu
// Date        : September 30th, 2017
// Version     :
// Copyright   : Copyright (C). CS Department Foothill College 2017. All rights reserved.
// Program purpose: Simulate basic functionalities of a movie theatre by demonstrating the ability to loop through an array using pointers
// Revisions history: (date/programmer/what changed)
//					** All changes/revisions made by Cynthia Wu **
// 		09.30.17 - Declared all necessary variables/functions for Movie & MovieTicketMaster Class
//		10.01.17 - Define variables/functions for above classes, code int main()
//		10.02.17 - Refactor code to work with cin, getline(), and cin.ignore()
//				 - Add extra endl to make it look better on console
//				 - Refactor code so arrays are declared in main() and included as parameters in Init()
//		10.03.17 - Declare/Set g_kSize at top, remove from main()
//		10.04.17 - Create array of pointers, and code SelectionSort(), SetPointers()
//		10.05.17 - Modify ViewMovies() to work with array of pointers
//		10.07.17 - Implement BinarySearch()
//=====================================================================================

#include <iostream>
#include <iomanip>
#include <iterator>
#include <string>
using namespace std;

const int g_kSize = 8;

//Movie class members and methods declaration
class Movie {
	private:
		string movie_name_;
		int seats_avaliable_;
		double ticket_price_;

	public:
		Movie();
		Movie(string movie_name, int seats_avaliable, double ticket_prices);
		~Movie();
		string get_movie_name () const;
		int get_number_of_seats() const;
		double get_ticket_price() const;
		void set_movie_name(string movie_name);
		void set_number_of_seats(int new_capacity);
		void set_ticket_price(double ticket_price);
		double PurchaseTicket(int number_of_tickets);
		void Display() const;
};

//MovieTicketMaster class members and methods declaration
class MovieTicketMaster {
	private:
		const string theatre_name_;
		const string theatre_location_;
		Movie * list_of_movies_[g_kSize];

	public:
		MovieTicketMaster();
		MovieTicketMaster(string theatre_name, string theatre_location);
		~MovieTicketMaster();
		void Init();
		void Run();
		void Menu() const;
		void ViewMovies();
		void SearchMovie();
		Movie * FindMovie(string movie_name);
		void PurchaseTicket();
};

//Global function declarations
Movie * BinarySearch(Movie * movie_list[], int start, int end, string movie_name);
void SelectionSort(Movie * movie_list[]);
void SetPointer(Movie * &p_swap_a, Movie * &p_swap_b);

// Define Movie class constructor (using initializer list syntax) and destructor
Movie::Movie() : movie_name_(""), seats_avaliable_(0), ticket_price_(0.00) {};
Movie::Movie(string movie_name, int seats_avaliable, double ticket_price) : movie_name_(movie_name), seats_avaliable_(seats_avaliable), ticket_price_(ticket_price) {};
Movie::~Movie(){
	cout << "Movie '" << movie_name_ << "' object is no longer showing..." << endl;
}

//Movie class accessors and mutator methods
string Movie::get_movie_name() const {return movie_name_;}

int Movie::get_number_of_seats() const {return seats_avaliable_;}

double Movie::get_ticket_price() const {return ticket_price_;}

void Movie::set_movie_name(string movie_name) {movie_name_ = movie_name;}

void Movie::set_number_of_seats(int new_capacity) {seats_avaliable_ = new_capacity;}

void Movie::set_ticket_price(double ticket_price) {ticket_price_ = ticket_price;}

//Purchases a given amount of tickets based on input and deducts it from the total amount for the movie
double Movie::PurchaseTicket(int number_of_tickets) {
	double total_price;
	int avaliable_seats = get_number_of_seats();
	if (number_of_tickets > avaliable_seats) {
		return -1;
	} else {
		total_price = number_of_tickets * ticket_price_ ;
		set_number_of_seats(avaliable_seats-number_of_tickets);
		return total_price;
	}
}

//Displays the movie name, avaliable seats, and ticket price for a movie
void Movie::Display() const {
	cout << "Movie Name: " << setw(20) << left << movie_name_ << setw(10) << "Avaliable Seat(s): " << setw(10) << seats_avaliable_ << setw(10) << "Ticket Price: $" << setw(10) << ticket_price_ << endl << endl;
}

//MovieTicketMaster constructor and deconstructors
MovieTicketMaster::MovieTicketMaster() : theatre_name_(""), theatre_location_("") {};

MovieTicketMaster::MovieTicketMaster(string theatre_name, string theatre_location) : theatre_name_(theatre_name), theatre_location_(theatre_location) {
	//Extra Credit: Set array of pointers (list_of_movies_) to null
	 Movie ** p_p_movie = NULL;
	 for (p_p_movie = &list_of_movies_[0]; p_p_movie < &list_of_movies_[g_kSize]; p_p_movie++) {
	 	(*p_p_movie) = NULL;
	 }
};

MovieTicketMaster::~MovieTicketMaster(){
	// Extra Credit: deallocate all movie object from memory without using array syntax.
	cout << "Theatre " << theatre_name_<< " at " << theatre_location_ << " has been closed..." << endl;
	 Movie ** p_p_movie = NULL;
	 for (p_p_movie = &list_of_movies_[0]; p_p_movie < &list_of_movies_[g_kSize]; p_p_movie++) {
	 	delete (*p_p_movie);
	 }
}

void MovieTicketMaster::Init() {
	//Extra Credit: assign each pointer to a movie object & dynamically allocate memory for them
	list_of_movies_[0] = new Movie("Camp Rock", 100, 10.50);
	list_of_movies_[1] = new Movie("It", 110, 11.00);
	list_of_movies_[2] = new Movie("Flatliners", 120, 12.00);
	list_of_movies_[3] = new Movie("Blade Runner 2049", 130, 12.50);
	list_of_movies_[4] = new Movie("The Foreigner", 140, 10.00);
	list_of_movies_[5] = new Movie("The Stray", 150, 11.00);
	list_of_movies_[6] = new Movie("American Assassin", 100, 11.99);
	list_of_movies_[7] = new Movie("Toy Story 3", 100, 10.00);

	//Call SelectionSort to sort array based on movie_name
	::SelectionSort(list_of_movies_);
}

void MovieTicketMaster::Run() {
	int user_selection;
	user_selection = 0;
	do {
		Menu();
		cin >> user_selection;
		switch (user_selection) {
			case 1:
				ViewMovies();
				break;

			case 2:
				SearchMovie();
				break;

			case 3:
				PurchaseTicket();
				break;

			case 4:
				// user selected 'Quit', stop running.
				return;

			default:
				cout << "You selected an invalid option. Please select a number between 1-4 from the list." << endl;
				user_selection = 0;
				break;
		}
	}
	while (user_selection != 0);
}

//Displays a list of options a user can choose from
void MovieTicketMaster::Menu() const{
	cout << "MOVIE TICKET MASTER" << endl;
	cout << "Theatre: " << theatre_name_ << " at " << theatre_location_ << endl;
	cout << "Come visit " << theatre_name_ << " today! Show this ad and recieve a buy one get one free deal on our award-winning buttered popcorn!" << endl << endl;
	cout << "1. View All Movies" << endl;
	cout << "2. Search A Movie" << endl;
	cout << "3. Purchase A Ticket" << endl;
	cout << "4. Quit" << endl;
}

//View all movie information
void MovieTicketMaster::ViewMovies() {
	//EXTRA CREDIT: modified ViewMovies() to iterate each Movie object without using array syntax
	 Movie ** p_p_movie = NULL;
	 for (p_p_movie = &list_of_movies_[0]; p_p_movie < &list_of_movies_[g_kSize]; p_p_movie++) {
		(*p_p_movie)->Display();
	 }
}

//Search for a particular movie based on user input
void MovieTicketMaster::SearchMovie() {
	string movie_name;
	Movie * p_movie = NULL;
	cout << "Please enter a movie name." << endl;
	cin.ignore();
	getline(cin, movie_name);
	p_movie = FindMovie(movie_name);
	if (p_movie != NULL) {
		p_movie -> Display();
	} else {
		cout << "Error, we do not have the movie " << movie_name << ". Please try again." << endl << endl;
	}
}

//Finds a movie object with the same name. Returns a pointer to the Movie object if found or a null pointer if not
Movie * MovieTicketMaster::FindMovie(string movie_name) {
	// EXTRA CREDIT: Implement binary search instead of a linear search.
	// Note: array of pointers are already sorted based on movie name
	return ::BinarySearch(list_of_movies_, 0, g_kSize - 1, movie_name);
}

void MovieTicketMaster::PurchaseTicket() {
	string movie_name;
	string number_of_tickets_as_string;
	Movie * p_movie = NULL;
	int number_of_tickets;
	double total_cost;

	cout << "Please enter a movie name. " << endl;
	cin.ignore();
	getline(cin, movie_name);
	p_movie = FindMovie(movie_name);
	if (p_movie != NULL) {
		cout << "How many tickets would you like to purchase?" << endl;
		cin >> number_of_tickets;
		total_cost = p_movie -> PurchaseTicket(number_of_tickets);
		if (total_cost == -1) {
			cout << "The show has been sold out or not enough tickets avaliable per your request." << endl << endl;
		} else {
			cout << "Thank you for your purchase of " << number_of_tickets << " tickets to " << movie_name << " for a total of $" << total_cost << endl << endl;
		}
	} else {
		cout << "Error, no movie found." << endl << endl;
	}
}

Movie * BinarySearch(Movie * movie_list[], int start, int end, string movie_name) {
	//EXTRA CREDIT: Implement Binary Search to search for matching movie_name
	while (start <= end) {
		int middle = (start + end) / 2;
		if (movie_list[middle]-> get_movie_name() == movie_name) {
			return movie_list[middle];
		} else if (movie_list[middle]->get_movie_name() > movie_name ) {
			end = middle - 1;
		} else {
			start = middle + 1;
		}
	}
	return NULL;
}

void SetPointer(Movie * &p_swap_a, Movie * &p_swap_b) {
	//Extra Credit: Helper function to point p_swap_a's pointer to p_swap_b's pointer and vice versa
	Movie * p_temp = NULL;
	p_temp = p_swap_a;
	p_swap_a = p_swap_b;
	p_swap_b = p_temp;
}

void SelectionSort(Movie * movie_list []) {
	//Extra Credit: Sort movies using SelectionSort()
	int min_index = 0;
	//leftmost boundary for unsorted array
	for (int begin_unsorted = 0; begin_unsorted < g_kSize - 1; begin_unsorted++) {

		// assume minimum is the the first & leftmost element in unsorted array
		min_index = begin_unsorted;

		// check if any of the elements after is smaller than the leftmost element in unsorted array
		for (int j = min_index+1; j < g_kSize; j++) {
			if (movie_list[j] -> get_movie_name() < movie_list[min_index] -> get_movie_name()) {
				min_index = j;
			}
		}
		//swap leftmost element of unsorted array with the min_index element
		SetPointer(movie_list[begin_unsorted], movie_list[min_index]);
	}
}

int main () {
	MovieTicketMaster * p_movie = NULL;
	p_movie = new MovieTicketMaster("Century's Theatre", "111 Skyline Blvd");
	p_movie -> Init();
	p_movie -> Run();
	delete p_movie; // delete will also call MovieTicketMaster destructor which will deallocate the Movie objects.
}
