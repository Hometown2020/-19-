#include "Date.h"

Date::Date() {
    setYear(0);
    setMonth(0);
    setDay(0);
    setHour(0);
    setMinute(0);
}
Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
    setYear(t_year);
    setMonth(t_month);
    setDay(t_day);
    setHour(t_hour);
    setMinute(t_minute);
}

Date::Date(const std::string& dateString) {
    *this = stringToDate(dateString);
}

int Date::getYear(void) const {
    return m_year;
}

void Date::setYear(const int t_year) {
    m_year = t_year;
}


int Date::getMonth(void) const {
    return m_month;
}


void Date::setMonth(const int t_month) {
    m_month = t_month;
}

int Date::getDay(void) const {
    return m_day;
}


void Date::setDay(const int t_day) {
    m_day = t_day;
}


int Date::getHour(void) const {
    return m_hour;
}

void Date::setHour(const int t_hour) {
    m_hour = t_hour;
}


int Date::getMinute(void) const {
    return m_minute;
}

void Date::setMinute(const int t_minute) {
    m_minute = t_minute;
}



bool isNum(char c) {
    return c <= '9' && c >= '0';
}
bool isLeap(int y) {
    if (y % 400 == 0 || y % 4 == 0 && y % 100 != 0)return true;
    return false;
}
bool isData(int y, int m, int d) {
    if (y > 9999 || y < 1000)return false;
    if (m > 12 || m < 1)return false;
    switch (m)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        if (d >= 1 && d <= 31)return true;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if (d >= 1 && d <= 30)return true;
        break;
    default:
        if (d >= 1 && d <= 28 || isLeap(y) && d == 29)return true;
        break;
    }
    return false;
}
bool Date::isValid(const Date& t_date) {
    if (isData(t_date.getYear(), t_date.getMonth(), t_date.getDay())) {
        if (t_date.getHour() <= 23 && t_date.getHour() >= 0 && t_date.getMinute() >= 0 && t_date.getMinute() <= 59) {
            return true;
        }

    }
    return false;
}
Date Date::stringToDate(const std::string& t_dateString) {
    Date tem;
    if (isNum(t_dateString[0]) && isNum(t_dateString[1])
        && isNum(t_dateString[2]) && isNum(t_dateString[3])
        && isNum(t_dateString[5]) && isNum(t_dateString[6])
        && isNum(t_dateString[8]) && isNum(t_dateString[9])
        && isNum(t_dateString[11]) && isNum(t_dateString[12])
        && isNum(t_dateString[14]) && isNum(t_dateString[15])
        && t_dateString[4] == '-' && t_dateString[7] == '-'
        && t_dateString[10] == '/' && t_dateString[13] == ':') {

        int hour = (t_dateString[11] - '0') * 10 + t_dateString[12] - '0';
        int min = (t_dateString[14] - '0') * 10 + t_dateString[15] - '0';

        if (hour >= 0 && hour <= 23 && min >= 0 && min <= 59) {

            int year = (t_dateString[0] - '0') * 1000 + (t_dateString[1] - '0') * 100
                + (t_dateString[2] - '0') * 10 + t_dateString[3] - '0';
            int mon = (t_dateString[5] - '0') * 10 + t_dateString[6] - '0';
            int day = (t_dateString[8] - '0') * 10 + t_dateString[9] - '0';

            if (isData(year, mon, day)) {
                tem.setYear(year);
                tem.setMonth(mon);
                tem.setDay(day);
                tem.setHour(hour);
                tem.setMinute(min);
                return tem;
            }
        }
    }

    tem.setYear(0);
    tem.setMonth(0);
    tem.setDay(0);
    tem.setHour(0);
    tem.setMinute(0);
    return tem;
}

std::string Date::dateToString(const Date& t_date) {
    if (!isValid(t_date)) {
        char str[17] = "0000-00-00/00:00";
        return str;
    }
    char str[17];
    str[0] = t_date.getYear() / 1000 + '0';
    str[1] = t_date.getYear() / 100 % 10 + '0';
    str[2] = t_date.getYear() / 10 % 10 + '0';
    str[3] = t_date.getYear() % 10 + '0';
    str[4] = '-';
    str[5] = t_date.getMonth() / 10 + '0';
    str[6] = t_date.getMonth() % 10 + '0';
    str[7] = '-';
    str[8] = t_date.getDay() / 10 + '0';
    str[9] = t_date.getDay() % 10 + '0';
    str[10] = '/';
    str[11] = t_date.getHour() / 10 + '0';
    str[12] = t_date.getHour() % 10 + '0';
    str[13] = ':';
    str[14] = t_date.getMinute() / 10 + '0';
    str[15] = t_date.getMinute() % 10 + '0';
    str[16] = '\0';
    return str;
}


Date& Date::operator=(const Date& t_date) {
    setYear(t_date.getYear());
    setMonth(t_date.getMonth());
    setDay(t_date.getDay());
    setHour(t_date.getHour());
    setMinute(t_date.getMinute());
    return *this;
}


bool Date::operator==(const Date& t_date) const {
    if (getYear() != t_date.getYear())return false;
    if (getMonth() != t_date.getMonth())return false;
    if (getDay() != t_date.getDay())return false;
    if (getHour() != t_date.getHour())return false;
    if (getMinute() != t_date.getMinute())return false;
    return true;
}


bool Date::operator>(const Date& t_date) const {
    if (getYear() > t_date.getYear())return true;
    if (getYear() == t_date.getYear() && getMonth() > t_date.getMonth())return true;
    if (getYear() == t_date.getYear() && getMonth() == t_date.getMonth()
        && getDay() > t_date.getDay())return true;
    if (getYear() == t_date.getYear() && getMonth() == t_date.getMonth()
        && getDay() == t_date.getDay() && getHour() > t_date.getHour())return true;
    if (getYear() == t_date.getYear() && getMonth() == t_date.getMonth()
        && getDay() == t_date.getDay() && getHour() == t_date.getHour()
        && getMinute() > t_date.getMinute())return true;
    return false;
}


bool Date::operator<(const Date& t_date) const {
    return t_date > (*this);
}


bool Date::operator>=(const Date& t_date) const {
    return (*this) == t_date || (*this) > t_date;
}


bool Date::operator<=(const Date& t_date) const {
    return t_date >= (*this);
}
