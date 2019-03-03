#include <bits/stdc++.h>
using namespace std;

int total_no_of_transactions = 0;
int MAX = INT_MIN;
double min_sup, sup;

vector <set<int>> total_transactions; // to hold itemsets of each transaction
vector <map<set<int>, int>> level_passes; // to hold L1, L2, ....., Lk itemsets generated in k passes or k iterations
map <set<int>, string> closedFrequentOrNot;
map <set<int>,int> itemsets;

// to convert string representation of a number to integer value
int string_to_int_val(string &x)
{
    int ans = 0;
    
    for(int i = 0; i < x.size(); i++){
        ans = ans * 10 + (x[i] - '0');
    }

    return ans;
}

// to show frequent itemsets generated in k iterations
void display()
{
	for(int i = 0; i < level_passes.size(); i++){
		
        cout << "Iteration " << i + 1 << endl;
		map <set<int>, int>  m = level_passes[i];
		
        for(auto it = m.begin(); it != m.end(); it++){
			
            set <int> s = it -> first;
			cout << "[ ";

            for(auto it1 = s.begin(); it1 != s.end(); it1++){
				cout << *it1 << " ";
			}
			
            cout << "] --> Support Count : " << it -> second << " --> " << closedFrequentOrNot[s] << endl;
		}

		cout << endl;
	}
}

// to decide whether a frequent itemset is closed or not
void check_closed()
{
	for(int i = 0; i < level_passes.size(); i++){
		
        if(i == level_passes.size() - 1){

			map <set<int>, int> m_last = level_passes[i];
			
            for(auto it = m_last.begin(); it != m_last.end(); it++){
				
                set <int> s = it -> first;
				closedFrequentOrNot[s] = "Closed Frequent Itemset";
			}

			continue;
		}

		map <set<int>, int> m_current = level_passes[i], m_next = level_passes[i + 1];

		for(auto it = m_current.begin(); it != m_current.end(); it++){
			
            int flag = 0; // to test if itemset is closed or not
			set <int> s1 = it -> first;
			
            for(auto it2 = m_next.begin(); it2 != m_next.end(); it2++){
				
                set <int> s2 = it2 -> first;
				
                if(includes(s2.begin(), s2.end(), s1.begin(), s1.end())){
					
                    if(it -> second == it2 -> second){
						flag = 1;
                        closedFrequentOrNot[s1] = "Frequent Itemset";
						break;
					}
				}
			}

			if(flag == 0){
				closedFrequentOrNot[s1] = "Closed Frequent Itemset";
			}	
		}
	}

	display();
}

// converting string numbers to integer values of each transaction
void transaction_string_to_int(string line)
{
    string x = "";
    
    set<int> transaction;
    
    for(int i = 0; i < line.size(); i++){
       
        if(line[i] == ' '){
            transaction.insert(string_to_int_val(x));
            MAX = max(MAX, string_to_int_val(x));
            x = "";
        } else {
            x += line[i];
        }    
    }

    transaction.insert(string_to_int_val(x));
    MAX = max(MAX, string_to_int_val(x));
    
    total_transactions.push_back(transaction);
}

void apriori_algo()
{
    // for generation of 1 itemset candidate set that is C1
    for(int i = 1; i <= MAX; i++){
        
        int count = 0;
        set <int> s;
        s.insert(i);
        
        for(int j = 0; j < total_transactions.size(); j++){
            if(total_transactions[j].find(i) != total_transactions[j].end()){
                count++;
            }
        }

        double calculated_sup = (double)count / (double)total_no_of_transactions;
        if(calculated_sup >= min_sup){ 
     		itemsets[s] = count;
        }
    }

    map <set<int>, int>::iterator it, it_adj;
    level_passes.push_back(itemsets);

    // for generation of 2, ...., k itemset candidate sets that is C2, ...., Ck
    while(1){
        
        map <set<int>, int> Lk;

        // Generation of k itemset from k-1 itemset
        for(it = itemsets.begin(); it != itemsets.end(); it++){
            it_adj = ++it;
            it--;

            for(; it_adj != itemsets.end(); it_adj++){
                
                set <int> s1 = (it -> first), s2 = (it_adj -> first);
                set <int> sk;

                set <int> :: iterator it1 = s1.begin(), it2 = s2.begin();
                int flag = 0; // to test for matching k - 2 elements during Ck generation
                
                // for matching k - 2 elements during Ck generation
                for(int i = 0; i < s1.size() - 1; i++){
                    
                    if(*it1 != *it2){
                        flag = 1;
                        break;
                    }

                    sk.insert(*it1);
                    
                    it1++;
                    it2++;
                }

                // if k - 2 elements of two itemsets match then generate k itemset element from the two k - 1 itemsets element selected
                if(flag == 0){

                    int flag1 = 0; // to test if all k - 1 elements subset of Ck itemset generated lie in the k - 1 itemsets list (L(k-1))
                    
                    sk.insert(*it1);
                    sk.insert(*it2);
                    
                    set <int> subsets_sk = sk;
                    it1 = sk.begin();

                    // Checking whether all k - 1 elements subsets generated from k itemset element formed do lie in the k - 1 itemsets list (L(k-1))
                    for(int i = 0; i < sk.size(); i++){
                        
                        int tmp = *it1;
                        subsets_sk.erase(tmp);
                        
                        if(itemsets.find(subsets_sk) == itemsets.end()){
                            flag1 = 1;
                            break;
                        }

                        subsets_sk.insert(tmp);
                        it1++;
                    }

                    // To find the support count of k itemset element generated
                    if(flag1 == 0){
                        
                        int count = 0;
                        
                        for(int i = 0; i < total_transactions.size(); i++){
                            
                            int flag2 = 0; // to test whether all elements of k itemset lies in a given single transaction for calculating count

                            for(it1 = sk.begin(); it1 != sk.end(); it1++){
                                
                                if(total_transactions[i].find(*it1) == total_transactions[i].end()){
                                    flag2 = 1;
                                    break;
                                }
                            }

                            if(flag2 == 0){
                                count++;
                            }
                        }

                        double calculated_sup = (double)count / (double)total_no_of_transactions;
                        if(calculated_sup >= min_sup) 
                            Lk[sk] = count;
                    }
                }
            }
        }

        if(Lk.size() == 0){
            break;
        } else {
            itemsets = Lk;
            level_passes.push_back(itemsets);
        }
    }

    check_closed();
}

int main(int argc, char * argv[]){
    
    ifstream myfile(argv[1]);
   	cout << "Enter Support (%) ";
   	cin >> sup;
   	min_sup = sup / 100;

    if(!myfile.is_open())
        exit(EXIT_FAILURE);

    string line;
    
    while ((getline(myfile, line))){
        transaction_string_to_int(line);
        total_no_of_transactions++;
    }

    cout << "Total Number of Transactions are " << total_no_of_transactions << endl;
    apriori_algo();
    
    myfile.close();
}
