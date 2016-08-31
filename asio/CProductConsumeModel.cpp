/// *******************************************************************
///
/// {}
///
/// *******************************************************************
///
/// @file       CProductConsumeModel.h
/// @brief      生产消费模型
/// @attention  对端口监听的数据进行业务消费
/// @todo
/// @version    1.0v
/// @author     王畅
/// @date       2016/08/29
/// @note
/// <author>	<time>		<version >		<desc> \n
/// 徐阳迪       2016/08/20   新增接口建立类
/// 王畅         2016/08/25   新增数据消费类
/// *******************************************************************
///
#include "CProductConsumeModel.h"
#include <cstdlib>
#include <unistd.h>
#define __main__
#include <logger.h>
//#include "MIPMemoryManager.h"
#define KB 1024
#define MAX_RESPONSE (20*KB)
//#define LOGGER(x,...)
#define CRC32(x,y) 0
//using namespace MIPPublicDefine;

namespace network
{
    CRequest::CRequest(void * _handle,char *_data,uint32_t _sz):
        handle(_handle),data(_data),sz(_sz)
    {
    }

    const char *CRequest::get_data()
    {
        return data+12;
    }

    bool CRequest::response(const char *buf,uint32_t sz)
    {
        if(sz > MAX_RESPONSE-12)
        {
            LOGGER (EN_ERROR,"超过响应的最大缓冲区长度[%d]",MAX_RESPONSE);
            return false;
        }

        char outbuf[MAX_RESPONSE];
        uint32_t size=htonl(sz);
        uint32_t crc=htonl(CRC32((uint8_t*)buf,sz));
        memcpy(outbuf,"json",4);
        memcpy(outbuf+4,&size,sizeof(size));
        memcpy(outbuf+8,&crc,sizeof(crc));
        memcpy(outbuf+12,buf,sz);
        //uv_buf_t t_uv_buf={outbuf,sz+12};

        int ret = 0;//uv_try_write((uv_stream_t *) handle,&t_uv_buf,1);
        if(ret<=0)
        {
            LOGGER (EN_ERROR,"当调用uv_try_write时，发生错误");
            return false;
        }

        return true;
    }

    CRequest::~CRequest()
    {
        if(data)
        {
            delete data;
            data = NULL;
        }
    }
}
namespace MIP_PRODUCT_CONSUMER
{
    CNetAcceptor::CNetAcceptor(const char *ip,unsigned short port):m_ip(ip),
        m_port(port),m_is_run(false),m_is_dispatch(false),
        m_thread(NULL)
    {
        assert(ip);
    }

    CNetAcceptor::~CNetAcceptor()
    {
    }

    bool CNetAcceptor::run()
    {
        acceptor = new ip::tcp::acceptor(ios,ip::tcp::endpoint(ip::address::from_string("0.0.0.0"),22222));
        m_thread = new boost::thread(boost::bind(&CNetAcceptor::action,this));
        assert(m_thread);
        m_is_run=true;

        if(!m_is_dispatch)
            m_thread->join();

        return true;
    }

    void CNetAcceptor::detach()
    {
        m_is_dispatch=true;
    }

    bool CNetAcceptor::stop()
    {
        if(m_thread&& m_is_run)
        {
            m_thread->interrupt();

            delete m_thread;
            m_thread = NULL;

            m_is_run = false;
        }

        return true;
    }

    void CNetAcceptor::action()
    {
        do
        {
		   ip::tcp::socket *sock=new ip::tcp::socket(ios);
		   
		   acceptor->async_accept(*sock,boost::bind(&CNetAcceptor::accept_hander,this,sock));
		   ios.run();
		   sleep(1);
		   LOGGER(EN_DEBUG,"end run\n");
        }while(0);

        m_is_run=false;
    }
	void CNetAcceptor::accept_hander(ip::tcp::socket * sock)
	{
		printf("get sock\n");
		char *str=new char[KB]();
		sock->async_read_some(buffer(str,KB),
			boost::bind(&CNetAcceptor::read_handler,this,str,placeholders::error,placeholders::bytes_transferred,sock));
		action();
	}

	void CNetAcceptor::read_handler(char *str,const boost::system::error_code& error,
			size_t bytes_transferred,ip::tcp::socket * sock)
	{
		if(!error)
		{
			printf("read bytes %d [%s]\n",bytes_transferred,str);
			char *str=new char[KB]();
			sock->async_read_some(buffer(str,KB),
			boost::bind(&CNetAcceptor::read_handler,this,str,placeholders::error,placeholders::bytes_transferred,sock));
		}
		else
		{
			printf("lost a sock\n");
			//abort();
			delete sock;
			sock=NULL;
		}
		//action();
	}

    bool CNetAcceptor::reset(const char *ip,unsigned short port,int data_cache_type)
    {
        if (m_is_run)
        {
            return false;
        }

        m_ip = ip;
        m_port = port;
        m_data_cache_type = data_cache_type;

        return true;
    }

    
}

int main()
{
	MIP_PRODUCT_CONSUMER::CNetAcceptor acceptor;
	acceptor.run();
}
