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


alter table nsl.dataset partition by range(id)(
   partition p11 values less than (123065),
   partition p12 values less than (136734),
   partition p21 values less than (259798),
   partition p22 values less than (273467),
   partition p31 values less than (396531),
   partition p32 values less than (410200),
   partition p41 values less than (533264),
   partition p42 values less than (546933),
   partition p51 values less than (669997),
   partition p52 values less than (683666),
   partition p61 values less than (806730),
   partition p62 values less than (820399),
   partition p71 values less than (943463),
   partition p72 values less than (957132),
   partition p81 values less than (1080196),
   partition p82 values less than (1093865),
   partition p91 values less than (1216929),
   partition p92 values less than (1230598),
   partition p101 values less than (1353619),
   partition p102 values less than (1367331),
   partition p111 values less than (1378230),
   partition p211 values less than (2351048),
   partition p212 values less than (2459137),
   partition p221 values less than (3431955),
   partition p222 values less than (3540044),
   partition p231 values less than (4512862),
   partition p232 values less than (4620951),
   partition p241 values less than (5593769),
   partition p242 values less than (5701858),
   partition p251 values less than (6674676),
   partition p252 values less than (6782765),
   partition p261 values less than (7755583),
   partition p262 values less than (7863672),
   partition p271 values less than (8836490),
   partition p272 values less than (8944579),
   partition p281 values less than (9917397),
   partition p282 values less than (10025486),
   partition p291 values less than (10998304),
   partition p292 values less than (11106393),
   partition p2101 values less than (12079194),
   partition p2102 values less than (12187300),
   partition p2111 values less than (12191389),
   partition p12341 values less than (12273446),
   partition p12342 values less than (12328122),
   partition p23451 values less than (12410179),
   partition p23452 values less than (12464855),
   partition p34561 values less than (12546912),
   partition p34562 values less than (12601588),
   partition p45671 values less than (12683645),
   partition p45672 values less than (12738321),
   partition p56781 values less than (12820378),
   partition p56782 values less than (12875054),
   partition p67891 values less than (12957111),
   partition p67892 values less than (13011787),
   partition p789101 values less than (13093801),
   partition p789102 values less than (13148520),
   partition p812341 values less than (13797071),
   partition p812342 values less than (14229427),
   partition p823451 values less than (14877978),
   partition p823452 values less than (15310334),
   partition p834561 values less than (15958885),
   partition p834562 values less than (16391241),
   partition p845671 values less than (17039792),
   partition p845672 values less than (17472148),
   partition p856781 values less than (18120699),
   partition p856782 values less than (18553055),
   partition p867891 values less than (19201606),
   partition p867892 values less than (19633962),
   partition p8789101 values less than (20282496),
   partition p8789102 values less than (20714869))
