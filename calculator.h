// contains the I/O, vector, string, map, etc header files
#include<bits/stdc++.h>
#include <conio.h>
// file including some helper functions
#include "helper.h"
#define INF 10e7
#define DIFDIR "E:\\A_ST2\\ST2_2022\\Assignment2\\"

// Required globlal variables
stack<string>store_values;
stack<string>operators;
map<string, double>memory;
string dir = "";
string filename = "";

// this function is used to get the filename as an input from the user
bool get_file_name() {
	cout<<"ENTER THE FILENAME: "<<endl;
	cin>>filename;
	cout<<"FILENAME: "<<DIFDIR + filename<<endl;
	ifstream file;
	file.open(filename);
	// checks whether the file exists or not
	if(file) {
		file.close();
		return true;
	}
	file.close();
	return false;
}

// this function is used to set the user-defined directory
void set_directory() {
	cout<<"ENTER DIRECTORY: "<<endl;
	cin>>dir;
	cout<<"CURRENT DIRECTORY IS: "<<dir<<endl;
}

// this function is used to read a user-defined file and executes it
// and produces the output
void read_single_test_file_run() {
	if(!get_file_name()) {
		cout<<"SORRY... THE FILENAME YOU'VE ENTERED DOESN'T EXIST!"<<endl;
		display_menu();		
	} else {
		char x;
		string temp = "";
		vector<string>bytes;
		ifstream file(filename);
		// gets the input char by char from the user-defined file
		// until the end of file
		while(file.get(x)) {
			// we'll ignore any whitespaces (\t, \n, " ")
			if(x == ' ' || x == '\t' || x == '\n') continue;
			else if(!bytes.empty() && bytes[bytes.size()-1] == ".") {
				temp = bytes[bytes.size()-2] + bytes[bytes.size()-1];
				// removes last two elements
				bytes.erase(bytes.begin()+(bytes.size()-2), bytes.end());
				while(file.get(x)) {
					if((x >= '0' && x <= '9')) {
						temp.push_back(x);
					} else break;
				}
				bytes.push_back(temp);
				temp = "";
				temp.push_back(x);
				bytes.push_back(temp);
				temp = "";
			} 
			else {
				if((x >= '0' && x <= '9')) {
					temp.push_back(x);
					while(file.get(x)) {
						if((x >= '0' && x <= '9')) {
							temp.push_back(x);
						} else break;
 					}
					bytes.push_back(temp);
					temp = "";
					temp.push_back(x);
					bytes.push_back(temp);
					temp = "";
				} 
				else {
					temp.push_back(x);
					bytes.push_back(temp);
					temp = "";
				}
			}
		}
		file.close();	
		compute_file_input(bytes);
	}
}

// this function is used to clear the current values displayed on the
// screen and also removes the last operator
void clear_current_display_last_op() {
	if(!store_values.empty() && !operators.empty()) {
		store_values.pop();
		operators.pop();
	}
	display_screen("", "?", "");
	return;
}

// this function is used to add the current value, displayed on the screen,
// to the memory in a map using a key-value pair
void add_display_value_to_memory(string val, string index) {
	string key = "M" + index;
	memory.insert({key, stod(val)});
}

// this function is used to return the value that's present in the memory
double return_memory_content(string index) {
	if(memory.find("M"+index) != memory.end())
		return memory["M"+index];
	else return INF;
}

// this function is used to generate the output based on the input file
void compute_file_input(vector<string>val) {
	string last = "";
	for(int i = 0; i < (int)val.size(); ) {
		if(val[i] == " " || val[i] == "\t" || val[i] == "\n") i++;
		else if(count(val[i].begin(), val[i].end(), '.') >= 2) {
			cout<<"A DECIMAL NUMBER CAN'T CONTAIN MORE THAN ONE DECIMAL POINT!"<<endl;
			break;
		}
		else if(val[i] == "=") {
			last = store_values.top();
			cout<<"key in: ="<<endl;
			compute();
			display_screen(store_values.top(), "?", last);
			i++;
		}
		else if(val[i] == "c" || val[i] == "C") {
			cout<<"key in: C"<<endl;
			memory.clear();
			clear_current_display_last_op();
			i++;
		}
		else if(val[i] == "k" || val[i] == "K") {
			clear_current_display_last_op(); 
			cout<<"key in: K"<<endl;
			i++;
		}
		else if(val[i] == "M" || val[i] == "m") {
			cout<<"key in: M"<<endl;
			display_screen(store_values.top(), "M", last);
			last = store_values.top();
			add_display_value_to_memory(last, val[i+1]);
			cout<<"\nDEBUG: SETTING M"<<val[i+1]<<" TO "<<last<<endl<<endl;;
			display_screen(store_values.top(), "?", "");
			store_values.pop();
			i += 2;
		} 
		else if(val[i] == "R" || val[i] == "r") {
			cout<<"key in: R"<<endl;
			cout<<"last: "<<last<<endl;
			display_screen(last, "R", last);
			last = val[i+1];
			cout<<"last: "<<last<<endl;
			cout<<"R"<<last<<": "<<return_memory_content(last)<<endl;
			store_values.push(to_string(return_memory_content(last)));
			display_screen(last, "", store_values.top());
			i += 2;
		}
		else if(val[i] == "s") {
			cout<<"key in: s"<<endl;
			if(store_values.empty()) display_screen("", "?", "");
			else {
				last = store_values.top();
				compute();
				double ans = sqrt(stod(store_values.top()));
				display_screen(to_string(ans), "?", last);
				store_values.pop();
				store_values.push(to_string(ans));
			}
			i++;
		}
		else if(val[i] >= "0" && val[i] <= "9") {
			if(store_values.empty()) last = val[i];
			else last = store_values.top();
			cout<<"key in: "<<val[i]<<endl;
			store_values.push(val[i]); 
			display_screen(val[i], "?", last);
			i++;
		}
		else if(val[i] == "!") {
			cout<<"key in: !"<<endl;
			double val = stod(store_values.top());
			if(val < 0) val = 2*(abs(val)) + val;
			else val = val - 2*val;
			store_values.pop();
			store_values.push(to_string(val));
			display_screen(store_values.top(), "?", last);
			i++;
		}
		else {
			cout<<"key in: "<<val[i]<<endl;
			if(operators.empty()) operators.push(val[i]);
			else if(!operators.empty() && operator_precedence(val[i]) == operator_precedence(operators.top())) {
				set_result(val[i]);
			} else if(!operators.empty() && operator_precedence(val[i]) > operator_precedence(operators.top())) {
				operators.push(val[i]);
			} else set_result(val[i]);
			display_screen("", val[i], store_values.top());
			i++;
		}
	}
	// clears the numbers and operators stack respectively plus the memory
	while(!store_values.empty()) store_values.pop();
	while(!operators.empty()) operators.pop();
	if(!memory.empty()) memory.clear();
	display_menu();
}

// this function is used to perform the arithmetic operation and generates
// the result based on the operator and push the result on top of the stack
void calculate(string op) {
	double op_1 = stod(store_values.top());
	store_values.pop();
	double op_2 = stod(store_values.top());
	store_values.pop();
	if(op == "-") {
		if(op_1 > op_2) store_values.push(to_string(op_1-op_2));
		else store_values.push(to_string(op_2-op_1));
	} else if(op == "+") store_values.push(to_string(op_1+op_2)); 
	else if(op == "*") store_values.push(to_string(op_1*op_2));
	else store_values.push(to_string(op_2/op_1));
}

// computes the answer over the entire stack of values when '=' is pressed
void compute() {
	while(!operators.empty()) {
		calculate(operators.top());
		operators.pop();
	}
}

// if, the operator on the top of stack and current input operator both are the same,
// then we do the calculations until the stack is empty or the operator at the
// top of stack is different but of same precedence
void set_result(string op) {
	while(!operators.empty()) {
		if(operator_precedence(op) == operator_precedence(operators.top())) {
			if(op != operators.top()) operators.push(op);
			else {
				calculate(operators.top());
				operators.pop();
				operators.push(op);
			}
			break;
		}
		else {
			calculate(operators.top());
			operators.pop();
		}
	}
}

// this function is used to perform the mathematical operations such as
// +, -, *, /, sqrt and some extra functionalities like clear the screen,
// add current displayed value to the memory, retain the value from the memory,
// computes the result when "=" is pressed, and negates a number using "!"
void four_function_cal() {
	string input = "", last = "", last_op = "", EOL = "";
	display_screen(input, "?", last);
	while(true) {
		input = "";
		EOL = getchar();
		do {
			input += EOL;
			EOL = getchar();
		} while(EOL != "\n");

		if(count(input.begin(), input.end(), '.') >= 2) {
			cout<<"A DECIMAL NUMBER CAN'T CONTAIN MORE THAN ONE DECIMAL POINT!"<<endl;
			continue;
		}
		if(input == ";") {
			while(!store_values.empty()) store_values.pop();
			while(!operators.empty()) operators.pop();
			memory.clear();
			break;
		}
		else if(input == "=") {
			last = store_values.top();
			compute();
			display_screen(store_values.top(), "?", last);
		}
		else if(input == "c" || input == "C") {
			memory.clear();
			clear_current_display_last_op();
		}
		else if(input == "k" || input == "K") clear_current_display_last_op();
		else if(input == "M" || input == "m") {
			display_screen(store_values.top(), "M", last);
			input = getch();
			while(true) {
				if(input >= "0" && input <= "9") break;
				else {
					cout<<"INPUT SHOULD BE BETWEEN 0 TO 9 (BOTH INCLUSIVE)...!"<<endl;
					input = getch();
				}
			}
			last = store_values.top();
			store_values.pop();
			add_display_value_to_memory(last, input);
			cout<<"\nDEBUG: SETTING M"<<input<<" TO "<<last<<endl<<endl;;
			display_screen(last, "?", "");
		} 
		else if(input == "R" || input == "r") {
			if(!store_values.empty()) display_screen(store_values.top(), "R", last);
			else display_screen("", "R", last);
			input = getch();
			while(true) {
				if(input >= "0" && input <= "9") break;
				else input = getch();
			}
			last = input;
			if(return_memory_content(input) == INF) display_screen(store_values.top(), "R", last);
			else {
				cout<<"R"<<input<<": "<<return_memory_content(input)<<endl;
				store_values.push(to_string(return_memory_content(input)));
				display_screen(last, "", store_values.top());
			}
		}
		else if(input == "s") {
			if(store_values.empty()) display_screen("", "?", "");
			else {
				last = store_values.top();
				compute();
				double ans = sqrt(stod(store_values.top()));
				display_screen(to_string(ans), "?", last);
				store_values.pop();
				store_values.push(to_string(ans));
			}
		}
		else if(input == "!") {
			double val = stod(store_values.top());
			if(val < 0) val = 2*(abs(val)) + val;
			else val = val - 2*val;
			store_values.pop();
			store_values.push(to_string(val));
			display_screen(store_values.top(), "?", last);
		}
		else if(input >= "0" && input <= "9") { 
			if(store_values.empty()) last = input;
			else last = store_values.top();
			store_values.push(input);
			display_screen(input, "?", last);
		}
		else if(input == "+" || input == "-" || input == "*" || input == "/") {
			last = input;
			if(operators.empty()) operators.push(input);
			else if(!operators.empty() && operator_precedence(input) == operator_precedence(operators.top())) {
				if(input != operators.top()) operators.push(input);
				else set_result(input);
			} else if(!operators.empty() && operator_precedence(input) > operator_precedence(operators.top())) {
				operators.push(input);
			} else set_result(input);

			display_screen("", input, store_values.top());
		} else {
			cout<<"PLEASE ENTER A VALID INPUT!"<<endl;
		}	
	}

	display_menu();
}

// this function is used to display the calculator screen
void display_screen(string input, string op, string last) {
	if(input == "" && last == "") {
		cout<<"("<<op<<")    :0.000000"<<endl;
		cout<<" +-------------+"<<endl;
		cout<<" |             |"<<endl;
		cout<<" +-------------+"<<endl;
	}
	else {
		if(last == "") {
			cout<<fixed<<setprecision(6)<<"("<<op<<")   :0.000000"<<endl;
			cout<<" +-------------+"<<endl;
		}
		else {
			if(op == "M" || op == "R") {
				cout<<fixed<<setprecision(6)<<"(?)   :0.000000"<<endl;
				cout<<" +"<<op<<"------------+"<<endl;
			}
			else {
				if(last == "+" || last == "-" || last == "*" || last == "/") {
					cout<<fixed<<setprecision(6)<<"("<<last<<")   :"<<stod(store_values.top())<<endl;	
				} else cout<<fixed<<setprecision(6)<<"("<<op<<")   :"<<stod(last)<<endl;
				cout<<" +-------------+"<<endl;
			}
		}
		if(input == "") cout<<" |             "<<input<<"|"<<endl;
		else cout<<fixed<<" |\t   "<<setprecision(2)<<stod(input)<<"|"<<endl;  
		cout<<" +-------------+"<<endl<<endl;
	}
	
	return;
}

// this function is used to compute the precedence value of an operator
int operator_precedence(string op) {
	int op_precedence;
	switch(op[0]) {
		case '/':
			op_precedence = 2;
			break;
		case '*':
			op_precedence = 2;
			break;
		case '+':
			op_precedence = 1;
			break;
		case '-':
			op_precedence = 1;
			break;
	}

	return op_precedence;
}

// this function is used to display the main menu
void display_menu() {
	char choice;
	do {
		cout<<"|===============================================================|"<<endl;
		cout<<"|                        CALCULATOR MENU                        |"<<endl;
		cout<<"|===============================================================|"<<endl;
		cout<<"|; - ALSO EXIT                                                  |"<<endl;
		cout<<"|M - SHOW MEMORY                                                |"<<endl;
		cout<<"|E - EXIT                                                       |"<<endl;
		cout<<"|S - SET DIRECTORY                                              |"<<endl;
		cout<<"|4 - RUN 4 FUNCTION CALCULATOR FROM KEYBOARD INPUT              |"<<endl;
		cout<<"|R - READ SINGLE TEST FILE AND RUN                              |"<<endl;
		cout<<"|===============================================================|"<<endl;
		cout<<"\nSELECT AN OPTION FROM THE MENU:"<<endl;
		choice = getch();
		cout<<"OPTION SELECTED: "<<choice<<endl<<endl;

		if(choice == ';' || choice == 'E') {
			cout<<"THANK YOU FOR USING OUR CALCULATOR... :)"<<endl;
			break;
		}

		switch(choice) {
			case 'M':
				if(memory.empty()) cout<<"MEMORY IS EMPTY!"<<endl;
				else {
					cout<<"MEMEORY"<<endl;
					cout<<"======="<<endl;
					for(auto it = memory.begin(); it != memory.end(); it++) {
						cout<<"("<<it->first<<","<<it->second<<")"<<endl;
					}
					cout<<"======="<<endl;
				}
				break;
			case 'S':
				set_directory();
				break;
			case '4':
				four_function_cal();
				break;
			case 'R':
				read_single_test_file_run();
				break;
			default:
				cout<<"PLEASE SELECT A VALID OPTION!"<<endl;
				break;
		}

	} while(true);
}