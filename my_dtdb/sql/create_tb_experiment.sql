create table experiment (
id varchar(255) not null,
depth_awal int,
depth_akhir int,
depth_step int,
min_sample_awal int,
min_sample_akhir int,
min_sample_step int,
threshold_awal int,
threshold_akhir int,
threshold_step int,
credal_awal double,
credal_akhir double,
credal_step double,
id_dt_train int,
jns_dt_train int,
partition_train varchar(255),
id_dt_test int,
jns_dt_test int,
partition_test varchar(255),
mulai datetime,
akhir datetime,
primary key (id)
);