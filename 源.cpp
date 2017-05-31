#include "БъЭЗ.h"

int main() {
	
	char choice;
	string T;
	string S;
	SAM sam;
	int num;
	char symbol;

	for (;;)
	{
		cout << endl << endl;
		cout << "***************************************************************************\nThe operation lists:\n";
		cout << "1. Build a suffix automaton.\n";
		cout << "2. Insert a symbol to the suffix automaton\n";
		cout << "3. Verify if an particular string is the suffix of the text context.\n";
		cout << "4. Verify if an particular string is the substring of the text context.\n";
		cout << "5. Count the unique substring of the text context.\n";
		cout << "6. Find the first position of a substring.\n";
		cout << "7. Get the occur times of an particular string.\n";
		cout << "8. Clean the SAM.\n";
		cout << "q. quit.\n***************************************************************************\n";
		cout << "Enter your choice: ";
		cin >> choice;
		if (choice == 'q')
			goto quit;
		else if (choice == '1') {
			cout << "Please input text context:\n";
			cin >> T;
			sam.create_sam(T);
			cout << "SAM builted\n";
			
			//sam.path_count_help(sam.get_last());
		}
		else if (choice == '2')
		{
			cout << "Please input the symbol: ";
			cin >> symbol;
			sam.insert_new_symbol(symbol);
			cout << "New symbol inserted.\n";
		}
		else if (choice == '3')
		{
			cout << "Please input the string:";
			cin >> S;
			sam.verify_suffix(S);
		}
		else if (choice == '4')
		{
			cout << "Please input the string:";
			cin >> S;
			sam.verify_substring(S);
		}
		else if (choice == '5') {
			num = sam.get_unique_substring_num();
			if (num!=-1)
				cout << "The num of unique_substring is: " <<num << endl;
		}
		else if (choice == '6') {
			cout << "Please input the string:";
			cin >> S;
			num = sam.get_first_position(S);
			if (num != -1)
				cout << "The fisrt position of the substring is: " << num << endl;
			else cout << "Not a substring.\n";
		}
		else if (choice == '7') {
			cout << "Please input the string:";
			cin >> S;
			num = sam.get_occured_times(S);
			if (num != -1)
				cout << "The substring occur times is: " << num << endl;
			else cout << "Not a substring.\n";
		}
		else if (choice == '8') {
			sam.clear();
			cout << "SAM clear.\n";
		}
		else cout << "Not such opition.\n";
	}
	
quit:
	cout << "Program terminated.\n";
	system("pause");
	return 0;
}