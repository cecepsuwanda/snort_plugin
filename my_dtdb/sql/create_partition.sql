alter table dataset partition by range(id)
(
   partition p11 values less than (128895),
   partition p12 values less than (143217),
   partition p13 values less than (1014750),
   partition p21 values less than (2154510),
   partition p22 values less than (2281150),
   partition p23 values less than (2283270)
);