// this file contains the definitions or function signatures of those 
// necessary functions which are implemented in the "calculator.h" file...
using namespace std;

void display_menu();
void compute();
void display_screen(string input, string op, string last);
int operator_precedence(string op);
void four_function_cal();
void set_result(string op);
void calculate(char op);
void read_file_as_input();
bool get_file_name();
void set_directory();
void add_display_value_to_memory(string val, string index);
double return_memory_content(string index);
void compute_file_input(vector<string>val);