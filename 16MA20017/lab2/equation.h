#include <iostream>
#include <stdlib.h>
#include <map>

using namespace std;


class EQN{
	public:
		char vars[];
		// map<char,int> coeffs;
		vector<float> coeffs;
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
		// cout << num_vars << "sdfds";/
		for (int i = 0; i < num_vars; ++i)
		{
			cout << "Enter co-efficient for " << i << " :- ";
			int temp;
			cin >> temp;
			// coeffs[vars[i]] = temp;
			coeffs.push_back(temp);
		}
	}

	vector<float> prep_gauss(){
		vector<float> combo;
		// cout << num_vars;
		for (int i = 0; i < num_vars; ++i)
		{	
			// cout << i << " sdf";
			combo.push_back(coeffs[i]);
		}
		combo.push_back(bias);
		return combo;
		
	}

	void get_type(){
		cout << "Enter the sign ( 0 for <, 1 for <=, 2 for >, 3 for >=, 4 for = ) of the equation :- ";
		cin >> sign;
	}

	void get_bias(){
		cout << "Enter b of the eqn :- ";
		cin >> bias;
	}

	int check_feasible(vector<float> sol){
		float sum = 0;
		vector<float> combo = this->prep_gauss();

		for (int i = 0; i < sol.size(); ++i)
		{
			sum += combo[i]*sol[i];
		}

		if( sign == 0 ){
			return sum < bias;
		}
			
		if( sign == 1 )
			return sum <= bias;
		if( sign == 2 )
			return sum > bias;
		if( sign == 3 )
			return sum >= bias;
		if( sign == 4 )
			return sum = bias;
	}

	float compute(vector<float> sol){
		float sum = 0;
		vector<float> combo = this->prep_gauss();

		for (int i = 0; i < sol.size(); ++i)
		{
			sum += combo[i]*sol[i];
		}
		return sum;
	}

	EQN(int nvars){
		// EQN e1 = EQN();
		// cout << "Enter number of variables :- ";
		// cin >> num_vars;
	 	// e1.get_vars();
	 	num_vars = nvars;
	 	get_coeffs();
	 	get_type();
	 	get_bias();
	 	// print_eqn();
	 	// return e1;
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