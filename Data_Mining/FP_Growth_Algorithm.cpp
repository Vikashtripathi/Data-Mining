#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vec;

map<int, map<set<int>, int>> L;
map<int, int> item_frequency; // items along with their frequency

void setprint(set<int> items)
{
	cout <<"[ ";
	
	for (auto i = items.begin(); i != items.end(); i++){
		cout << *i <<" ";
	}

	cout <<"]";
}

struct node
{
	map<int, node*> next;
	node* back;
	int value;
	int counter;

	node()
	{
		back = NULL;
		value = INT_MIN;
		counter = 1;
	}
	node(int v, node *parent)
	{
		back = parent;
		value = v;
		counter = 1;
	}
	node(int v, node *parent, int increment)
	{
		back = parent;
		value = v;
		counter = increment;
	}
};

class tree
{
	public:
		
		node head;
		map<int, vector<node *>> item_pointers;
		node* current_node;
		
		tree()
		{
			head = node();
			current_node = &head;
		}

		void add(int x)
		{
			if ((current_node -> next).find(x) == (current_node -> next).end()){
			
				node *temp = new node(x, current_node);
				current_node -> next[x] = temp;
				current_node = temp;
				item_pointers[x].push_back(temp);
			
			} else {
				
				node *temp = (current_node -> next)[x];
				temp -> counter = temp -> counter + 1;
				current_node = temp;

			}
		}

		void add_c(int x,int increment)
		{
			if ((current_node -> next).find(x) == (current_node -> next).end()){

				node *temp = new node(x, current_node, increment);
				current_node -> next[x] = temp;
				current_node = temp;
				item_pointers[x].push_back(temp);

			} else {
				
				node *temp = (current_node -> next)[x];
				temp -> counter = temp -> counter + increment;
				current_node = temp;
				
			}
		}

		void reset()
		{
			current_node = &head;
		}
};

void fmaprint(map<int, map<set<int>, int>> x)
{	
	for (auto it = x.begin(); it != x.end(); it++){
		
		map<set<int>, int> y = x[it -> first];
		
		for (auto it1 = y.begin(); it1 != y.end(); it1++){

			setprint(it1 -> first);
			cout << " --> ";
			cout << (it1 -> second) << "\n";
		}
	}
}

int numsup = -1;

void fpgrowth(tree fp, set<int> initial)
{
	//Formation of Frequent pattern bases
	map<int, vector<node *>> id_pointers = fp.item_pointers;

	for (auto it = id_pointers.begin(); it != id_pointers.end(); it++)
	{
		set<int> initial_1 = initial;
		initial_1.insert(it -> first);
		
		int current = it -> first;
		vector<node *> X = (fp.item_pointers)[current];
		
		int sum = 0;
		
		for (int j = 0; j < X.size(); j++)
		{
			sum += X[j] -> counter;
		}

		(L[initial_1.size()])[initial_1] = sum;
		
		vector<vector<int>> Transaction_1;
		vector<int> counts;
		tree fp_1 = tree();
		map<int, int> item_frequency_1;

		for (int j = 0; j < X.size(); j++)
		{
			Transaction_1.push_back(vec());
			stack<int> S;

			node* temp = X[j] -> back;
			
			while((temp -> value) != INT_MIN)
			{
				S.push(temp -> value);
				temp = temp -> back;
			}

			while(!S.empty())
			{
				Transaction_1[Transaction_1.size() - 1].push_back(S.top());
				int num = S.top();

				if (item_frequency_1.find(num) == item_frequency_1.end()){

					item_frequency_1[num] = X[j] -> counter;
				
				} else {
					
					item_frequency_1[num] = item_frequency_1[num] + X[j] -> counter;
				}
				S.pop();
			}

			counts.push_back(X[j] -> counter);
		}

		for (int j = 0; j < Transaction_1.size(); j++){
			
			fp_1.reset();
			for (int k = 0; k < Transaction_1[j].size(); k++){

				if(item_frequency_1[Transaction_1[j][k]] >= numsup)
				{
					fp_1.add_c(Transaction_1[j][k], counts[j]);
				}
			}
		}

		fpgrowth(fp_1, initial_1);
	}
}

bool cmp(const int &x, const int &y)
{
	return item_frequency[x] > item_frequency[y];
}

int main(int argc, char * argv[])
{
	ifstream input_file(argv[1]);

	double minsup;
	double support;

	cout << "Enter Support (%) ";
   	cin >> support;
   
   	minsup = support / 100;

	vector<vector<int>> Transaction;
	string line;

	while(getline(input_file, line))
	{
		Transaction.push_back(vec());
		
		stringstream sin;
		sin << line;
		
		int num;

		while(sin >> num)
		{
			Transaction[Transaction.size() - 1].push_back(num);

			if (item_frequency.find(num) == item_frequency.end()){

				item_frequency[num] = 1;

			} else {

				item_frequency[num] = item_frequency[num] + 1;
			}
		}
	}

	numsup = (round)((double)Transaction.size() * minsup);

	//Sorting of items in each transaction as per their frequency
	for (int j = 0; j < Transaction.size(); j++)
	{
		sort(Transaction[j].begin(), Transaction[j].end(), cmp);
	}
	
	//Tree creation 
	tree fp = tree();

	for (int j = 0; j < Transaction.size(); j++){
		
		fp.reset();
		
		for (int k = 0; k < Transaction[j].size(); k++){
			
			if(item_frequency[Transaction[j][k]] >= numsup)
			{
				fp.add(Transaction[j][k]);
			}
		}
	}

	set<int> initial;
	
	fpgrowth(fp, initial);
	fmaprint(L);

	return 0;
}
