// Copyright (c) 2019 The SecureCloud developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_NEWSRECORD_H
#define BITCOIN_QT_NEWSRECORD_H

#include <QString>

/** UI model for a News
 */
class NewsRecord
{
public:
    NewsRecord() : time(0), text(""), url(""), author(""), description("")
    {
    }


    NewsRecord(qint64 time, const std::string& text, const std::string& url, const std::string& author, const std::string& description) : time(time), text(text), url(url), author(author), description(description)
    {
    }

    qint64 time;
    std::string text;
    std::string url;
    std::string author;
    std::string description;
};

#endif // BITCOIN_QT_NEWSRECORD_H
