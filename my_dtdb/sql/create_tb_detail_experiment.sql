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

select depth,threshold,credal,TP,TN,FP,FN
from detail_experiment where id_dt_train=1 and minsample=200 and threshold=10
order by depth desc,threshold desc,credal desc

CREATE TABLE `detail_experiment_nsl` (
  `id` varchar(255) NOT NULL,
  `id_experiment` varchar(255) NOT NULL,
  `id_dt_train` int(11) DEFAULT NULL,
  `jns_dt_train` int(11) DEFAULT NULL,
  `id_dt_test` int(11) DEFAULT NULL,
  `jns_dt_test` int(11) DEFAULT NULL,
  `depth` int(11) DEFAULT NULL,
  `minsample` int(11) DEFAULT NULL,
  `threshold` double DEFAULT NULL,
  `credal` double DEFAULT NULL,
  `TP` int(11) DEFAULT NULL,
  `TN` int(11) DEFAULT NULL,
  `FP` int(11) DEFAULT NULL,
  `FN` int(11) DEFAULT NULL,
  `f1` double DEFAULT NULL,
  `start_train` datetime DEFAULT NULL,
  `end_train` datetime DEFAULT NULL,
  `start_test` datetime DEFAULT NULL,
  `end_test` datetime DEFAULT NULL,
  PRIMARY KEY (`id`,`id_experiment`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1


CREATE TABLE `detail_experiment_darpa` (
  `id` varchar(255) NOT NULL,
  `id_experiment` varchar(255) NOT NULL,
  `id_dt_train` int(11) DEFAULT NULL,
  `jns_dt_train` int(11) DEFAULT NULL,
  `id_dt_test` int(11) DEFAULT NULL,
  `jns_dt_test` int(11) DEFAULT NULL,
  `depth` int(11) DEFAULT NULL,
  `minsample` int(11) DEFAULT NULL,
  `threshold` double DEFAULT NULL,
  `credal` double DEFAULT NULL,
  `TP` int(11) DEFAULT NULL,
  `TN` int(11) DEFAULT NULL,
  `FP` int(11) DEFAULT NULL,
  `FN` int(11) DEFAULT NULL,
  `f1` double DEFAULT NULL,
  `start_train` datetime DEFAULT NULL,
  `end_train` datetime DEFAULT NULL,
  `start_test` datetime DEFAULT NULL,
  `end_test` datetime DEFAULT NULL,
  PRIMARY KEY (`id`,`id_experiment`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1

select depth,minsample,threshold,credal,FP,FN from detail_experiment_nsl a 
where a.id_dt_train=a.id_dt_test and threshold=10
order by FP asc,FN asc, depth asc,minsample asc,threshold asc,credal asc

select jns_dt_test,depth,minsample,threshold,
       sum(if(credal=0,FP,0)) as credal0,
       sum(if(credal=0.5,FP,0)) as credal05,
       sum(if(credal=1,FP,0)) as credal1,
       sum(if(credal=1.5,FP,0)) as credal15,
       sum(if(credal=2,FP,0)) as credal2      
from detail_experiment 
group by jns_dt_test,depth,minsample,threshold