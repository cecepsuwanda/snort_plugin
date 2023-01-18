create table tmp_dataset_train_svm (
  id_row int not null,
  child_depth int,
  child_branch int,
  child_branch_number int,
  parent_depth int,
  parent_branch int,
  parent_branch_number int,
  no_svm int,
  label varchar(255),  
  primary key (id_row)  
);

create table tmp_dataset_test_svm (
  id_row int not null,
  child_depth int,
  child_branch int,
  child_branch_number int,
  parent_depth int,
  parent_branch int,
  parent_branch_number int,
  no_svm int,
  label varchar(255),  
  primary key (id_row)  
);