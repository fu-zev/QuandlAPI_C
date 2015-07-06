

I modified the Zafuer code in order to obtain a cross platform version + ssl compliant (https).
Please take note that in the current version the quandl certificate is not checked.

Recommended reading : the following post if you are not familiar with ssl:

 http://h71000.www7.hp.com/doc/83final/ba554_90007/ch04s03.html
 
Compilation Info :

You 'll need to link the project with boost asio and openssl libs. Both need to be build before to be linked.

Info : 

 Boost asio : http://www.boost.org/doc/libs/1_58_0/doc/html/boost_asio.html

 Open ssl : http://developer.covenanteyes.com/building-openssl-for-visual-studio/  ( good post)

QuandlAPI_C
===========

Quandl API (C/C++) is developed to download numeric data from quandl.com, making it easy for those C/C++ developers.

The API is encapsulated within a header file: 1) quandl.h for the Windows and the Linux environment.

It is reccommended that users sign up on http://www.quandl.com/users/sign_up, to obtain an authentication token for downloading increased volume of data per day.

1.After downloading the right header file, please include it in your code by:

   
    #include "quandl.h"

2.Now you can verify your account by by following command. This step can be skipped if you don't have an token or your don't want to use it.

    //to register with the authentication token denote by the string "code".
    quandl::auth(code);

3.Then you can download the csv file by specifying the Quandl code.

    //to download the csv file defined by the quandlcode of a stock
    quandl::get(quandlcode);

Alternatively, you can set your preferred values for the available parameters by:

    //to download the file specified by eight parameters:
    //quandlcode -- quandl code for a stock;
    //order -- "asc" / "desc";
    //StartDate -- in the format of YYYY-MM-DD, the date from which prices are recorded; 
    //EndDate -- in the format of YYYY-MM-DD, the date until prices are recorded;
    //transformation -- "cummul" / "diff" / "rdiff" / "normalize" / "cumulative";
    //collapse -- "daily" / "weekly" / "monthly" / "quarterly" / "annual";
    //rows  -- the number of rows to record;
    //type  -- the type of downloaded files ("csv" / "json" / "xml" );
    quandl::get(quandlcode, order, StartDate, EndDate, transformation, collapse, rows, type); 
    
    
A case study: download the stock price of TENCENT HOLDINGS LTD (TCTZF). The quandlcode is GOOG/PINK_TCTZF. Then you might write the following code:

    //#include "quandl.h"
    int main() {
        quandl ql;
        ql.auth("<code>"); // Replace <code> with your own token.
        ql.get("GOOG/PINK_TCEHY");
        return 0;
    }
    
    
Please visit http://www.quandl.com/help/api to better understand the Quandl API design.


