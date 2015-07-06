/*
 *  Created on: 07/2015 
 *      Original Author: MALICK FALL
 *       
 */

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/asio.hpp>  
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

using namespace std;
using boost::asio::ip::tcp;
enum { max_length = 2024 };

class client
{

public:
  client(boost::asio::io_service& io_service, boost::asio::ssl::context& context,
  boost::asio::ip::tcp::resolver::iterator endpoint_iterator, std::string target,
  std::string command, std::string method)
    : socket_(io_service, context),target_(target),
      command_(command),method_(method)
      
  {
      boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
	  socket_.lowest_layer().async_connect(endpoint,
      boost::bind(&client::handle_connect, this,
      boost::asio::placeholders::error, ++endpoint_iterator));
  }


  void handle_connect(const boost::system::error_code& error,
        boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
  { 
	IsSucess = 1 ;
    if (!error) {     
      socket_.async_handshake(boost::asio::ssl::stream_base::client,
        boost::bind(&client::handle_handshake, this,
        boost::asio::placeholders::error));
    }
    else if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator()) {
      socket_.lowest_layer().close();
      boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
      socket_.lowest_layer().async_connect(endpoint,
        boost::bind(&client::handle_connect, this,
        boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else 
	{
    	IsSucess = 0 ; // Connect failed   
    }
  }
  

  void handle_handshake(const boost::system::error_code& error)
  {
    if (!error) {
	std::ostream request_stream(&request_);
    request_stream << "GET " << command_ << " HTTP/1.1\r\n";
    request_stream << "Host: " <<  target_ << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";
	  boost::asio::async_write(socket_, request_,
          boost::bind(&client::handle_write_request, this,
            boost::asio::placeholders::error));
    }
    else 
	{ 
	IsSucess = 0 ; // Handshake failed
    }
  }

  
  void handle_write_request(const boost::system::error_code& err)
  {
    if (!err)
    {
      // Read the response status line. The response_ streambuf will
      // automatically grow to accommodate the entire line. The growth may be
      // limited by passing a maximum size to the streambuf constructor.
      boost::asio::async_read_until(socket_, response_, "\r\n",
          boost::bind(&client::handle_read_status_line, this,
            boost::asio::placeholders::error));
    }
    else
    {  
	IsSucess = 0 ;
    }
  }

  void handle_read_status_line(const boost::system::error_code& err)
  {
    if (!err)
    {
      // Check that response is OK.
      std::istream response_stream(&response_);
      std::string http_version;
      response_stream >> http_version;
      unsigned int status_code;
      response_stream >> status_code;
      std::string status_message;
      std::getline(response_stream, status_message);
      if (!response_stream || http_version.substr(0, 5) != "HTTP/")
      {
		IsSucess = 0 ;
        return; // Invalid response
      }

      if (status_code != 200)
      {
		IsSucess = 0 ;// Invalid status code 
        return;   
      }

      // Read the response headers, which are terminated by a blank line.
      boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
          boost::bind(&client::handle_read_headers, this,
            boost::asio::placeholders::error));
    }
    else
    {  
	  IsSucess = 0 ;
    }
  }

  void handle_read_headers(const boost::system::error_code& err)
  {
    if (!err)
    {
      // Process the response headers.
      std::istream response_stream(&response_);
      std::string header;
	   while (std::getline(response_stream, header) && header != "\r"){  //Connection: Close\r
		   // SKIP HEADER
	  }

      // Write whatever content we already have to output.
      if (response_.size() > 0) {
	 result << &response_;
	  } 
      // Start reading remaining data until EOF.
      boost::asio::async_read(socket_, response_,
          boost::asio::transfer_at_least(1),
          boost::bind(&client::handle_read_content, this,
            boost::asio::placeholders::error));
    }
    else
    {  
	IsSucess = 0 ;
    }
  }

  void handle_read_content(const boost::system::error_code& err)
  {	
    if (!err)
    {
	  result << &response_;
      // Continue reading remaining data until EOF.
      boost::asio::async_read(socket_, response_,
          boost::asio::transfer_at_least(1),
          boost::bind(&client::handle_read_content, this,
            boost::asio::placeholders::error));	
    }
    else if (err != boost::asio::error::eof)
    {  
	IsSucess = 0 ;
    }
  }

int  WriteFile(string filename)
  {
	  if (IsSucess ==1)
	  {
		  fid.open(filename.c_str());
			if(fid.fail()){
				   //cerr << "*** Error when openning file! ***\n" << endl;
				   return 0;
       			}
	  string TempStr = result.str();
	  fid.write(TempStr.c_str(), TempStr.size());
	  fid.close();
		 return 1; /// download success"
	  }
	  else
		  return 0; // failure
  }

private:
  boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;	 
  std::string target_;
  std::string method_;
  std::ofstream of ;
  std::string command_;
  int  IsSucess ; 
  char body_[max_length];
  ofstream myfile;
  ofstream fid;
  boost::asio::streambuf request_;
  boost::asio::streambuf response_;
  stringstream result;
};
void download(const string &get,string filename)
{
	string method ,target,command;
	char port[] =  "https";
    target=  "www.quandl.com";
	method = "GET";
	command = get ;  
	boost::asio::io_service io_service;
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(target,port);
    boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
    boost::asio::ssl::context ctx(io_service, boost::asio::ssl::context::sslv23);
    ctx.set_verify_mode(boost::asio::ssl::context::verify_none);
    client c(io_service, ctx, iterator,target,command,method);
    io_service.run();
    io_service.reset();
	int res = c.WriteFile(filename); /// return 1 if success..
	 
}
