#pragma once
#include <chrono>
#include <string>
#include <sstream>

using namespace std;

namespace DateUtils
{
    using namespace std::chrono;

    inline sys_days parseDate(const string& date)
    {
        int y, m, d;
        char dash;

        stringstream ss(date);
        ss >> y >> dash >> m >> dash >> d;

        return year(y) / month(m) / day(d);
    }

    inline string today()
    {
        auto now = floor<days>(system_clock::now() + hours(7));
        year_month_day ymd{now};

        stringstream ss;
        ss << int(ymd.year()) << "-";

        if (unsigned(ymd.month()) < 10) ss << "0";
        ss << unsigned(ymd.month()) << "-";

        if (unsigned(ymd.day()) < 10) ss << "0";
        ss << unsigned(ymd.day());

        return ss.str();
    }

    inline int daysBetween(const string& d1,
                           const string& d2)
    {
        auto date1 = parseDate(d1);
        auto date2 = parseDate(d2);

        return (date2 - date1).count();
    }

    inline int daysUntil(const string& dueDate)
    {
        return daysBetween(today(), dueDate);
    }

    inline bool isOverdue(const string& dueDate)
    {
        return daysUntil(dueDate) < 0;
    }

    inline int overdueDays(const string& dueDate)
    {
        int remain = daysUntil(dueDate);

        return remain < 0 ? -remain : 0;
    }

    inline string addDays(const string& date, int daysToAdd)
    {
        auto datePoint =
            parseDate(date) + days(daysToAdd);

        year_month_day ymd{datePoint};

        stringstream ss;

        ss << int(ymd.year()) << "-";

        if (unsigned(ymd.month()) < 10) ss << "0";
        ss << unsigned(ymd.month()) << "-";

        if (unsigned(ymd.day()) < 10) ss << "0";
        ss << unsigned(ymd.day());

        return ss.str();
    }
}