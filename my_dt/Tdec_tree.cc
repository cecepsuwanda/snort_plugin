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
  cout << "determine_best_split " << endl;
  float current_overall_metric, best_overall_metric = 999;
  bool first_iteration = true;
  string current_split_value;
  map<string, pot_struct> _col_pot_split;

  for (int i = 0; i < (df.getjmlcol() - 1) ; ++i)
  {
    //cout << "Kolom " << i << endl;
    df.get_col_pot_split(i, _col_pot_split);
    df.calculate_overall_metric(i, _col_pot_split, current_overall_metric, current_split_value);

    if (first_iteration or (current_overall_metric <= best_overall_metric))
    {
      first_iteration = false;

      best_overall_metric = current_overall_metric;
      split_column = i;
      split_value = current_split_value;

      //cout << current_overall_metric << endl;
      //cout << split_column << endl;
      //cout << split_value << endl;
    }

  }


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


  if (check_purity(df) or (df.getjmlrow() < min_samples) or (counter == max_depth) )
  {
    string tmp_str = create_leaf(df);
    cout << "label : " << tmp_str << endl;
    tree[node_index].isLeaf = true;
    tree[node_index].attrValue = tmp_str;
  } else {

    cout << "tree level : " << counter << endl;
    counter++;

    int split_column;
    string split_value;


    determine_best_split(df, split_column, split_value);

    cout << "split_column : " << split_column << " " << df.get_nm_header(split_column) << " split_value : " << split_value << endl;

    Tdataframe df_below, df_above;
    df_below = df;
    df_above = df;
    df.split_data(split_column, split_value, df_below, df_above);

    if ((df_below.getjmlrow() == 0) or (df_above.getjmlrow() == 0)) {
      string tmp_str = create_leaf(df);
      cout << "label : " << tmp_str << endl;
      tree[node_index].isLeaf = true;
      tree[node_index].attrValue = tmp_str;
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
      cout << tree[node_index].criteriaAttrIndex << " " << df.get_nm_header(tree[node_index].criteriaAttrIndex) << (nextNode.opt == 0 ? "<=" : "==") << nextNode.attrValue << endl;
      train(df_below, nextNode.treeIndex, counter, min_samples, max_depth);

      Node nextNode1;
      nextNode1.treeIndex = (int)tree.size();
      nextNode1.attrValue = split_value;
      nextNode1.opt = df.get_opt(split_column, 0);
      treeIndex_no = nextNode1.treeIndex;
      tree[node_index].children.push_back(nextNode1.treeIndex);
      tree.push_back(nextNode1);

      cout << tree[node_index].criteriaAttrIndex << " " << df.get_nm_header(tree[node_index].criteriaAttrIndex) << (nextNode1.opt == 1 ? ">" : "!=") << nextNode1.attrValue << endl;
      train(df_above, nextNode1.treeIndex, counter, min_samples, max_depth);

      if (((tree[treeIndex_yes].isLeaf == true) and (tree[treeIndex_no].isLeaf == true)) and (tree[treeIndex_yes].attrValue == tree[treeIndex_no].attrValue))
      {
        cout << "label sama " << endl;
        tree[node_index].isLeaf = true;
        tree[node_index].attrValue = tree[treeIndex_yes].attrValue;
        tree[node_index].children.clear();
        tree[node_index].children.shrink_to_fit();
        tree.erase(tree.begin() + treeIndex_no);
        tree.erase(tree.begin() + treeIndex_yes);
        cout << "label : " << tree[node_index].attrValue << endl;
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

    if (tree[next].isLeaf)
    {
       return next;
    }else{
      if (df.is_pass(tree[next].opt, data[criteriaAttrIndex], tree[next].attrValue)) {
         return dfs(df, data, next);
      }
    }  

  }

  return -1;
}

string Tdec_tree::guess(Tdataframe &df, vector<string> &data)
{
  string label = "";
  int leafNode = dfs(df, data, 0);
  if (leafNode == -1) {
    return "dfs failed";
  }
  label = tree[leafNode].attrValue;
  return label;
}

void Tdec_tree::test(Tdataframe &df)
{
  vector<string> tmp_data;
  string tmp_str, tmp_str1;
  int TP=0,TN=0,FP=0,FN=0;

  if (df.open_file())
  {
    int jml_data = 0;
    int tepat = 0;
    int failed = 0;
    df.read_file();
    while (!df.is_eof()) {

      tmp_data = df.get_record();
      //cout << tmp_data.size() << endl;

      tmp_str1 = tmp_data[tmp_data.size() - 1];
      //cout << tmp_str1 << endl;

      tmp_data.erase(tmp_data.end());
      //cout << tmp_data.size() << endl;

      tmp_str = guess(df, tmp_data);
      
      if(tmp_str!="dfs failed")
       { 
        if (tmp_str == tmp_str1)
        {
          tepat++;
        }

        if((tmp_str1 != "normal") and (tmp_str!="normal"))
        {
          TP++; 
        }

        if((tmp_str1 == "normal") and (tmp_str=="normal"))
        {
          TN++; 
        }

        if((tmp_str1 == "normal") and (tmp_str!="normal"))
        {
          FP++; 
        }

        if((tmp_str1 != "normal") and (tmp_str=="normal"))
        {
          FN++; 
        }

       }else{
          failed++;
       } 

      df.next_record();
      jml_data++;
    }

    df.close_file();
    cout << "Jumlah Data : " << jml_data << " Prediksi Tepat : " << tepat << " Failed : " << failed << " Prosentase : " << ((tepat/(double) jml_data)*100) << endl;
    cout << "TP : " << TP << " TN : " << TN << " FP : " << FP << " FN : " << FN << endl;    
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
    tmp_str = to_string(tree[i].criteriaAttrIndex) + "," + tree[i].attrValue + "," + to_string(tree[i].treeIndex) + "," + (tree[i].isLeaf == true ? "1" : "0") + "," + to_string(tree[i].opt) ;
    if (tree[i].children.size() > 0)
    {
      tmp_str += "," + to_string(tree[i].children[0]) + "," + to_string(tree[i].children[1]);
    } else {
      tmp_str += ",-1,-1";
    }
    cout << tmp_str << endl;
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
      newnode.criteriaAttrIndex = tmp_data[0]=="-1" ?  -1 : stoi(tmp_data[0]);
      newnode.attrValue = tmp_data[1];
      //cout << tmp_data[2] << endl;
      newnode.treeIndex = tmp_data[2]=="-1" ? -1 : stoi(tmp_data[2]);
      newnode.isLeaf = tmp_data[3] == "1";
      //cout << tmp_data[4] << endl;
      newnode.opt = tmp_data[4]=="-1" ? -1 : stoi(tmp_data[4]);
      //cout << tmp_data[5] << endl;
      newnode.children.push_back(tmp_data[5]=="-1" ? -1 :  stoi(tmp_data[5]));
      //cout << tmp_data[6] << endl;
      newnode.children.push_back(tmp_data[6]=="-1" ? -1 :  stoi(tmp_data[6]));

      tree.push_back(newnode);

      df.next_record();
    }

    df.close_file();
  } else {
    cout << "Gagal buka file !!!" << endl;
  }
}
