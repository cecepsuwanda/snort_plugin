create table dataset (
  id int not null auto_increment,
  id_dt int,
  jns_dt int,
  duration double,
  protocol_type varchar(255),
  service varchar(255),
  flag varchar(255),
  src_bytes double,
  dst_bytes double,
  land varchar(255),
  wrong_fragment double,
  urgent double,
  count double,
  srv_count double,
  serror_rate double,
  srv_serror_rate double,
  rerror_rate double,
  srv_rerror_rate double,
  same_srv_rate double,
  diff_srv_rate double,
  srv_diff_host_rate double,
  dst_host_count double,
  dst_host_srv_count double,
  dst_host_same_srv_rate double,
  dst_host_diff_srv_rate double,
  dst_host_same_src_port_rate double,
  dst_host_srv_diff_host_rate double,
  dst_host_serror_rate double,
  dst_host_srv_serror_rate double,
  dst_host_rerror_rate double,
  dst_host_srv_rerror_rate double,
  label varchar(255), 
  primary key (id)
);

create table tmp_dataset (
  id int not null,
  depth int,
  branch int,  
  duration double,
  protocol_type varchar(255),
  service varchar(255),
  flag varchar(255),
  src_bytes double,
  dst_bytes double,
  land varchar(255),
  wrong_fragment double,
  urgent double,
  count double,
  srv_count double,
  serror_rate double,
  srv_serror_rate double,
  rerror_rate double,
  srv_rerror_rate double,
  same_srv_rate double,
  diff_srv_rate double,
  srv_diff_host_rate double,
  dst_host_count double,
  dst_host_srv_count double,
  dst_host_same_srv_rate double,
  dst_host_diff_srv_rate double,
  dst_host_same_src_port_rate double,
  dst_host_srv_diff_host_rate double,
  dst_host_serror_rate double,
  dst_host_srv_serror_rate double,
  dst_host_rerror_rate double,
  dst_host_srv_rerror_rate double,
  label varchar(255), 
  primary key (id),
  KEY `idx_duration_label` (`depth`,`branch`,`duration`,`label`),
  KEY `idx_protocol_type_label` (`depth`,`branch`,`protocol_type`,`label`),
  KEY `idx_service_label` (`depth`,`branch`,`service`,`label`),
  KEY `idx_lag_label` (`depth`,`branch`,`flag`,`label`),
  KEY `idx_src_bytes_label` (`depth`,`branch`,`src_bytes`,`label`),
  KEY `idx_dst_bytes_label` (`depth`,`branch`,`dst_bytes`,`label`),
  KEY `idx_land_label` (`depth`,`branch`,`land`,`label`),
  KEY `idx_wrong_fragment_label` (`depth`,`branch`,`wrong_fragment`,`label`),
  KEY `idx_urgent_label` (`depth`,`branch`,`urgent`,`label`),
  KEY `idx_count_label` (`depth`,`branch`,`count`,`label`),
  KEY `idx_srv_count_label` (`depth`,`branch`,`srv_count`,`label`),
  KEY `idx_serror_rate_label` (`depth`,`branch`,`serror_rate`,`label`),
  KEY `idx_srv_serror_rate_label` (`depth`,`branch`,`srv_serror_rate`,`label`),
  KEY `idx_rerror_rate_label` (`depth`,`branch`,`rerror_rate`,`label`),
  KEY `idx_srv_rerror_rate_label` (`depth`,`branch`,`srv_rerror_rate`,`label`),
  KEY `idx_same_srv_rate_label` (`depth`,`branch`,`same_srv_rate`,`label`),
  KEY `idx_diff_srv_rate_label` (`depth`,`branch`,`diff_srv_rate`,`label`),
  KEY `idx_srv_diff_host_rate_label` (`depth`,`branch`,`srv_diff_host_rate`,`label`),
  KEY `idx_dst_host_count_label` (`depth`,`branch`,`dst_host_count`,`label`),
  KEY `idx_dst_host_srv_count_label` (`depth`,`branch`,`dst_host_srv_count`,`label`),
  KEY `idx_dst_host_same_srv_rate_label` (`depth`,`branch`,`dst_host_same_srv_rate`,`label`),
  KEY `idx_dst_host_diff_srv_rate_label` (`depth`,`branch`,`dst_host_diff_srv_rate`,`label`),
  KEY `idx_dst_host_same_src_port_rate_label` (`depth`,`branch`,`dst_host_same_src_port_rate`,`label`),
  KEY `idx_dst_host_srv_diff_host_rate_label` (`depth`,`branch`,`dst_host_srv_diff_host_rate`,`label`),
  KEY `idx_dst_host_serror_rate_label` (`depth`,`branch`,`dst_host_serror_rate`,`label`),
  KEY `idx_dst_host_srv_serror_rate_label` (`depth`,`branch`,`dst_host_srv_serror_rate`,`label`),
  KEY `idx_dst_host_rerror_rate_label` (`depth`,`branch`,`dst_host_rerror_rate`,`label`),
  KEY `idx_dst_host_srv_rerror_rate_label` (`depth`,`branch`,`dst_host_srv_rerror_rate`,`label`),
  KEY `idx_label` (`depth`,`branch`,`label`)
);

create table tmp_dataset (
  id_row int not null,
  child_depth int,
  child_branch int,
  parent_depth int,
  parent_branch int,  
  primary key (id_row)  
);

insert into tb_index1(idx_row) (select dataset.id from dataset partition(p21) inner join tb_index on dataset.id = idx_row  where ((dataset.dst_host_count > 92.500000)))
