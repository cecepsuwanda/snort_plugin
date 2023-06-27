
#include <string>

#include "global.h"
#include "Tquery_builder.h"


using namespace std;

#ifndef Included_tb_missing_branch_H

#define Included_tb_missing_branch_H


class tb_missing_branch
{
private:
    Tquery_builder global_query_builder;

    bool _is_pure;
    bool _is_min_sample;
    bool _is_depth_limit;
    bool _is_same_label;
    bool _is_pruning;
    bool _is_not_split;
    bool _is_lanjut;
    string _label;

    bool cabang_exixst(Tposisi_cabang posisi);


public:
    tb_missing_branch();
    ~tb_missing_branch();

    void clear_table();
    void add_branch(Tposisi_cabang posisi, int attrindex, int opt, Tmy_dttype attrvalue);
    void insert_not_split(Tposisi_cabang posisi, string label, int is_lanjut);
    void insert_same_label(Tposisi_cabang posisi, string label, int is_lanjut);
    void insert_cut_off(Tposisi_cabang posisi, string label, int is_pure, int is_min_sample, int is_depth_limit, int is_lanjut);
    void insert_pruning(Tposisi_cabang posisi, string label, int is_lanjut);

    void get_split(Tposisi_cabang posisi, int &attrindex, int &opt, Tmy_dttype &attrvalue);

    bool parent_exixst(Tposisi_cabang posisi);
    

    void get_stat(Tposisi_cabang posisi);

    bool get_is_pure();
    bool get_is_min_sample();
    bool get_is_depth_limit();
    bool get_is_same_label();
    bool get_is_pruning();
    bool get_is_not_split();
    bool get_is_lanjut();
    string get_label();

    void delete_cabang(Tposisi_cabang posisi);
    void delete_non_missing();


};


#endif