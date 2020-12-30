#include "parameterserver.h"
#include "mongoose.h"
#include "urldecode.h"

#include <iostream>

#define CONFIGURU_IMPLEMENTATION 1
#define TARGET_WEB_DIR_NAME "/home/tinyoh/develop/xunit/build/bin/res/paramsserverweb"
#define CONFIGURU_JSON_PARSE_ERROR_LOG ""
#define CACHE_MAX_SIZE (128*1024)
#define STATUS_DISPLAY_TIME_INTERVAL 1000
#define DEBUG_PARAM_SERV
#define CONFIG_HIDEN_PARAM
#define WITH_HTTP_PAGE

#include "configuru.hpp"
using namespace configuru;
static const char *s_http_port = "8099";
static struct mg_serve_http_opts s_http_server_opts;
static char cache[CACHE_MAX_SIZE];

// EASY README
/* exp ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  using namespace configuru;
 *  Config cfg = Config::object();
    cfg["pi"]     = 3.14;
    cfg["array"]  = Config::array{ 1, 2, 3 };
    cfg["object"] = Config::object({
            { "key1", "value1" },
            { "key2", "value2" },
    });
 *
    Config cfg{
      {"pi",     3.14},
      {"array",  Config::array({ 1, 2, 3 })},
      {"object", {
              { "key1", "value1" },
              { "key2", "value2" },
      }},
      { "fps",   fps_usage }
    };
    -------------------------------------------------

 *  if you want to update a existed parm in code

    cfg["target_parm_index"] << your_new_param_val;

    -------------------------------------------------

 *  if you want to add your custom param in manager

    step 1:
    cfg["target_parm_index"] = your_initial_val;

    step 2:
    cfg["target_parm_index"].add_callback([this](configuru::Config &a, const configuru::Config &b)->bool{
      if (b == "a") {
        // your logic code
        return true;-----------------------
      } else if (b == "b") {              |
        // your logic code                |
        return true;------------------    |
      } else if (b == "c") {         |    |
        // your logic code           |    |
      }                              |    |
      return false;--------------    |    |
                               \|/  \|/  \|/
      // !!!!!!!!!!important!!!!!!!!!!!!
      // if you judge the new config change already work, and the config tree should update with the new parm b,then return true;
      // if not, return false will refuse the param changed with new parms b;
    });

 *  as I said, if you want to add a const value just show to the params server user.
    cfg_ctrl["target_parm_index"] = your_const_val;
    cfg_ctrl["target_parm_index"].add_callback([](configuru::Config &, const configuru::Config &)->bool{
      return false;
    });
    so you next time update this value in code should use '=' instead of '<<';
    and '<<' operate will always refuse the change when you set.

    -------------------------------------------------
 *  if you want to load the config from a file, you can use it to init your own module.
 *  read:
    cfg["target_parm_index"] = configuru::parse_file("your/path/to/config.json", configuru::JSON / configuru::CFG);
 *  write:
    configuru::dump_file("your/path/to/out_put_config.json", cfg["target_parm_index"], configuru::JSON / configuru::CFG);
    -------------------------------------------------
 *  the second level config node such as
    cfg["first_level"]["second_level"] = "your_value" will be crush when the cfg["first_level"] is exist as a object;
    to solve this problem, you should use cfg.judge_or_create_key("first_level") to make sure the first level object is exist or you will create it;

 *  sometimes we want to use some config but not show on the web server, then we should use hiden
    cfg["target_parm_index"].set_hiden(true);
    you also can use the index in code as useral. but on http web , wen can only set the value by add object and value.
    #define CONFIG_HIDEN_PARAM is the feature enable switch.
*exp ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

static void handle_get_device_usage(struct mg_connection *nc)
{
    // Use chunked encoding in order to avoid calculating Content-Length
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

    auto cfg = ParameterServer::instance()->GetCfgRoot();
    uint64_t mem = 0, vmem = 0, r = 0, w = 0;

    //  cpu = get_cpu_usage();
    //  get_memory_usage(&mem, &vmem);
    //  get_io_bytes(&r, &w);

    if (cfg.has_key("dev_status") && cfg["dev_status"].is_object())
    {
    } else {
        cfg["dev_status"]= Config::object();
    }
    auto dev_status = cfg["dev_status"];
    dev_status["mem"] = mem;
    dev_status["vmem"] = vmem;
    dev_status["io_r"] = r;
    dev_status["io_w"] = w;

    mg_printf_http_chunk(nc, dump_string(dev_status, JSON).c_str());

    // Send empty chunk, the end of response
    mg_send_http_chunk(nc, "", 0);
}

static void handle_set_dev_ctrl(struct mg_connection *nc,struct http_message *hm) {
    // Use chunked encoding in order to avoid calculating Content-Length
    char * res = urlDecode(hm->message.p);
    char *custom_head = strstr(res, "code_res=");
    char *end =  strstr(res, "HTTP/1.1");

    if (!(custom_head && end))
    {
        std::cout << __FUNCTION__ << "error";
        std::cout.flush();
        free(res);
        mg_http_send_error(nc, 403, NULL);
        return;
    }

    memset(cache, 0, CACHE_MAX_SIZE);
    memcpy(cache, custom_head + 9,end - custom_head - 9);
    free(res);

    auto config_in = parse_string(cache, JSON, CONFIGURU_JSON_PARSE_ERROR_LOG);
    auto dev_ctrl = ParameterServer::instance()->GetCfgCtrlRoot();

    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    if (Config::deep_async(dev_ctrl, config_in))
    {
#ifdef DEBUG_PARAM_SERV
        std::cout << std::endl << "Nothing in config changed." << std::endl;
        std::cout.flush();
#endif
    }
#ifdef CONFIG_HIDEN_PARAM
    mg_printf_http_chunk(nc, dump_string_with_hiden(dev_ctrl, JSON).c_str());
#else
    mg_printf_http_chunk(nc, dump_string(dev_ctrl, JSON).c_str());
#endif

    // Send empty chunk, the end of response
    mg_send_http_chunk(nc, "", 0);
}

static void handle_get_dev_ctrl(struct mg_connection *nc)
{
    // Use chunked encoding in order to avoid calculating Content-Length
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

    auto dev_ctrl = ParameterServer::instance()->GetCfgCtrlRoot();
#ifdef CONFIG_HIDEN_PARAM
    mg_printf_http_chunk(nc, dump_string_with_hiden(dev_ctrl, JSON).c_str());
#else
    mg_printf_http_chunk(nc, dump_string(dev_ctrl, JSON).c_str());
#endif

    // Send empty chunk, the end of response
    mg_send_http_chunk(nc, "", 0);
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    struct http_message *hm = (struct http_message *) ev_data;
    switch (ev) {
      case MG_EV_HTTP_REQUEST:
        if (mg_vcmp(&hm->uri, "/get_dev_status") == 0) {
            handle_get_device_usage(nc);
        } else if (mg_vcmp(&hm->uri, "/get_dev_ctrl") == 0) {
            handle_get_dev_ctrl(nc);
        } else if (mg_vcmp(&hm->uri, "/set_dev_ctrl") == 0) {
            handle_set_dev_ctrl(nc, hm);
        } else {
            mg_serve_http(nc, hm, s_http_server_opts);  // Serve static content
        }
        break;
      default:
        break;
    }
}

typedef enum
{
    INIT,
    RUNNING,
    STOP
} ThreadState;

class ServerThread : public Runnable
{
private:
    std::mutex mmutex;
    Condition *condition;
    ThreadState requestedState=INIT;
    ThreadState currentState=INIT;
    int id;

public:
    ServerThread(int id){
        this->id=id;
        condition=new Condition(mmutex);
    }
    ~ServerThread(){
        delete condition;
    }
    int getId(){
        return id;
    }
    void setState(ThreadState nState){
        {
        Synchronized x(mmutex);
        requestedState=nState;
        condition->notifyAll(x);
        }
    };
    ThreadState getState(){
        Synchronized x(mmutex);
        return currentState;
    };

    virtual void run()
    {
        {
            Synchronized x(mmutex);
            currentState=RUNNING;
        }
#ifdef WITH_HTTP_PAGE
        struct mg_mgr mgr;
        struct mg_connection *nc;
        cs_stat_t st;

        mg_mgr_init(&mgr, NULL);
        nc = mg_bind(&mgr, s_http_port, ev_handler);
        if (nc == NULL)
        {
#ifdef DEBUG_PARAM_SERV
            std::cout << __FUNCTION__ << "Cannot bind to %s\n" << s_http_port << std::endl;
            std::cout.flush();
#endif
            exit(1);
        }

        mg_set_protocol_http_websocket(nc);
        QString path = TARGET_WEB_DIR_NAME;
        QByteArray th= path.toLatin1();
        s_http_server_opts.document_root = th.data();

        if (mg_stat(s_http_server_opts.document_root, &st) != 0)
        {
#ifdef DEBUG_PARAM_SERV
            std::cout << __FUNCTION__ << "Cannot find web_root directory, continue without params server.\n" << std::endl;
            std::cout.flush();
#endif
            while (requestedState!=STOP) {
                // waiting for close;
                sleep(1);
            }
            return;
        }
#ifdef DEBUG_PARAM_SERV
        std::cout << __FUNCTION__ << "Starting web server on port " << s_http_port << std::endl;
        std::cout.flush();
#endif
        while (requestedState!=STOP) {
            mg_mgr_poll(&mgr, STATUS_DISPLAY_TIME_INTERVAL);
        }
        currentState=STOP;
        mg_mgr_free(&mgr);
        return;
#endif
    }

    virtual void stop()
    {
        requestedState=STOP;
        currentState=STOP;
    }
};

ParameterServer::ParameterServer(QObject *parent) :
QObject(parent),
m_ServerThreadContext(nullptr),
m_ServerThread(nullptr)
{
    m_ServerThreadContext = std::make_shared<ServerThread>(0);
    m_ServerThread = std::make_shared<Thread>(m_ServerThreadContext);
#ifdef WITH_HTTP_PAGE
    debug_ = true;
#else
    debug_ = false;
#endif
}

void ParameterServer::stop_server()
{
    m_ServerThreadContext->stop();
}

void ParameterServer::start_server()
{
    m_ServerThread->start();
}

void ParameterServer::init()
{
    _cfgRoot = Config::object();
}
