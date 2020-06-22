
#include "Tread_file.h"
#include "Tolah_label.h"
#include <iostream>

#include <map>
#include <iterator>
using namespace std;





int main()
{
    Tolah_label olah_label;

    olah_label.setnm_f("data/master_identifications.list");
    olah_label.baca_file();

    Tread_file f;
    Tread_file g;

    g.setnm_f("/home/cecep/python/mit_darpa_99/data/alert_csv_mit_darpa_99.csv");
    g.setseparator(", ");

    g.open_file("w+");

    string path = "data/snort_alert_csv/any_rule/";
    string hari[5] = {"monday", "tuesday", "wednesday", "thursday", "friday"};
    int week[2] = {4, 5};
    string dir[2] = {"out", "in"};

    string nm_file;
    string label, tmp_str = "";
    vector<string> row;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {

                nm_file = path + "any_rule_" + hari[i] + "_" + to_string(week[j]) + "_" + dir[k] + ".csv";
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

                            for (int l = 0; l < row.size(); ++l)
                            {
                                tmp_str += row[l] + ",";
                            }
                            //cout << row[0] <<","<< row[6]<< "," << row[7] << endl;

                            label = olah_label.labeli(row);

                            tmp_str += label + "," + "any_rule_" + hari[i] + "_" + to_string(week[j]) + "_" + dir[k] + ".csv";

                            g.write_file(tmp_str);

                            if (label != "normal,Nan")
                            {
                                cout << tmp_str << endl;
                            }

                        }
                        f.next_record();
                        row.clear();
                        row.shrink_to_fit();

                        if ((jml_dt % 100000) == 0)
                        {
                            cout << jml_dt << endl;
                        }
                        jml_dt++;
                    }

                    f.close_file();

                } else {
                    cout << "Gagal Buka File !!!" << endl;
                }
            }

        }
    }

    g.close_file();

    return 0;
}
