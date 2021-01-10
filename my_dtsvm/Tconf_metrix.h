
#include <map>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

#ifndef Included_Tconf_metrix_H

#define Included_Tconf_metrix_H

struct Tdata
{
  int TP,FP,TN,FN,jml;
  float accuracy,recall,specificity; 	
};

class Tconf_metrix
{
private:
   map<string, map<string, int>> matrik;
   map<string, Tdata> matrik1;
   float accuracy;
   int tepat,tdk_tepat,jml_data;
public:
	Tconf_metrix();
	~Tconf_metrix();
	void add_jml(string asli,string tebakan,int jml);
    void kalkulasi();
    int get_TP(string kelas);
    int get_TN(string kelas);
    int get_FP(string kelas);
    int get_FN(string kelas);



	friend ostream & operator << (ostream &out, const Tconf_metrix &tc);
};

#endif