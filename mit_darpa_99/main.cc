#include <iostream>
#include "Tdataframe_darpa.h"
#include "Tdataframe_label.h"
#include "Tconf_metrix.h"
#include <map>
#include <iterator>
using namespace std;


int main(int argc, char *argv[])
{

    cout << "Labeling : " << argv[2] << endl;
   

    Tdataframe_darpa df_darpa;
    df_darpa.read_data(argv[1]);

    //df_darpa.info();

    Tdataframe_label df_label;
    df_label.read_data(argv[2], ",");

    df_label.info();

    if (df_label.getjmlrow() > 0)
    {
        if (df_label.open_file())
        {
            Tconf_metrix conf_metrix;
            vector<string> tmp_data;
            string label;
            while (!df_label.is_eof())
            {
                tmp_data = df_label.get_record();
                if (tmp_data.size() > 1)
                {
                    //cout << tmp_data[tmp_data.size() - 3] << " " << tmp_data[tmp_data.size()-2] << " " << tmp_data[tmp_data.size()-5] << " " << tmp_data[tmp_data.size()-4] << " " << tmp_data[tmp_data.size() - 9] << " " << tmp_data[tmp_data.size() - 8] << " " << tmp_data[tmp_data.size() - 7] << " " << tmp_data[tmp_data.size() - 6] << " " << tmp_data[1] <<endl;
                    label = df_darpa.search(tmp_data[tmp_data.size() - 3], tmp_data[tmp_data.size() - 2], tmp_data[tmp_data.size() - 5], tmp_data[tmp_data.size() - 4], tmp_data[tmp_data.size() - 9], tmp_data[tmp_data.size() - 8], tmp_data[tmp_data.size() - 7], tmp_data[tmp_data.size() - 6], tmp_data[1]);
                    conf_metrix.add_jml(label, tmp_data[tmp_data.size() - 1], 1);
                    string tmp_str = "";
                    for (int j = 0; j < tmp_data.size(); ++j)
                    {
                        tmp_str += tmp_data[j] + ",";
                    }
                    
                      // cout << tmp_str<<label<< endl;
                    

                } else {

                }

                tmp_data.clear();
                tmp_data.shrink_to_fit();
                df_label.next_record();

            }
            df_label.close_file();

            conf_metrix.kalkulasi();
            cout << conf_metrix << endl;

        } else {
            cout << "Gagal Buka File !!!" << endl;
        }
    }



    df_label.clear_memory();

    /*Tolah_label olah_label;
    Tconf_metrix conf_metrix;

    olah_label.setnm_f(argv[1]); //"data/master_identifications.list"
    olah_label.baca_file();

    Tread_file f;
    Tread_file g;




    string nm_file;
    string label, tmp_str = "";
    vector<string> row;


    nm_file = argv[2];//path+"/4w_friday_out.csv"  ; + "any_rule_" + hari[i] + "_" + to_string(week[j]) + "_" + dir[k] + ".csv";
    cout << nm_file << endl;

    f.setnm_f(nm_file);
    f.setseparator(",");


    if (f.open_file())
    {
        g.setnm_f(argv[3]);
        g.setseparator(", ");

        g.open_file("w+");

        int jml_dt = 0;
        while (!f.is_eof())
        {
            row = f.get_record();
            if (row.size() > 0) {
                tmp_str = "";

                for (int l = 0; l < row.size(); ++l)
                {
                    tmp_str += row[l] + ",";
                }


                label = olah_label.labeli(row);

                conf_metrix.add_jml(label, row[row.size() - 1], 1);

                tmp_str += label ; //+ "," + "any_rule_" + hari[i] + "_" + to_string(week[j]) + "_" + dir[k] + ".csv";

                g.write_file(tmp_str);
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

        g.close_file();

    } else {
        cout << "Gagal Buka File !!!" << endl;
    }
    */



    return 0;
}
