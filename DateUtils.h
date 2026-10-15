#pragma once
#include <chrono>
#include <string>

using namespace std;

namespace DateUtils
{
    using namespace std::chrono;

    // doc 1 so nguyen tu vi tri pos (bo qua ky tu khong phai so o dau)
    inline int readInt(const string& s, size_t& pos)
    {
        while (pos < s.size() && (s[pos] < '0' || s[pos] > '9')) pos++;
        int v = 0;
        while (pos < s.size() && s[pos] >= '0' && s[pos] <= '9')
        {
            v = v * 10 + (s[pos] - '0');
            pos++;
        }
        return v;
    }

    // so co 2 chu so: 5 -> "05"
    inline string twoDigits(unsigned v)
    {
        return v < 10 ? "0" + to_string(v) : to_string(v);
    }

    // ngay -> chuoi YYYY-MM-DD
    inline string formatDate(const year_month_day& ymd)
    {
        return to_string(int(ymd.year())) + "-"
             + twoDigits(unsigned(ymd.month())) + "-"
             + twoDigits(unsigned(ymd.day()));
    }

    inline sys_days parseDate(const string& date)
    {
        size_t pos = 0;
        int y = readInt(date, pos);
        int m = readInt(date, pos);
        int d = readInt(date, pos);

        return year(y) / month(m) / day(d);
    }

    // ngay hien tai theo gio Viet Nam (UTC+7)
    inline string today()
    {
        auto now = floor<days>(system_clock::now() + hours(7));
        return formatDate(year_month_day{now});
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

        return formatDate(year_month_day{datePoint});
    }
}
