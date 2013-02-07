#ifndef QHALLINK_H
#define QHALLINK_H

#include <QSharedDataPointer>

class QUrl;
class QVariant;

class QHalLinkData;

class QHalLink
{
public:
    QHalLink();
    QHalLink(const QString &rel, const QUrl &href, bool templated = false);
    QHalLink(const QString &rel, const QList<QHalLink> &links);
    QHalLink(const QHalLink &);
    QHalLink &operator=(const QHalLink &);
    ~QHalLink();

    QString rel() const;
    void setRel(const QString &rel);

    QUrl href() const;
    void setHref(const QUrl &href);

    bool isTemplated() const;
    void setTemplated(bool templated);

    bool isList() const;
    int count() const;
    const QHalLink &operator [](int i) const;
    void append(const QHalLink &link);

    QVariant toVariant() const;

    static QHalLink fromVariant(const QVariant &variant);
    
private:
    QSharedDataPointer<QHalLinkData> data;
};

#endif // QHALLINK_H
