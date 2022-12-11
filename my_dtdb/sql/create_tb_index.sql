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
  id int(11) NOT NULL AUTO_INCREMENT,
  idx_row int(11) NOT NULL,
  PRIMARY KEY (id,idx_row)  
)

insert into tb_index1(idx_row) (select dataset.id from dataset partition(p11) inner join tb_index on dataset.id = idx_row  where 
  (( dataset.duration<=5.500000) and 
    ( dataset.service!='other') and 
    ( dataset.flag!='RSTR') and 
    ( dataset.src_bytes<=16210.500000) and 
    ( dataset.src_bytes>28.500000) and 
    ( dataset.dst_bytes<=8120.500000) and 
    ( dataset.rerror_rate<=0.290000) and 
    ( dataset.srv_rerror_rate<=0.450000) and 
    ( dataset.dst_host_count>1.500000) and 
    ( dataset.dst_host_srv_count>2.500000) and 
    ( dataset.dst_host_diff_srv_rate<=0.155000) and 
    ( dataset.dst_host_serror_rate<=0.195000) and 
    ( dataset.dst_host_rerror_rate<=0.305000)))

insert into tb_index1(idx_row) (select dataset.id from dataset partition(p11) inner join tb_index on dataset.id = idx_row  where (( dataset.protocol_type!='udp') and ( dataset.service!='smtp') and ( dataset.flag!='SF') and ( dataset.src_bytes<=8.500000) and ( dataset.dst_bytes<=3.500000) and ( dataset.land='0') and ( dataset.count>1.500000) and ( dataset.dst_host_count>0.500000) and ( dataset.dst_host_srv_count<=249.500000) and ( dataset.dst_host_same_srv_rate<=0.975000) and ( dataset.dst_host_same_src_port_rate<=0.825000) and ( dataset.dst_host_srv_diff_host_rate<=0.115000)))