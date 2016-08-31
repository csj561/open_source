/// *******************************************************************
///
/// {}
///
/// *******************************************************************
///
/// @file       CProductConsumeModel.h
/// @brief      ��������ģ��
/// @attention  �Զ˿ڼ��������ݽ���ҵ������
/// @todo
/// @version    1.0v
/// @author     ����
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
        /// @brief     ���캯��
        /// @details
        /// @pre
        /// @post
        /// @param[in]  _handle �յ���Ϣ���׽���ָ��
        /// @param[in]  _data �յ�������
        /// @param[in]  _sz �յ������ݰ���С
        /// @param[out] ��
        /// @return    ��
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        CRequest(void * _handle,char *_data,uint32_t _sz);

        ///************************************************************
        /// @brief     Ĭ�Ϲ��캯��
        /// @details
        /// @pre
        /// @post
        /// @param[in]  ��
        /// @param[out] ��
        /// @return    ��
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        CRequest(){};

        ///************************************************************
        /// @brief     ��ȡ�����ǵ����ݣ���ȥ��ͷ��12�ֽ�
        /// @details
        /// @pre
        /// @post
        /// @param[in]  ��
        /// @param[out] ��
        /// @return    ������(json)
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        const char *get_data();

        ///************************************************************
        /// @brief     ��Ӧ����
        /// @details
        /// @pre
        /// @post
        /// @param[in]  buf ��Ӧ�����ݰ���������12�ֽڵİ�ͷ���ɸú����Զ����룩
        /// @param[in]  sz ��Ӧ�����ݰ��ĳ��ȣ�������12�ֽڵİ�ͷ��
        /// @param[out] ��
        /// @return   ��Ӧ�ɹ�����true����������false
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        bool response(const char *buf,uint32_t sz);

        ///************************************************************
        /// @brief     ��������
        /// @details
        /// @pre
        /// @post
        /// @param[in]  ��
        /// @param[out] ��
        /// @return   ��
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
        /// @brief     ���캯��
        /// @details
        /// @pre
        /// @post
        /// @param[in]  ip ��Ҫ������IP
        /// @param[in]  port ��Ҫ�����Ķ˿�
        /// @param[out] ��
        /// @return    ��
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        CNetAcceptor(const char *ip,unsigned short port);

        ///************************************************************
        /// @brief     Ĭ�Ϲ��캯��
        /// @details
        /// @pre
        /// @post
        /// @param[in]  ��
        /// @param[out] ��
        /// @return    ��
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        CNetAcceptor(){}

        ///************************************************************
        /// @brief     ��������
        /// @details
        /// @pre
        /// @post
        /// @param[in]  ��
        /// @param[out] ��
        /// @return   ��
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        virtual ~CNetAcceptor();

        ///************************************************************
        /// @brief     ���������߳�
        /// @details
        /// @pre
        /// @post
        /// @param[in]  ��
        /// @param[out] ��
        /// @return   �ɹ�����true��ʧ��false
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        bool run();

        ///************************************************************
        /// @brief     ��������߳�
        /// @details
        /// @pre
        /// @post
        /// @param[in]  ��
        /// @param[out] ��
        /// @return   ��
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        void detach();

        ///************************************************************
        /// @brief     ֹͣ����
        /// @details
        /// @pre
        /// @post
        /// @param[in]  ��
        /// @param[out] ��
        /// @return    �ɹ�����true,ʧ�ܻ�ȥfalse
        /// @exception
        /// @todo
        /// @attention
        ///************************************************************/
        bool stop();

        ///************************************************************
        /// @brief     ���ü�����Ϣ
        /// @details
        /// @pre
        /// @post
        /// @param[in]  ip ��Ҫ������IP
        /// @param[in]  port ��Ҫ�����Ķ˿�
        /// @param[out] ��
        /// @return    ��
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
