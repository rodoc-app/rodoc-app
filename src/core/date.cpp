/// @file date.cpp Source file for date formats.
#include "date.h"

// =====================================================================
// Date
// =====================================================================

Date::Date(int y, int m, int d) : day(d), month(m), year(y){}

int Date::getDay() const{
	return day;
}

int Date::getMonth() const{
	return month;
}

int Date::getYear() const{
	return year;
}

bool Date::isEmpty() const{
	return 0 == day && 0 == month && 0 == year;
}

bool Date::isExact() const{
	return day != 0 && month != 0 && year != 0;
}

void Date::read(const std::string& date){
	year = month = day = 0;
	int number = 0;
	bool y,m;
	y = m = false;
	for(size_t i = 0; i < date.length(); ++i){
		if(date[i] <= '9' && date[i] >= '0'){
			number *= 10;
			number += date[i] - '0';
		}
		else if(date[i] == '-'){
			if(!y){
				year = number;
				y = true;
			}
			else if(!m){
				month = number;
				m = true;
			}
			number = 0;
		}
	}
	day = number;
}

void Date::setDay(int d){
	day = d;
}

void Date::setMonth(int m){
	month = m;
}

void Date::setYear(int y){
	year = y;
}

std::string Date::str() const{
    std::string formated;
    std::stringstream iss;
    if(day == 0) iss << "??";
    else{
        formated = std::to_string(day);
        while(formated.length() < 2) formated = '0' + formated;
        iss << formated;
    }
    iss << ". ";
    if(month == 0) iss << "??";
    else{
        formated = std::to_string(month);
        while(formated.length() < 2) formated = '0' + formated;
        iss << formated;
    }
    iss << ". ";
    if(year == 0) iss << "????";
    else{
        formated = std::to_string(year);
        while(formated.length() < 4) formated = '0' + formated;
        iss << formated;
    }
    return iss.str();
}

// =====================================================================
// functions for Date
// =====================================================================

std::ostream& operator<<(std::ostream& os, const Date& d){
	os << d.getYear();
	os << "-";
	os << d.getMonth();
	os << "-";
	os << d.getDay();
	return os;
}

// =====================================================================
// WrappedDate
// =====================================================================

WrappedDate::WrappedDate(){
	std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    date1_.setYear(now->tm_year + 1900);
    date1_.setMonth(now->tm_mon + 1);
    date1_.setDay(now->tm_mday);
}

void WrappedDate::clear(){
    date1_ = Date();
    date2_ = Date();
    text_ = EMPTY_STRING;
    updateDate();
}

const Date& WrappedDate::getFirstDate() const{
	return date1_;
}

const Date& WrappedDate::getSecondDate() const{
	return date2_;
}

const std::string& WrappedDate::getText() const{
	return text_;
}

bool WrappedDate::isExact() const{
	return date1_.isExact() && date2_.isEmpty();
}

bool WrappedDate::isUnknown() const{
    return date1_.isEmpty() && date2_.isEmpty() && text_ == EMPTY_STRING;
}

void WrappedDate::readJson(const Json::Value& value){
	date1_.read(value[jsonlabel::DATE].asString());
    date2_.read(value[jsonlabel::LAST_DATE].asString());
    text_ = value[jsonlabel::TEXT].asString();
}

void WrappedDate::setFirstDate(int y, int m, int d){
	date1_ = Date(y,m,d);
}

void WrappedDate::setSecondDate(int y, int m, int d){
	date2_ = Date(y,m,d);
}

std::string& WrappedDate::setText(){
	return text_;
}

std::string WrappedDate::str() const{
    std::stringstream ss;
    if(date1_.isEmpty() && date2_.isEmpty() && text_ == EMPTY_STRING)
        return person::UNKNOWN_STR;
    else if(date1_.isEmpty() && date2_.isEmpty())
        return text_;
    else if(date2_.isEmpty())
        ss << date1_.str();
    else
        ss << person::DATE_FROM << date1_.str() << person::DATE_TO << date2_.str();
    return ss.str();
}

void WrappedDate::updateDate(){
	if(date1_.isEmpty() && !date2_.isEmpty()){
		date1_ = date2_;
		date2_ = Date();
	}
}

// =====================================================================
// functions for WrappedDate
// =====================================================================

std::ostream& operator<<(std::ostream& os, const WrappedDate& wd){
	os << "\"" << jsonlabel::DATE << "\":{";
    os << "\"" << jsonlabel::DATE << "\":\"" << wd.getFirstDate() << "\"";
    os << ", \"" << jsonlabel::LAST_DATE << "\":\"" << wd.getSecondDate() << "\"";
    os << ", \"" << jsonlabel::TEXT << "\":\"" << wd.getText() << "\"";
	os << "}";
	return os;
}
