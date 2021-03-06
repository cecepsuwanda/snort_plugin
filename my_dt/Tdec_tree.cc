#include "Tdec_tree.h"


Tdec_tree::Tdec_tree()
{

}

Tdec_tree::~Tdec_tree()
{

}

bool Tdec_tree::check_purity(Tdataframe &df)
{
  map<string, int> unique;
  //unique = df.get_unique_value(df.getjmlcol()-1);
  unique = df.get_stat_label();
  if (unique.size() == 1)
  {
    unique.clear();
    return true;
  } else {
    unique.clear();
    return false;
  }

}

map<int, map<string, int>> Tdec_tree::get_potential_splits(Tdataframe &df)
{
  cout << "get_potential_splits" << endl;
  map<int, map<string, int>> tmp;
  for (int i = 0; i < (df.getjmlcol() - 1); ++i)
  {
    tmp.insert(pair<int, map<string, int>> (i, df.get_unique_value(i)));
  }
  return tmp;
}



void Tdec_tree::determine_best_split(Tdataframe &df, int &split_column, string &split_value)
{
  // cout << "determine_best_split " << endl;
  float current_overall_metric, best_overall_metric = 999;
  bool first_iteration = true;
  string current_split_value;
  map<string, pot_struct> _col_pot_split;

  for (int i = 0; i < (df.getjmlcol() - 1)  ; ++i)
  {
    
      // cout << "          Kolom " << df.get_nm_header(i) << endl;
      df.get_col_pot_split(i, _col_pot_split);
      df.calculate_overall_metric(i, _col_pot_split, current_overall_metric, current_split_value);
      
      // cout << "          current_overall_metric : " << current_overall_metric << endl;

    if (first_iteration or (current_overall_metric <= best_overall_metric))
    {
      first_iteration = false;

      best_overall_metric = current_overall_metric;
      split_column = i;
      split_value = current_split_value;

      // cout << "                    best_overall_metric : " << best_overall_metric << endl;
      // cout << "                    split_column        : " << df.get_nm_header(split_column) << endl;
      // cout << "                    split_value         : " << split_value << endl;
    }    

  }

  // cout << "Enter key to continue ..." << endl;cin.get();
}

string Tdec_tree::create_leaf(Tdataframe &df)
{
  map<string, int>tmp_stat = df.get_stat_label();
  bool is_first = true;
  int vmax;
  string vlabel;
  map<string, int>::iterator it = tmp_stat.begin();
  while (it != tmp_stat.end())
  {
    if (is_first)
    {
      is_first = false;
      vlabel = it->first;
      vmax   = it->second;
    } else {
      if (it->second > vmax)
      {
        vlabel = it->first;
        vmax   = it->second;
      }
    }
    it++;
  }

  return vlabel;
}

void Tdec_tree::train(Tdataframe &df, int node_index , int counter, int min_samples, int max_depth)
{
  if (node_index == 0)
  {
    Node root;
    root.treeIndex = 0;
    tree.push_back(root);
  }

  //cout << "tree level : " << counter << endl;

  if (check_purity(df) or (df.getjmlrow() < min_samples) or (counter == max_depth) )
  {
    string tmp_str = create_leaf(df);
    //cout << "base case" << endl;
    //cout << "label : " << tmp_str << endl;
    tree[node_index].isLeaf = true;
    tree[node_index].label = tmp_str;
    df.clear_memory();
  } else {

    counter++;

    int split_column;
    string split_value; 

    // clock_t start, end;
    // start = clock();

    determine_best_split(df, split_column, split_value);

    // end = clock();

    // double time_taken = double(end - start) / double(CLOCKS_PER_SEC);

    // cout << fixed << time_taken << setprecision(5) << endl;

    // cout << "split_column : " << split_column << " " << df.get_nm_header(split_column) << " split_value : " << split_value <<  endl;

    Tdataframe df_below, df_above;
    df_below = df;
    df_above = df;
    df.split_data(split_column, split_value, df_below, df_above);

    if ((df_below.getjmlrow() == 0) or (df_above.getjmlrow() == 0)) {
      string tmp_str = create_leaf(df);
      //cout << "empty data" << endl;
      //cout << "label : " << tmp_str << endl;
      tree[node_index].isLeaf = true;
      tree[node_index].label = tmp_str;
    } else {
      tree[node_index].criteriaAttrIndex = split_column;


      int treeIndex_yes, treeIndex_no;

      Node nextNode;
      nextNode.treeIndex = (int)tree.size();
      nextNode.attrValue = split_value;
      nextNode.opt = df.get_opt(split_column, 1);
      treeIndex_yes = nextNode.treeIndex;
      tree[node_index].children.push_back(nextNode.treeIndex);
      tree.push_back(nextNode);

      // cout << tree[node_index].criteriaAttrIndex << " " << df.get_nm_header(tree[node_index].criteriaAttrIndex) << (nextNode.opt == 0 ? "<=" : "==") << nextNode.attrValue << endl;
      train(df_below, nextNode.treeIndex, counter, min_samples, max_depth);

      Node nextNode1;
      nextNode1.treeIndex = (int)tree.size();
      nextNode1.attrValue = split_value;
      nextNode1.opt = df.get_opt(split_column, 0);
      treeIndex_no = nextNode1.treeIndex;
      tree[node_index].children.push_back(nextNode1.treeIndex);
      tree.push_back(nextNode1);

      // cout << tree[node_index].criteriaAttrIndex << " " << df.get_nm_header(tree[node_index].criteriaAttrIndex) << (nextNode1.opt == 1 ? ">" : "!=") << nextNode1.attrValue << endl;
      train(df_above, nextNode1.treeIndex, counter, min_samples, max_depth);

      df_above.clear_memory();
      df_below.clear_memory();

      if (((tree[treeIndex_yes].isLeaf == true) and (tree[treeIndex_no].isLeaf == true)) and (tree[treeIndex_yes].label == tree[treeIndex_no].label))
      {
        // cout << "tree level : " << counter << endl;
        // cout << "label sama " << endl;
        tree[node_index].isLeaf = true;
        //tree[node_index].attrValue = tree[treeIndex_yes].attrValue;
        tree[node_index].label = tree[treeIndex_yes].label;
        tree[node_index].children.clear();
        tree[node_index].children.shrink_to_fit();
        tree.erase(tree.begin() + treeIndex_no);
        tree.erase(tree.begin() + treeIndex_yes);
        tree.shrink_to_fit();
        // cout << "label : " << tree[node_index].label << endl;
      }

    }

  }


}

int Tdec_tree::dfs(Tdataframe &df, vector<string> &data, int treeIndex)
{
  if (tree[treeIndex].isLeaf)
  {
    return treeIndex;
  }

  int criteriaAttrIndex = tree[treeIndex].criteriaAttrIndex;

  for (int i = 0; i < tree[treeIndex].children.size(); i++) {
    int next = tree[treeIndex].children[i];

    //if (tree[next].isLeaf)
    //{
     // return next;
   // } else {
      if (df.is_pass(tree[next].opt, data[criteriaAttrIndex], tree[next].attrValue)) {
        return dfs(df, data, next);
      }
   // }

  }

  return -1;
}

string Tdec_tree::guess(Tdataframe &df, vector<string> &data)
{
  string label = "failed.";
  int leafNode = dfs(df, data, 0);
  if (leafNode == -1) {
    return "dfs_failed.";
  }else{
     label = tree[leafNode].label;
  }
  return label;
}

void Tdec_tree::test(Tdataframe &df)
{
  
  Tconf_metrix conf_metrix;
  vector<string> tmp_data;
  string tmp_str, tmp_str1;
  

  if (df.open_file())
  {
    
    int failed = 0;
    df.read_file();
    while (!df.is_eof()) {

      tmp_data = df.get_record();

      tmp_str1 = tmp_data[tmp_data.size() - 1];

      tmp_data.erase(tmp_data.end());

      tmp_str = guess(df, tmp_data);
      if (tmp_str != "dfs failed")
      {
        //cout << tmp_str1 << "  " << tmp_str << endl;
        conf_metrix.add_jml(tmp_str1,tmp_str,1);

      } else {
        failed++;
      }

      df.next_record();
      
    }

    df.close_file();
    //cout << " Jumlah Data : " << jml_data << " Prediksi Tepat : " << tepat << " Failed : " << failed << " Prosentase : " << ((tepat / (double) jml_data) * 100) << endl;
    conf_metrix.kalkulasi();
    cout << conf_metrix << endl;
  } else {
    cout << "Gagal buka file !!!" << endl;
  }
}

void Tdec_tree::save_tree(Tdataframe &df)
{
  string tmp_str = "";
  vector<string> vec;

  for (int i = 0; i < tree.size(); ++i)
  {
    tmp_str = to_string(tree[i].criteriaAttrIndex) + "," + tree[i].attrValue + "," + tree[i].label + "," + to_string(tree[i].treeIndex) + "," + (tree[i].isLeaf == true ? "1" : "0") + "," + to_string(tree[i].opt) ;
    
    if (tree[i].children.size() > 0)
    {
      if(tree[i].children.size()==2){
        tmp_str += "," + to_string(tree[i].children[0]) + "," + to_string(tree[i].children[1]);
      }else{
        cout << "Ada yg kurang !!!";
      }
    
    } else {
      tmp_str += ",-1,-1";
    }

    // cout << tmp_str << endl;
    vec.push_back(tmp_str);
  }

  df.write_data(vec);
}


void Tdec_tree::read_tree(Tdataframe &df)
{
  vector<string> tmp_data;

  if (df.open_file())
  {
    df.read_file();
    while (!df.is_eof()) {

      tmp_data = df.get_record();

      //cout << tmp_data.size() << endl;

      Node newnode;
      //cout << tmp_data[0] << endl;
      newnode.criteriaAttrIndex = tmp_data[0] == "-1" ?  -1 : stoi(tmp_data[0]);
      newnode.attrValue = tmp_data[1];
      newnode.label = tmp_data[2];
      //cout << tmp_data[2] << endl;
      newnode.treeIndex = tmp_data[3] == "-1" ? -1 : stoi(tmp_data[3]);
      newnode.isLeaf = tmp_data[4] == "1";
      //cout << tmp_data[4] << endl;
      newnode.opt = tmp_data[5] == "-1" ? -1 : stoi(tmp_data[5]);
      //cout << tmp_data[5] << endl;
      newnode.children.push_back(tmp_data[6] == "-1" ? -1 :  stoi(tmp_data[6]));
      //cout << tmp_data[6] << endl;
      newnode.children.push_back(tmp_data[7] == "-1" ? -1 :  stoi(tmp_data[7]));

      tree.push_back(newnode);

      df.next_record();
    }

    df.close_file();
  } else {
    cout << "Gagal buka file !!!" << endl;
  }
}
