
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

	bool cabang_exixst(posisi_cabang posisi);

public:
	tb_missing_branch();
	~tb_missing_branch();

    void clear_table();
    void insert_not_split(posisi_cabang posisi,int attrindex,string attrvalue);
    void insert_same_label(posisi_cabang posisi,int attrindex,string attrvalue);
    void insert_cut_off(posisi_cabang posisi,int is_pure,int is_min_sample,int is_depth_limit);
    void update_split(posisi_cabang posisi,int attrindex,string attrvalue);
    void insert_pruning(posisi_cabang posisi,int attrindex,string attrvalue); 
	
};


#endif