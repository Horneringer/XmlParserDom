#include "dialog.h"
#include "./ui_dialog.h"

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);

    filename = "osm-metro.xml";

    QStringList headers;

    //заголовки столбцов
    headers.append("Структура документа");
    headers.append("Кол-во");
    headers.append("Тип элемента");

    model.setHorizontalHeaderLabels(headers);

    createXmlTree();

    ui->treeView->setModel(&model);
}

Dialog::~Dialog()
{
    delete ui;
}


//чтение файла
void Dialog::createXmlTree()
{
    //QDomDocument класс для работы с xml документом, обеспечивающий доступ к его содержимому
    QDomDocument document;

    QFile file(filename);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){

        //чтение xml файла из QDevice
        document.setContent(&file);

        file.close();
    }

    // тэг ul
    QDomElement xmlroot = document.documentElement();
    //тэг li
    QDomElement liTag = xmlroot.firstChildElement();

    //тэг typaname
    QDomElement typanameTag = liTag.firstChildElement();

    //тэг a
    QDomElement aTag = liTag.lastChildElement();

    //тэг some
    QDomElement someTag = liTag.lastChildElement();

    // список всех тэгов li
    QDomNodeList lis = xmlroot.elementsByTagName("li");

    //список всех тэгов typaname
    QDomNodeList typanames  = xmlroot.elementsByTagName("typaname");

    //список всех тэгов a
    QDomNodeList aList  = xmlroot.elementsByTagName("a");

    //список всех тэгов some
    QDomNodeList someList  = xmlroot.elementsByTagName("some");


    //добавление в модель корневого элемента
    QStandardItem *root = new QStandardItem(xmlroot.tagName());

    //добавление в модель li
    QStandardItem *li = new QStandardItem(liTag.tagName());

    qDebug() << li->index();

    //добавление в модель typaname
    QStandardItem *typaname = new QStandardItem(typanameTag.tagName());

    //добавление в модель a
    QStandardItem *a = new QStandardItem(aTag.tagName());

    //добавление в модель some
    QStandardItem *some = new QStandardItem(someList.at(0).nodeName());

    auto indrow = a->index().row() + 1;

    auto indcol = a->index().column() +2;

    //добавление строки в модель
    model.appendRow(root);
    root->appendRow(li);
    li->appendRow(typaname);
    li->appendRow(a);
    li->appendRow(some);

    QDomNamedNodeMap map = aTag.attributes();

    QSet <QString> attrs;

    for(auto i=0; i < map.count(); i++){

        auto mapItem =  map.item(i).toAttr().name();

        if(mapItem == "data-lat" || mapItem == "data-lon" || mapItem == "data-min-lat" || mapItem == "data-max-lat" || mapItem == "data-min-lon" || mapItem == "data-max-lon" || mapItem == "data-prefix" ||
                mapItem == "data-name" || mapItem == "data-id" || mapItem == "way" || mapItem == "data-type" || mapItem == "href" || mapItem == "newid"){

            attrs.insert(mapItem);
        }
    }

    for(auto i=0; i < aList.count(); i++){

         QDomElement elem = aList.at(i).toElement();

         attrs.insert(elem.attributeNode("data-type").toAttr().name());
         attrs.insert(elem.attributeNode("href").toAttr().name());
         attrs.insert(elem.attributeNode("newid").toAttr().name());
    }

    QList<QString> listattrs(attrs.begin(),attrs.end());

    //добавление атрибутов
    for(auto i=0; i<listattrs.count(); i++){

        if(listattrs.at(i) == ""){

            listattrs.erase(listattrs.begin()+i);
        }

        a->appendRow(new QStandardItem(listattrs.at(i)));

        // количество аттрибутов каждого типа
        int attrCount = 0;

        for(auto j=0; j < aList.count(); j++){

            QDomElement elem = aList.at(j).toElement();

            if(elem.hasAttribute(listattrs.at(i))){

                attrCount++;
            }
        }

        a->setChild(indrow,indcol,new QStandardItem(QString::number(attrCount)));

        indrow++;
    }

    //добавление количества тэгов в модель
    root->setChild(0,1, new QStandardItem(QString::number(lis.count())));
    li->setChild(0,1, new QStandardItem(QString::number(typanames.count())));
    li->setChild(1,1, new QStandardItem(QString::number(aList.count())));
    li->setChild(2,1, new QStandardItem(QString::number(someList.count())));


}


