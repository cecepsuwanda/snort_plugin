#include "Tread_file.h"
#include <string>
#include <iostream>


using namespace std;

class Tolah_label
{

private:
   string _attack_file,_host_file;

   void baca_attack_file();
   void baca_host_file();

public:
	Tolah_label();
	~Tolah_label();
    
    void setnm_f(string nm_attack_f,string nm_host_f);
    void baca_file();
    string labeli(vector<string> row);
};