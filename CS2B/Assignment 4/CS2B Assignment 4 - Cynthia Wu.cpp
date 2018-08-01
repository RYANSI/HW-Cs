//=====================================================================================
// Name        : CS 2B Assignment #4 - Cynthia Wu.cpp
// Author      : Cynthia Wu
// Date        : November 24th, 2017
// Version     :
// Copyright   : Copyright (C). CS Department Foothill College 2017. All rights reserved.
// Program purpose: utilize STL containers (vector/map) and exception handling
//					to create an application to track customer's SmartPhone message
//					usages/charges and provide account disconnect services
// Revisions history: (date/programmer/what changed)
//					** All changes/revisions made by Cynthia Wu **
// 		11.24.17 - Declare and define Message base class and derived classes.
//		11.25.17 - Declare SmartCarrier class and methods
//		11.26.17 - Define SmartCarrier methods
//		12.01.17 - Continue declaring SmartCarrier methods
//		12.02.17 - Define SmartCarrier methods
//		12.11.12 - Fix SmartCarrier methods
//=====================================================================================
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <iomanip>
#include <exception>
#include <fstream>
using namespace std;

class Message {
	private:
		string from_; // sender's phone number
		string to_; // reciever's phone number

	public:
		Message();
		Message(string from, string to);
		virtual ~Message();

		//accessor + mutator functions
		string get_from() const;
		void set_from(string from);
		string get_to() const;
		void set_to(string to) ;

		// pure virtual function
		virtual double ComputeCharge() const = 0;

		//overloaded equality operator
		bool operator== (const Message &message) const;

};

class TextMessage: public Message {
	private:
		string text_message_;

	public:
		TextMessage();
		TextMessage(string from, string to, string text_message);
		virtual ~TextMessage();

		//accessor + mutator functions
		string get_text_message() const;
		void set_text_message(string text_message);

		//redefined pure virtual method
		virtual double ComputeCharge() const;

		//overloading << operator
		friend ostream & operator<<(ostream & os, const TextMessage & t1);

};

class VoiceMessage: public Message {
	private:
		int time_; //in seconds

	public:
		VoiceMessage();
		VoiceMessage(string from, string to, int time);
		virtual ~VoiceMessage();

		// accessor + mutator methods
		int get_time() const;
		void set_time(int time);

		// redefined pure virtual method
		virtual double ComputeCharge() const;

		//overloading << operator
		friend ostream & operator<<(ostream & os, const VoiceMessage & v1);
};

class MediaMessage: public Message {
	private:
		int media_size_; // in MB

	public:
		MediaMessage();
		MediaMessage(string from, string to, int media_size);
		virtual ~MediaMessage();

		// accessor + mutator functions
		int get_media_size() const;
		void set_media_size(int media_size);

		// redefined pure virtual method
		virtual double ComputeCharge() const;

		//overloading << operator
		friend ostream & operator<<(ostream & os, const MediaMessage & m1);
};

class SmartCarrier {
	private:
		string carrier_name_;
		map<string, vector<Message *> > accounts_;

		//private member functions
		void Menu();
		int GetChoice();
		void ListAll();
		void SearchMessage();
		void EraseMessage();
		void DisconnectAccount();
		void StopService();

	public:
		SmartCarrier();
		SmartCarrier(string carrier_name);
		~SmartCarrier();

		//accessor + mutator functions
		string get_carrier_name() const;
		void set_carrier_name(string carrier_name);
		const map<string, vector<Message *> > & get_accounts() const;

		// public member functions
		void Init();
		void StartService();
};

Message::Message(): from_(""), to_("") {};
Message::Message(string from, string to): from_(from), to_(to) {};
Message::~Message() {};
string Message::get_from() const { return from_; };
void Message::set_from(string from) { from_ = from; };
string Message::get_to() const { return to_; };
void Message::set_to(string to) { to_=to; };
double Message::ComputeCharge() const { return 0.0; }; // pure virtual function with implementation
bool Message::operator== (const Message &message) const {
	return (this->from_ == message.get_from() && this->to_ == message.get_to());
};

TextMessage::TextMessage(): Message(), text_message_("") {};
TextMessage::TextMessage(string from, string to, string text_message): Message(from, to), text_message_(text_message) {};
TextMessage::~TextMessage() {};
string TextMessage::get_text_message() const { return text_message_; };
void TextMessage::set_text_message(string text_message) { text_message_ = text_message; };
double TextMessage::ComputeCharge() const { return .20; }; // 20 cents/message
ostream & operator<<(ostream & os, const TextMessage & t1) {
	os << "From: " << t1.get_from() << "\t To: " << t1.get_to() << "\t Message: " << t1.get_text_message();
	return os;
}

VoiceMessage::VoiceMessage(): Message(), time_(0) {};
VoiceMessage::VoiceMessage(string from, string to, int time): Message(from, to), time_(time) {};
VoiceMessage::~VoiceMessage() {};
int VoiceMessage::get_time() const { return time_; };
void VoiceMessage::set_time(int time) { time_=time; };
double VoiceMessage::ComputeCharge() const { return .15 * this->time_; }; // 15 cents per second
ostream & operator<<(ostream & os, const VoiceMessage & v1) {
	os << "From: " << v1.get_from() << "\t To: " << v1.get_to() << "\t Length of Voice Message: " << v1.get_time();
	return os;
}

MediaMessage::MediaMessage(): Message(), media_size_(0) {};
MediaMessage::MediaMessage(string from, string to, int media_size): Message(from, to), media_size_(media_size) {};
MediaMessage::~MediaMessage() {};
int MediaMessage::get_media_size() const { return media_size_; };
void MediaMessage::set_media_size(int media_size) { media_size_ = media_size; };
double MediaMessage::ComputeCharge() const { return .1*this->media_size_; }; // 10 cents per size
ostream & operator<<(ostream & os, const MediaMessage & m1) {
	os << "From: " << m1.get_from() << "\t To: " << m1.get_to() << "\t Size of Media Message: " << m1.get_media_size();
	return os;
}

SmartCarrier::SmartCarrier(): carrier_name_("") {}
SmartCarrier::SmartCarrier(string carrier_name): carrier_name_(carrier_name) {};
SmartCarrier::~SmartCarrier() {
	map<string, vector<Message * > >::iterator m_iter;
	vector<Message *>::iterator v_iter;
	for (m_iter = accounts_.begin(); m_iter != accounts_.end(); ++m_iter) {
		//loop through each element in the vector and free its memory
		for (v_iter = m_iter->second.begin(); v_iter != m_iter->second.end(); ++v_iter) {
			delete (*v_iter); // deallocate message object
		}
		m_iter -> second.clear();
	}
	accounts_.clear();
};
string SmartCarrier::get_carrier_name() const { return carrier_name_;};
void SmartCarrier::set_carrier_name(string carrier_name) { carrier_name_ = carrier_name; };
const map<string, vector<Message *> >& SmartCarrier::get_accounts() const { return accounts_; };

void SmartCarrier::Init() {
	// manually initialize accounts_
	accounts_["000-000-0000"].push_back(new TextMessage("000-000-0000", "111-111-1111", "Do you want to learn how to program in C++?"));
	accounts_["000-000-0000"].push_back(new VoiceMessage("000-000-0000", "333-333-3333", 36));
	accounts_["000-000-0000"].push_back(new MediaMessage("444-444-4444", "000-000-0000", 20));

	accounts_["111-111-1111"].push_back(new TextMessage("555-555-5555", "111-111-1111", "Do you want to grab dinner?"));
	accounts_["111-111-1111"].push_back(new TextMessage("111-111-1111", "555-555-5555", "Sure."));
	accounts_["111-111-1111"].push_back(new TextMessage("666-666-6666", "111-111-1111", "OK."));
	accounts_["111-111-1111"].push_back(new VoiceMessage("111-111-1111", "777-777-7777", 60));
	accounts_["111-111-1111"].push_back(new VoiceMessage("222-222-2222", "111-111-1111", 3));
	accounts_["111-111-1111"].push_back(new VoiceMessage("111-111-1111", "333-333-3333", 120));
	accounts_["111-111-1111"].push_back(new MediaMessage("111-111-1111", "666-666-6666", 36));
	accounts_["111-111-1111"].push_back(new MediaMessage("777-777-7777", "111-111-1111", 21));
	accounts_["111-111-1111"].push_back(new MediaMessage("888-888-8888", "111-111-1111", 2));
	accounts_["111-111-1111"].push_back(new MediaMessage("000-000-0000", "111-111-1111", 4));

	accounts_["222-222-2222"].push_back(new TextMessage("222-222-2222", "000-000-0000", "Where are you?"));
	accounts_["222-222-2222"].push_back(new TextMessage("000-000-0000", "222-222-2222", "LOL"));
	accounts_["222-222-2222"].push_back(new MediaMessage("777-777-7777", "222-222-2222", 8));
	accounts_["222-222-2222"].push_back(new MediaMessage("222-222-2222", "555-555-5555", 45));
	accounts_["222-222-2222"].push_back(new MediaMessage("444-444-4444", "222-222-2222", 30));

	accounts_["333-333-3333"].push_back(new VoiceMessage("333-333-3333", "000-000-0000", 2));
	accounts_["333-333-3333"].push_back(new MediaMessage("888-888-8888", "333-333-3333", 45));

	//no value for key 444-444-4444 (i.e. this account has no messages)
	accounts_["444-444-4444"];

	accounts_["555-555-5555"].push_back(new TextMessage("555-555-55555", "666-666-6666", "What's up"));
	accounts_["555-555-5555"].push_back(new VoiceMessage("111-111-1111", "555-555-5555", 58));
	accounts_["555-555-5555"].push_back(new VoiceMessage("000-000-0000", "555-555-5555", 90));
	accounts_["555-555-5555"].push_back(new VoiceMessage("555-555-5555", "444-444-4444", 32));
	accounts_["555-555-5555"].push_back(new VoiceMessage("666-666-6666", "555-555-5555", 45));

	accounts_["666-666-6666"].push_back(new MediaMessage("666-666-6666", "333-333-3333", 10));
	accounts_["666-666-6666"].push_back(new MediaMessage("555-555-5555", "666-666-6666", 4));
	accounts_["666-666-6666"].push_back(new MediaMessage("111-111-1111", "666-666-6666", 23));

	accounts_["777-777-7777"].push_back(new TextMessage("777-777-7777", "000-000-0000", "not really"));
	accounts_["777-777-7777"].push_back(new TextMessage("222-222-2222", "777-777-7777", "yeah, how about you?"));
	accounts_["777-777-7777"].push_back(new TextMessage("777-777-7777", "111-111-1111", ":)"));
	accounts_["777-777-7777"].push_back(new TextMessage("444-444-4444", "777-777-7777", "lets go!"));
	accounts_["777-777-7777"].push_back(new VoiceMessage("777-777-7777", "999-999-9999", 75));
	accounts_["777-777-7777"].push_back(new VoiceMessage("999-999-9999", "777-777-7777", 65));
	accounts_["777-777-7777"].push_back(new MediaMessage("777-777-7777", "000-000-0000", 4));
	accounts_["777-777-7777"].push_back(new MediaMessage("777-777-7777", "888-888-8888", 34));
	accounts_["777-777-7777"].push_back(new MediaMessage("888-888-8888", "777-777-7777", 11));

	accounts_["888-888-8888"].push_back(new TextMessage("888-888-8888", "999-999-9999", "are you there yet?"));
	accounts_["888-888-8888"].push_back(new VoiceMessage("888-888-8888", "999-999-9999", 120));
	accounts_["888-888-8888"].push_back(new VoiceMessage("999-999-9999", "888-888-8888", 54));
	accounts_["888-888-8888"].push_back(new MediaMessage("555-555-5555", "888-888-8888", 23));
	accounts_["888-888-8888"].push_back(new MediaMessage("888-888-8888", "666-666-6666", 43));

	accounts_["999-999-9999"].push_back(new TextMessage("555-555-5555", "999-999-9999", "hello"));
	accounts_["999-999-9999"].push_back(new TextMessage("999-999-9999", "333-333-3333", "see ya later"));
	accounts_["999-999-9999"].push_back(new TextMessage("999-999-9999", "111-111-1111", "sure"));
}

void SmartCarrier::StartService() {
	int user_input;
	user_input = 0;
	do {
		Menu();
		user_input = GetChoice();
		switch (user_input) {
			case 1:
				ListAll();
				break;

			case 2:
				SearchMessage();
				break;

			case 3:
				EraseMessage();
				break;

			case 4:
				DisconnectAccount();
				break;

			case 5:
				StopService();
				return;
		}
	}
	while (user_input != 0);
}

void SmartCarrier::Menu() {
	cout << setw(15) << right<< "" << "FOOTHILL WIRELESS" << endl;
	cout << setw(10) << "" << "MESSAGE UTILIZATION AND ACCOUNT ADMIN" << endl;
	cout << "1. List summary of message usages/charges for all accounts" << endl;
	cout << "2. Search a message from an account" << endl;
	cout << "3. Erase a message from an account" << endl;
	cout << "4. Disconnect an account" << endl;
	cout << "5. Quit" << endl;

};

int SmartCarrier::GetChoice() {
	int user_choice;
	cin >> user_choice;
	return user_choice;
};

void SmartCarrier::ListAll() {
	double total_charge;
	int message_count;
	int text_count;
	int voice_count;
	int media_count;
	map <string, vector<Message *> >::iterator m_iter1;
	map <string, vector<Message *> >::iterator m_iter2;
	vector<Message *>::iterator v_iter;
	message_count = 0;
	cout << setw(20) << left << "Phone Number" << setw(20) << "Total Messages" << setw(15) << "Text" << setw(15) << "Voice" << setw(15) << "Media" << setw(20) << "Total Charges" << endl;

	for (m_iter1 = accounts_.begin(); m_iter1 != accounts_.end(); ++ m_iter1) { // get "master" account
		total_charge = 0;
		message_count = 0;
		text_count = 0;
		media_count = 0;
		voice_count = 0;
		TextMessage * p_text_message = NULL;
		VoiceMessage * p_voice_message = NULL;
		MediaMessage * p_media_message = NULL;
		for (m_iter2 = accounts_.begin(); m_iter2 != accounts_.end(); ++m_iter2) { // iterate through map from beginning to see if account matches any of the from/to Messages
			for (v_iter = m_iter2->second.begin(); v_iter != m_iter2->second.end(); ++v_iter) { // loop through vector
				if (!m_iter2->second.empty() &&
						((*v_iter) -> Message::get_from() == m_iter1 -> first || (*v_iter) -> Message::get_to() == m_iter1 -> first)) { // check if "master" accounts match from/to of all messages
					// cast to check what type of message and get the counts for them
					if ((p_text_message = dynamic_cast<TextMessage *>(*v_iter)) != NULL) {
						++text_count;
					} else if ((p_voice_message = dynamic_cast<VoiceMessage *>(*v_iter)) != NULL) {
						++voice_count;
					} else if ((p_media_message = dynamic_cast<MediaMessage *>(*v_iter)) != NULL) {
						++media_count;
					}
					total_charge += (*v_iter) -> ComputeCharge();
					++message_count;
				}
			}
		}
		cout << setw(20) << m_iter1->first << setw(20) << left << message_count << setw(15) << text_count << setw(15) << voice_count << setw(15) << media_count << setw(3) << "$" << setprecision(2) << fixed << total_charge << endl;
	}
};

void SmartCarrier::SearchMessage() {
	string message_type;
	string account;
	string from;
	string to;
	map<string, vector<Message *> >:: iterator m_iter;
	vector<Message *>:: iterator v_iter;
	TextMessage * p_text = NULL;
	VoiceMessage * p_voice = NULL;
	MediaMessage * p_media = NULL;
	Message * p_user_message = NULL;
	try {
		cout << "Please enter a hyphen separated account number to search for (ex: 123-456-7890)." << endl;
		cin >> account;
		cout << "Please enter a message type (Text, Voice, or Media)." << endl;
		cin >> message_type;
		cout << "Who is the message from? Please enter response as a hyphen seperated account number (ex: 123-456-7890)." << endl;
		cin >> from;
		cout << "Who is the message to? Please enter response as a hyphen seperated account number (ex: 123-456-7890)." << endl;
		cin >> to;

		// search for message
		m_iter = accounts_.find(account);
		if (m_iter == accounts_.end()) { // first check that we have a valid account
			throw runtime_error("Cannot find account.");
		} else {
			TextMessage user_message(from, to, "Test"); // create dummy Message derived obj to use == operator overloading
			p_user_message = &user_message;
			for (v_iter = m_iter->second.begin(); v_iter != m_iter->second.end(); ++v_iter) {
				if(**v_iter == *p_user_message) {
					if (message_type == "Text" && (p_text = dynamic_cast<TextMessage *>(*v_iter)) != NULL) {
						cout << *p_text << endl;
						return;

					} else if (message_type == "Voice" && (p_voice = dynamic_cast<VoiceMessage *>(*v_iter)) != NULL) {
						cout << *p_voice << endl;
						return;

					} else if (message_type == "Media" && (p_media = dynamic_cast<MediaMessage *>(*v_iter)) != NULL) {
						cout << *p_media << endl;
						return;

					}
				}
			}
			throw runtime_error("Cannot find message."); // found account but doesn't match any other condition
		}
	}
	catch (exception & e) {
		cout << "Exception: " << e.what() << endl;
		return;
	}
};

void SmartCarrier::EraseMessage() {
	string message_type;
	string account;
	string from;
	string to;
	map<string, vector<Message *> >:: iterator m_iter;
	vector<Message *>:: iterator v_iter;
	TextMessage * p_text = NULL;
	VoiceMessage * p_voice = NULL;
	MediaMessage * p_media = NULL;
	Message * p_user_message = NULL;
	try {
		cout << "Please enter a hyphen separated account number to search for (ex: 123-456-7890)." << endl;
		cin >> account;
		cout << "Please enter a message type (Text, Voice, or Media)." << endl;
		cin >> message_type;
		cout << "Who is the message from? Please enter response as a hyphen seperated account number (ex: 123-456-7890)." << endl;
		cin >> from;
		cout << "Who is the message to? Please enter response as a hyphen seperated account number (ex: 123-456-7890)." << endl;
		cin >> to;

		// search for message
		m_iter = accounts_.find(account);
		if (m_iter == accounts_.end()) { // first check that we have a valid account
			throw runtime_error("Cannot account, and so cannot erase message.");
		} else {
			TextMessage user_message(from, to, "Test");
			p_user_message = &user_message;
			for (v_iter = m_iter->second.begin(); v_iter != m_iter->second.end(); ++v_iter) {
				if(**v_iter == *p_user_message && //check from & to using == operator
					(
						(message_type == "Text" && (p_text = dynamic_cast<TextMessage *>(*v_iter)) != NULL) ||
						(message_type == "Voice" && (p_voice = dynamic_cast<VoiceMessage *>(*v_iter)) != NULL) ||
						(message_type == "Media" && (p_media = dynamic_cast<MediaMessage *>(*v_iter)) != NULL))
					) {
					delete (*v_iter); // deallocate message object
					// TODO: check if it invalidates any of my iterators
					m_iter -> second.erase(v_iter); // remove from vector
					cout << "Message erased." << endl;
					return;
				}
			}
			throw runtime_error("Cannot find message."); // found account but doesn't match from/to/or message type provided by user
		}
	}
	catch (exception & e) {
		cout << "Exception: " << e.what() << endl;
		return;
	}
};

void SmartCarrier::DisconnectAccount() {
	string user_input;
	map<string, vector<Message *> >::iterator m_iter;
	vector<Message *>::iterator v_iter;
	cout << "Enter an account phone number to disconnect." << endl;
	cin >> user_input;
	try {
		m_iter = accounts_.find(user_input);
		if (m_iter != accounts_.end()) {
			for (v_iter = m_iter->second.begin(); v_iter != m_iter -> second.end(); ++v_iter) { //loop through each element in vector and deallocate memory
				delete (*v_iter); // deallocate message object
			}
			m_iter->second.clear();
			accounts_.erase(m_iter->first); // remove entry from map
			cout << "Account deleted." << endl;
		}
	}
	catch (exception & e) {
		cout << "Cannot find account." << endl;
		return;
	}
};

void SmartCarrier::StopService() {
	ofstream fout("AccountsInfo.txt");
	map <string, vector<Message *> >::iterator m_iter;
	vector<Message *>::iterator v_iter;
	TextMessage * p_text = NULL;
	VoiceMessage * p_voice = NULL;
	MediaMessage * p_media = NULL;

	// below variables needed for text output formatting
	bool first_text;
	bool first_media;
	bool first_voice;
	if (!fout) {
		cout << "ERROR: Unable to open output file.\n";
		exit(-2);
	}

	ListAll();
	for (m_iter = accounts_.begin(); m_iter != accounts_.end(); ++m_iter) {
		first_text = false;
		first_media = false;
		first_voice = false;
		fout << m_iter ->first << endl;
		for (v_iter = m_iter->second.begin(); v_iter != m_iter->second.end(); ++v_iter) {
			if ((p_text = dynamic_cast<TextMessage *>(*v_iter)) != NULL) {
				if (!first_text) {
					fout << "\t Text:" << endl;
				}
				fout << "\t\t" << p_text -> Message::get_from() << ';' << p_text -> Message::get_to() << ';' <<  p_text -> get_text_message() << endl;
				first_text = true;
			} else if ((p_voice = dynamic_cast<VoiceMessage *>(*v_iter)) != NULL) {
				if (!first_voice) {
					fout << "\t Voice:" << endl;
				}
				fout << "\t\t" << p_voice -> Message::get_from() << ';' << p_voice -> Message::get_to() << ';' << p_voice -> get_time() << endl;
				first_voice = true;
			} else if ((p_media = dynamic_cast<MediaMessage *>(*v_iter)) != NULL) {
				if (!first_media) {
					fout << "\t Media:" << endl;
				}
				fout << "\t\t" << p_media -> Message::get_from() << ";" << p_media -> Message::get_to() << ';' << p_media -> get_media_size() << endl;
				first_media = true;
			}
		}
	}
	cout << "Service has been stopped. Bye!" << endl;
	fout.close();
	return;
};

int main() {
	SmartCarrier * p_smart_carrier = NULL;
	p_smart_carrier = new SmartCarrier("AT&T");
	p_smart_carrier -> Init();
	p_smart_carrier -> StartService();
	delete p_smart_carrier;
}
