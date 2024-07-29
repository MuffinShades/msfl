#include <iostream>
#include "balloon.hpp"
#include "msutil.hpp"

class Date {
private:
    void _i(std::string date = "", std::string time = "");
public:
    i16 year, ms;
    byte day, month, hour, second;
    Date(std::string date = "");
    Date(std::string date = "", std::string time = "") {
        this->_i(date, time);
    } 
};