QuandlAPI_C
===========

Quandl API (C/C++) developed to download numerical data from quandl.com 
---

quandl::auth(code);  -- to register with the authentication token denote by the string "code".



quandl::get(quandlcode); -- to download the csv file defined by the quandlcode of a stock



or more specifically,



quandl::get(quandlcode, order, StartDate, EndDate, transformation, collapse, rows ); -- to download the file specified by:

quandlcode -- quandl code for a stock;

order -- "asc" / "desc";

StartDate -- in the format of YYYY-MM-DD, the date from which prices are recorded; 

EndDate -- in the format of YYYY-MM-DD, the date until prices are recorded;

transformation -- "cummul" / "diff" / "rdiff" / "normalize" / "cumulative";

collapse -- "daily" / "weekly" / "monthly" / "quarterly" / "annual";

rows  -- the number of rows to record;

type  -- the type of downloaded files ("csv" / "json" / "xml" );
