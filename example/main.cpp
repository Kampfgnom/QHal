#include <QCoreApplication>

#include <qhallink.h>
#include <qhalresource.h>

#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QJsonDocument>
#include <QUrl>

void printLink(const QHalLink &link, int indent)
{
    QString indents = QString("   ").repeated(indent);
    // Is a list
    if(link.isList()) {
        for(int i = 0; i < link.count(); ++i) {
            qDebug("%s%d:",qPrintable(indents), i);
            printLink(link[i], indent + 1);
        }
    }
    // Is a link
    else {
        qDebug() << qPrintable(indents) << "HREF: " << link.href() << " TEMPLATED: " << link.isTemplated();
    }
}

void printResource(const QHalResource &resource, int indent)
{
    QString indents = QString("   ").repeated(indent);

    // Is a list of resources
    if(resource.isList()) {
        for(int i = 0; i < resource.count(); ++i) {
            qDebug("%s   %d:",qPrintable(indents), i);
            printResource(resource[i], indent + 1);
        }
    }
    // Is a resource
    else {
        qDebug() << qPrintable(indents) << "HREF:" << resource.href();

        qDebug() << qPrintable(indents) << "PROPERTIES:";
        QHashIterator<QString, QVariant> itProperties(resource.properties());
        while(itProperties.hasNext()) {
            itProperties.next();
            qDebug() << qPrintable(indents) << itProperties.key() << ":" << itProperties.value();
        }

        qDebug() << qPrintable(indents) << "LINKS:";
        QHashIterator<QString, QHalLink> itLinks(resource.links());
        while(itLinks.hasNext()) {
            itLinks.next();
            qDebug("%s   %s:",qPrintable(indents), qPrintable(itLinks.key()));
            printLink(itLinks.value(), indent+1);
        }

        qDebug() << qPrintable(indents) << "EMBEDDED:";
        QHashIterator<QString, QHalResource> itEmbedded(resource.embeddedResources());
        while(itEmbedded.hasNext()) {
            itEmbedded.next();
            qDebug("%s   %s:",qPrintable(indents), qPrintable(itEmbedded.key()));
            printResource(itEmbedded.value(), indent+1);
        }
    }
}

void inspectFile(const QString &fileName)
{
    QFile collectionFile(fileName);

    if (!collectionFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QByteArray data;
    while (!collectionFile.atEnd()) {
        QByteArray line = collectionFile.readLine();
        data += line;
    }

    qDebug() << "\n\nParsing " << fileName;

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(data, &error);

    if(error.error != QJsonParseError::NoError) {
        QString dataString(data);
        qDebug() << error.errorString() << "at offset" << error.offset;
        qDebug() << dataString.mid(error.offset - 30, 30) ;
        qDebug() << ">> " << dataString.mid(error.offset,1);
        qDebug() << dataString.mid(error.offset+1,30);
        return;
    }

    QHalResource resource = QHalResource::fromVariant(document.toVariant());

    printResource(resource, 0);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    inspectFile(":/examples/example.json");

    QHalResource resource;
    resource.setHref(QUrl("http://example.com/friends"));
    resource.setProperty("count", QVariant(2));
    resource.addLink(QHalLink("blogs", QUrl("http://example.com/blogs{/name}"), true));
    resource.addLink(QHalLink("feeds", QList<QHalLink>()
                              << QHalLink("", QUrl("http://example.com/friends?atom"))
                              << QHalLink("", QUrl("http://example.com/friends?rss"))));

    QHalResource friends;

    QHalResource friend1;
    friend1.setProperty("name", "Niklas");
    friend1.addLink(QHalLink("blog", QUrl("http://example.com/blogs/Niklas")));
    friends.append(friend1);

    QHalResource friend2;
    friend2.setProperty("name", "Isa");
    friend2.addLink(QHalLink("blog", QUrl("http://example.com/blogs/Isa")));
    friends.append(friend2);

    resource.embed("friends", friends);

    QVariant documentVariant = resource.toVariant();
    QJsonDocument jsonDoc = QJsonDocument::fromVariant(documentVariant);
    qDebug() << jsonDoc.toJson();

    return 0;
}
