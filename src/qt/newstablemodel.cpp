// Copyright (c) 2019 The SecureCloud developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "newstablemodel.h"

#include "guiconstants.h"
#include "guiutil.h"
#include "newsrecord.h"
#include "walletmodel.h"

#include "main.h"
#include "sync.h"
#include "uint256.h"
#include "util.h"
#include "wallet.h"

#include <QColor>
#include <QDateTime>
#include <QDebug>
#include <QIcon>
#include <QList>

// Amount column is right-aligned it contains numbers
static int column_alignments[] = {
    Qt::AlignLeft | Qt::AlignVCenter, /* Date */
    Qt::AlignLeft | Qt::AlignVCenter  /* Text */
};

// Comparison operator for sort/binary search of model tx list
struct NewsGreaterThan {
    bool operator()(const NewsRecord& a, const NewsRecord& b) const
    {
        return a.time > b.time;
    }
    bool operator()(const NewsRecord& a, const qint64& b) const
    {
        return a.time > b;
    }
    bool operator()(const qint64& a, const NewsRecord& b) const
    {
        return a > b.time;
    }
};

// Private implementation
class NewsTablePriv
{
public:
    NewsTablePriv(CWallet* wallet, NewsTableModel* parent) : wallet(wallet),
                                                             parent(parent)
    {
    }

    CWallet* wallet;
    NewsTableModel* parent;

    /* Local cache of news.
     */
    QList<NewsRecord> cachedNews;

    /* Query entire news anew from core.
     */
    void clearNews()
    {
        cachedNews.clear();
    }

    /* Update our model of the wallet incrementally, to synchronize our model of the wallet
       with that of the core.

       Call with transaction that was added, removed or changed.
     */
    void updateNews(const qint64& time, const std::string& text, const std::string& url, const std::string& author, const std::string& description, int status)
    {
        qDebug() << "NewsTablePriv::updateNews : " + QString::number(time) + " " + QString::fromStdString(text) + " " + QString::fromStdString(author) + " " + QString::fromStdString(description) + " " + QString::number(status);

        // Find bounds of this news in model
        QList<NewsRecord>::iterator lower = qLowerBound(cachedNews.begin(), cachedNews.end(), time, NewsGreaterThan());
        QList<NewsRecord>::iterator upper = qUpperBound(cachedNews.begin(), cachedNews.end(), time, NewsGreaterThan());
        int lowerIndex = (lower - cachedNews.begin());
        int upperIndex = (upper - cachedNews.begin());
        bool inModel = (lower != upper);

        if (status == CT_UPDATED) {
            if (!inModel)
                status = CT_NEW; /* Not in model, but want to show, treat as new */
        }

        switch (status) {
        case CT_NEW:
            if (inModel) {
                qWarning() << "NewsTablePriv::updateNews : Warning: Got CT_NEW, but news is already in model";
                break;
            }

            {
              // Added -- insert at the right position
              NewsRecord rec = NewsRecord(time,text,url,author,description);

              parent->beginInsertRows(QModelIndex(), lowerIndex, lowerIndex);
              cachedNews.insert(lowerIndex, rec);
              parent->endInsertRows();
            }
            break;
        case CT_DELETED:
            if (!inModel) {
                qWarning() << "NewsTablePriv::updateNews : Warning: Got CT_DELETED, but news is not in model";
                break;
            }
            // Removed -- remove entire news from table
            parent->beginRemoveRows(QModelIndex(), lowerIndex, upperIndex - 1);
            cachedNews.erase(lower, upper);
            parent->endRemoveRows();
            break;
        case CT_UPDATED:
            break;
        }
    }

    int size()
    {
        return cachedNews.size();
    }

    NewsRecord* index(int idx)
    {
        if (idx >= 0 && idx < cachedNews.size()) {
            NewsRecord* rec = &cachedNews[idx];
            return rec;
        }
        return 0;
    }
};

NewsTableModel::NewsTableModel(CWallet* wallet, WalletModel* parent) : QAbstractTableModel(parent),
                                                                       wallet(wallet),
                                                                       walletModel(parent),
                                                                       priv(new NewsTablePriv(wallet, this))
{
    columns << tr("Date") << tr("Text");
    clearNews();

    subscribeToCoreSignals();
}

NewsTableModel::~NewsTableModel()
{
    unsubscribeFromCoreSignals();
    delete priv;
}

void NewsTableModel::clearNews()
{
    priv->clearNews();
}

void NewsTableModel::updateNews(qint64 time, const QString& text, const QString& url, const QString& author, const QString& description, int status)
{
    priv->updateNews(time, text.toStdString(), url.toStdString(), author.toStdString(), description.toStdString(), status);
}

int NewsTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return priv->size();
}

int NewsTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return columns.length();
}

QString NewsTableModel::formatNewsDate(const NewsRecord* rec) const
{
    if (rec->time) {
        return GUIUtil::dateTimeStr(rec->time);
    }
    return QString();
}

QString NewsTableModel::formatNewsText(const NewsRecord* rec) const
{
    return QString::fromStdString(rec->text);
}

QString NewsTableModel::formatNewsUrl(const NewsRecord* rec) const
{
    return QString::fromStdString(rec->url);
}

QString NewsTableModel::formatNewsAuthor(const NewsRecord* rec) const
{
    return QString::fromStdString(rec->author);
}

QString NewsTableModel::formatNewsDescription(const NewsRecord* rec) const
{
    return QString::fromStdString(rec->description);
}

QVariant NewsTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    NewsRecord* rec = static_cast<NewsRecord*>(index.internalPointer());

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch (index.column()) {
        case Date:
          return QDateTime::fromTime_t(static_cast<uint>(rec->time));
        case Text:
          return QString::fromStdString(rec->text);
        case Url:
          return QString::fromStdString(rec->url);
        case Author:
          return QString::fromStdString(rec->author);
        case Description:
          return QString::fromStdString(rec->description);
        }
        break;
    case Qt::ToolTipRole:
        return formatNewsDescription(rec);
    case Qt::TextAlignmentRole:
        return column_alignments[index.column()];
    case Qt::ForegroundRole:
        // To avoid overriding above conditional formats a default text color for this QTableView is not defined in stylesheet,
        // so we must always return a color here
        return COLOR_BLACK;
    case DateRole:
        return QDateTime::fromTime_t(static_cast<uint>(rec->time));
    case TextRole:
        return QString::fromStdString(rec->text);
    case UrlRole:
        return QString::fromStdString(rec->url);
    case AuthorRole:
        return QString::fromStdString(rec->author);
    case DescriptionRole:
        return QString::fromStdString(rec->description);
    }
    return QVariant();
}

QVariant NewsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            return columns[section];
        } else if (role == Qt::TextAlignmentRole) {
            return column_alignments[section];
        }
    }
    return QVariant();
}

QModelIndex NewsTableModel::index(int row, int column, const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    NewsRecord* data = priv->index(row);
    if (data) {
        return createIndex(row, column, priv->index(row));
    }
    return QModelIndex();
}

void NewsTableModel::subscribeToCoreSignals()
{
    // Connect signals to wallet
}

void NewsTableModel::unsubscribeFromCoreSignals()
{
    // Disconnect signals from wallet
}
