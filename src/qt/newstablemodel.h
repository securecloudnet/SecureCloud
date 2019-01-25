// Copyright (c) 2019 The SecureCloud developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_NEWSTABLEMODEL_H
#define BITCOIN_QT_NEWSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>

class NewsRecord;
class NewsTablePriv;

class WalletModel;

class CWallet;

/** UI model for the news table.
 */
class NewsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit NewsTableModel(CWallet* wallet, WalletModel* parent = 0);
    ~NewsTableModel();

    enum ColumnIndex {
        Date = 1,
        Text = 2,
        Url = 3
    };

    /** Roles to get specific information from a transaction row.
        These are independent of column.
    */
    enum RoleIndex {
        /** Date and time this news was created */
        DateRole = Qt::UserRole,
        /** Content of the news (HTML format) */
        TextRole,
        /** URL of the news */
        UrlRole
    };

    void subscribeToCoreSignals();
    void unsubscribeFromCoreSignals();

    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;

    void updateNews(qint64 time, const QString& text, const QString& url, int status);

private:
    CWallet* wallet;
    WalletModel* walletModel;
    QStringList columns;
    NewsTablePriv* priv;

    QString formatNewsDate(const NewsRecord* rec) const;
    QString formatNewsText(const NewsRecord* rec) const;
    QString formatNewsUrl(const NewsRecord* rec) const;

public slots:

    friend class NewsTablePriv;
};

#endif // BITCOIN_QT_NEWSTABLEMODEL_H
