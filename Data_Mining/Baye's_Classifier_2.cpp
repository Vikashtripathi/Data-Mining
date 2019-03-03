#include <bits/stdc++.h>
using namespace std;

vector <vector<string>> dataset;
vector<string> input;

map<string, int> mp;
map<string, int>::iterator it;

void display()
{
	cout << '\n';
	cout << "DATASET IS AS FOLLOWS : " << endl;
	cout << '\n';

	for (int i = 0; i < dataset.size(); i++){
		for (int j = 0; j < dataset[0].size(); j++){
			cout << dataset[i][j] << "  ";
		}
		cout << endl;
	}
}

void class_type()
{
	int l = dataset[0].size();

	for (int i = 0; i < dataset.size() - 1; i++){
		if (mp.find(dataset[i][l - 1]) == mp.end()){
			
			mp[dataset[i][l - 1]] = 0;
			mp[dataset[i][l - 1]]++;
		
		} else {
			mp[dataset[i][l - 1]]++;
		}
	}
}

void bayesian_classification()
{
	int total_number_of_tuples = dataset.size() - 1, i, j, k;
	
	class_type();
	int l = dataset[0].size();

	map<string, float> class_probability;
	
	for (it = mp.begin(); it != mp.end(); it++){

		float val = (float)it -> second / (float)total_number_of_tuples;
		class_probability[it -> first] = val;
	}
	
	map<string, float> class_classifier;

	for (it = mp.begin(); it != mp.end(); it++){
		
		string class_val = it -> first;
		int class_count = it -> second;
		
		float px_c = 1;

		for (j = 0; j < input.size(); j++){
			
			string input_val = input[j];
			int count = 1;

			for (k = 0; k < dataset.size() - 1; k++){
				if (dataset[k][j + 1] == input_val && dataset[k][l - 1] == class_val){
					count++;
				}
			}
			
			px_c = px_c * ((float)count / (float)(class_count + 1));
		}

		px_c = px_c * class_probability[class_val];
		class_classifier[class_val] = px_c;
	}	

	string probable_class;
	float probability = 0;

	for (auto ch = class_classifier.begin(); ch != class_classifier.end(); ch++){
		if (ch -> second > probability){
			probability = ch -> second;
			probable_class = ch -> first;
		}
	}

	cout << "Classes along with P( X | Ci ) * P( Ci ) where X is the input tuple to be classified in one of the classes \n" << endl;

	for (auto ch = class_classifier.begin(); ch != class_classifier.end(); ch++){
		cout << ch -> first << " " << ch -> second << endl;
	}

	cout << "\nProbable class for input tuple with Laplace correction is : " << probable_class << endl;
}

// trim from start (in place)
void ltrim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(),not1(ptr_fun<int, int>(isspace))));
}

// trim from end (in place)
void rtrim(string &s) {
    s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
}

// trim from both ends (in place)
void trim(string &s) {
    ltrim(s);
    rtrim(s);
}

int main()
{		
	int i;

	cout << "Enter attribute values for input tuple : \n" << endl;

	string heading[4] = {"Age", "Income", "Student", "Credit_rating"};
	
	for (i = 0; i < 4; i++){
		string x;
		cout << heading[i] << " = ";
		cin >> x;
		transform(x.begin(), x.end(), x.begin(), ::toupper);
		input.push_back(x);
	}

	ifstream excel_file("DATASET_b.csv");

	if (excel_file.is_open()){
		
		string a;
		getline(excel_file, a);

		a = "";

		while (!excel_file.eof()){
			vector<string> temp;

			for (i = 0; i <= 4; i++){
				getline(excel_file, a, ',');
				trim(a);
				transform(a.begin(), a.end(), a.begin(), ::toupper);
				temp.push_back(a);
				a = "";
			}
			
			getline(excel_file, a, '\n');
			trim(a);
			transform(a.begin(), a.end(), a.begin(), ::toupper);
			temp.push_back(a);
			a = "";

			dataset.push_back(temp);
		}

		display();
		bayesian_classification();
		
		excel_file.close();

	} else {
		cout << "File does not exist" << endl;
	}

	return 0;
}