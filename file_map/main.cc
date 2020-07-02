
#include <iostream>
#include "Tread_file.h"

using namespace std;


int main()
{
    Tread_file f;
    vector<string> vec;

    f.setnm_f("../my_dt/data/corrected");
    f.setseparator(",");
    f.file_map();

    if (f.open_file())
    {
        f.read_file();
        while (!f.is_eof())
        {
            vec = f.get_record();

            for (int i = 0; i < vec.size(); ++i)
            {
                cout << vec[i] ;

                if ( i < (vec.size() - 1))
                {
                    cout << ",";
                }

            }
            cout << endl;

            vec.clear();
            vec.shrink_to_fit();
            f.next_record();
        }
        f.close_file();
    } else {
        cout << "Gagal Buka File !!!" << endl;
    }

    return 0;
}
