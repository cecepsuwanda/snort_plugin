create index idx_label on dataset(id_dt,jns_dt,label);
create index idx_duration_label on dataset(id_dt,jns_dt,duration,label); 
create index idx_protocol_type_label on dataset(id_dt,jns_dt,protocol_type,label); 
create index idx_service_label on dataset(id_dt,jns_dt,service,label); 
create index idx_lag_label on dataset(id_dt,jns_dt,flag,label); 
create index idx_src_bytes_label on dataset(id_dt,jns_dt,src_bytes,label); 
create index idx_dst_bytes_label on dataset(id_dt,jns_dt,dst_bytes,label); 
create index idx_land_label on dataset(id_dt,jns_dt,land,label); 
create index idx_wrong_fragment_label on dataset(id_dt,jns_dt,wrong_fragment,label); 
create index idx_urgent_label on dataset(id_dt,jns_dt,urgent,label); 
create index idx_count_label on dataset(id_dt,jns_dt,count,label); 
create index idx_srv_count_label on dataset(id_dt,jns_dt,srv_count,label); 
create index idx_serror_rate_label on dataset(id_dt,jns_dt,serror_rate,label); 
create index idx_srv_serror_rate_label on dataset(id_dt,jns_dt,srv_serror_rate,label); 
create index idx_rerror_rate_label on dataset(id_dt,jns_dt,rerror_rate,label); 
create index idx_srv_rerror_rate_label on dataset(id_dt,jns_dt,srv_rerror_rate,label); 
create index idx_same_srv_rate_label on dataset(id_dt,jns_dt,same_srv_rate,label); 
create index idx_diff_srv_rate_label on dataset(id_dt,jns_dt,diff_srv_rate,label); 
create index idx_srv_diff_host_rate_label on dataset(id_dt,jns_dt,srv_diff_host_rate,label); 
create index idx_dst_host_count_label on dataset(id_dt,jns_dt,dst_host_count,label); 
create index idx_dst_host_srv_count_label on dataset(id_dt,jns_dt,dst_host_srv_count,label); 
create index idx_dst_host_same_srv_rate_label on dataset(id_dt,jns_dt,dst_host_same_srv_rate,label); 
create index idx_dst_host_diff_srv_rate_label on dataset(id_dt,jns_dt,dst_host_diff_srv_rate,label); 
create index idx_dst_host_same_src_port_rate_label on dataset(id_dt,jns_dt,dst_host_same_src_port_rate,label); 
create index idx_dst_host_srv_diff_host_rate_label on dataset(id_dt,jns_dt,dst_host_srv_diff_host_rate,label); 
create index idx_dst_host_serror_rate_label on dataset(id_dt,jns_dt,dst_host_serror_rate,label); 
create index idx_dst_host_srv_serror_rate_label on dataset(id_dt,jns_dt,dst_host_srv_serror_rate,label); 
create index idx_dst_host_rerror_rate_label on dataset(id_dt,jns_dt,dst_host_rerror_rate,label); 
create index idx_dst_host_srv_rerror_rate_label on dataset(id_dt,jns_dt,dst_host_srv_rerror_rate,label); 



create index idx_duration on dataset(id_dt,jns_dt,duration); 
create index idx_protocol_type on dataset(id_dt,jns_dt,protocol_type); 
create index idx_service on dataset(id_dt,jns_dt,service); 
create index idx_lag_label on dataset(id_dt,jns_dt,flag ); 
create index idx_src_bytes  on dataset(id_dt,jns_dt,src_bytes ); 
create index idx_dst_bytes  on dataset(id_dt,jns_dt,dst_bytes ); 
create index idx_land  on dataset(id_dt,jns_dt,land ); 
create index idx_wrong_fragment  on dataset(id_dt,jns_dt,wrong_fragment ); 
create index idx_urgent  on dataset(id_dt,jns_dt,urgent ); 
create index idx_count  on dataset(id_dt,jns_dt,count ); 
create index idx_srv_count  on dataset(id_dt,jns_dt,srv_count ); 
create index idx_serror_rate  on dataset(id_dt,jns_dt,serror_rate ); 
create index idx_srv_serror_rate  on dataset(id_dt,jns_dt,srv_serror_rate ); 
create index idx_rerror_rate  on dataset(id_dt,jns_dt,rerror_rate ); 
create index idx_srv_rerror_rate  on dataset(id_dt,jns_dt,srv_rerror_rate ); 
create index idx_same_srv_rate  on dataset(id_dt,jns_dt,same_srv_rate ); 
create index idx_diff_srv_rate  on dataset(id_dt,jns_dt,diff_srv_rate ); 
create index idx_srv_diff_host_rate  on dataset(id_dt,jns_dt,srv_diff_host_rate ); 
create index idx_dst_host_count  on dataset(id_dt,jns_dt,dst_host_count ); 
create index idx_dst_host_srv_count  on dataset(id_dt,jns_dt,dst_host_srv_count ); 
create index idx_dst_host_same_srv_rate  on dataset(id_dt,jns_dt,dst_host_same_srv_rate ); 
create index idx_dst_host_diff_srv_rate  on dataset(id_dt,jns_dt,dst_host_diff_srv_rate ); 
create index idx_dst_host_same_src_port_rate  on dataset(id_dt,jns_dt,dst_host_same_src_port_rate ); 
create index idx_dst_host_srv_diff_host_rate  on dataset(id_dt,jns_dt,dst_host_srv_diff_host_rate ); 
create index idx_dst_host_serror_rate  on dataset(id_dt,jns_dt,dst_host_serror_rate ); 
create index idx_dst_host_srv_serror_rate  on dataset(id_dt,jns_dt,dst_host_srv_serror_rate ); 
create index idx_dst_host_rerror_rate  on dataset(id_dt,jns_dt,dst_host_rerror_rate ); 
create index idx_dst_host_srv_rerror_rate  on dataset(id_dt,jns_dt,dst_host_srv_rerror_rate ); 




alter table dataset drop index idx_label;
alter table dataset drop index idx_duration_label; 
alter table dataset drop index idx_protocol_type_label; 
alter table dataset drop index idx_service_label; 
alter table dataset drop index idx_lag_label; 
alter table dataset drop index idx_src_bytes_label; 
alter table dataset drop index idx_dst_bytes_label; 
alter table dataset drop index idx_land_label; 
alter table dataset drop index idx_wrong_fragment_label; 
alter table dataset drop index idx_urgent_label; 
alter table dataset drop index idx_count_label; 
alter table dataset drop index idx_srv_count_label; 
alter table dataset drop index idx_serror_rate_label; 
alter table dataset drop index idx_srv_serror_rate_label; 
alter table dataset drop index idx_rerror_rate_label; 
alter table dataset drop index idx_srv_rerror_rate_label; 
alter table dataset drop index idx_same_srv_rate_label; 
alter table dataset drop index idx_diff_srv_rate_label; 
alter table dataset drop index idx_srv_diff_host_rate_label; 
alter table dataset drop index idx_dst_host_count_label; 
alter table dataset drop index idx_dst_host_srv_count_label; 
alter table dataset drop index idx_dst_host_same_srv_rate_label; 
alter table dataset drop index idx_dst_host_diff_srv_rate_label; 
alter table dataset drop index idx_dst_host_same_src_port_rate_label; 
alter table dataset drop index idx_dst_host_srv_diff_host_rate_label; 
alter table dataset drop index idx_dst_host_serror_rate_label; 
alter table dataset drop index idx_dst_host_srv_serror_rate_label; 
alter table dataset drop index idx_dst_host_rerror_rate_label; 
alter table dataset drop index idx_dst_host_srv_rerror_rate_label;


alter table dataset drop index idx_duration ; 
alter table dataset drop index idx_protocol_type ; 
alter table dataset drop index idx_service ; 
alter table dataset drop index idx_lag ; 
alter table dataset drop index idx_src_bytes ; 
alter table dataset drop index idx_dst_bytes ; 
alter table dataset drop index idx_land ; 
alter table dataset drop index idx_wrong_fragment ; 
alter table dataset drop index idx_urgent ; 
alter table dataset drop index idx_count ; 
alter table dataset drop index idx_srv_count ; 
alter table dataset drop index idx_serror_rate ; 
alter table dataset drop index idx_srv_serror_rate ; 
alter table dataset drop index idx_rerror_rate ; 
alter table dataset drop index idx_srv_rerror_rate ; 
alter table dataset drop index idx_same_srv_rate ; 
alter table dataset drop index idx_diff_srv_rate ; 
alter table dataset drop index idx_srv_diff_host_rate ; 
alter table dataset drop index idx_dst_host_count ; 
alter table dataset drop index idx_dst_host_srv_count ; 
alter table dataset drop index idx_dst_host_same_srv_rate ; 
alter table dataset drop index idx_dst_host_diff_srv_rate ; 
alter table dataset drop index idx_dst_host_same_src_port_rate ; 
alter table dataset drop index idx_dst_host_srv_diff_host_rate ; 
alter table dataset drop index idx_dst_host_serror_rate ; 
alter table dataset drop index idx_dst_host_srv_serror_rate ; 
alter table dataset drop index idx_dst_host_rerror_rate ; 
alter table dataset drop index idx_dst_host_srv_rerror_rate ;


create index idx_duration_label on attr_dataset_train(duration,label); 
create index idx_protocol_type_label on attr_dataset_train(protocol_type,label); 
create index idx_service_label on attr_dataset_train(service,label); 
create index idx_lag_label on attr_dataset_train(flag,label); 
create index idx_src_bytes_label on attr_dataset_train(src_bytes,label); 
create index idx_dst_bytes_label on attr_dataset_train(dst_bytes,label); 
create index idx_land_label on attr_dataset_train(land,label); 
create index idx_wrong_fragment_label on attr_dataset_train(wrong_fragment,label); 
create index idx_urgent_label on attr_dataset_train(urgent,label); 
create index idx_count_label on attr_dataset_train(count,label); 
create index idx_srv_count_label on attr_dataset_train(srv_count,label); 
create index idx_serror_rate_label on attr_dataset_train(serror_rate,label); 
create index idx_srv_serror_rate_label on attr_dataset_train(srv_serror_rate,label); 
create index idx_rerror_rate_label on attr_dataset_train(rerror_rate,label); 
create index idx_srv_rerror_rate_label on attr_dataset_train(srv_rerror_rate,label); 
create index idx_same_srv_rate_label on attr_dataset_train(same_srv_rate,label); 
create index idx_diff_srv_rate_label on attr_dataset_train(diff_srv_rate,label); 
create index idx_srv_diff_host_rate_label on attr_dataset_train(srv_diff_host_rate,label); 
create index idx_dst_host_count_label on attr_dataset_train(dst_host_count,label); 
create index idx_dst_host_srv_count_label on attr_dataset_train(dst_host_srv_count,label); 
create index idx_dst_host_same_srv_rate_label on attr_dataset_train(dst_host_same_srv_rate,label); 
create index idx_dst_host_diff_srv_rate_label on attr_dataset_train(dst_host_diff_srv_rate,label); 
create index idx_dst_host_same_src_port_rate_label on attr_dataset_train(dst_host_same_src_port_rate,label); 
create index idx_dst_host_srv_diff_host_rate_label on attr_dataset_train(dst_host_srv_diff_host_rate,label); 
create index idx_dst_host_serror_rate_label on attr_dataset_train(dst_host_serror_rate,label); 
create index idx_dst_host_srv_serror_rate_label on attr_dataset_train(dst_host_srv_serror_rate,label); 
create index idx_dst_host_rerror_rate_label on attr_dataset_train(dst_host_rerror_rate,label); 
create index idx_dst_host_srv_rerror_rate_label on attr_dataset_train(dst_host_srv_rerror_rate,label);  
