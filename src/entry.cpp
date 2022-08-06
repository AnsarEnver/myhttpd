#include "myhttpd.h"

int main(int argc, char *argv[]){
    std::map<std::string, std::string> mapping_table;
    myhttpd _myhttpd(12, "127.0.0.1", 80, "D:/web_root", std::move(mapping_table));
    _myhttpd.start();
    _myhttpd.join();
}