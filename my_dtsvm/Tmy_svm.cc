#include "Tmy_svm.h"



Tmy_svm::Tmy_svm()
{
	param.svm_type = ONE_CLASS;
	param.kernel_type = RBF;
	param.degree = 3;
	param.gamma = 0.0001;    // 1/num_features
	param.coef0 = 0;
	param.nu = 0.01;
	param.cache_size = 100; //100
	param.C = 1;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;

	void (*print_func)(const char*) = NULL;	// default printing to stdout
	print_func = &print_null;
	svm_set_print_string_function(print_func);

	is_read_problem = false;
}

Tmy_svm::Tmy_svm(Tconfig* v_config)
{
	param.svm_type = ONE_CLASS;
	param.kernel_type = RBF;
	param.degree = 3;
	param.gamma = 0.0001;    // 1/num_features
	param.coef0 = 0;
	param.nu = 0.01;
	param.cache_size = 100; //100
	param.C = 1;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;

	void (*print_func)(const char*) = NULL;	// default printing to stdout
	print_func = &print_null;
	svm_set_print_string_function(print_func);

	is_read_problem = false;

	config = v_config;
}


Tmy_svm::~Tmy_svm()
{
	// cetak("{ my_svm");
	// svm_free_and_destroy_model(&model);
	// svm_destroy_param(&param);
	// if (is_read_problem) {
	// 	free(prob.y);
	// 	free(prob.x);
	// 	free(x_space);
	// }

	// cetak(" }");
}

static void cetak_stdout(const char *s)
{
	fputs(s, stdout);
	fflush(stdout);
}

void Tmy_svm::cetak ( const char * format, ... )
{
	char buffer[256];
	va_list args;
	va_start (args, format);
	vsprintf (buffer, format, args);
	//perror (buffer);
	va_end (args);
	cetak_stdout(buffer);
}



void Tmy_svm::read_problem(vector<vector<string>> table)
{
	// df.ReFilter();
	// df.clear_col_split();

	// Twrite_file tmp_wf;
	// if (config.save_train) {
	// 	tmp_wf.setnm_f(config.path_model + "/train/train_model_" + to_string(_idx_svm) + ".csv");
	// }

	size_t elements, j, i;
	char *endptr;

	// df.reset_file();

	// map<string, int> stat_label = df.get_stat_label();

	size_t prm1 = table.size();//df.getjmlrow_svm();
	size_t prm2 = (table[0].size()-1);//(df.getjmlcol_svm() - 1);// (_feature_selection ? kolom.size() :)

	// if ((stat_label.size() > 1) and (!_normal_only))
	// {
	// 	prm1 = df.getjmlrow();
	// } else {
	// 	prm1 = stat_label["normal"];
	// }


	prob.l = prm1;
	elements = (prm1 * prm2) + prm1;


	prob.y = Malloc(double, prob.l);
	prob.x = Malloc(struct svm_node *, prob.l);

	x_space = Malloc(struct svm_node, elements);

	j = 0;
	i = 0;
	vector<string> tmp;
	// while (!df.is_eof())
	for (int l = 0; l < prm1; ++l)
	 {
		tmp =  table[l];//df.get_record_svm();

		// bool is_pass = (config.normal_only ? (tmp[tmp.size() - 1].compare("normal") == 0) : true);

		// if (is_pass) 
		// {
			prob.x[i] = &x_space[j];
			prob.y[i] = 1;

			string tmp_str = "";

			

			for (size_t k = 0; k < (prm2); k++) {

				x_space[j].index = k;
				string str = tmp[k];
				x_space[j].value = strtod(str.c_str(), &endptr);
				// tmp_str = tmp_str + str + ",";

				++j;
			}

			// tmp_str = tmp_str + tmp[df.getjmlcol_svm() - 1];
			// //cout << tmp_str << endl;
			// if (config.save_train) {
			// 	tmp_wf.write_file(tmp_str);
			// }

			x_space[j++].index = -1;
			i++;
		// }

		tmp.clear();
		tmp.shrink_to_fit();

	// 	df.next_record();

	 }



//cout << i << endl;
	is_read_problem = true;

	// if (config.save_train) {
	// 	tmp_wf.close_file();
	// }

	
	// df.clear_memory();

}


void Tmy_svm::train(vector<vector<string>> table)
{
	param.gamma = config->gamma;
	param.nu = config->nu;

	read_problem(table);

	const char *error_msg;
	error_msg = svm_check_parameter(&prob, &param);

	if (error_msg)
	{
		fprintf(stderr, "ERROR: %s\n", error_msg);
		exit(1);
	}

	// cetak(" {train");
	model = svm_train(&prob, &param);
	// cetak("}");



	//munmap(x_space, elements_size);

}

void Tmy_svm::save_model(string nm_file)
{

	// cetak("{ save nSV = ");
	// cetak(to_string(model->l).c_str());
	if (svm_save_model(nm_file.c_str(), model))
	{
		fprintf(stderr, "can't save model to file %s\n", nm_file.c_str());
		exit(1);
	}
	// cetak(" }");

	svm_free_and_destroy_model(&model);
	svm_destroy_param(&param);
	free(prob.y);
	free(prob.x);
	free(x_space);
}

void Tmy_svm::load_model(string nm_file)
{
	if ((model = svm_load_model(nm_file.c_str())) == 0)
	{
		fprintf(stderr, "can't open model file %s\n", nm_file.c_str());
		exit(1);
	}

}

void Tmy_svm::destroy_model()
{
	svm_free_and_destroy_model(&model);
}

void Tmy_svm::test(Tdataframe &df)
{
	x_space = (struct svm_node *) malloc(df.getjmlcol() * sizeof(struct svm_node));

	Tconf_metrix conf_metrix;
	double predict_label;
	string asli, tebakan;
	char *endptr;

	asli = "inside";

	df.reset_file();

	while (!df.is_eof())
	{
		vector<string> tmp = df.get_record();

		size_t k = 0;
		for (; k < (tmp.size() - 1); k++) {

			string str = tmp[k];
			x_space[k].index = k;
			x_space[k].value = strtod(str.c_str(), &endptr);

		}
		x_space[k].index = -1;

		predict_label = svm_predict(model, x_space);

		tebakan = "inside";
		if (predict_label == -1)
		{
			tebakan = "outside";
		}

		//cout << predict_label << endl;

		conf_metrix.add_jml(asli, tebakan, 1);

		tmp.clear();
		tmp.shrink_to_fit();

		df.next_record();

	}
	df.close_file();

	free(x_space);

	conf_metrix.kalkulasi();
	cout << conf_metrix << endl;
}

string Tmy_svm::guess(vector<string> &data)
{

	char *endptr;
	x_space = (struct svm_node *) malloc(data.size() * sizeof(struct svm_node));

	size_t k = 0;
	for (; k < data.size(); k++) {
		string str = data[k];
		x_space[k].index = k;
		x_space[k].value = strtod(str.c_str(), &endptr);
	}
	x_space[k].index = -1;


	double predict_label = svm_predict(model, x_space);

	//cout << "{" << predict_label << "}";

	free(x_space);
	//svm_free_and_destroy_model(&model);
	//svm_destroy_param(&param);

	string label = "normal";
	if (predict_label == -1)
	{
		label = "unknown";
	}
	return label;
}

