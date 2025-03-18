#include <bits/stdc++.h>
using namespace std;


struct table {
    int rid; 
    map<string, string> attributes;
    
};

struct de_tree {
    string field; 
    map<string, de_tree*> children; 
};

double get_entropy(vector<table> data, string att) {
    map<string, int> freq;
    for (auto& i : data) {
        freq[i.attributes[att]]++;
    }

    int n = data.size();
    double entropy = 0;
    for (auto& i : freq) {
        double prob = (double)i.second / (double)n;
        entropy -= prob * log2(prob);
    }
    return entropy;
}

vector<table> filter_data(vector<table> data, string att_val, string curr_att) {
    vector<table> res;
    for (auto& i : data) {
        if (i.attributes[curr_att] == att_val) {
            res.push_back(i);
        }
    }
    return res;
}

string find_majority(vector<table> data, string att) {
    map<string, int> freq;
    for (auto& i : data) {
        freq[i.attributes[att]]++;
    }

    int majority_class_freq = 0;
    string majority_class;
    for (auto& i : freq) {
        if (majority_class_freq < i.second) {
            majority_class_freq = i.second;
            majority_class = i.first;
        }
    }
    return majority_class;
}


double get_gain(vector<table> data, string att, double entropy) {
    int n = data.size();
    map<string, int> att_freq;
    double gain = 0;
    for (auto& i : data) {
        att_freq[i.attributes[att]]++;
    }
    for (auto& i : att_freq) {
        vector<table> curr_att_data = filter_data(data, i.first, att);
        double att_val_entropy = get_entropy(curr_att_data, "buyscomputer");
        gain += ((double)i.second / (double)n) * att_val_entropy;
    }
    return (entropy - gain);
}


de_tree* create_tree(vector<string> attributes, vector<table> data) {
    double entropy = get_entropy(data, "buyscomputer");

    if (entropy == 0) {
        de_tree* root = new de_tree();
        root->field = data[0].attributes["buyscomputer"];
        return root;
    }

    if (attributes.empty()) {
        de_tree* root = new de_tree();
        root->field = find_majority(data, "buyscomputer");
        return root;
    }

    double highest_gain = 0;
    string highest_gain_att;
    for (auto& att : attributes) {
        double gain = get_gain(data, att, entropy);
        if (gain > highest_gain) {
            highest_gain = gain;
            highest_gain_att = att;
        }
    }

    de_tree* root = new de_tree();
    root->field = highest_gain_att;

    auto it = find(attributes.begin(), attributes.end(), highest_gain_att);
    if (it != attributes.end()) attributes.erase(it);

    map<string, int> root_att_freq;
    for (auto& i : data) {
        root_att_freq[i.attributes[highest_gain_att]]++;
    }

    for (auto& i : root_att_freq) {
        vector<table> curr_att_data = filter_data(data, i.first, highest_gain_att);
        root->children[i.first] = create_tree(attributes, curr_att_data);
    }
    return root;
}


vector<table> bootstrap_sample(const vector<table>& data) {
    vector<table> sample;
    set<int> used_rids;
    int n = data.size();
    int sample_size = n;

    while (sample.size() < sample_size) {
        int random_index = rand() % n;
        const table& selected_row = data[random_index];

        if (used_rids.find(selected_row.rid) == used_rids.end()) {
            sample.push_back(selected_row);
            used_rids.insert(selected_row.rid);
        }
    }

	fstream file2;
	file2.open("output.txt",ios::app);
   
    file2 << "Bootstrap Sample:" << endl;
    for (const auto& row : sample) {
        file2 << "RID: " << row.rid
             << " age: " << row.attributes.at("age")
             << " income: " << row.attributes.at("income")
             << " student: " << row.attributes.at("student")
             << " credit_rating: " << row.attributes.at("credit_rating")
             << " buyscomputer: " << row.attributes.at("buyscomputer") << endl;
    }
    file2 << "End of Bootstrap Sample" << endl;
	file2.close();
    return sample;
}

void print_tree(de_tree* node, string indent = "") {
    if (node == nullptr) return;
	
	fstream file2;
	file2.open("output.txt",ios::app);
    file2 << indent << "Node: " << node->field << endl;
    if (node->children.empty()) {
        file2 << indent << "  Leaf: " << node->field << endl;
    } else {
        for (const auto& child : node->children) {
            file2 << indent << "  Branch: " << child.first << " ->" << endl;
            print_tree(child.second, indent + "    ");
        }
    }
    file2.close();
}


vector<string> select_random_attributes(const vector<string>& attributes, int num_attributes) {
    num_attributes = min(num_attributes, (int)attributes.size());
    vector<string> selected_attributes = attributes;

    
    for (size_t i = 0; i < selected_attributes.size(); ++i) {
        int random_index = rand() % selected_attributes.size();
        swap(selected_attributes[i], selected_attributes[random_index]);
    }

    selected_attributes.resize(num_attributes);

	fstream file2;
	file2.open("output.txt",ios::app);
    file2 << "Selected Unique Attributes for Tree: ";
    for (const auto& attr : selected_attributes) {
        file2 << attr << " ";
    }
    file2 << endl;
    file2.close();

    return selected_attributes;
}


vector<de_tree*> create_random_forest(vector<table> data, vector<string> attributes, int num_trees, int num_attributes) {
    vector<de_tree*> forest;
    for (int i = 0; i < num_trees; ++i) {
    	
    	fstream file2;
		file2.open("output.txt",ios::app);
        file2 << "Creating Tree " << i + 1 << "..." << endl;
        file2.close();
      
        vector<table> sample = bootstrap_sample(data);
        vector<string> selected_attributes = select_random_attributes(attributes, num_attributes);

        
        de_tree* tree = create_tree(selected_attributes, sample);
        forest.push_back(tree);

       	
		file2.open("output.txt",ios::app);
        file2 << "Tree " << i + 1 << " structure:" << endl;
        file2.close();
        print_tree(tree);
        
        
		file2.open("output.txt",ios::app);
        file2 << "End of Tree " << i + 1 << endl << endl;
        file2.close();
    }
    return forest;
}


string solve_query(de_tree* root, table& query) {
    if (root == nullptr) return "";
    if (root->children.empty()) return root->field;

    auto it = root->children.find(query.attributes[root->field]);
    if (it != root->children.end()) {
        return solve_query(it->second, query);
    } else {
        return "";
    }
}


string forest_predict(vector<de_tree*> forest, table& query) {
    map<string, int> votes;
    for (auto& tree : forest) {
        string prediction = solve_query(tree, query);
        votes[prediction]++;
    }

    string majority_vote;
    int max_votes = 0;
    for (const auto& vote : votes) {
        if (vote.second > max_votes) {
            max_votes = vote.second;
            majority_vote = vote.first;
        }
    }
    return majority_vote;
}

int main() {
	
	fstream file2;
	file2.open("output.txt",ios::out);
	file2.close();
	
	
    ifstream file("input.txt");
    vector<table> data;
    string str;
    getline(file, str); 
    while (getline(file, str)) {
        stringstream ss(str);
        table t;
        string value;
        getline(ss, value, ' ');
        t.rid = stoi(value);
        getline(ss, t.attributes["age"], ' ');
        getline(ss, t.attributes["income"], ' ');
        getline(ss, t.attributes["student"], ' ');
        getline(ss, t.attributes["credit_rating"], ' ');
        getline(ss, t.attributes["buyscomputer"], ' ');
        data.push_back(t);
    }
    file.close();

    vector<string> attributes{"age", "income", "student", "credit_rating"};
    int num_trees = 3; 
    int num_attributes = 4;

    vector<de_tree*> forest = create_random_forest(data, attributes, num_trees, num_attributes);

  
    ifstream query_file("query.txt");
    while (getline(query_file, str)) {
        stringstream ss(str);
        table query;
        getline(ss, query.attributes["age"], ' ');
        getline(ss, query.attributes["income"], ' ');
        getline(ss, query.attributes["student"], ' ');
        getline(ss, query.attributes["credit_rating"], ' ');

		
        string prediction = forest_predict(forest, query);
        
		fstream file2;
		file2.open("output.txt",ios::app);
        file2 << "Query: " << str << " -> Prediction: " << prediction << endl;
        file2.close();

    }
    query_file.close();

    return 0;
}