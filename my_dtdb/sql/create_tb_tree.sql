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