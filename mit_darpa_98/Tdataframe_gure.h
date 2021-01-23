#include "Tbase_dataframe.h"
#include "Tdatetime_holder.h"
#include "Tip_port_holder.h"

using namespace std;

#ifndef Included_Tdataframe_gure_H

#define Included_Tdataframe_gure_H


class Tdataframe_gure : public Tbase_dataframe 
{
private:
	

public:
	
	void stat_tabel() override;
    bool is_pass(vector<string> &data) override;
    void head() override;
};

#endif