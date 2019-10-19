#include <vector>

void combinationUtil(vector<int> arr, int n, int r, int index, vector<int> data, int i, vector<vector<int> > &comb){ 
	// Current cobination is ready, print it 
	if (index == r){
		comb.push_back(data);
		return; 
	} 
  
	// When no more elements are there to put in data[] 
	if (i >= n) 
		return; 
  
	// current is included, put next at next location 
	data[index] = arr[i]; 
	combinationUtil(arr, n, r, index+1, data, i+1, comb); 
  
	// current is excluded, replace it with next (mote that 
	// i+1 is passed, but index is not changed) 
	combinationUtil(arr, n, r, index, data, i+1, comb); 
} 

void combination(vector<int> arr, int n, int r, vector<vector<int> > &comb){ 
	// A temporary array to store all combination one by one 
	vector<int> data(r);
	// int data[r]; 
	combinationUtil(arr, n, r, 0, data, 0, comb); 
} 