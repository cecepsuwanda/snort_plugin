create table tree (
id int not null auto_increment,
id_dt int,
jns_dt int,
depth int,
minsample int,
threshold int,
credal double,
attrindex int,
attrvalue varchar(255),
label varchar(255),
treeindex int,
isleaf int,
opt int,
child1 int,
child2 int,
idx_svm int,
primary key (id)
);

create table tree (
id int not null auto_increment,
id_detail varchar(255) not null,
id_experiment varchar(255) not null,
attrindex int,
attrvalue varchar(255),
label varchar(255),
treeindex int,
isleaf int,
opt int,
child1 int,
child2 int,
idx_svm int,
primary key (id)
);

create table missing_branch (
child_depth int,
child_branch int,
child_branch_number int,
parent_depth int,
parent_branch int,
parent_branch_number int,
attrindex int default -1,
attrvalue varchar(255) default "-1",
label varchar(255) default "-1",
opt int default -1,
is_pure int default -1,
is_min_sample int default -1,
is_depth_limit int default -1,
is_same_label int default -1,
is_pruning int default -1,
is_not_split int default -1,
is_lanjut int default 1,
primary key (child_depth,child_branch,child_branch_number,parent_depth,parent_branch,parent_branch_number)
);

drop table detail_missing_branch;
create table detail_missing_branch (  
  id_row int,
  child_depth int,
  child_branch int,
  child_branch_number int,
  parent_depth int,
  parent_branch int,
  parent_branch_number int,  
  primary key (id_row,child_depth,child_branch,child_branch_number,parent_depth,parent_branch,parent_branch_number)  
);


drop trigger after_missing_branch_delete;
create trigger after_missing_branch_delete
      after delete on missing_branch
      for each row
      begin
         delete from detail_missing_branch where child_depth = old.child_depth and child_branch = old.child_branch and child_branch_number = old.child_branch_number and parent_depth = old.parent_depth and parent_branch = old.parent_branch and parent_branch_number old.parent_branch_number;           
      end; 




CREATE TABLE `tree_nsl` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `attrindex` int(11) DEFAULT NULL,
  `attrvalue` varchar(255) DEFAULT NULL,
  `label` varchar(255) DEFAULT NULL,
  `treeindex` int(11) DEFAULT NULL,
  `isleaf` int(11) DEFAULT NULL,
  `opt` int(11) DEFAULT NULL,
  `child1` int(11) DEFAULT NULL,
  `child2` int(11) DEFAULT NULL,
  `idx_svm` int(11) DEFAULT NULL,
  `id_detail` varchar(255) DEFAULT NULL,
  `id_experiment` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_tree` (`id_detail`),
  CONSTRAINT `fk_tree_nsl` FOREIGN KEY (`id_detail`) REFERENCES `detail_experiment_nsl` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1

CREATE TABLE `tree_darpa` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `attrindex` int(11) DEFAULT NULL,
  `attrvalue` varchar(255) DEFAULT NULL,
  `label` varchar(255) DEFAULT NULL,
  `treeindex` int(11) DEFAULT NULL,
  `isleaf` int(11) DEFAULT NULL,
  `opt` int(11) DEFAULT NULL,
  `child1` int(11) DEFAULT NULL,
  `child2` int(11) DEFAULT NULL,
  `idx_svm` int(11) DEFAULT NULL,
  `id_detail` varchar(255) DEFAULT NULL,
  `id_experiment` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_tree` (`id_detail`),
  CONSTRAINT `fk_tree_darpa` FOREIGN KEY (`id_detail`) REFERENCES `detail_experiment_darpa` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1