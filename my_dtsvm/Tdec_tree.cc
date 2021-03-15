#include "Tdec_tree.h"


Tdec_tree::Tdec_tree()
{
  idx_svm = 0;
  id_df = 1;
}

Tdec_tree::~Tdec_tree()
{

}

Tdec_tree::Tdec_tree(int v_train_svm, int v_min_sample, int v_depth,int v_save_train,int v_save_test)
{

  train_svm = v_train_svm == 1;
  _min_sample = v_min_sample;
  _depth = v_depth;
  save_train = v_save_train == 1;
  save_test = v_save_test == 1;

  idx_svm = 0;
  id_df = 1;
}

void Tdec_tree::set_svm_param(int v_feature_selection, int v_normal_only, double v_gamma, double v_nu)
{
  feature_selection = v_feature_selection == 1;
  normal_only = v_normal_only == 1;
  _gamma = v_gamma;
  _nu = v_nu;
}

void Tdec_tree::set_f_train(string f_train)
{
  _f_train = f_train;
}

void Tdec_tree::set_f_test(string f_test)
{
  _f_test = f_test;
}

void Tdec_tree::set_f_datatype(string f_datatype)
{
  _f_datatype = f_datatype;
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
  return df.is_single_label();
}

void Tdec_tree::col_pot_split(Tdataframe df, int i, float & current_overall_metric, string & current_split_value)
{
  map<Tmy_dttype, Tlabel_stat> _col_pot_split;
  df.get_col_pot_split(i, _col_pot_split);
  df.calculate_overall_metric(i, _col_pot_split, current_overall_metric, current_split_value);
}

void Tdec_tree::determine_best_split(Tdataframe &df, int &split_column, string &split_value)
{
  //cout << "determine_best_split " << endl;
  float max_gain = 0;
  bool  first_iteration = true;

  string current_split_value;
  float current_overall_metric;

  split_column = -1;
  split_value = "-1";

  map<Tmy_dttype, Tlabel_stat> _col_pot_split;

  for (int i = 0; i < (df.getjmlcol() - 1)  ; ++i)
  {
    df.get_col_pot_split(i, _col_pot_split);
    df.calculate_overall_metric(i, _col_pot_split, current_overall_metric, current_split_value);

    if (first_iteration or (max_gain < current_overall_metric))
    {
      first_iteration = false;
      max_gain = current_overall_metric;

      split_column = i;
      split_value = current_split_value;
    }

  }
}

string Tdec_tree::create_leaf(Tdataframe & df)
{
  string vlabel = df.get_max_label();
  return vlabel;
}

void Tdec_tree::f_train_svm(Tdataframe &df, int v_idx_svm)
{
  Tmy_svm my_svm(feature_selection, normal_only, v_idx_svm, model_path, svm_path,save_train);
  my_svm.train(df, _gamma, _nu);
  my_svm.save_model(svm_path + "/svm_model_" + to_string(v_idx_svm) + ".csv");
}

void Tdec_tree::del_model_train(int idx)
{
  string filename = svm_path + "/svm_model_" + to_string(idx) + ".csv";
  remove(filename.c_str());

  filename = model_path + "/train/train_model_" + to_string(idx) + ".csv";
  remove(filename.c_str());
}

void Tdec_tree::train(Tdataframe & df, int node_index , int counter, int min_samples, int max_depth)
{
  if (node_index == 0)
  {
    Node root;
    root.treeIndex = 0;
    tree.push_back(root);
  }

  //cout << counter;
  cetak("%d", counter);

  if (check_purity(df) or (df.getjmlrow() < min_samples) or (counter == max_depth) )
  {
    string tmp_str = create_leaf(df);

    if (tmp_str == "normal") {

      cetak("*");

      if ((train_svm))
      {
        idx_svm++;
        tree[node_index].idx_svm = idx_svm;

        cetak("{v %d %d ",idx_svm,df.getjmlrow());
        f_train_svm(df, idx_svm);
        cetak("}");

      }

    }

    tree[node_index].isLeaf = true;
    tree[node_index].label = tmp_str;


    df.clear_memory();

  } else {

    cetak("?");

    counter++;

    int split_column;
    string split_value;

    determine_best_split(df, split_column, split_value);

    Tdataframe df_below, df_above;
    df_below = df;
    df_below.set_id(id_df++);
    df_above = df;
    df_above.set_id(id_df++);

    df.split_data(split_column, split_value, df_below, df_above);


    if ((df_below.getjmlrow() == 0) or (df_above.getjmlrow() == 0)) {
      string tmp_str = create_leaf(df);

      if (tmp_str == "normal") {

        cetak("-");

        if ((train_svm))
        {

          idx_svm++;
          tree[node_index].idx_svm = idx_svm;

          cetak("{v %d %d ",idx_svm,df.getjmlrow());
          f_train_svm(df, idx_svm);
          cetak("}");

        }

      }

      tree[node_index].isLeaf = true;
      tree[node_index].label = tmp_str;

      df_below.clear_memory();
      df_above.clear_memory();

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
      train(df_below, nextNode.treeIndex, counter, min_samples, max_depth);


      Node nextNode1;
      nextNode1.treeIndex = (int)tree.size();
      nextNode1.attrValue = split_value;
      nextNode1.opt = df.get_opt(split_column, 0);
      treeIndex_no = nextNode1.treeIndex;
      tree[node_index].children.push_back(nextNode1.treeIndex);
      tree.push_back(nextNode1);

      // cout << tree[node_index].criteriaAttrIndex << " " << df.get_nm_header(tree[node_index].criteriaAttrIndex) << (nextNode1.opt == 1 ? ">" : "!=") << nextNode1.attrValue << endl;
      cetak("<-");
      train(df_above, nextNode1.treeIndex, counter, min_samples, max_depth);

      if (((tree[treeIndex_yes].isLeaf == true) and (tree[treeIndex_no].isLeaf == true)) and (tree[treeIndex_yes].label == tree[treeIndex_no].label))
      {

        tree[node_index].isLeaf = true;

        string tmp_str = tree[treeIndex_yes].label;

        if (tmp_str == "normal") {

          cetak("+");

          if ((train_svm) )
          {
            idx_svm++;
            tree[node_index].idx_svm = idx_svm;

            cetak("{v {j %d %d} {d %d %d} ",idx_svm,df.getjmlrow(),tree[treeIndex_yes].idx_svm,tree[treeIndex_no].idx_svm);
            f_train_svm(df, idx_svm);
            cetak("}");

            del_model_train(tree[treeIndex_yes].idx_svm);
            del_model_train(tree[treeIndex_no].idx_svm);
          }

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

  /*if (node_index == 0)
  {
    cetak("\n");
  }*/

}

void Tdec_tree::build_tree()
{
  Tdataframe df_train;
  df_train.read_data(_f_train);
  df_train.read_data_type(_f_datatype);
  df_train.set_id(0);
  df_train.info();

  cetak("Train : Jumlah Baris : %d Jumlah Kolom : %d \n", df_train.getjmlrow(), df_train.getjmlcol());
  cetak("Depth : %d Minimum Sample : %d gamma : %.4f nu : %.2f train : %s \n", _depth, _min_sample, _gamma, _nu, _f_train.c_str(), _f_test.c_str());
  cetak("Start Train Decission Tree : \n");
  auto start = std::chrono::steady_clock::now();
  train(df_train, 0, 0, _min_sample, _depth);
  auto end = std::chrono::steady_clock::now();
  double elapsed_time = double(std::chrono::duration_cast<std::chrono::seconds>(end - start).count());
  cetak("\nLama Training : %f \n", ((float)(elapsed_time / 60)));
  cetak("End Train Decission Tree : \n");

  cetak("Start Prunning Decission Tree : \n");
  post_pruning(df_train);
  cetak("\nEnd Prunning Decission Tree : \n");

  save_tree();

  df_train.close_file();
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
    pass = tmp2 <= tmp1;
    break;
  }
  case 1 : {
    //cout << value1 << " " << value2 << endl;
    pass = tmp1 < tmp2;
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

int Tdec_tree::dfs(vector<string> &data, int treeIndex)
{
  if (tree[treeIndex].isLeaf)
  {
    auto it = vec_attr.find(tree[treeIndex].criteriaAttrIndex);
    if (it == vec_attr.end())
    {
      vec_attr.insert(pair<int, int>(tree[treeIndex].criteriaAttrIndex, 1));
    } else {
      it->second += 1;
    }

    return treeIndex;
  }

  int criteriaAttrIndex = tree[treeIndex].criteriaAttrIndex;

  string tmp_str = "";
  for (size_t i = 0; i < tree[treeIndex].children.size(); i++) {
    int next = tree[treeIndex].children[i];
    tmp_str += "{" + to_string(tree[next].opt) + "," + to_string(criteriaAttrIndex) + "," + tree[next].attrValue + "," + data[criteriaAttrIndex] + "},";
    // if (tree[next].isLeaf)
    // {
    //   return next;
    // } else {
    if (is_pass(tree[next].opt, tree[next].attrValue, data[criteriaAttrIndex]) ) {

      auto it = vec_attr.find(criteriaAttrIndex);
      if (it == vec_attr.end())
      {
        vec_attr.insert(pair<int, int>(criteriaAttrIndex, 1));
      } else {
        it->second += 1;
      }

      return dfs(data, next);
    }
    // }

  }

  cetak("\n");
  cetak(tmp_str.c_str());
  cetak("\n");

  return -1;
}

string Tdec_tree::guess(vector<string> &data)
{
  string label = "failed.";
  vec_attr.clear();
  int leafNode = dfs(data, 0);

  if (leafNode == -1) {
    return "dfs_failed.";
  } else {
    label = tree[leafNode].label;
    if (train_svm)
    {
      if ((label == "normal") and (tree[leafNode].idx_svm != -1))
      {
        Twrite_file tmp_wf;

        if (save_test) {
          tmp_wf.setnm_f(model_path + "/test/test_model_" + to_string(tree[leafNode].idx_svm) + ".csv");
        }
        //cetak("{v {model ");
        Tmy_svm my_svm(feature_selection, normal_only, 0, "", "",save_train);
        string nm_model = svm_path + "/svm_model_" + to_string(tree[leafNode].idx_svm) + ".csv";
        my_svm.load_model(nm_model);
        //cetak("save_model_");
        //cetak(to_string(tree[leafNode].idx_svm).c_str());
        //cetak(".csv} ");

        vector<string> tmp_data;
        string tmp_str = "";
        if (feature_selection)
        {

          for (auto itr = vec_attr.begin(); itr != vec_attr.end(); ++itr)
          {
            if (itr->first != -1) {
              tmp_data.push_back(data[itr->first]);
              tmp_str = tmp_str + data[itr->first] + ",";
            }
          }

        } else {
          for (size_t i = 0; i < (data.size() - 1); ++i)
          {
            tmp_data.push_back(data[i]);
            tmp_str = tmp_str + data[i] + ",";
          }
        }

        tmp_str = tmp_str + data[data.size() - 1];

        if (save_test)
        {
          tmp_wf.write_file(tmp_str);
          tmp_wf.close_file();
        }
        //cetak(" {guess ");
        label = my_svm.guess(tmp_data);
        //cetak("}");

        tmp_data.clear();
        tmp_data.shrink_to_fit();

      }
    }

  }
  return label;
}

void Tdec_tree::test()
{
  Tdataframe df;
  df.read_data(_f_test);
  df.read_data_type(_f_datatype);
  df.info();

  cetak("Test : Jumlah Baris : %d Jumlah Kolom : %d \n", df.getjmlrow(), df.getjmlcol());
  cetak("Depth : %d Minimum Sample : %d gamma : %.4f nu : %.2f test : %s \n", _depth, _min_sample, _gamma, _nu, _f_train.c_str(), _f_test.c_str());
  cetak("Test Decission Tree : \n");

  Tconf_metrix conf_metrix;
  vector<string> tmp_data;
  string tmp_str, tmp_str1;

  df.reset_file();
  int i = 0;
  while (!df.is_eof()) {

    if ((i % 1000) == 0) {
      cetak(".");
    }

    tmp_data = df.get_record();

    tmp_str1 = tmp_data[tmp_data.size() - 1];

    //tmp_data.erase(tmp_data.end());

    tmp_str = guess(tmp_data);
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

}

void Tdec_tree::save_tree()
{
  Twrite_file tmp_wf;
  tmp_wf.setnm_f(model_path + "/dtsvm_model.csv");

  string tmp_str = "";
  vector<string> vec;

  for (size_t i = 0; i < tree.size(); ++i)
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

  tmp_wf.write_file(vec);
  tmp_wf.close_file();
}


void Tdec_tree::read_tree()
{
  vector<string> tmp_data;
  Tdataframe df;
  df.read_data(model_path + "/dtsvm_model.csv");

  df.reset_file();
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

    if (idx_svm < newnode.idx_svm)
    {
      idx_svm = newnode.idx_svm;
    }

    tree.push_back(newnode);

    df.next_record();
  }

  df.close_file();

}

void Tdec_tree::set_model_path(string path)
{
  model_path = path;
}

void Tdec_tree::set_svm_path(string path)
{
  svm_path = path;
}

void Tdec_tree::pruning_dfs(int node_index , Tdataframe &df_train)
{
  cetak(".");

  int left = tree[node_index].children[0];
  int right = tree[node_index].children[1];


  if ((left != -1) and (!tree[left].isLeaf)) {
    Tdataframe df_left;
    df_left = df_train;
    df_left.add_filter(tree[node_index].criteriaAttrIndex, tree[left].opt, tree[left].attrValue);
    pruning_dfs(left, df_left);
    df_left.clear_memory();
  }
  if ((right != -1) and (!tree[right].isLeaf) ) {
    Tdataframe df_right;
    df_right = df_train;
    df_right.add_filter(tree[node_index].criteriaAttrIndex, tree[right].opt, tree[right].attrValue);
    pruning_dfs(right, df_right);
    df_right.clear_memory();
  }


  if ((left != -1) and (right != -1) ) {
    if ((tree[left].isLeaf) and (tree[right].isLeaf))
    {

      cetak("+");

      float error_node, error_left, error_right, sum_error;

      string left_label = tree[left].label;
      string right_label = tree[right].label;

      error_node = df_train.get_estimate_error();
      string node_label = df_train.get_max_label();

      Tdataframe df_left, df_right;
      df_left = df_train;
      df_right = df_train;

      df_left.add_filter(tree[node_index].criteriaAttrIndex, tree[left].opt, tree[left].attrValue);
      error_left = df_left.get_estimate_error();
      df_right.add_filter(tree[node_index].criteriaAttrIndex, tree[right].opt, tree[right].attrValue);
      error_right = df_right.get_estimate_error();

      sum_error = (((float) df_left.getjmlrow() / df_train.getjmlrow()) * error_left) + (((float) df_right.getjmlrow() / df_train.getjmlrow()) * error_right);

      df_left.clear_memory();
      df_right.clear_memory();
      df_train.clear_memory();

      if (error_node < sum_error)
      {
        cetak("*");
        tree[node_index].children[0] = -1;
        tree[node_index].children[1] = -1;
        tree[node_index].isLeaf = true;
        tree[node_index].label = node_label;

        if (node_label == "normal")
        {
          if ((train_svm) ) //and (df.getjmlrow() < 10000)
          {
            idx_svm++;
            tree[node_index].idx_svm = idx_svm;

            cetak("{v {j %d %d} ",idx_svm,df_train.getjmlrow());
            f_train_svm(df_train, idx_svm);
            
            cetak("{d ");

            if (left_label == "normal")
            {
              del_model_train(tree[left].idx_svm);
              cetak(" %d ",tree[left].idx_svm);              
            }

            if (right_label == "normal")
            {
              del_model_train(tree[right].idx_svm );
              cetak(" %d ",tree[right].idx_svm);
            }

            cetak("} }");

          }

        }

        tree[left].idx_svm = -1;
        tree[right].idx_svm = -1;
      }

    }

  }
}

void Tdec_tree::post_pruning(Tdataframe &df_train)
{
  pruning_dfs(0, df_train);
}

void Tdec_tree::learn_svm()
{
  Tdataframe df_train;
  df_train.read_data(_f_train);
  df_train.read_data_type(_f_datatype);
  df_train.set_id(0);
  df_train.info();

  cetak("Train : Jumlah Baris : %d Jumlah Kolom : %d \n", df_train.getjmlrow(), df_train.getjmlcol());
  cetak("Depth : %d Minimum Sample : %d gamma : %.4f nu : %.2f train : %s test : %s \n", _depth, _min_sample, _gamma, _nu, _f_train.c_str(), _f_test.c_str());
  cetak("Start Train Decission Tree : \n");
  auto start = std::chrono::steady_clock::now();
  svm_dfs(0, 0, df_train);
  auto end = std::chrono::steady_clock::now();
  double elapsed_time = double(std::chrono::duration_cast<std::chrono::seconds>(end - start).count());
  cetak("\nLama Training : %f \n", ((float)(elapsed_time / 60)));
  cetak("End Train Decission Tree : \n");
  df_train.close_file();
}

void Tdec_tree::svm_dfs(int depth , int node_index , Tdataframe &df_train)
{
  cetak("%d?", depth);

  if (tree[node_index].isLeaf)
  {
    cetak("*");

    string label = tree[node_index].label;

    if ((label == "normal") and (tree[node_index].idx_svm != -1))
    {
      cetak("{v {j %d %d}",tree[node_index].idx_svm,df_train.getjmlrow());
      f_train_svm(df_train, tree[node_index].idx_svm);
      cetak("}");
    }

  } else {

    cetak("|");

    int left = tree[node_index].children[0];
    int right = tree[node_index].children[1];

    if (left != -1) {
      Tdataframe df_left;
      df_left = df_train;
      df_left.add_filter(tree[node_index].criteriaAttrIndex, tree[left].opt, tree[left].attrValue);
      cetak("->");
      svm_dfs(depth + 1, left, df_left);

    }

    if (right != -1) {
      Tdataframe df_right;
      df_right = df_train;
      df_right.add_filter(tree[node_index].criteriaAttrIndex, tree[right].opt, tree[right].attrValue);
      cetak("<-");
      svm_dfs(depth + 1, right, df_right);

    }
  }
  df_train.clear_memory();
}
