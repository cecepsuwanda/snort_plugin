#include "Tmy_svm.h"


Tmy_svm::Tmy_svm(Tconfig *v_config){
	_config = v_config;
	_my_alpha = new Tmy_alpha(_config); 
}

Tmy_svm::~Tmy_svm(){
   delete _my_alpha;
}

void Tmy_svm::train(Tdataframe &df){   
   _my_alpha->init(df.getjmlrow_svm());
}