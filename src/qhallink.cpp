#include "qhallink.h"
#include <QSharedData>

#include <QList>
#include <QVariant>
#include <QUrl>

class QHalLinkData : public QSharedData {
public:
    QHalLinkData() : QSharedData(),
        templated(false),
        isList(false)
    {}

    QString rel;
    QUrl href;
    bool templated;

    bool isList;
    QList<QHalLink> links;
};

QHalLink::QHalLink() : data(new QHalLinkData)
{
}

QHalLink::QHalLink(const QString &rel, const QUrl &href, bool templated) : data(new QHalLinkData)
{
    setRel(rel);
    setHref(href);
    setTemplated(templated);
}

QHalLink::QHalLink(const QString &rel, const QList<QHalLink> &links) : data(new QHalLinkData)
{
    setRel(rel);
    data->isList = true;
    data->links.append(links);
}

QHalLink::QHalLink(const QHalLink &rhs) : data(rhs.data)
{
}

QHalLink &QHalLink::operator=(const QHalLink &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

QHalLink::~QHalLink()
{
}

QString QHalLink::rel() const
{
    return data->rel;
}

void QHalLink::setRel(const QString &rel)
{
    data->rel = rel;
}

QUrl QHalLink::href() const
{
    return data->href;
}

void QHalLink::setHref(const QUrl &href)
{
    data->href = href;
}

bool QHalLink::isTemplated() const
{
    return data->templated;
}

void QHalLink::setTemplated(bool templated)
{
    data->templated = templated;
}

bool QHalLink::isList() const
{
    return data->isList;
}

int QHalLink::count() const
{
    return data->links.size();
}

const QHalLink &QHalLink::operator [](int i) const
{
    return data->links[i];
}

void QHalLink::append(const QHalLink &link)
{
    data->isList = true;
    data->links.append(link);
}

static const QString HREF("href");
static const QString TEMPLATED("templated");

QVariant QHalLink::toVariant() const
{
    if(isList()) {
        QVariantList result;
        for(int i = 0; i < count(); ++i) {
            result.append(data->links[i].toVariant());
        }
        return result;
    }
    else {
        QVariantMap result;

        result[HREF] = href();
        if(isTemplated()) result[TEMPLATED] = true;

        return result;
    }
}

QHalLink QHalLink::fromVariant(const QVariant &variant)
{
    QHalLink result;

    // List of links
    if(variant.canConvert<QVariantList>()) {
        foreach(const QVariant v, variant.toList()) {
            result.append(QHalLink::fromVariant(v));
        }
        result.data->isList = true;
        return result;
    }

    // Link
    if(variant.canConvert<QVariantMap>()) {
        QVariantMap map = variant.toMap();

        result.setHref(map[HREF].toUrl());
        result.setTemplated(map[TEMPLATED].toBool());
    }

    return result;
}
