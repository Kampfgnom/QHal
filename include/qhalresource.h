#ifndef QHALRESOURCE_H
#define QHALRESOURCE_H

#include <QSharedDataPointer>

template<class K, class V>
class QHash;
class QVariant;
class QUrl;
class QHalLink;

class QHalResourceData;

class QHalResource
{
public:
    QHalResource();
    QHalResource(const QHalResource &);
    QHalResource &operator=(const QHalResource &);
    ~QHalResource();

    QUrl href() const;
    void setHref(const QUrl &href);
    QHalLink selfLink() const;

    QHash<QString, QVariant> properties() const;
    void setProperty(const QString &key, const QVariant &value);

    QHash<QString, QHalLink> links() const;
    void addLink(const QHalLink &link);

    QHash<QString, QHalResource> embeddedResources() const;
    void embed(const QString &rel, const QHalResource &resource);

    bool isList() const;
    int count() const;
    const QHalResource &operator [](int i) const;
    void append(const QHalResource &resource);

    QVariant toVariant() const;

    static QHalResource fromVariant(const QVariant &variant);

private:
    QSharedDataPointer<QHalResourceData> data;
};

#endif // QHALRESOURCE_H
