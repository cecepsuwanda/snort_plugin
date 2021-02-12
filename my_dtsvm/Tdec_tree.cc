#include "Tdec_tree.h"


Tdec_tree::Tdec_tree()
{
  idx_svm = 0;
  id_df=1;
}

Tdec_tree::~Tdec_tree()
{

}

Tdec_tree::Tdec_tree(int v_train_svm,int v_feature_selection,int v_normal_only)
{
  
  train_svm = v_train_svm==1;
  feature_selection = v_feature_selection==1;
  normal_only = v_normal_only==1;

  idx_svm = 0;
  id_df=1;
}

static void cetak_stdout(const char *s)
{
  fputs(s, stdout);
  fflush(stdout);
}

void Tdec_tree::cetak ( const char * format, ... )
{
  char buffer[256];
  va_list args;
  va_start (args, format);
  vsprintf (buffer, format, args);
  //perror (buffer);
  va_end (args);
  cetak_stdout(buffer);
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
  //cout << "determine_best_split " << endl;
  float current_overall_metric, max_gain = 0;
  bool first_iteration = true;
  string current_split_value;
  map<Tmy_dttype, Tlabel_stat> _col_pot_split;

  split_column = -1;
  split_value = "-1";

  for (int i = 0; i < (df.getjmlcol() - 1)  ; ++i)
  {

    //cout << "          Kolom " << df.get_nm_header(i) << endl;
    // if((i%10)==0)
    // {
    //   cetak(".");
    // }

    df.get_col_pot_split(i, _col_pot_split);
    df.calculate_overall_metric(i, _col_pot_split, current_overall_metric, current_split_value);

    //cout << "          current_overall_metric : " << current_overall_metric << endl;

    //gain = pure_entropy - sum_entropy;
    // cetak("{");
    // cetak(to_string(i).c_str());
    // cetak(",");
    // cetak(to_string(current_overall_metric).c_str());
    // cetak("}");

    if (first_iteration or (max_gain < current_overall_metric))
    {
      // if (gain >= 0)
      // {

      first_iteration = false;
      max_gain = current_overall_metric;

      // cetak(" max{");
      // cetak(to_string(i).c_str());
      // cetak(",");
      // cetak(to_string(max_gain).c_str());
      // cetak("}");


      split_column = i;
      split_value = current_split_value;
      // }
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

void Tdec_tree::train(Tdataframe &df, int node_index , int counter, int min_samples, int max_depth, double gamma, double nu)
{
  if (node_index == 0)
  {
    Node root;
    root.treeIndex = 0;
    tree.push_back(root);
  }

  //cout << counter;
  cetak(to_string(counter).c_str());

  if (check_purity(df) or (df.getjmlrow() < min_samples) or (counter == max_depth) )
  {
    string tmp_str = create_leaf(df);

    if (tmp_str == "normal") {
      //cout << "tree level : " << counter << endl;
      //cout << "base case" << endl;
      //cout << "jml data " << df.getjmlrow() << endl;
      cetak("*");


      // Tdataframe df_svm;
      //df_svm = df;

      idx_svm++;
      tree[node_index].idx_svm = idx_svm;

      //df_svm.clear_memory();
      if ((train_svm) ) //and (df.getjmlrow()<10000)
      {
        //df.ReFilter();
        cetak("{v");
        Tmy_svm my_svm(feature_selection, normal_only);
        my_svm.train(df, gamma, nu);
        my_svm.save_model(model_path + "/svm_model_" + to_string(idx_svm) + ".csv");
        cetak("}");
        //cetak("\n");
      }

    }
    //cout << "label : " << tmp_str << endl;
    tree[node_index].isLeaf = true;
    tree[node_index].label = tmp_str;
    
    //cetak("{del_mem_1}");
    df.clear_memory();
    cetak("\n");
  } else {

    cetak("?");

    counter++;

    int split_column;
    string split_value;

    // clock_t start, end;
    // start = clock();

    determine_best_split(df, split_column, split_value);

    // end = clock();

    // double time_taken = double(end - start) / double(CLOCKS_PER_SEC);

    // cout << fixed << time_taken << setprecision(5) << endl;

    //cout << "split_column : " << split_column << " " << df.get_nm_header(split_column) << " split_value : " << split_value <<  endl;

    Tdataframe df_below, df_above;
    df_below = df;
    df_below.set_id(id_df++);
    df_above = df;
    df_above.set_id(id_df++);

    df.split_data(split_column, split_value, df_below, df_above);


    if ((df_below.getjmlrow() == 0) or (df_above.getjmlrow() == 0)) {
      string tmp_str = create_leaf(df);

      if (tmp_str == "normal") {
        //cout << "tree level : " << counter << endl;
        //cout << "empty data" << endl;
        //cout << "jml data " << df.getjmlrow() << endl;

        cetak("-");

        

        idx_svm++;
        tree[node_index].idx_svm = idx_svm;

        if ((train_svm)) //and (df.getjmlrow()<10000)
        {
          
          cetak("{v"); 
          Tmy_svm my_svm(feature_selection, normal_only);
          my_svm.train(df, gamma, nu);
          my_svm.save_model(model_path + "/svm_model_" + to_string(idx_svm) + ".csv");
          cetak("}"); 
          
        }

      }
      //cout << "label : " << tmp_str << endl;
      tree[node_index].isLeaf = true;
      tree[node_index].label = tmp_str;

      // cetak("\n");

    } else {

      cetak("|");

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
      cetak("->");
      df_below.ReFilter();
      train(df_below, nextNode.treeIndex, counter, min_samples, max_depth, gamma, nu);
      df_below.clear_memory();

      Node nextNode1;
      nextNode1.treeIndex = (int)tree.size();
      nextNode1.attrValue = split_value;
      nextNode1.opt = df.get_opt(split_column, 0);
      treeIndex_no = nextNode1.treeIndex;
      tree[node_index].children.push_back(nextNode1.treeIndex);
      tree.push_back(nextNode1);

      // cout << tree[node_index].criteriaAttrIndex << " " << df.get_nm_header(tree[node_index].criteriaAttrIndex) << (nextNode1.opt == 1 ? ">" : "!=") << nextNode1.attrValue << endl;
      cetak("<-");
      df_above.ReFilter();
      train(df_above, nextNode1.treeIndex, counter, min_samples, max_depth, gamma, nu);
      df_above.clear_memory();
      

      if (((tree[treeIndex_yes].isLeaf == true) and (tree[treeIndex_no].isLeaf == true)) and (tree[treeIndex_yes].label == tree[treeIndex_no].label))
      {


        tree[node_index].isLeaf = true;
        tree[node_index].attrValue = tree[treeIndex_yes].attrValue;

        string tmp_str = tree[treeIndex_yes].label;

        if (tmp_str == "normal") {
          //cetak(to_string(counter).c_str());
          cetak("+");

          //cout << "label sama " << endl;
          //cout << "Jml data1 " << df_above.getjmlrow() << endl;
          //cout << "Jml data2 " << df_below.getjmlrow() << endl;
          //cout << "Jml data3 " << df.getjmlrow() << endl;

          //df.stat_tabel();
          //cout << "Jml data3 " << df.getjmlrow() << endl;

          

          idx_svm++;
          tree[node_index].idx_svm = idx_svm;

          if ((train_svm) ) //and (df.getjmlrow()<10000)
          {
            
            cetak("{v");
            Tmy_svm my_svm(feature_selection, normal_only);
            my_svm.train(df, gamma, nu);
            my_svm.save_model(model_path + "/svm_model_" + to_string(idx_svm) + ".csv");
            cetak("}"); 
            
          }


          string filename = model_path + "/svm_model_" + to_string(tree[treeIndex_yes].idx_svm) + ".csv";
          remove(filename.c_str());
          filename = model_path + "/svm_model_" + to_string(tree[treeIndex_no].idx_svm) + ".csv";
          remove(filename.c_str());

          

        }

        tree[node_index].label = tree[treeIndex_yes].label;
        tree[node_index].children.clear();
        tree[node_index].children.shrink_to_fit();
        tree.erase(tree.begin() + treeIndex_no);
        tree.erase(tree.begin() + treeIndex_yes);
        tree.shrink_to_fit();
        //cetak("\n"); 


      }

    }

  }

  if (node_index == 0)
  {
    cetak("\n");
  }

}

bool Tdec_tree::is_pass(int opt, string value1, string value2)
{
  bool pass = false;

  Tmy_dttype tmp1(value1, ((opt == 0) or (opt == 1)));
  Tmy_dttype tmp2(value2, ((opt == 0) or (opt == 1)));

  switch (opt)
  {
  case 0 : {
    //cout << value1 << " " << value2 << endl;
    pass = tmp1 <= tmp2;
    break;
  }
  case 1 : {
    //cout << value1 << " " << value2 << endl;
    pass = tmp1 > tmp2;
    break;
  }
  case 2 : {
    pass = tmp1 == tmp2;
    break;
  }
  case 3 : {
    pass = tmp1 != tmp2;
    break;
  }
  }

  return pass;
}

int Tdec_tree::dfs(Tdataframe &df, vector<string> &data, int treeIndex)
{
  if (tree[treeIndex].isLeaf)
  {
    vec_attr.push_back(tree[treeIndex].criteriaAttrIndex);
    return treeIndex;
  }

  int criteriaAttrIndex = tree[treeIndex].criteriaAttrIndex;

  for (int i = 0; i < tree[treeIndex].children.size(); i++) {
    int next = tree[treeIndex].children[i];

    //if (tree[next].isLeaf)
    //{
    // return next;
    // } else {
    if (is_pass(tree[next].opt, data[criteriaAttrIndex], tree[next].attrValue)) {
      vec_attr.push_back(criteriaAttrIndex);
      return dfs(df, data, next);
    }
    // }

  }

  return -1;
}

string Tdec_tree::guess(Tdataframe &df, vector<string> &data)
{
  string label = "failed.";
  vec_attr.clear();
  vec_attr.shrink_to_fit();
  int leafNode = dfs(df, data, 0);

  if (leafNode == -1) {
    return "dfs_failed.";
  } else {
    label = tree[leafNode].label;
    if (train_svm)
    {
      if ((label == "normal") and (tree[leafNode].idx_svm != -1))
      {
        Tmy_svm my_svm(feature_selection, normal_only);
        string nm_model = model_path + "/svm_model_" + to_string(tree[leafNode].idx_svm) + ".csv";
        my_svm.load_model(nm_model);

        vector<string> tmp_data;

        if (feature_selection)
        {
          for (int i = 0; i < vec_attr.size(); ++i)
          {
            if (vec_attr[i] != -1) {
              tmp_data.push_back(data[vec_attr[i]]);
            }
          }

        } else {
          for (int i = 0; i < (data.size() - 1); ++i)
          {
            tmp_data.push_back(data[i]);
          }
        }

        label = my_svm.guess(df, tmp_data);

        tmp_data.clear();
        tmp_data.shrink_to_fit();

      }
    }

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

    //int failed = 0;
    df.read_file();
    int i = 0;
    while (!df.is_eof()) {

      if ((i % 1000) == 0) {
        cetak(".");
      }

      tmp_data = df.get_record();

      tmp_str1 = tmp_data[tmp_data.size() - 1];

      tmp_data.erase(tmp_data.end());

      tmp_str = guess(df, tmp_data);
      //if (tmp_str != "dfs failed")
      //{
      //cout << tmp_str1 << "  " << tmp_str << endl;
      conf_metrix.add_jml(tmp_str1, tmp_str, 1);

      //} else {
      //failed++;
      //}
      i++;
      df.next_record();

    }

    df.close_file();
    cetak("\n");
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
      if (tree[i].children.size() == 2) {
        tmp_str += "," + to_string(tree[i].children[0]) + "," + to_string(tree[i].children[1]);
      } else {
        cout << "Ada yg kurang !!!";
      }

    } else {
      tmp_str += ",-1,-1";
    }

    tmp_str += "," + to_string(tree[i].idx_svm);

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

      newnode.idx_svm = tmp_data[8] == "-1" ? -1 : stoi(tmp_data[8]);

      tree.push_back(newnode);

      df.next_record();
    }

    df.close_file();
  } else {
    cout << "Gagal buka file !!!" << endl;
  }
}

void Tdec_tree::set_model_path(string path)
{
  model_path = path;
}
