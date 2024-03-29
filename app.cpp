#include <event.h>
#include <evhttp.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>
#include <ctime>
#include<fstream>
using namespace std;
#define PORT 2500
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////http request handle process////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
void http_handler(struct evhttp_request *req)
{
    struct evbuffer *buf;
    buf = evbuffer_new();
    
    
    struct evkeyvalq * postHead = req->input_headers;
    
    const char *http_cId = evhttp_find_header(postHead, "contourType");	//解析head，根据key获取value
    
    /////////////////分析POST报文body信息/////////////////////
    int buffer_data_len;
    buffer_data_len = EVBUFFER_LENGTH(req->input_buffer);
    //printf("%d\n\n",buffer_data_len);
    /////////////////////////获得body信息/////////////////////////////////////////
    
    if(buffer_data_len == 0)
    {
        evhttp_add_header(req->output_headers,"error_status","no picture!");
        evbuffer_add_printf(buf,"no picture! please check your input");
        /////////////////////////////////////////////////////////////////////////////////
        //返回code 400
        evhttp_send_reply(req, 400, "Error", buf);
        //释放内存
        evbuffer_free(buf);
        return;
        
    }
    
    unsigned char *buffer_data = (unsigned char *)malloc(buffer_data_len+1);
    memset(buffer_data, '\0', buffer_data_len+1);
    
    memcpy (buffer_data, EVBUFFER_DATA(req->input_buffer), buffer_data_len);
    
    //cout<<"pic:"<<endl<<buffer_data<<endl;
    
    string resultURL = "thank you";
    cout<<"query end"<<endl;
    //  evbuffer_add_printf(buf, resultURL.c_str());
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    
    cout<<"search end"<<endl;
    // 输出
    evhttp_clear_headers(postHead);
    free(buffer_data);
    evbuffer_free(buf);
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////libevent//////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
namespace servers {
    namespace util {
        
        class HTTPServer {
        public:
            HTTPServer() {}
            ~HTTPServer() {}
            int serv(int port, int nthreads);
        protected:
            static void* Dispatch(void *arg);
            static void GenericHandler(struct evhttp_request *req, void *arg);
            void ProcessRequest(struct evhttp_request *request);
            int BindSocket(int port);
        };
        
        int HTTPServer::BindSocket(int port) {
            int r;
            int nfd;
            nfd = socket(AF_INET, SOCK_STREAM, 0);
            if (nfd < 0) return -1;
            
            int one = 1;
            r = setsockopt(nfd, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(int));
            
            struct sockaddr_in addr;
            memset(&addr, 0, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = INADDR_ANY;
            addr.sin_port = htons(port);
            return -1; 
            // r = bind(nfd, (struct sockaddr*)&addr, sizeof(addr));
            // if (r < 0) return -1;
            // r = listen(nfd, 10240);
            // if (r < 0) return -1;
            
            int flags;
            if ((flags = fcntl(nfd, F_GETFL, 0)) < 0
                || fcntl(nfd, F_SETFL, flags | O_NONBLOCK) < 0)
                return -1;
            
            return nfd;
        }
        
        int HTTPServer::serv(int port, int nthreads) {
            int r;
            int nfd = BindSocket(port);
            if (nfd < 0) return -1;
            pthread_t ths[nthreads];
            for (int i = 0; i < nthreads; i++) {
                struct event_base *base = event_init();
                if (base == NULL) return -1;
                struct evhttp *httpd = evhttp_new(base);
                if (httpd == NULL) return -1;
                r = evhttp_accept_socket(httpd, nfd);
                if (r != 0) return -1;
                
                int timeout = 60;
                evhttp_set_timeout(httpd, timeout);
                evhttp_set_gencb(httpd, HTTPServer::GenericHandler, this);
                r = pthread_create(&ths[i], NULL, HTTPServer::Dispatch, base);
                if (r != 0) return -1;
            }
            for (int i = 0; i < nthreads; i++) {
                pthread_join(ths[i], NULL);
            }
        }
        
        void* HTTPServer::Dispatch(void *arg) {
            event_base_dispatch((struct event_base*)arg);
            return NULL;
        }
        
        void HTTPServer::GenericHandler(struct evhttp_request *req, void *arg) {
            ((HTTPServer*)arg)->ProcessRequest(req);
        }
        
        void HTTPServer::ProcessRequest(struct evhttp_request *req) {
            http_handler(req);
        }
        
    }
}
int main(int argv,char*argc[]){
    
    servers::util::HTTPServer s;
    s.serv(PORT, 10);
}