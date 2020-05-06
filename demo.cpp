#include <cqcppsdk/cqcppsdk.h>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <cstring>
#include <time.h>
using namespace cq;
using namespace std;
using Message = cq::message::Message;
using MessageSegment = cq::message::MessageSegment;
/*特殊事件触发标志位*/
/*复读机启用       1*/
static int event_flag = 0;



 CQ_INIT {
    on_enable([] { logging::info("启用", "插件已启用"); });
    
	/*彩虹屁功能模块*/
    on_private_message([](const PrivateMessageEvent &event){
        if (event.message == "快夸我") {
            map<int, string> rainbow_fart_map;
            int rainbow_fart_key = 0; //彩虹屁map的key
            string rainbow_fart_value;
            ifstream ifile("./data/library/rainbow_fart.txt"); //打开彩虹屁语句库
			//Release 版本
            if (!ifile) {
                logging::warning("文件操作", "打开文件失败或不存在 ");
            } //错误信息处理
            while (!ifile.eof()) {
                getline(ifile, rainbow_fart_value);
                rainbow_fart_map.insert(pair<int, string>(rainbow_fart_key, rainbow_fart_value));
                rainbow_fart_key++;
            }//遍历彩虹屁库，保存在map中
            ifile.close();
            rainbow_fart_key = 0;//重新初始化key值供下次使用
            /*随机发送彩虹屁，50的值取决于当前彩虹屁库内的行数*/
            srand((unsigned)time(NULL));
            int randNumtemp = rand() % 50; 
			/***************************************************/
            if (rainbow_fart_map.count(1) > 0) {
                logging::info_success("私聊", "彩虹屁库正常读取  ");
            }else
                logging::warning("私聊", "彩虹屁库非正常读取 ");
           try {
                auto msgid = send_private_message(event.user_id, rainbow_fart_map[randNumtemp]);
                logging::info_success("私聊", "彩虹屁发送成功, 消息 Id: " + to_string(msgid));
            } catch (ApiError &err) {
                logging::warning("私聊", "私聊消息发送失败, 错误码: " + to_string(err.code));
            }
        }
    });

    /*帮助功能块 */
    on_private_message([](const PrivateMessageEvent &event) {
        if (event.message == ".help") {
            ifstream ifile("./data/library/help.txt");//Release版本
            if (!ifile) {
                logging::warning("文件操作", "打开文件失败或不存在 ");
            }
            string helpstr_temp, helpstr_final; // final为最终输出
            while (!ifile.eof()) {
                getline(ifile, helpstr_temp);
                helpstr_final = helpstr_final +"\n"+ helpstr_temp;
            }
            ifile.close(); //关闭文件
            try {
                auto msgid = send_private_message(event.user_id, helpstr_final);
                logging::info_success("私聊", "帮助文档请求成功，消息ID" + to_string(msgid));
            } catch (ApiError &err) {
                logging::warning("私聊", "私聊消息发送失败, 错误码: " + to_string(err.code));
            }
        }
    });

    /*复读机功能块 标志位1*/
    on_private_message([](const PrivateMessageEvent &event) {
        if (event.message == "开启复读机") {
            event_flag = 1;
            try {
                auto msgid = send_private_message(event.user_id, "复读机已开机");
                logging::info_success("私聊", "复读机开启完成, 消息 Id: " + to_string(msgid));
            } catch (ApiError &err) {
                logging::warning("私聊", "私聊消息发送失败, 错误码: " + to_string(err.code));
            }
            event.block();
        }
        if (event.message == "砸烂复读机") {
            event_flag = 0;
            try {
                auto msgid = send_private_message(event.user_id,"[CQ:face,id=107]"); 
                logging::info_success("私聊", "复读机已关闭, 消息 Id: " + to_string(msgid));
            } catch (ApiError &err) {
                logging::warning("私聊", "消息发送失败, 错误码: " + to_string(err.code));
            }
        } 
        if ((event_flag == 1) && (event.message != "开启复读机")) {
            try {
                auto msgid = send_private_message(event.user_id, event.message); // 直接复读消息
                logging::info_success("私聊", "私聊消息复读完成, 消息 Id: " + to_string(msgid));
            } catch (ApiError &err) {
                logging::warning("私聊", "私聊消息复读失败, 错误码: " + to_string(err.code));
            }
        }
    });

    on_message([](const MessageEvent &event) {
        logging::debug("消息", "收到消息: " + event.message + "\n实际类型: " + typeid(event).name());
    });
}

CQ_MENU(menu_demo_1) {
    logging::info("菜单", "点击菜单1");
}

CQ_MENU(menu_demo_2) {
    send_private_message(10000, "测试");
}
