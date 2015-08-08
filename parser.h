/* 
 * File:   parser.h
 * Author: newworld
 *
 * Created on 7 Август 2015 г., 3:38
 */

#ifndef PARSER_H
#define	PARSER_H
#include <vector>
using namespace std;
class parser {
public:
    parser(vector<unsigned char> msg);
    parser(const parser& orig);
    vector<unsigned char> from;
    vector<unsigned char> to;
    vector<unsigned char> body;
    virtual ~parser();
private:

};

#endif	/* PARSER_H */

