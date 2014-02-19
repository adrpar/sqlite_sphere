.load ../build/libsqlitesphere.so

--
-- ellipse and path
--
  
-- negators , commutator @,&&

SELECT srcontainsl( spath( '{(280d, -9d),(280d, -8d)}' )  ,  sellipse( '<{10d,5d},(280d,-20d),90d>' ) );
SELECT srcontainsl( spath( '{(280d, -9d),(280d,-12d)}' )  ,  sellipse( '<{10d,5d},(280d,-20d),90d>' ) );
SELECT srcontainsl( spath( '{(280d,-11d),(280d,-12d)}' )  ,  sellipse( '<{10d,5d},(280d,-20d),90d>' ) );
SELECT soverlaps( spath( '{(280d, -9d),(280d, -8d)}' ) ,  sellipse( '<{10d,5d},(280d,-20d),90d>' ) );
SELECT soverlaps( spath( '{(280d, -9d),(280d,-12d)}' ) ,  sellipse( '<{10d,5d},(280d,-20d),90d>' ) );
SELECT soverlaps( spath( '{(280d,-11d),(280d,-12d)}' ) ,  sellipse( '<{10d,5d},(280d,-20d),90d>' ) );
SELECT srnotcontainsl( spath( '{(280d, -9d),(280d, -8d)}' ) ,  sellipse( '<{10d,5d},(280d,-20d),90d>' ) );
SELECT srnotcontainsl( spath( '{(280d, -9d),(280d,-12d)}' ) ,  sellipse( '<{10d,5d},(280d,-20d),90d>' ) );
SELECT srnotcontainsl( spath( '{(280d,-11d),(280d,-12d)}' ) ,  sellipse( '<{10d,5d},(280d,-20d),90d>' ) );
SELECT snotoverlaps( spath( '{(280d, -9d),(280d, -8d)}' ),  sellipse( '<{10d,5d},(280d,-20d),90d>' ) );
SELECT snotoverlaps( spath( '{(280d, -9d),(280d,-12d)}' ),  sellipse( '<{10d,5d},(280d,-20d),90d>' ) );
SELECT snotoverlaps( spath( '{(280d,-11d),(280d,-12d)}' ),  sellipse( '<{10d,5d},(280d,-20d),90d>' ) );
SELECT slcontainsr( sellipse( '<{10d,5d},(280d,-20d),90d>' )  ,  spath( '{(280d, -9d),(280d, -8d)}' ) );
SELECT slcontainsr( sellipse( '<{10d,5d},(280d,-20d),90d>' )  ,  spath( '{(280d, -9d),(280d,-12d)}' ) );
SELECT slcontainsr( sellipse( '<{10d,5d},(280d,-20d),90d>' )  ,  spath( '{(280d,-11d),(280d,-12d)}' ) );
SELECT soverlaps( sellipse( '<{10d,5d},(280d,-20d),90d>' )  ,  spath( '{(280d, -9d),(280d, -8d)}' ) );
SELECT soverlaps( sellipse( '<{10d,5d},(280d,-20d),90d>' )  ,  spath( '{(280d, -9d),(280d,-12d)}' ) );
SELECT soverlaps( sellipse( '<{10d,5d},(280d,-20d),90d>' )  ,  spath( '{(280d,-11d),(280d,-12d)}' ) );
SELECT slnotcontainsr( sellipse( '<{10d,5d},(280d,-20d),90d>' ) ,  spath( '{(280d, -9d),(280d, -8d)}' ) );
SELECT slnotcontainsr( sellipse( '<{10d,5d},(280d,-20d),90d>' ) ,  spath( '{(280d, -9d),(280d,-12d)}' ) );
SELECT slnotcontainsr( sellipse( '<{10d,5d},(280d,-20d),90d>' ) ,  spath( '{(280d,-11d),(280d,-12d)}' ) );
SELECT snotoverlaps( sellipse( '<{10d,5d},(280d,-20d),90d>' ) ,  spath( '{(280d, -9d),(280d, -8d)}' ) );
SELECT snotoverlaps( sellipse( '<{10d,5d},(280d,-20d),90d>' ) ,  spath( '{(280d, -9d),(280d,-12d)}' ) );
SELECT snotoverlaps( sellipse( '<{10d,5d},(280d,-20d),90d>' ) ,  spath( '{(280d,-11d),(280d,-12d)}' ) );

-- path is a line , ellipse is point
SELECT srcontainsl( spath( '{(280d, -8d),(280d, -9d)}' )  ,  sellipse( '<{0d,0d},(280d,-20d),90d>' ) );
SELECT srcontainsl( spath( '{(280d, -8d),(280d, -9d)}' )  ,  sellipse( '<{0d,0d},(280d, -8d),90d>' ) );
SELECT soverlaps( spath( '{(280d, -8d),(280d, -9d)}' ) ,  sellipse( '<{0d,0d},(280d,-20d),90d>' ) );
SELECT soverlaps( spath( '{(280d, -8d),(280d, -9d)}' ) ,  sellipse( '<{0d,0d},(280d, -8d),90d>' ) );
-- path is a line , ellipse is circle
SELECT srcontainsl( spath( '{(280d, -8d),(280d, -9d)}' )  ,  sellipse( '<{5d,5d},(280d,-20d),90d>' ) );
SELECT srcontainsl( spath( '{(280d, -8d),(280d, -9d)}' )  ,  sellipse( '<{5d,5d},(280d, -8d),90d>' ) );
SELECT soverlaps( spath( '{(280d, -8d),(280d, -9d)}' ) ,  sellipse( '<{5d,5d},(280d,-20d),90d>' ) );
SELECT soverlaps( spath( '{(280d, -8d),(280d, -9d)}' ) ,  sellipse( '<{5d,5d},(280d, -8d),90d>' ) );
-- path is a line , ellipse is path
SELECT srcontainsl( spath( '{(280d, -8d),(280d, -9d)}' )  ,  sellipse( '<{5d,0d},(280d,-20d),90d>' ) );
SELECT srcontainsl( spath( '{(280d, -8d),(280d, -9d)}' )  ,  sellipse( '<{5d,0d},(280d, -8d),90d>' ) );
SELECT soverlaps( spath( '{(280d, -8d),(280d, -9d)}' ) ,  sellipse( '<{5d,0d},(280d,-20d),90d>' ) );
SELECT soverlaps( spath( '{(280d, -8d),(280d, -9d)}' ) ,  sellipse( '<{5d,0d},(280d, -8d),90d>' ) );
-- path is a line , ellipse is a real ellipse
SELECT srcontainsl( spath( '{(280d, -8d),(280d, -9d)}' )  ,  sellipse( '<{10d,5d},(280d,-20d),90d>' ) );
SELECT srcontainsl( spath( '{(280d, -8d),(280d, -9d)}' )  ,  sellipse( '<{10d,5d},(280d, -8d),90d>' ) );
SELECT soverlaps( spath( '{(280d, -8d),(280d, -9d)}' ) ,  sellipse( '<{10d,5d},(280d,-20d),90d>' ) );
SELECT soverlaps( spath( '{(280d, -8d),(280d, -9d)}' ) ,  sellipse( '<{10d,5d},(280d, -8d),90d>' ) );

--
-- checking path other operators
--
  
SELECT slength( spath( '{(0,0),(1,0),(2,0),(3,0)}' ) );
SELECT sequal( spath ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ) ,  spath ('{(0,0),(1,0),(2,0),(3,0)}' ));
SELECT sequal( spath ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ) ,  spath ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ));
SELECT snotequal( spath ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ) ,  spath ('{(0,0),(1,0),(2,0),(3,0)}' ) );
SELECT snotequal( spath ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ) ,  spath ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ) );
SELECT soverlaps( spoly ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ) , spath ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ) );
SELECT soverlaps( spath ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ), spoly ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' )  );
SELECT srcontainsl( spath ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ),  spoly ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' )  );
SELECT srcontainsl( spath ('{(0,0),(1,0),(2,0),(3,0)}' ),  spoly ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' )  );
SELECT soverlaps( spath ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ), spath ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ) );
SELECT soverlaps( spath ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ), spath ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ) );
SELECT soverlaps( spoly ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ) , spath ('{(0,0),(1,0),(2,0),(3,0)}' ) );
SELECT soverlaps( spath ('{(0,0),(1,0),(2,0),(3,0)}' ), spoly ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' )  );
SELECT soverlaps( spath ('{(0,0),(1,0),(2,0),(3,0)}' ), spath ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ) );
SELECT soverlaps( spoly ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ) , spath ('{(0,0),(0,1),(0,1.5)}' ) );
SELECT soverlaps( spath ('{(0,0),(0,1),(0,1.5)}' ), spoly ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' )  );
SELECT soverlaps( spath ('{(0,0),(0,1),(0,1.5)}' ), spath ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ) );
SELECT soverlaps( spath ('{(0,0),(0,1),(0,1.5)}' ), spath ('{(0,0),(1,0),(2,0),(3,0)}' ) );
SELECT srcontainsl( spath ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ),  scircle( '<(0,1),1>' ) );
SELECT srcontainsl( spath ('{(0,0),(0,1),(0,1.5)}' ),  scircle( '<(0,1),1>' ) );
SELECT srcontainsl( spath ('{(0,0),(0,1),(0,1.5)}' ),  scircle( '<(0,1),0.7>' ) );
SELECT soverlaps( spath ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ), scircle( '<(0,1),1>' ) );
SELECT soverlaps( spath ('{(0,0),(0,1),(0,1.5)}' ), scircle( '<(0,1),1>' ) );
SELECT soverlaps( spath ('{(0,0),(0,1),(0,1.5)}' ), scircle( '<(0,1),0.7>' ) );
SELECT soverlaps( spath ('{(0,0),(0,1),(0,1.5)}' ), scircle( '<(0,-1),0.7>' ) );
SELECT srcontainsl( spath ('{(0,0),(0,1),(0,1.5)}' ),  scircle( '<(0,-1),0.7>' ) );
SELECT soverlaps( spath ('{(0,0),(0,1),(0,1.5)}' ), sline ( spoint( '(0,-1)' ), spoint( '(0,1)' ) ) );
SELECT soverlaps( spath ('{(0,0),(0,1),(0,1.5)}' ), sline ( spoint( '(-1,0)' ), spoint( '(1,0)' ) ) );
SELECT soverlaps( spath ('{(0,0),(0,1),(0,1.5)}' ), sline ( spoint( '(-1,0)' ), spoint( '(-0.3,0)' ) ) );
SELECT srcontainsl( spath( '{(0.11,0.15),(0.12,0.15),(0.13,0.15)}' ), spoly ('{(0.1,0),(0.2,0),(0.2,0.1),(0.3,0.1),(0.3,-0.1),(0.4,-0.1),(0.5,0.1),(0.4,0.2),(0.1,0.2)}' ) );

-- create path
SELECT sstr( spath_aggr(data.p), "RAD", 8 ) FROM ( SELECT spoint( '(0,1)' ) as p UNION ALL SELECT spoint( '(1,1)' ) UNION ALL SELECT spoint( '(1,0)' ) ) AS data ;

-- test stored data
SELECT sstr( spoint(path, 2), "DEG", 8 ) FROM spherepath;

