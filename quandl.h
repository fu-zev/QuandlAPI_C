/*
 * quandl.h
 *
 *  Created on: 14/11/2013 (Updated on 10/09/2014)
 *      Original Author: Zhiwei Fu
 *       
 */

/* This programme is free software. It is developed by Dr Zhiwei Fu as a product
   contributing to quandl.com.

   This programme is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY, without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef QUANDL_H_
#define QUANDL_H_
#define WIN32_LEAN_AND_MEAN
#include <string>
#include<fstream>
#include <sstream>
#include <iostream>
using namespace std;
 
void download(const string &get,string filename); /// from httpsClient.cpp

class quandl {
	public:
		quandl(){};
		~quandl(){};
		// To store the token in "AuthCode", which is a public variable in the class.
		void quandl::auth(string code){
			AuthCode = code;
		}

		//To download file from the website defined by the first 
		//argument.
		// To determine the website address by the token stored in "code"
		// and call the function "download"
		void get(string code){
			//Set optional variables by default
			string order = "asc"; 
			string type = "csv";

			string website = "/api/v1/datasets/" + code 
				+ "." + type + "?sort_order=" + order;
			if(AuthCode.length() == 0){
				cout << "It would appear you are\'nt using an authentication"
				  << " token. Please visit https://www.quandl.com/help/api for getting one"
				  << "; otherwise your usage may be limited.\n";
			}
			else{
				website += "&auth_token=" + AuthCode;
			}

			string FileName;
			int iLength = code.length();
			for (int i=0; i < iLength; i++){
				if(code.substr(i, 1) == "/"){
					FileName = code.substr(i+1, iLength - i -1);
					break;
				}
			}

			string fullfilename = (FileName + "." + type).c_str() ;
			download(website,fullfilename);
			return;
		}
		// All parameters are prescribed by users.
		// 1. Quandl code;
		// 2. Ascending/descending order;
		// 3. Start date;
		// 4. End date;
		// 5. Transformation;
		// 6. collapse;
		// 7. Rows;
		// 8. Output type
		// There are 7 optional arguments compared to the one above.
		void get(string code, string order, string StartDate, string EndDate,
			string transformation, string collapse, string rows, string type){

			string website = "/api/v1/datasets/" + code 
				+ "." + type + "?sort_order=" + order;
			if(AuthCode.length() == 0){
				cout << "It would appear you are\'nt using an authentication"
				  << " token. Please visit https://www.quandl.com/help/c++"
				  << " or your usage may be limited.\n";
			}
			else{
				website += "&auth_token=" + AuthCode;
			}

			website += "&trim_start=" + StartDate;
			website += "&trim_end=" + EndDate;
			website += "&transformation=" + transformation;
			website += "&collapse=" + collapse;
			website += "&rows=" + rows;

			string FileName;
			int iLength = code.length();
			for (int i=0; i < iLength; i++){
				if(code.substr(i, 1) == "/"){
					FileName = code.substr(i+1, iLength - i -1);
					break;
				}
			}
			string fullfilename = (FileName + "." + type).c_str() ;
			download(website,fullfilename);

		 
		}
	private:
		string AuthCode;
	 
};

#endif /* QUANDL_H_ */
