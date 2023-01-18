drop table tb_index;
CREATE TABLE tb_index (
  id int(11) NOT NULL AUTO_INCREMENT,
  depth int NOT NULL ,
  branch int NOT NULL, 
  idx_row int(11) DEFAULT NULL,
  PRIMARY KEY (id),
  key idx_depth_branch(depth,branch)
)

drop table tb_index1;
CREATE TABLE tb_index1 (  
  idx_row int(11) NOT NULL,
  PRIMARY KEY (idx_row)  
)

drop table tb_index;
CREATE TABLE tb_index (  
  idx_row int(11) NOT NULL,
  PRIMARY KEY (idx_row)  
)

