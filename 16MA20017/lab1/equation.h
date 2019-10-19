#include <iostream>
#include <stdlib.h>
#include <map>

using namespace std;


class EQN{
	public:
		char vars[];
		map<char,int> coeffs;
		int num_vars;
		int sign;
		int bias;
		int catcher;

	void get_vars(){
		cout << "Enter number of variables :- ";
		cin >> num_vars;
		for (int i = 0; i < num_vars; ++i)
		 {
		 	cout << "Enter the symbol of variable " << i+1 << " :- ";
		 	cin >> vars[i];
		 } 
	}
	void get_coeffs(){
		for (int i = 0; i < num_vars; ++i)
		{
			cout << "Enter co-efficient for " << vars[i] << " :- ";
			int temp;
			cin >> temp;
			coeffs[vars[i]] = temp;
		}
	}

	void get_type(){
		cout << "Enter the sign ( 0 for <, 1 for <=, 2 for >, 3 for >=, 4 for = ) of the equation :- ";
		cin >> sign;
	}

	void get_bias(){
		cout << "Enter b of the eqn :- ";
		cin >> bias;
	}	

	EQN init_eqn(){
		EQN e1 = EQN();
	 	e1.get_vars();
	 	e1.get_coeffs();
	 	e1.get_type();
	 	e1.get_bias();
	 	e1.print_eqn();
	 	return e1;
	}

	void print_eqn(){
		for (int i = 0; i < num_vars; ++i)
		{	
			char flag = '+';
			flag = (coeffs[vars[i]] > 0) ? '+' : '\0';
			if(coeffs[vars[i]] == 0 ){
				continue;
			}
			cout << flag << coeffs[vars[i]] << vars[i] << " " ; 
		}
		if( sign == 0 )
			cout << " < ";
		if( sign == 1 )
			cout << " <= ";
		if( sign == 2 )
			cout << " > ";
		if( sign == 3 )
			cout << " >= ";
		if( sign == 4 )
			cout << " = ";

		cout << bias;
		cout << endl;
	}

};