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
  child_branch_number int,
  parent_depth int,
  parent_branch int,
  parent_branch_number int,  
  primary key (id_row)  
);

drop table tmp_dataset;
create table tmp_dataset_train (
  id_row int not null,
  child_depth int,
  child_branch int,
  child_branch_number int,
  parent_depth int,
  parent_branch int,
  parent_branch_number int,  
  primary key (id_row)  
);

create index posisi_child_train on tmp_dataset_train(child_depth,child_branch_number,child_branch);
create index posisi_parent_train on tmp_dataset_train(parent_depth,parent_branch_number,parent_branch); 

create index posisi_child_test on tmp_dataset_test(child_depth,child_branch_number,child_branch);
create index posisi_parent_test on tmp_dataset_test(parent_depth,parent_branch_number,parent_branch);  


create table tmp_dataset_test (
  id_row int not null,
  child_depth int,
  child_branch int,
  child_branch_number int,
  parent_depth int,
  parent_branch int,
  parent_branch_number int,  
  primary key (id_row)  
);


drop trigger after_tmp_dataset_test_update;

drop trigger after_tmp_dataset_train_update;

-- create trigger after_tmp_dataset_train_update
--       after update on tmp_dataset_train
--       for each row
--       begin         
--             if (new.child_depth>new.parent_depth) then
--               replace into detail_missing_branch(id_row,child_depth,child_branch,child_branch_number,parent_depth,parent_branch,parent_branch_number) values(new.id_row,new.child_depth,new.child_branch,new.child_branch_number,new.parent_depth,new.parent_branch,new.parent_branch_number);
--             end if;      
--       end; 


create trigger after_missing_branch_insert
    after insert on missing_branch
    for each row
    begin 
        if not exists(select * from tmp_dataset_train where child_depth=new.child_depth and child_branch=new.child_branch and child_branch_number=new.child_branch_number and parent_depth=new.parent_depth and parent_branch=new.parent_branch and parent_branch_number=new.parent_branch_number) then
           insert into detail_missing_branch (select * from tmp_dataset_train where child_depth=new.child_depth and child_branch=new.child_branch and child_branch_number=new.child_branch_number and parent_depth=new.parent_depth and parent_branch=new.parent_branch and parent_branch_number=new.parent_branch_number);
        end if;
    end;

create procedure sp_child_to_tmp_dataset(in nm_table text,in partisi text)
begin 
  set @sql := CONCAT('truncate ',nm_table,';');
  PREPARE stmt FROM @sql;
  EXECUTE stmt;
  DEALLOCATE PREPARE stmt;
  set @sql := CONCAT('insert into ',nm_table,' (select id,0,0,0,0,0,0 from dataset partition( ',partisi,' ) order by label,id);');
  PREPARE stmt FROM @sql;
  EXECUTE stmt;
  DEALLOCATE PREPARE stmt;
  truncate tb_index;
  set @sql := CONCAT('insert into tb_index(idx_row) (select id_row from ',nm_table,' where child_depth=0 and child_branch=0 and child_branch_number=0 order by id_row);');
  PREPARE stmt FROM @sql;
  EXECUTE stmt;
  DEALLOCATE PREPARE stmt;
end;


create procedure sp_filter(in child_depth int,in child_branch int,in child_branch_number int,in parent_depth int,in parent_branch int,in parent_branch_number int,in query text,in nm_table text,in partisi text)
begin
  set @sql := CONCAT('insert into tb_index(idx_row) (select id_row from ',nm_table,' where child_depth=',parent_depth,' and child_branch=',parent_branch,' and child_branch_number=',parent_branch_number,' order by id_row);');
  PREPARE stmt FROM @sql;
  EXECUTE stmt;
  DEALLOCATE PREPARE stmt;  

  set @sql := CONCAT('insert into tb_index1(idx_row) (select dataset.id from dataset partition(',partisi,') inner join tb_index on dataset.id = idx_row  where (',query,' ));');
  PREPARE stmt FROM @sql;
  EXECUTE stmt;
  DEALLOCATE PREPARE stmt;  

  set @sql := CONCAT('update (',nm_table,' inner join tb_index1 on id_row=idx_row) set ',nm_table,'.child_depth=',child_depth,', ',nm_table,'.child_branch=',child_branch,', ',nm_table,'.child_branch_number=',child_branch_number,', ',nm_table,'.parent_depth=',parent_depth,', ',nm_table,'.parent_branch=',parent_branch,', ',nm_table,'.parent_branch_number=',parent_branch_number,' where (',nm_table,'.child_depth=',parent_depth,' and ',nm_table,'.child_branch=',parent_branch,' and ',nm_table,'.child_branch_number=',parent_branch_number,');');
  PREPARE stmt FROM @sql;
  EXECUTE stmt;
  DEALLOCATE PREPARE stmt; 
end;

create procedure sp_filter1(in child_depth int,in child_branch int,in child_branch_number int,in parent_depth int,in parent_branch int,in parent_branch_number int,in nm_table text)
begin  

  set @sql := CONCAT('insert into tb_index1(idx_row) (select id_row from detail_missing_branch  where (child_depth=',child_depth,' and child_branch=',child_branch,' and child_branch_number=',child_branch_number,' and parent_depth=',parent_depth,' and parent_branch=',parent_branch,' and parent_branch_number=',parent_branch_number,'));');
  PREPARE stmt FROM @sql;
  EXECUTE stmt;
  DEALLOCATE PREPARE stmt;  

  set @sql := CONCAT('update (',nm_table,' inner join tb_index1 on id_row=idx_row) set ',nm_table,'.child_depth=',child_depth,', ',nm_table,'.child_branch=',child_branch,', ',nm_table,'.child_branch_number=',child_branch_number,', ',nm_table,'.parent_depth=',parent_depth,', ',nm_table,'.parent_branch=',parent_branch,', ',nm_table,'.parent_branch_number=',parent_branch_number,';');
  PREPARE stmt FROM @sql;
  EXECUTE stmt;
  DEALLOCATE PREPARE stmt; 
end;


create procedure sp_filter2(in child_depth int,in child_branch int,in child_branch_number int,in parent_depth int,in parent_branch int,in parent_branch_number int,in query text,in nm_table text,in partisi text)
begin  
  set @sql := CONCAT('insert into tb_index1(idx_row) (select dataset.id from dataset partition(',partisi,') where (',query,' ));');
  PREPARE stmt FROM @sql;
  EXECUTE stmt;
  DEALLOCATE PREPARE stmt;  

  set @sql := CONCAT('update (',nm_table,' inner join tb_index1 on id_row=idx_row) set ',nm_table,'.child_depth=',child_depth,', ',nm_table,'.child_branch=',child_branch,', ',nm_table,'.child_branch_number=',child_branch_number,', ',nm_table,'.parent_depth=',parent_depth,', ',nm_table,'.parent_branch=',parent_branch,', ',nm_table,'.parent_branch_number=',parent_branch_number,';');
  PREPARE stmt FROM @sql;
  EXECUTE stmt;
  DEALLOCATE PREPARE stmt; 
end;


create procedure sp_filter3(in child_depth int,in child_branch int,in child_branch_number int,in parent_depth int,in parent_branch int,in parent_branch_number int,in nm_table text)
begin
  set @sql := CONCAT('insert into tb_index(idx_row) (select id_row from ',nm_table,'  where child_depth=',child_depth,' and child_branch=',child_branch,' and child_branch_number=',child_branch_number,' and parent_depth=',parent_depth,' and parent_branch=',parent_branch,' and parent_branch_number=',parent_branch_number,' order by id_row);');
  PREPARE stmt FROM @sql;
  EXECUTE stmt;
  DEALLOCATE PREPARE stmt;  
end;
