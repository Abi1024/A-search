//Abiyaz Chowdhury: this program uses A* search to compute the least number of edits needed to transform a word into another
//word of equal length. All intermediate words must be valid English words. Hamming distance is used as an admissible heuristic //to guide the search.
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
using namespace std;
map<int,vector<string> > words_by_length;
map<string,vector<string> > adjacent_words;
void computeAdjacentWords();
string shortest_path(const string& a, const string& b);

class Node{
	public:
		string word;
		int distance;
		Node(string w, int d): word(w),distance(d){}
		bool operator> (const Node& n2) const{
			return (distance > n2.distance);
		}
};

void printAdjacent(string s){
	const vector<string>& words = adjacent_words[s];
	for (int p = 0; p < words.size(); p++){
		cout << words[p] << endl;
	}
}

int main(){
	ifstream input;
	input.open("wordsEn.txt");
	string v;
	while (input >> v){
		words_by_length[v.length()].push_back(v);
	}
	computeAdjacentWords();
	string input1,input2;
	cout << "Enter the first string: ";
	cin >> input1;
	cout << "Enter the second string: ";
	cin >> input2;
	cout << shortest_path(input1,input2) << endl;
	return 0;
}

int hamming(const string& a, const string& b){
	int counter = 0;
	if (a.length() != b.length()){
		return -1;
	}
	for (int i = 0; i < a.length(); i++){
		if (a[i] != b[i]){
			counter++;
		}
	}
	return counter; 
	//return 0;
}

string shortest_path(const string& a, const string& b){ //from a to b
	int count = 0;
	string result;
	map<string,string> path_trace;
	map<string,int> path_length;
	path_trace[a] = "";
	path_length[a] = hamming(a,b);
	priority_queue<Node,vector<Node>,greater<Node> > q;
	q.push(Node(a,path_length[a]));
	int goal = 0;
	while (!(q.empty()) and  (goal == 0)){
		Node v = q.top();
		q.pop();
		if (path_length[v.word] == v.distance){
			count++;
			//cout << "Expanding node: " << v.word << " Current f value: " << v.distance << endl;
			vector<string> adjacency_list = adjacent_words[v.word];
			vector<string>::const_iterator itr;
			for (itr = adjacency_list.begin(); itr != adjacency_list.end(); itr++){
				string w = *itr;
				//cout << "\t" << w;
				if (path_length.find(w) == path_length.end()){
					path_length[w] = 1+path_length[v.word]+hamming(w,b)-hamming(v.word,b);
					path_trace[w] = v.word;
					q.push(Node(w,path_length[w]));
				}else if (1+path_length[v.word]+hamming(w,b)-hamming(v.word,b) < path_length[w]){
					path_length[w] = 1+path_length[v.word]+hamming(w,b)-hamming(v.word,b);
					path_trace[w] = v.word;
					q.push(Node(w,path_length[w]));
				}
			}
			if (v.word == b){
				goal = 1;
			}
		}
		//cout << endl;
	}
	string current_word = b;
	while (path_trace[current_word] != ""){
		result = current_word + " " + result;
		current_word = path_trace[current_word];
		if (path_trace[current_word] == ""){
			result = current_word + " " + result;
		}
	}
	cout << "Words searched: " << count++ << endl;
	return result;
}

void computeAdjacentWords(){
	map<int,vector<string> >::const_iterator itr;
	for (itr = words_by_length.begin(); itr != words_by_length.end(); itr++){ //for each word of a particular length
		const vector<string>& groupWords = itr->second;
		int groupNum = itr->first;
		for (int i = 0; i < groupNum; i++){ //for each position
			map<string,vector<string> > repToWord;
			for (int j = 0; j < groupWords.size(); j++){
				string rep = groupWords[j];
				rep.erase(i,1);
				repToWord[rep].push_back(groupWords[j]);
			}
			map<string,vector<string> >::const_iterator itr2;
			for (itr2 = repToWord.begin(); itr2 != repToWord.end(); itr2++){
				const vector<string>& clique = itr2->second;
				if (clique.size() >= 2){
					for (int p = 0; p < clique.size(); p++){
						for (int q = p+1; q < clique.size(); q++){
							adjacent_words[clique[p]].push_back(clique[q]);
							adjacent_words[clique[q]].push_back(clique[p]);
						}
					}
				}
			}
		}
	}
}