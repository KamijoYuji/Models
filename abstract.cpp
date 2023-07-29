#include "abstract.h"
#include "QSqlDatabase"
#include "QSqlQuery"
#include "QDebug"
#include "QSqlError"
#include "QLabel"

Abstract::Abstract(QObject *parent)
    : QAbstractTableModel(parent)
{
    getData();
}
int Abstract::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _data.count();
}

int Abstract::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant Abstract::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole or role == Qt::EditRole)
    {
        if(_data.count()>index.row())
        {
            if(index.column()==0)
                return _data[index.row()].name;
            if(index.column()==1)
                return _data[index.row()].number;
            if(index.column()==2)
                return _data[index.row()].id;
        }
    } else if (role == Qt::UserRole) {
            return _data[index.row()].id;
    }
    return QVariant();
}

bool Abstract::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole)
    {
        QSqlQuery query;
        QString a = "UPDATE Numbers SET %2 WHERE id=%1";
        a=a.arg(_data[index.row()].id);
        if(index.column()==0)
        {
            a=a.arg("name = '%1'").arg(value.toString());
            _data[index.row()].name = value.toString();
        }
        if(index.column()==1)
        {
            a=a.arg("number = %1").arg(value.toInt());
            _data[index.row()].number = value.toInt();
        }
        emit dataChanged(index, index);
        if(query.exec(a))
        {
            return true;
        }
        else
        {
            qDebug()<<query.lastError().text();
        }
    }
    return false;
}

void Abstract::getData()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./2mydb.db");
    QSqlQuery* query;
    query = new QSqlQuery(db);
    if(db.open())
    {
        qDebug("open");
    }
    else
    {
        qDebug("no open");
    }

    query->exec("SELECT name, number, id FROM Numbers");
    while (query->next()) {
        Data a;
        a.name=query->value(0).toString();
        a.number=query->value(1).toInt();
        a.id=query->value(2).toInt();
        _data.push_back(a);
    }
}

QVariant Abstract::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
    {
        if(section==0)
            return QString("Name");
        if(section==1)
            return QString("Number");
        if(section==2)
            return QString("ID");
    }

    if(orientation == Qt::Vertical)
    {
        return QAbstractTableModel::headerData(section,orientation,role);
    }
    return false;
}

Qt::ItemFlags Abstract::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}


bool Abstract::insertRows(int row, int count, const QModelIndex &parent)
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./2mydb.db");
    QSqlQuery* query;
    query = new QSqlQuery(db);
    if(db.open())
    {
        qDebug("open");
        beginInsertRows(parent, row, count+row-1);
        for(int i = 0; i < count; ++i){
            query->prepare("INSERT INTO Numbers (name, number) "
                           "VALUES (:name, :number)");
            query->bindValue(":name"," ");
            query->bindValue(":number", 0);
            if(query->exec() && query->exec("select * FROM Numbers WHERE id = last_insert_rowid()"))
            {
                query->next();
                qDebug() << query->value(0);
                Data b;
                b.id = query->value("id").toInt();
                b.name = query->value("name").toString();
                b.number = query->value("number").toInt();
                _data.insert(row, b);
            }
            else
            {
                QLabel* lbl = new QLabel;
                lbl->setText(query->lastError().text());
                lbl->show();
            }
        }
        endInsertRows();
    }
    else
    {
        qDebug("no open");
        return false;
    }
    return true;
}

bool Abstract::removeRow(int id)
{
        QSqlDatabase db;
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("./2mydb.db");
        QSqlQuery* query;
        query = new QSqlQuery(db);
        if(db.open())
        {

            if(query->exec(QString("DELETE FROM Numbers WHERE id = %1").arg(id)))
            {
                Data temp;
                temp.id = id;
                int r = _data.indexOf(temp);
                beginRemoveRows(QModelIndex(), r, r);
                _data.remove(r, 1);
                endRemoveRows();
                return  true;
            }
        }
    return false;
}
