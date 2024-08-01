#include <iostream>
#include "balloon.hpp"
#include "msutil.hpp"

class Date {
private:
    void _i(std::string date = "", std::string time = "");
public:
    i16 year = 0, ms = 0;
    byte day = 0, month = 0, hour = 0, minute = 0, second = 0;
    Date(std::string date, std::string time = "") {
        this->_i(date, time);
    } 
    Date(u64 iVal);
    Date(time_t t);
    Date(){};
    u64 getLong();
    std::string getString();
};