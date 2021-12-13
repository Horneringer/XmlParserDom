#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QtCore>
#include<QDebug>
#include<QtGui>
#include<QtXml>


QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;

    QStandardItemModel model;

    //xml файл
    QString filename;

    //чтение xml файла
    void createXmlTree();


};
#endif // DIALOG_H
