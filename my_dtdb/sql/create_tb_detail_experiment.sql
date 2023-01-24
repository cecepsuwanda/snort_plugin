drop table detail_experiment;
create table detail_experiment (
id varchar(255) not null,
id_experiment varchar(255) not null,
id_dt_train int,
jns_dt_train int,
id_dt_test int,
jns_dt_test int,
depth int,
minsample int,
threshold int,
credal double,
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

drop table detail_experiment;
create table detail_experiment (
id varchar(255) not null,
id_experiment varchar(255) not null,
id_dt_train int,
jns_dt_train int,
id_dt_test int,
jns_dt_test int,
depth int,
minsample int,
threshold double,
credal double,
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

select depth,FP,FN,timediff(if(end_train is null,now(),end_train),start_train) as lama_train from detail_experiment 
where id_dt_train = 2 order by depth desc,start_train desc;

alter table detail_experiment modify column threshold double;