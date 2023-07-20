
#include <string>

#include "global.h"
#include "Tquery_builder.h"


using namespace std;

#ifndef Included_tb_missing_branch_H

#define Included_tb_missing_branch_H


class tb_missing_branch
{
private:
    static Tquery_builder global_query_builder;

    bool _is_pure;
    bool _is_min_sample;
    bool _is_depth_limit;
    bool _is_same_label;
    bool _is_pruning;
    bool _is_not_split;
    bool _is_lanjut;

    bool _is_split;
    bool _is_cut_off;
    
    int _attrindex;
    Tmy_dttype _attrvalue;
    int _opt;

    string _label;
    int _jml_known;
    int _jml_normal;

    bool cabang_exixst(Tposisi_cabang posisi);
    bool cabang_exist(Tposisi_cabang posisi);


public:
    tb_missing_branch();
    ~tb_missing_branch();

    void open_connection();
    void close_connection();

    void clear_table();
    void add_branch(Tposisi_cabang posisi);
    void add_split(Tposisi_cabang posisi, int attrindex, Tmy_dttype attrvalue);
    void add_opt_label(Tposisi_cabang posisi, int opt, string label, int jml_known, int jml_normal);
    void add_cut_off(Tposisi_cabang posisi, int kd_cut_off);

    void get_branch_stat(Tposisi_cabang posisi);
    void delete_histori(Tposisi_cabang posisi);
    void delete_split(Tposisi_cabang posisi);    
    
    bool get_split(int depth,int attrindex,Tmy_dttype attrvalue,Tposisi_cabang &posisi);
    bool get_opt_label(int opt,Tposisi_cabang posisi_root,Tposisi_cabang &posisi_child);
    bool get_opt_label_child(int child_branch,Tposisi_cabang posisi_root,Tposisi_cabang &posisi_child);

    vector<Tposisi_cabang> get_branch(Tposisi_cabang posisi);

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

    bool get_is_cut_off();
    bool get_is_split();
    
    string get_label();
    int get_jml_known();
    int get_jml_normal();

    int get_attrindex();
    int get_opt();
    Tmy_dttype get_attrvalue();

    void delete_cabang(Tposisi_cabang posisi);
    void delete_non_missing();


};


#endif