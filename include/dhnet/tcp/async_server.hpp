#include <vector>
#include <boost/bind.hpp>
#include "../dhasio.hpp"

namespace dhnet {
namespace tcp {

template<typename RequestHandler>
class async_session
{
public:
    typedef async_session   this_class;
    typedef RequestHandler  request_handler_t;

public:
    async_session(request_handler_t& request_handler, asio::io_service& io_service)
        : request_handler_(request_handler)
        , socket_(io_service)
    {
    }

    asio::ip::tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
    	post_read();
    }

private:
	void post_read()
	{
        socket_.async_read_some(asio::buffer(request_, max_length),
				boost::bind(&this_class::handle_read, this,
					asio::placeholders::error,
					asio::placeholders::bytes_transferred));
	}
	void post_write()
	{
#ifdef _DEBUG
        //std::cerr<<"reply size:"<<reply_.size()<<std::endl;
#endif
		asio::async_write(socket_,
				asio::buffer(reply_),
				boost::bind(&this_class::handle_write, this,
					asio::placeholders::error));
	}

    void handle_read(const asio::error_code& error, size_t bytes_transferred)
    {
        if (!error)
        {
        	request_handler_.handle_request(&request_[0], &request_[bytes_transferred]);
        	reply_.clear();
        	if (request_handler_.fetch_reply(std::back_inserter(reply_))>0)
        	{
				post_write();	///< write reply of request
        	}
        	else
        	{
        		post_read();	///< no reply of request, then read next request
        	}
        }
        else
        {
            delete this;
        }
    }

    void handle_write(const asio::error_code& error)
    {
        if (!error)
        {
        	reply_.clear();
        	if ( request_handler_.fetch_reply(std::back_inserter(reply_))>0 )
        	{
        		post_write();	///< write the relpy of previous request
        	}
        	else
        	{
        		post_read();	///< read new request until all reply of previouse request are wrote
        	}
        }
        else
        {
            delete this;
        }
    }

private:
    request_handler_t& request_handler_;
    asio::ip::tcp::socket socket_;
    enum { max_length = 2048 };
    uint8_t request_[max_length];
    std::vector<uint8_t> reply_;
};

template<typename RequestHandler>
class async_server
{
public:
    typedef async_server this_class;
    typedef RequestHandler  request_handler_t;
    typedef async_session<request_handler_t> session_t;

public:
    async_server(request_handler_t& request_handler, short port)
		: request_handler_(request_handler)
        , io_service_()
        , acceptor_(io_service_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    {
    	post_accept();
    }

    void run(void)
    {
        io_service_.run();
    }

private:
	void post_accept()
	{
        session_t* new_session = new session_t(request_handler_, io_service_);
        acceptor_.async_accept(new_session->socket(),
			boost::bind(&this_class::handle_accept, this, new_session,
				asio::placeholders::error));
	}

    void handle_accept(session_t* session, const asio::error_code& error)
    {
        if (!error)
        {
            session->start();
            post_accept();
        }
        else
        {
            delete session;
        }
    }

private:
    request_handler_t& request_handler_;
    asio::io_service io_service_;
    asio::ip::tcp::acceptor acceptor_;
};

} //namespace tcp
} //namespace dhnet
