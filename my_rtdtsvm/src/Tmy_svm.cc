#include "Tmy_svm.h"



Tmy_svm::Tmy_svm()
{
	param.svm_type = ONE_CLASS;
	param.kernel_type = RBF;
	param.degree = 3;
	param.gamma = 0.0001;    // 1/num_features
	param.coef0 = 0;
	param.nu = 0.01;
	param.cache_size = 100;
	param.C = 1;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 0;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;

	void (*print_func)(const char*) = NULL;	// default printing to stdout
	print_func = &print_null;
	svm_set_print_string_function(print_func);

	is_read_problem = false;
}

Tmy_svm::~Tmy_svm()
{
	//cout << "my_svm destroy" << endl;
	svm_free_and_destroy_model(&model);
	svm_destroy_param(&param);
	if (is_read_problem) {
		free(prob.y);
		free(prob.x);
	}
	free(x_space);
}



void Tmy_svm::read_problem(Tdataframe &df)
{

	size_t elements, j, i;
	char *endptr;

	vector<field_filter> df_filter;
	df_filter = df.get_filter();

	// for (int i = 0; i < df_filter.size(); ++i)
	// {

	//   cout << df_filter[i].idx_col << endl;
	// }
	// cout << "-----" << endl;

	if (df.open_file())
	{
		df.read_file();

		map<string, int> stat_label = df.get_stat_label();

		//cout << stat_label["normal"] << endl;

		prob.l = stat_label["normal"];
		elements = (stat_label["normal"] * (df_filter.size())) + stat_label["normal"];

		prob.y = Malloc(double, prob.l);
		prob.x = Malloc(struct svm_node *, prob.l);
		x_space = Malloc(struct svm_node, elements);

		j = 0;
		i = 0;
		while (!df.is_eof())
		{
			vector<string> tmp = df.get_record();
			if (df.is_pass(tmp) and (tmp[tmp.size() - 1] == "normal"))
			{
				prob.x[i] = &x_space[j];
				prob.y[i] = 1;

				for (int k = 0; k < (df_filter.size()); k++) {

					x_space[j].index = k;
					string str = tmp[df_filter[k].idx_col];
					x_space[j].value = strtod(str.c_str(), &endptr);

					++j;
				}
				x_space[j++].index = -1;
				i++;
			}

			tmp.clear();
			tmp.shrink_to_fit();

			df.next_record();

		}
		df.close_file();
		//cout << i << endl;
		is_read_problem = true;
	}

	df_filter.clear();
	df_filter.shrink_to_fit();
}


void Tmy_svm::train(Tdataframe &df, double gamma, double nu)
{
	param.gamma = gamma;
	param.nu = nu;

	read_problem(df);

	const char *error_msg;
	error_msg = svm_check_parameter(&prob, &param);

	if (error_msg)
	{
		fprintf(stderr, "ERROR: %s\n", error_msg);
		exit(1);
	}

	//cout << "start train " << endl;
	model = svm_train(&prob, &param);
	//cout << "end train " << endl;

	//free(prob.y);
	//free(prob.x);
	//free(x_space);

}

void Tmy_svm::save_model(string nm_file)
{
	if (svm_save_model(nm_file.c_str(), model))
	{
		fprintf(stderr, "can't save model to file %s\n", nm_file.c_str());
		exit(1);
	}

	//svm_free_and_destroy_model(&model);
	//svm_destroy_param(&param);
}

void Tmy_svm::load_model(string nm_file)
{
	if ((model = svm_load_model(nm_file.c_str())) == 0)
	{
		fprintf(stderr, "can't open model file %s\n", nm_file.c_str());
		exit(1);
	}

}

void Tmy_svm::test(Tdataframe &df)
{
	x_space = (struct svm_node *) malloc(df.getjmlcol() * sizeof(struct svm_node));

	Tconf_metrix conf_metrix;
	double predict_label;
	string asli, tebakan;
	char *endptr;

	asli = "inside";

	df.open_file();
	df.read_file();

	while (!df.is_eof())
	{
		vector<string> tmp = df.get_record();

		int k = 0;
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

	conf_metrix.kalkulasi();
	cout << conf_metrix << endl;
}

string Tmy_svm::guess(Tdataframe &df, vector<string> &data)
{
	char *endptr;
	x_space = (struct svm_node *) malloc(data.size() * sizeof(struct svm_node));

	int k = 0;
	for (; k < data.size(); k++) {

		string str = data[k];
		x_space[k].index = k;
		x_space[k].value = strtod(str.c_str(), &endptr);

	}
	x_space[k].index = -1;

	double predict_label = svm_predict(model, x_space);

	//cout << predict_label << endl;

	string label = "normal";
	if (predict_label == -1)
	{
		label = "attack";
	}
	return label;
}

