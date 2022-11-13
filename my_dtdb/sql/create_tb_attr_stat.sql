drop table attr_stat;
create table attr_stat (
  id int not null,
  attr varchar(255),
  is_continue int,
  jml int, 
  primary key (id)
);
