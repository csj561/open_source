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
/// @date       2016/08/19
/// @note
/// <author>	<time>		<version >		<desc> \n
/// xuyangdi    2016/08/20   add CRequet and CNetAcceptor class
/// wangchang   2016/08/25   add CBaseProductConsumeModel class
/// *******************************************************************
///
#ifndef CPRODUCTCONSUMEMODEL_H
#define CPRODUCTCONSUMEMODEL_H
#include <cstdio>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>


using namespace std;
using namespace boost::asio;


namespace network
{
    class CRequest
    {
        void *handle;
        char *data;
        uint32_t sz;
    public:
        ///************************************************************
        /// @brief     构造函数
        /// @details
        /// @pre
        /// @post
        /// @param[in]  _handle 收到消息的套接字指针
        /// @param[in]  _data 收到的数据
        /// @param[in]  _sz 收到的数据包大小
        /// @param[out] 无
        /// @return    无
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        CRequest(void * _handle,char *_data,uint32_t _sz);

        ///************************************************************
        /// @brief     默认构造函数
        /// @details
        /// @pre
        /// @post
        /// @param[in]  无
        /// @param[out] 无
        /// @return    无
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        CRequest(){};

        ///************************************************************
        /// @brief     获取请求是的数据，除去包头的12字节
        /// @details
        /// @pre
        /// @post
        /// @param[in]  无
        /// @param[out] 无
        /// @return    纯数据(json)
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        const char *get_data();

        ///************************************************************
        /// @brief     响应请求
        /// @details
        /// @pre
        /// @post
        /// @param[in]  buf 响应的数据包（不包含12字节的包头，由该函数自动补齐）
        /// @param[in]  sz 响应的数据包的长度（不包含12字节的包头）
        /// @param[out] 无
        /// @return   响应成功返回true，其它返回false
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        bool response(const char *buf,uint32_t sz);

        ///************************************************************
        /// @brief     析构函数
        /// @details
        /// @pre
        /// @post
        /// @param[in]  无
        /// @param[out] 无
        /// @return   无
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        virtual ~CRequest();
    };
}

namespace MIP_PRODUCT_CONSUMER
{
    class CNetAcceptor
    {
    public:
        ///************************************************************
        /// @brief     构造函数
        /// @details
        /// @pre
        /// @post
        /// @param[in]  ip 需要监听的IP
        /// @param[in]  port 需要监听的端口
        /// @param[out] 无
        /// @return    无
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        CNetAcceptor(const char *ip,unsigned short port);

        ///************************************************************
        /// @brief     默认构造函数
        /// @details
        /// @pre
        /// @post
        /// @param[in]  无
        /// @param[out] 无
        /// @return    无
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        CNetAcceptor(){}

        ///************************************************************
        /// @brief     析构函数
        /// @details
        /// @pre
        /// @post
        /// @param[in]  无
        /// @param[out] 无
        /// @return   无
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        virtual ~CNetAcceptor();

        ///************************************************************
        /// @brief     启动监听线程
        /// @details
        /// @pre
        /// @post
        /// @param[in]  无
        /// @param[out] 无
        /// @return   成功返回true，失败false
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        bool run();

        ///************************************************************
        /// @brief     分离监听线程
        /// @details
        /// @pre
        /// @post
        /// @param[in]  无
        /// @param[out] 无
        /// @return   无
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        void detach();

        ///************************************************************
        /// @brief     停止监听
        /// @details
        /// @pre
        /// @post
        /// @param[in]  无
        /// @param[out] 无
        /// @return    成功返回true,失败回去false
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        bool stop();

        ///************************************************************
        /// @brief     重置监听信息
        /// @details
        /// @pre
        /// @post
        /// @param[in]  ip 需要监听的IP
        /// @param[in]  port 需要监听的端口
        /// @param[out] 无
        /// @return    无
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        bool reset(const char *ip,unsigned short port,int data_cache_type);

    private:
        void read_handler(char *str,const boost::system::error_code& error,
			size_t bytes_transferred,ip::tcp::socket * sock);
        void accept_hander (ip::tcp::socket * sock);


    protected:
        virtual void action();

    private:

        string m_ip;

        unsigned short m_port;

        bool m_is_run;

        bool m_is_dispatch;
		
       	int m_data_cache_type;

		boost::thread *m_thread;

		io_service ios;

		ip::tcp::acceptor * acceptor;
    };

}

#endif // CPRODUCTCONSUMEMODEL_H
