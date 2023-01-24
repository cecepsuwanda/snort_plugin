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
