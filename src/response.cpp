#include "response.h"

response::~response(){
    if(this->content != nullptr){
        delete this->content;
    }
}