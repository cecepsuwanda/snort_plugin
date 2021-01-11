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
}

Tmy_svm::~Tmy_svm()
{
	cout << "my_svm destroy" << endl;
	svm_free_and_destroy_model(&model);
	svm_destroy_param(&param);
	free(prob.y);
	free(prob.x);
	free(x_space);
}

void Tmy_svm::read_problem(Tdataframe &df)
{
	int i;
	size_t elements, j;
	char *endptr;

	df.open_file();
	df.read_file();

	prob.l = df.getjmlrow();
	elements = (df.getjmlrow() * (df.getjmlcol() - 1)) + df.getjmlrow();

	prob.y = Malloc(double, prob.l);
	prob.x = Malloc(struct svm_node *, prob.l);
	x_space = Malloc(struct svm_node, elements);

	j = 0;
	i = 0;
	while (!df.is_eof())
	{
		vector<string> tmp = df.get_record();
		if (df.is_pass(tmp))
		{
			prob.x[i] = &x_space[j];
			prob.y[i] = 0;

			for (int k = 0; k < (tmp.size() - 1); k++) {

				x_space[j].index = k;
				string str = tmp[k];
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

}


void Tmy_svm::train(Tdataframe &df,double gamma, double nu)
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

	model = svm_train(&prob, &param);

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