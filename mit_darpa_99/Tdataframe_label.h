#include "Tbase_dataframe.h"
#include "Tip_port_holder.h"

using namespace std;

#ifndef Included_Tdataframe_label_H

#define Included_Tdataframe_label_H


class Tdataframe_label : public Tbase_dataframe 
{
private:
	

public:
	
	bool is_pass(vector<string> &data) override;
    
};

#endif