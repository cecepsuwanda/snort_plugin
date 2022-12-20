alter table dataset partition by range(id)
(
   partition p11 values less than (128895),
   partition p12 values less than (143217),
   partition p13 values less than (147859),
   partition p21 values less than (1287619),
   partition p22 values less than (1414259),
   partition p23 values less than (1416379)
);