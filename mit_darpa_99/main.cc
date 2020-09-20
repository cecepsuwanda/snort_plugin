
#include "Tread_file.h"
#include "Tolah_label.h"
#include "Tconf_metrix.h"
#include <iostream>

#include <map>
#include <iterator>
using namespace std;


int main(int argc, char *argv[])
{
    Tolah_label olah_label;
    Tconf_metrix conf_metrix;

    olah_label.setnm_f(argv[1]); //"data/master_identifications.list"
    olah_label.baca_file();

    Tread_file f;
    Tread_file g;

    /*g.setnm_f("/home/cecep/python/mit_darpa_99/data/alert_csv_mit_darpa_99.csv");
    g.setseparator(", ");

    g.open_file("w+");*/

    
    string nm_file;
    string label, tmp_str = "";
    vector<string> row;


    nm_file = argv[2];//path+"/4w_friday_out.csv"  ; + "any_rule_" + hari[i] + "_" + to_string(week[j]) + "_" + dir[k] + ".csv";
    cout << nm_file << endl;

    f.setnm_f(nm_file);
    f.setseparator(",");


    if (f.open_file())
    {
        int jml_dt = 0;
        while (!f.is_eof())
        {
            row = f.get_record();
            if (row.size() > 0) {
                tmp_str = "";

                /*for (int l = 0; l < row.size(); ++l)
                {
                    tmp_str += row[l] + ",";
                }*/


                label = olah_label.labeli(row);

                conf_metrix.add_jml(label, row[row.size() - 1], 1);

                //tmp_str += label ; //+ "," + "any_rule_" + hari[i] + "_" + to_string(week[j]) + "_" + dir[k] + ".csv";

                //g.write_file(tmp_str);
                // if(label == "smurf."){
                //   cout << row[0] << " " << row[1] << " " << row[2] << " " << row[4] << " " << jml_dt <<endl;  
                // } 


            }
            f.next_record();
            row.clear();
            row.shrink_to_fit();

            jml_dt++;
        }

        f.close_file();
        conf_metrix.kalkulasi();
        cout << conf_metrix << endl;

    } else {
        cout << "Gagal Buka File !!!" << endl;
    }


    //g.close_file();

    return 0;
}
