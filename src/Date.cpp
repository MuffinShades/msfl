#include "Date.hpp"

Date::Date(std::string date = "") {
    std::vector<std::string> dSplit = SplitString(date, 'T');

    switch (dSplit.size()) {
        case 1: {
            this->_i(dSplit[0]);
            break;
        }
        case 2:
            this->_i(dSplit[0], dSplit[1]);
        default:
            return;
    }
}

void Date::_i(std::string date = "", std::string time = "") {
    std::vector<std::string> dp = SplitString(date, '-');

    if (dp.size() <= 3)
        goto time;

    try {
        this->month = std::stoi(dp[0]) & 0xff;
        this->day = std::stoi(dp[1]) & 0xff;
        this->year = std::stoi(dp[2]) & 0xffff;
    } catch(std::exception e) {}

    time:

    //time stuff
    std::vector<std::string> tp = SplitString(time, ':');

    switch (tp.size()) {
        case 0: 
            break;
        case 1: {

        }
    }

    end:
        return;
};