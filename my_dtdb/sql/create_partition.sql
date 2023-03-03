alter table dataset partition by range(id)
(
   partition p11 values less than (128895),
   partition p12 values less than (143217),
   partition p13 values less than (147859),
   partition p21 values less than (1287619),
   partition p22 values less than (1414259),
   partition p23 values less than (1416379)
);

alter table dataset partition by range(id)
(
   partition p11 values less than (85931),
   partition p12 values less than (114574),
   partition p13 values less than (143217),
   partition p14 values less than (147859)   
);

alter table dataset partition by range(id)
(
   partition p11 values less than (42966),
   partition p12 values less than (93092),
   partition p13 values less than (143217),
   partition p14 values less than (147859)   
);

alter table dataset partition by range(id)
(
   partition p11 values less than (14323),
   partition p12 values less than (78770),
   partition p13 values less than (143217),
   partition p14 values less than (147859),   
   partition p21 values less than (274499),
   partition p22 values less than (844379),
   partition p23 values less than (1414259),
   partition p24 values less than (1416379)
);
