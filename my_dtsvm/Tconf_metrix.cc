#include "Tconf_metrix.h"

Tconf_metrix::Tconf_metrix()
{
	failed = 0;
	tepat = 0;
	tdk_tepat = 0;
	jml_data = 0;
}

Tconf_metrix::~Tconf_metrix()
{
	map<string, map<string, int>>::iterator it, it1;

	if (matrik.size() > 0)
	{
		for (it = matrik.begin(); it != matrik.end(); ++it)
		{
			it->second.clear();
		}
		matrik.clear();
	}

	matrik1.clear();
	guess.clear();
}

void Tconf_metrix::add_jml(string asli, string tebakan, int jml)
{

	if ((tebakan.compare("dfs_failed.") != 0))
	{
		guess.insert(pair<string, int>(tebakan, 0));

		jml_data++;
		if (asli == tebakan)
		{
			tepat++;
		} else {
			tdk_tepat++;
		}

		if (matrik.size() > 0)
		{
			map<string, map<string, int>>::iterator it;

			it = matrik.find(asli);
			if (it == matrik.end())
			{
				map<string, int> tmp_map;
				tmp_map.insert(pair<string, int>(tebakan, jml));
				matrik.insert(pair<string, map<string, int>>(asli, tmp_map));
			} else {
				map<string, int>::iterator it1;
				it1 = it->second.find(tebakan);
				if (it1 == it->second.end())
				{
					it->second.insert(pair<string, int>(tebakan, jml));
				} else {
					it1->second += jml;
				}
			}

		} else {
			map<string, int> tmp_map;
			tmp_map.insert(pair<string, int>(tebakan, jml));
			matrik.insert(pair<string, map<string, int>>(asli, tmp_map));
		}

	} else {
		failed++;
	}



}

void Tconf_metrix::kalkulasi()
{
	if (matrik.size() > 0) {


		for (auto itr = guess.begin(); itr != guess.end(); itr++)
		{
			auto itr1 = matrik.find(itr->first);
			if (itr1 == matrik.end())
			{
				map<string, int> tmp_map;
				tmp_map.insert(pair<string, int>(itr->first, 0));
				matrik.insert(pair<string, map<string, int>>(itr->first, tmp_map));

				itr1 = matrik.find(itr->first);
				for (auto itr2 = matrik.begin(); itr2 != matrik.end(); itr2++)
				{
					auto it1 = itr1->second.find(itr2->first);
					if (it1 == itr1->second.end())
					{
						itr1->second.insert(pair<string, int>(itr2->first, 0));
					}
				}
			}
		}


		int TP = 0;
		int FN = 0;
		int FP = 0;
		int TN = 0;

		for (auto it = matrik.begin(); it != matrik.end(); ++it)
		{
			Tdata tmp_data;
			tmp_data.TP = get_TP(it->first);
			TP += tmp_data.TP;
			tmp_data.FN = get_FN(it->first);
			FN += tmp_data.FN;
			tmp_data.FP = get_FP(it->first);
			FP += tmp_data.FP;
			tmp_data.TN = get_TN(it->first);
			TN += tmp_data.TN;
			tmp_data.jml = tmp_data.TP + tmp_data.FN;
			tmp_data.accuracy = 0;
			if ((tmp_data.TP + tmp_data.FP) > 0) {
				tmp_data.accuracy =  (tmp_data.TP + tmp_data.TN) / (double)(tmp_data.TP + tmp_data.FP + tmp_data.TN + tmp_data.FN);
			}
			tmp_data.recall = 0;
			if ((tmp_data.jml) > 0) {
				tmp_data.recall =  tmp_data.TP / (double) tmp_data.jml;
			}
			tmp_data.specificity = 0;
			if ((tmp_data.TN + tmp_data.FP) > 0) {
				tmp_data.specificity =  tmp_data.TN / (double)(tmp_data.TN + tmp_data.FP);
			}

			matrik1.insert(pair<string, Tdata> (it->first, tmp_data));
		}

		accuracy = (TP + TN) / (double)(TP + TN + FP + FN);
		precision = TP / (double)(TP + FP);
		recall = TP / (double)(TP + FN);
	}
}

int Tconf_metrix::get_TP(string kelas)
{
	int total = 0;

	if (matrik.size() > 0) {
		auto it = matrik.find(kelas);
		if (it != matrik.end())
		{
			auto it1 = it->second.find(kelas);
			if (it1 != it->second.end())
			{
				total = it1->second;
			}
		}
	}

	return total;
}

int Tconf_metrix::get_TN(string kelas)
{
	int total = 0;

	if (matrik.size() > 0) {
		for (auto it = matrik.begin(); it != matrik.end(); ++it)
		{
			if (it->first != kelas)
			{
				for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1)
				{
					if (it1->first != kelas) {
						total += it1->second;
					}
				}
			}
		}
	}

	return total;
}

int Tconf_metrix::get_FP(string kelas)
{
	int total = 0;

	if (matrik.size() > 0) {

		for (auto it = matrik.begin(); it != matrik.end(); ++it)
		{
			if (it->first != kelas) {
				auto it1 = it->second.find(kelas);
				if (it1 != it->second.end()) {
					total += it1->second;
				}
			}
		}
	}

	return total;
}

int Tconf_metrix::get_FN(string kelas)
{
	int total = 0;

	if (matrik.size() > 0) {
		auto it = matrik.find(kelas);
		if (it != matrik.end())
		{
			for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1)
			{
				if (it1->first != kelas) {
					total += it1->second;
				}
			}
		}
	}

	return total;
}

ostream & operator << (ostream &out, const Tconf_metrix &tc)
{
	out << "Jumlah Data: " << tc.jml_data;
	out << " Prediksi Tepat: " << tc.tepat;
	out << " Prediksi Tidak Tepat: " << tc.tdk_tepat;
	out << " Prediksi Failed: " << tc.failed;
	//out << " Prosentase: " << ((tc.tepat / (double) tc.jml_data) * 100);
	out << " Akurasi: " << tc.accuracy ;
	out << " Precision: " << tc.precision ;
	out << " Recall: " << tc.recall << endl;

	if (tc.matrik1.size() > 0) {
		out << "   Confusion Metrik     : " << endl;
		out << setw(30) << "kelas" << setw(10) << "TP" << setw(10) << "FN" << setw(10) << "jml" << setw(10) << "FP" << setw(10) << "TN" << setw(10) << "accuracy" << setw(10) << "recall" << setw(13) << "specificity" << endl;
		for (auto it = tc.matrik1.begin(); it != tc.matrik1.end(); ++it)
		{
			out << setw(30) << it->first;
			out << setw(10) << it->second.TP;
			out << setw(10) << it->second.FN;
			out << setw(10) << it->second.jml;
			out << setw(10) << it->second.FP;
			out << setw(10) << it->second.TN;
			out << setprecision(3) << setw(10) << it->second.accuracy;
			out << setprecision(3) << setw(10) << it->second.recall;
			out << setprecision(3) << setw(13) << it->second.specificity << endl;
		}
	}

	out << endl;

	if (tc.matrik.size() > 0)
	{
		out << setw(20);

		for (auto it = tc.matrik.begin(); it != tc.matrik.end(); it++)
		{
			out << it->first << setw(10);
		}

		out << endl;

		for (auto it = tc.guess.begin(); it != tc.guess.end(); it++)
		{
			out << setw(10) << it->first;
			for (auto it1 = tc.matrik.begin(); it1 != tc.matrik.end(); it1++)
			{
				auto it2 = it1->second.find(it->first);
				if(it2==it1->second.end())
				{
                  out << setw(10) << 0;
				}else{
                  out << setw(10) << it2->second;
				}
			}

			out << endl;

		}


	}



	return out;
}