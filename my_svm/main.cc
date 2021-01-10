
#include <iostream>
// #include "Tconf_metrix.h"
//#include "Tdataframe.h"
#include "Tmy_svm.h"
// #include "svm.h"
// #define Malloc(type,n) (type *)malloc((n)*sizeof(type))

using namespace std;


// void init_param();
// void read_problem(Tdataframe &df);
// void predict(Tdataframe &df);

// struct svm_parameter param;
// struct svm_problem prob;
// struct svm_model *model;
// struct svm_node *x_space;

int main(int argc, char *argv[])
{
  Tdataframe df_train, df_test, df_save;
  df_train.read_data(argv[2]);
  df_train.read_data_type(argv[1]);

  df_test.read_data(argv[3]);
  df_test.read_data_type(argv[1]);

  cout << "train " << df_train.getjmlrow() << " test " << df_test.getjmlrow() << endl;
  
  Tmy_svm my_svm;

  my_svm.train(df_train);
  my_svm.save_model(argv[4]);

  my_svm.load_model(argv[4]);
  my_svm.test(df_test);

  // init_param();
  // read_problem(df_train);

  // const char *error_msg;
  // error_msg = svm_check_parameter(&prob, &param);

  // if (error_msg)
  // {
  //   fprintf(stderr, "ERROR: %s\n", error_msg);
  //   exit(1);
  // }

  // model = svm_train(&prob, &param);
  // if (svm_save_model(argv[4], model))
  // {
  //   fprintf(stderr, "can't save model to file %s\n", argv[4]);
  //   exit(1);
  // }

  // svm_free_and_destroy_model(&model);
  // svm_destroy_param(&param);
  // free(prob.y);
  // free(prob.x);
  // free(x_space);

  // x_space = (struct svm_node *) malloc(df_test.getjmlcol() * sizeof(struct svm_node));

  // if ((model = svm_load_model(argv[4])) == 0)
  // {
  //   fprintf(stderr, "can't open model file %s\n", argv[4]);
  //   exit(1);
  // }

  // predict(df_test);

  // svm_free_and_destroy_model(&model);
  // free(x_space);

  return 0;
}

// void init_param()
// {
//   param.svm_type = ONE_CLASS;
//   param.kernel_type = RBF;
//   param.degree = 3;
//   param.gamma = 0.0001;    // 1/num_features
//   param.coef0 = 0;
//   param.nu = 0.01;
//   param.cache_size = 1024;
//   param.C = 1;
//   param.eps = 1e-3;
//   param.p = 0.1;
//   param.shrinking = 0;
//   param.probability = 0;
//   param.nr_weight = 0;
//   param.weight_label = NULL;
//   param.weight = NULL;

// }

// void read_problem(Tdataframe &df)
// {

//   int i;
//   size_t elements, j;
//   char *endptr;

//   df.open_file();
//   df.read_file();

//   prob.l = df.getjmlrow();
//   elements = (df.getjmlrow() * (df.getjmlcol() - 1)) + df.getjmlrow();

//   prob.y = Malloc(double, prob.l);
//   prob.x = Malloc(struct svm_node *, prob.l);
//   x_space = Malloc(struct svm_node, elements);

//   j = 0;
//   i = 0;
//   while (!df.is_eof())
//   {
//     vector<string> tmp = df.get_record();
//     if (df.is_pass(tmp))
//     {
//       prob.x[i] = &x_space[j];
//       prob.y[i] = 0;

//       for (int k = 0; k < (tmp.size() - 1); k++) {

//         x_space[j].index = k;
//         string str = tmp[k];
//         x_space[j].value = strtod(str.c_str(), &endptr);

//         ++j;
//       }
//       x_space[j++].index = -1;
//       i++;
//     }

//     tmp.clear();
//     tmp.shrink_to_fit();

//     df.next_record();

//   }
//   df.close_file();

// }

// void predict(Tdataframe &df)
// {
//   Tconf_metrix conf_metrix;
//   double predict_label;
//   string asli, tebakan;
//   char *endptr;

//   asli = "inside";

//   df.open_file();
//   df.read_file();

//   while (!df.is_eof())
//   {
//     vector<string> tmp = df.get_record();

//     int k = 0;
//     for (; k < (tmp.size() - 1); k++) {

//       string str = tmp[k];
//       x_space[k].index = k;
//       x_space[k].value = strtod(str.c_str(), &endptr);

//     }
//     x_space[k].index = -1;

//     predict_label = svm_predict(model, x_space);

//     tebakan = "inside";
//     if (predict_label == -1)
//     {
//       tebakan = "outside";
//     }

//     //cout << predict_label << endl;

//     conf_metrix.add_jml(asli, tebakan, 1);

//     tmp.clear();
//     tmp.shrink_to_fit();

//     df.next_record();

//   }
//   df.close_file();

//   conf_metrix.kalkulasi();
//   cout << conf_metrix << endl;
// }
