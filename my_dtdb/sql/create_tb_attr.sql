drop table attr0;
create table attr0 (
  id int not null auto_increment,
  duration double,
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr1;
create table attr1 (
  id int not null auto_increment,  
  protocol_type varchar(255),
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr2;
create table attr2 (
  id int not null auto_increment,  
  service varchar(255),  
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr3;
create table attr3 (
  id int not null auto_increment,  
  flag varchar(255),
  label varchar(255),
  jml int,  
  primary key (id)
);

drop table attr4;
create table attr4 (
  id int not null auto_increment,
  src_bytes double,  
  label varchar(255),
  jml int,
  primary key (id)
);


drop table attr5;
create table attr5 (
  id int not null auto_increment,  
  dst_bytes double,  
  label varchar(255),
  jml int,
  primary key (id)
);

drop table attr6;
create table attr6 (
  id int not null auto_increment,
  land varchar(255),  
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr7;
create table attr7 (
  id int not null auto_increment,
  wrong_fragment double,  
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr8;
create table attr8 (
  id int not null auto_increment,
  urgent double,
  label varchar(255), 
  jml int,
  primary key (id)
);

drop table attr9;
create table attr9 (
  id int not null auto_increment,
  count double,
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr10;
create table attr10 (
  id int not null auto_increment,
  srv_count double,  
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr11;
create table attr11 (
  id int not null auto_increment,
  serror_rate double,
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr12;
create table attr12 (
  id int not null auto_increment,
  srv_serror_rate double,
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr13;
create table attr13 (
  id int not null auto_increment,
  rerror_rate double,
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr14;
create table attr14 (
  id int not null auto_increment,
  srv_rerror_rate double,
  label varchar(255), 
  jml int,
  primary key (id)
);

drop table attr15;
create table attr15 (
  id int not null auto_increment,
  same_srv_rate double,  
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr16;
create table attr16 (
  id int not null auto_increment,
  diff_srv_rate double,
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr17;
create table attr17 (
  id int not null auto_increment,
  srv_diff_host_rate double,  
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr18;
create table attr18 (
  id int not null auto_increment,
  dst_host_count double,
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr19;
create table attr19 (
  id int not null auto_increment,
  dst_host_srv_count double,
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr20;
create table attr20 (
  id int not null auto_increment,
  dst_host_same_srv_rate double,
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr21;
create table attr21 (
  id int not null auto_increment,
  dst_host_diff_srv_rate double,
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr22;
create table attr22 (
  id int not null auto_increment,
  dst_host_same_src_port_rate double,
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr23;
create table attr23 (
  id int not null auto_increment,
  dst_host_srv_diff_host_rate double,
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr24;
create table attr24 (
  id int not null auto_increment,
  dst_host_serror_rate double,
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr25;
create table attr25 (
  id int not null auto_increment,
  dst_host_srv_serror_rate double,
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr26;
create table attr26 (
  id int not null auto_increment,
  dst_host_rerror_rate double,
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr27;
drop table attr27;
create table attr27 (
  id int not null auto_increment,
  dst_host_srv_rerror_rate double,  
  label varchar(255),
  jml int, 
  primary key (id)
);

drop table attr28;

