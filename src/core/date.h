/// @file date.h Header file for date formats.
#ifndef date_h_
#define date_h_

#include <string>
#include <ostream>
#include <sstream>
#include <json/json.h>
#include <ctime>
#include "strings.h"

/// Basic date format. Holding three numbers for year, month and day. If some of the numbers are not defined 0 is present.
class Date{
	public:
		/// Default constructor.
		/// @param d Day number.
		/// @param m Month number.
		/// @param y Year number.
		explicit Date(int y = 0, int m = 0, int d = 0);
		/// Get the day number.
		/// @return Day number (0 if it was not set).
		int getDay() const;
		/// Get the month number.
		/// @return Month number (0 if it was not set).
		int getMonth() const;
		/// Get the year number.
		/// @return Year number (0 if it was not set).
		int getYear() const;
		/// Check if the date is completely not defined.
		/// All values are non-defined if they are equal to 0.
		/// @return True if all values are 0.
		bool isEmpty() const;
		/// Check if the date is exact or not.
		/// Date is exact when all values are defined (non-zero).
		/// @return True if the date is exact.
		bool isExact() const;
		/// Load date from string in specified flexible format:
		/// yyyy-mm-dd (if there would be for example 05 it can only be 5).
		/// If it does not follow the format there will be corrupted data and won't raise an exception.
		/// @param date Written date in correct format.
		void read(const std::string& date);
		/// Set the day number.
		/// @param d Day number.
		void setDay(int d);
		/// Set the month number.
		/// @param m Month number.
		void setMonth(int m);
		/// Set the year number.
		/// @param y Year number.
		void setYear(int y);
		/// Get string representing the date in format yyyy-MM-DD.
		/// If one of the values are set to 0, then it is replaced by the same number of ?
		/// @return String in the format.
		std::string str() const;
	private:
		/// Day number.
		int day;
		/// Month number.
		int month;
		/// Year number.
		int year;
};

/// Writing date to the stream in format yyyy-mm-dd.
/// Instead of writing for example 05, there will be just 5.
/// @param os Given stream.
/// @param d Given date.
/// @return Reference to the changed stream.
std::ostream& operator<<(std::ostream& os, const Date& d);

/// Class holding two dates and date description. Always first date is the main one.
class WrappedDate{
	public:
		/// Default constructor.
		WrappedDate();
		/// Clear all values.
		void clear();
		/// Get the first date.
		/// @return Constant reference to the first date.
		const Date& getFirstDate() const;
		/// Get the second date.
		/// @return Constant reference to the second date.
		const Date& getSecondDate() const;
		/// Get the text.
		/// @return Constant reference to the text.
		const std::string& getText() const;
		/// Check if the date is exact or not. To be exact
		/// only the first date has to be exact, the second got to be empty.
		/// @return True if the first date is exact & second empty.
		bool isExact() const;
		/// Check if the date is unknown. To be unknown all values are 0 and text is empty.
		/// @return If the date is unknown.
		bool isUnknown() const;
		/// Read the data from JSON formatting.
		/// @param value Given JSON value.
		void readJson(const Json::Value& value);
		/// Set the first date.
		/// @param y Year number.
		/// @param m Month number.
		/// @param d Day number.
		void setFirstDate(int y, int m, int d);
		/// Set the second date.
		/// @param y Year number.
		/// @param m Month number.
		/// @param d Day number.
		void setSecondDate(int y, int m, int d);
		/// Set the text for the date by giving the reference to the text.
		/// @return Text used in the date.
		std::string& setText();
		/// Represent the date by a string.
		/// @return Newly constructed string.
		std::string str() const;
		/// Switch the dates if the first one is empty.
		void updateDate();
	private:
		/// Main date, if interval is set it is the first day.
		Date date1_;
		/// Second date for interval only, otherwise empty.
		Date date2_;
		/// If text was given.
		std::string text_;
};

/// Writing wrapped date to given stream in JSON formatting.
/// @param os Given stream.
/// @param wd Given WrappedDate.
/// @return Reference to the changed stream.
std::ostream& operator<<(std::ostream& os, const WrappedDate& wd);

#endif
