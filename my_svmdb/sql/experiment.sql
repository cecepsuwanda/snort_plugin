create table experiment_svm (
id varchar(255) not null,
id_experiment_dt varchar(255) not null,
id_detail_experiment_dt varchar(255) not null,
gamma_awal double,
gamma_akhir double,
gamma_step double,
nu_awal double,
nu_akhir double,
nu_step double,
mulai datetime,
akhir datetime,
primary key (id)
);

create table detail_experiment_svm (
id varchar(255) not null,
id_experiment varchar(255) not null,
id_experiment_dt varchar(255) not null,
id_detail_experiment_dt varchar(255) not null,
gamma double,
nu double,
TP int,
TN int,
FP int,
FN int,
f1 double,
start_train datetime,
end_train datetime,
start_test datetime,
end_test datetime,
primary key (id)
);

drop table more_detail_experiment_svm;
create table more_detail_experiment_svm (
id varchar(255) not null,
id_detail_experiment varchar(255) not null,
id_experiment varchar(255) not null,
id_experiment_dt varchar(255) not null,
id_detail_experiment_dt varchar(255) not null,
idx_svm int,
gamma double,
nu double,
TP int,
TN int,
FP int,
FN int,
f1 double,
start_train datetime,
end_train datetime,
start_test datetime,
end_test datetime,
primary key (id,idx_svm)
);

create table dtsvm_stat (
id int not null auto_increment,
id_detail_experiment varchar(255) not null,
id_experiment varchar(255) not null,
id_experiment_dt varchar(255) not null,
id_detail_experiment_dt varchar(255) not null,
label_org varchar(255),
label_pred varchar(255),
jml int,
primary key (id)
);

drop table detail_experiment_svm;
create table detail_experiment_svm (
id varchar(255) not null,
id_experiment varchar(255) not null,
id_experiment_dt varchar(255) not null,
id_detail_experiment_dt varchar(255) not null,
gamma double,
nu double,
TP_svm int,
TN_svm int,
FP_svm int,
FN_svm int,
f1_svm double,
TP int,
TN int,
FP int,
FN int,
f1 double,
start_train datetime,
end_train datetime,
start_test datetime,
end_test datetime,
primary key (id)
);

select a.id_detail_experiment,a.id_experiment,gamma,nu,sum(if(label_org='known' and label_pred='normal',jml,0)) as jml_known_normal,sum(if(label_org='known' and label_pred='unknown',jml,0)) as jml_known_unknown 
from dtsvm_stat a inner join detail_experiment_svm b on b.id = a.id_detail_experiment 
group by id_detail_experiment,id_experiment,gamma,nu 

create table detail_dtsvm_stat (
id int not null auto_increment,
id_more_detail_experiment varchar(255) not null,
id_detail_experiment varchar(255) not null,
id_experiment varchar(255) not null,
id_experiment_dt varchar(255) not null,
id_detail_experiment_dt varchar(255) not null,
idx_svm int,
label_org varchar(255),
label_pred varchar(255),
jml int,
primary key (id)
);


alter table dtsvm_stat add foreign key (ID) references PARENT_TABLE(ID) on DELETE CASCADE

CONSTRAINT `fk_tree` FOREIGN KEY (`id_detail`) REFERENCES `detail_experiment` (`id`) ON DELETE CASCADE

select a.id_experiment,label_org,label_pred,jml
from dtsvm_stat a inner join detail_experiment_svm b on a.id_detail_experiment=b.id and a.id_experiment=b.id_experiment
where a.id_experiment in ('1676107807','1676420886') and gamma=0.0001 and nu = 0.01

select depth,minsample,threshold,credal,FP,FN
from detail_experiment_nsl a where a.id_dt_train=a.id_dt_test
order by FP asc,FN asc,depth asc,minsample asc,threshold asc, credal asc