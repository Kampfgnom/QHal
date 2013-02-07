#include "qhalresource.h"
#include <QSharedData>

#include "qhallink.h"

#include <QHash>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>
#include <QUrl>

static const QString EMBEDDED("_embedded");
static const QString LINKS("_links");
static const QString SELF("self");

class QHalResourceData : public QSharedData {
public:
    QHalResourceData() : QSharedData(),
        isList(false)
    {}

    QHash<QString, QVariant> properties;
    QHash<QString, QHalLink> links;
    QHash<QString, QHalResource> embeddedRecources;

    bool isList;
    QList<QHalResource> resources;
};

QHalResource::QHalResource() : data(new QHalResourceData)
{
}

QHalResource::QHalResource(const QHalResource &rhs) : data(rhs.data)
{
}

QHalResource &QHalResource::operator=(const QHalResource &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

QHalResource::~QHalResource()
{
}

QUrl QHalResource::href() const
{
    return selfLink().href();
}

void QHalResource::setHref(const QUrl &href)
{
    QHalLink link = selfLink();
    link.setHref(href);
    data->links.insert(SELF, link);
}

QHalLink QHalResource::selfLink() const
{
    return data->links.value(SELF);
}

QHash<QString, QVariant> QHalResource::properties() const
{
    return data->properties;
}

void QHalResource::setProperty(const QString &key, const QVariant &value)
{
    data->properties.insert(key, value);
}

QHash<QString, QHalLink> QHalResource::links() const
{
    return data->links;
}

void QHalResource::addLink(const QHalLink &link)
{
    data->links.insert(link.rel(), link);
}

QHash<QString, QHalResource> QHalResource::embeddedResources() const
{
    return data->embeddedRecources;
}

void QHalResource::embed(const QString &rel, const QHalResource &resource)
{
    data->embeddedRecources.insert(rel, resource);
}

bool QHalResource::isList() const
{
    return data->isList;
}

int QHalResource::count() const
{
    if(!data->isList)
        return -1;

    return data->resources.size();
}

const QHalResource &QHalResource::operator [](int i) const
{
    return data->resources[i];
}

void QHalResource::append(const QHalResource &resource)
{
    data->isList = true;
    data->resources.append(resource);
}

QVariant QHalResource::toVariant() const
{
    if(isList()) {
        QVariantList result;
        for(int i = 0; i < count(); ++i) {
            result.append(data->resources[i].toVariant());
        }
        return result;
    }
    else {
        QVariantMap result;

        // Properties
        QHashIterator<QString, QVariant> itProperties(properties());
        while(itProperties.hasNext()) {
            itProperties.next();
            result[itProperties.key()] = itProperties.value();
        }

        // Links
        QHashIterator<QString, QHalLink> itLinks(links());
        QVariantMap linksMap;
        while(itLinks.hasNext()) {
            itLinks.next();
            linksMap[itLinks.key()] = itLinks.value().toVariant();
        }
        result[LINKS] = linksMap;

        // Embedded
        QHashIterator<QString, QHalResource> itEmbedded(embeddedResources());
        QVariantMap embeddedMap;
        while(itEmbedded.hasNext()) {
            itEmbedded.next();
            embeddedMap[itEmbedded.key()] = itEmbedded.value().toVariant();
        }
        result[EMBEDDED] = embeddedMap;
        return result;
    }
}

QHalResource QHalResource::fromVariant(const QVariant &variant)
{
    QHalResource result;

    // List of resources
    if(variant.canConvert<QVariantList>()) {
        foreach(const QVariant resourceVariant, variant.toList()) {
            result.append(QHalResource::fromVariant(resourceVariant));
        }
        result.data->isList = true;
        return result;
    }

    // Resource
    if(variant.canConvert<QVariantMap>()) {
        QVariantMap map = variant.toMap();
        QMapIterator<QString, QVariant> it(map);

        // Iterate values
        while(it.hasNext()) {
            it.next();

            // _embedded
            if(it.key() == EMBEDDED) {
                if(it.value().canConvert<QVariantMap>()) {
                    QMapIterator<QString, QVariant> it2(it.value().toMap());
                    while(it2.hasNext()) {
                        it2.next();
                        result.embed(it2.key(),
                                     QHalResource::fromVariant(it2.value()));
                    }
                }
            }
            // _links
            else if(it.key() == LINKS) {
                if(it.value().canConvert<QVariantMap>()) {
                    QMapIterator<QString, QVariant> it2(it.value().toMap());
                    while(it2.hasNext()) {
                        it2.next();
                        QHalLink link = QHalLink::fromVariant(it2.value());
                        link.setRel(it2.key());
                        result.addLink(link);
                    }
                }
            }
            // properties
            else {
                result.setProperty(it.key(), it.value());
            }
        }
    }

    return result;
}
