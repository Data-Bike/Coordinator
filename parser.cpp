/* 
 * File:   parser.cpp
 * Author: newworld
 * 
 * Created on 7 Август 2015 г., 3:38
 */

#include "parser.h"

parser::parser(vector<unsigned char> msg) {
    if (msg.size() >= 64) {
        this->from = vector<unsigned char>(msg.begin(), msg.begin() + 32);
        this->to = vector<unsigned char>(msg.begin() + 32, msg.begin() + 64);
        this->body = vector<unsigned char>(msg.begin() + 64, msg.end());
    } else if (msg.size() == 32) {
        this->from = vector<unsigned char>(msg.begin(), msg.begin() + 32);
    }
}

parser::parser(const parser& orig) {
}

parser::~parser() {
}

