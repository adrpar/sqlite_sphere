.load ../build/libsqlitesphere.so

-- operators
SELECT sequal( spoint( '(0, 90d)' )  , spoint( '(0, 90d)' ) );
SELECT sequal( spoint( '(0, 90d)' )  , spoint( '(0,-90d)' ) );
SELECT sequal( spoint( '(0,-90d)' )  , spoint( '(0,-90d)' ) );
SELECT snotequal( spoint( '(0, 90d)' ) , spoint( '(0, 90d)' ) );
SELECT snotequal( spoint( '(0, 90d)' ) , spoint( '(0,-90d)' ) );
SELECT snotequal( spoint( '(0,-90d)' ) , spoint( '(0,-90d)' ) );
SELECT sequal( spoint( '(0d,  0)' )  , spoint( '(360d,0)' ) );

-- I/O test --

-- Output ---

SELECT sstr( spoint( '( 1h 2m 30s , +1d 2m 30s)' ), "DEG", 8 );

SELECT sstr( spoint( '( 0h 2m 30s , +0d 2m 30s)' ), "DEG", 8 );

SELECT sstr( spoint( '( 1h 3m 30s , -1d 3m 3.6s)' ), "DEG", 8 );

SELECT sstr( spoint( '( 0h 3m 30s , -0d 3m 3.6s)' ), "DEG", 8 );

SELECT sstr( spoint( '( 0h 2m 30s , 90d 0m 0s)' ), "DEG", 8 );

SELECT sstr( spoint( '( 0h 2m 30s ,-90d 0m 0s)' ), "DEG", 8 );


SELECT sstr( spoint( '( 1h 2m 30s , +1d 2m 30s)' ), "DMS", 8 );

SELECT sstr( spoint( '( 0h 2m 30s , +0d 2m 30s)' ), "DMS", 8 );

SELECT sstr( spoint( '( 1h 2m 30s , -1d 2m 30s)' ), "DMS", 8 );

SELECT sstr( spoint( '( 0h 2m 30s , -0d 2m 30s)' ), "DMS", 8 );

SELECT sstr( spoint( '( 0h 2m 30s , 90d 0m 0s)' ), "DMS", 8 );

SELECT sstr( spoint( '( 0h 2m 30s ,-90d 0m 0s)' ), "DMS", 8 );



SELECT sstr( spoint( '( 1h 2m 30s , +1d 2m 30s)' ), "HMS", 8 );

SELECT sstr( spoint( '( 0h 2m 30s , +0d 2m 30s)' ), "HMS", 8 );

SELECT sstr( spoint( '( 1h 2m 30s , -1d 2m 30s)' ), "HMS", 8 );

SELECT sstr( spoint( '( 0h 2m 30s , -0d 2m 30s)' ), "HMS", 8 );

SELECT sstr( spoint( '( 0h 2m 30s , 90d 0m 0s)' ), "HMS", 8 );

SELECT sstr( spoint( '( 0h 2m 30s ,-90d 0m 0s)' ), "HMS", 8 );



SELECT sstr( spoint( '( 0h 0m 0s , 0d 0m 0s)' ), "RAD", 8 );

-- "incorrect dec. values"



SELECT sstr( spoint( '( 0h 2m 30s , 95d 0m 0s)' ), "DEG", 8 );

SELECT sstr( spoint( '( 24h 2m 30s , 5d 0m 0s)' ), "DEG", 8 );

SELECT sstr( spoint( '( -0h 2m 30s , -5d 0m 0s)' ), "DEG", 8 );

SELECT sstr( spoint( '( 0h 2m 30s , -95d 0m 0s)' ), "DEG", 8 );

-- Input --

SELECT sstr( spoint( '( 0.625d , -0.04166666667d)' ), "DEG", 8 );

SELECT sstr( spoint( '(0.0109083078249646 , -0.000727220521664407)' ), "DEG", 8 );

-- functions for point --------------

-- sstr( spoint(float8, float8 ))

SELECT sstr( spoint(0.0109083078249646 , -0.000727220521664407), "DEG", 8 );



SELECT sstr( spoint(7.28318530717958623 , 0.00), "RAD", 8 );

SELECT sstr( spoint(0.0 , 2.141592653589793116), "RAD", 8 );

-- sdist(spoint, spoint)

SELECT sdist(spoint( '( 0h 2m 30s , 0d 0m 0s)' ) , spoint( '( 0h 0m 30s , 0d 0m 0s)' ) );

SELECT sdist(spoint( '( 0h 2m 30s , 0d 0m 0s)' ) , spoint( '( 0h 2m 30s , 10d 0m 0s)' ) );

SELECT sdist(spoint( '( 0h 2m 30s , 0d 0m 0s)' ) , spoint( '( 0h 2m 30s , -10d 0m 0s)' ) );

SELECT sdist(spoint( '( 0h 2m 30s , 95d 0m 0s)' ) , spoint( '( 12h 2m 30s , 85d 0m 0s)' ) );

SELECT sdist(spoint( '( 24h 2m 30s , 10d 0m 0s)' ) , spoint( '( 0h 2m 30s , -10d 0m 0s)' ) );

SELECT sdist(spoint( '( 0h 2m 30s , 90d 0m 0s)' ) , spoint( '( 12h 2m 30s , 90d 0m 0s)' ) );

SELECT sdist(spoint( '( 0h 2m 30s , -90d 0m 0s)' ) , spoint( '( 12h 2m 30s , -90d 0m 0s)' ) );

-- spoint_long(spoint)

SELECT spoint_long(spoint( '( 0h 2m 30s , 0d 0m 0s)' ) );

SELECT spoint_long(spoint( '( 0h 2m 30s ,95d 0m 0s)' ) );

SELECT spoint_long(spoint( '( 0h 2m 30s ,85d 0m 0s)' ) );

SELECT spoint_long(spoint( '( 0h 2m 30s ,-95d 0m 0s)' ) );

SELECT spoint_long(spoint( '( 0h 2m 30s ,-85d 0m 0s)' ) );

SELECT spoint_long(spoint( '( 0h 2m 30s ,90d 0m 0s)' ) );

SELECT spoint_long(spoint( '( 0h 2m 30s ,-90d 0m 0s)' ) );

SELECT spoint_long(spoint( '(24h 2m 30s , 0d 0m 0s)' ) );

SELECT spoint_long(spoint( '(24h 2m 30s ,95d 0m 0s)' ) );

SELECT spoint_long(spoint( '(24h 2m 30s ,85d 0m 0s)' ) );

SELECT spoint_long(spoint( '(24h 2m 30s ,-95d 0m 0s)' ) );

SELECT spoint_long(spoint( '(24h 2m 30s ,-85d 0m 0s)' ) );

SELECT spoint_long(spoint( '(24h 2m 30s ,90d 0m 0s)' ) );

SELECT spoint_long(spoint( '(24h 2m 30s ,-90d 0m 0s)' ) );

-- spoint_lat(spoint)

SELECT spoint_lat(spoint( '( 0h 2m 30s , 0d 0m 0s)' ) );

SELECT spoint_lat(spoint( '( 0h 2m 30s ,95d 0m 0s)' ) );

SELECT spoint_lat(spoint( '( 0h 2m 30s ,85d 0m 0s)' ) );

SELECT spoint_lat(spoint( '( 0h 2m 30s ,-95d 0m 0s)' ) );

SELECT spoint_lat(spoint( '( 0h 2m 30s ,-85d 0m 0s)' ) );

SELECT spoint_lat(spoint( '( 0h 2m 30s ,90d 0m 0s)' ) );

SELECT spoint_lat(spoint( '( 0h 2m 30s ,-90d 0m 0s)' ) );

SELECT spoint_lat(spoint( '(24h 2m 30s , 0d 0m 0s)' ) );

SELECT spoint_lat(spoint( '(24h 2m 30s ,95d 0m 0s)' ) );

SELECT spoint_lat(spoint( '(24h 2m 30s ,85d 0m 0s)' ) );

SELECT spoint_lat(spoint( '(24h 2m 30s ,-95d 0m 0s)' ) );

SELECT spoint_lat(spoint( '(24h 2m 30s ,-85d 0m 0s)' ) );

SELECT spoint_lat(spoint( '(24h 2m 30s ,90d 0m 0s)' ) );

SELECT spoint_lat(spoint( '(24h 2m 30s ,-90d 0m 0s)' ) );

-- xyz(spoint)

SELECT spoint_x(spoint( '(0d,-90d)' ) );

SELECT spoint_y(spoint( '(0d,-90d)' ) );

SELECT spoint_z(spoint( '(0d,-90d)' ) );

-- operators for points -------------

-- opsequal( erator ---------------------- )-

SELECT sequal( spoint( '( 0h 2m 30s , 90d 0m 0s)' ) , spoint( '( 12h 2m 30s , 90d 0m 0s)' ) );

SELECT sequal( spoint( '( 12h 2m 30s , 90d 0m 0s)' ) , spoint( '( 12h 2m 30s , 90d 0m 0s)' ) );

SELECT sequal( spoint( '( 0h 2m 30s , 90d 0m 0s)' ) , spoint( '( 24h 2m 30s , 90d 0m 0s)' ) );

SELECT sequal( spoint( '( 0h 2m 30s , -90d 0m 0s)' ) , spoint( '( 12h 2m 30s , -90d 0m 0s)' ) );

SELECT sequal( spoint( '( 12h 2m 30s , -90d 0m 0s)' ) , spoint( '( 12h 2m 30s , -90d 0m 0s)' ) );

SELECT sequal( spoint( '( 0h 2m 30s , -90d 0m 0s)' ) , spoint( '( 24h 2m 30s , -90d 0m 0s)' ) );

SELECT sequal( spoint( '( 0h 2m 30s , 95d 0m 0s)' ) , spoint( '( 12h 2m 30s , 85d 0m 0s)' ) );

SELECT sequal( spoint( '( 24h 2m 30s , 10d 0m 0s)' ) , spoint( '( 0h 2m 30s , 10d 0m 0s)' ) );

SELECT sequal( spoint( '( 0h 2m 30s , 90d 0m 0s)' ) , spoint( '( 12h 2m 30s , -90d 0m 0s)' ) );

SELECT sequal( spoint( '( 12h 2m 30s , 90d 0m 0s)' ) , spoint( '( 12h 2m 30s , -90d 0m 0s)' ) );

SELECT sequal( spoint( '( 0h 2m 30s , 0d 0m 0s)' ) , spoint( '( 12h 2m 30s , 45d 0m 0s)' ) );

-- <> operator -----------------------

SELECT snotequal( spoint( '( 0h 2m 30s , 90d 0m 0s)' ) , spoint( '( 12h 2m 30s , 90d 0m 0s)' ) );

SELECT snotequal( spoint( '( 12h 2m 30s , 90d 0m 0s)' ) , spoint( '( 12h 2m 30s , 90d 0m 0s)' ) );

SELECT snotequal( spoint( '( 0h 2m 30s , 90d 0m 0s)' ) , spoint( '( 24h 2m 30s , 90d 0m 0s)' ) );

SELECT snotequal( spoint( '( 0h 2m 30s , -90d 0m 0s)' ) , spoint( '( 12h 2m 30s , -90d 0m 0s)' ) );

SELECT snotequal( spoint( '( 12h 2m 30s , -90d 0m 0s)' ) , spoint( '( 12h 2m 30s , -90d 0m 0s)' ) );

SELECT snotequal( spoint( '( 0h 2m 30s , -90d 0m 0s)' ) , spoint( '( 24h 2m 30s , -90d 0m 0s)' ) );

SELECT snotequal( spoint( '( 0h 2m 30s , 95d 0m 0s)' ) , spoint( '( 12h 2m 30s , 85d 0m 0s)' ) );

SELECT snotequal( spoint( '( 24h 2m 30s , 10d 0m 0s)' ) , spoint( '( 0h 2m 30s , 10d 0m 0s)' ) );

SELECT snotequal( spoint( '( 0h 2m 30s , 90d 0m 0s)' ) , spoint( '( 12h 2m 30s , -90d 0m 0s)' ) );

SELECT snotequal( spoint( '( 12h 2m 30s , 90d 0m 0s)' ) , spoint( '( 12h 2m 30s , -90d 0m 0s)' ) );

SELECT snotequal( spoint( '( 0h 2m 30s , 0d 0m 0s)' ) , spoint( '( 12h 2m 30s , 45d 0m 0s)' ) );

-- <-> operator ---------------------

SELECT sdist( spoint( '( 0h 2m 30s , 0d 0m 0s)' ) , spoint( '( 0h 0m 30s , 0d 0m 0s)' ) );

SELECT sdist( spoint( '( 0h 2m 30s , 0d 0m 0s)' ) , spoint( '( 0h 2m 30s , 10d 0m 0s)' ) );

SELECT sdist( spoint( '( 0h 2m 30s , 0d 0m 0s)' ) , spoint( '( 0h 2m 30s , -10d 0m 0s)' ) );

SELECT sdist( spoint( '( 0h 2m 30s , 95d 0m 0s)' ) , spoint( '( 12h 2m 30s , 85d 0m 0s)' ) );

SELECT sdist( spoint( '( 24h 2m 30s , 10d 0m 0s)' ) , spoint( '( 0h 2m 30s , -10d 0m 0s)' ) );

SELECT sdist( spoint( '( 0h 2m 30s , 90d 0m 0s)' ) , spoint( '( 12h 2m 30s , 90d 0m 0s)' ) );

SELECT sdist( spoint( '( 0h 2m 30s , -90d 0m 0s)' ) , spoint( '( 12h 2m 30s , -90d 0m 0s)' ) );

