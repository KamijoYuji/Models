#include "modelfortreeview.h"
#include "QSqlDatabase"
#include "QSqlQuery"
#include "QDebug"
#include "QSqlError"
#include "QLabel"
#include <QStandardItemModel>

ModelForTreeView::ModelForTreeView(QObject *parent)
    : QAbstractItemModel(parent)
{
    getData();
}

bool ModelForTreeView::hasId(int id)
{
    for(int i = 0; i < m_data.count(); i++)
        if(m_data[i].id==id)
            return true;
    return false;
}

int ModelForTreeView::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;
    return m_data.count();
}

int ModelForTreeView::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;
    return 4;
}

QVariant ModelForTreeView::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole or role == Qt::EditRole)
    {
        if(index.isValid())
        {
            if(index.column()==0)
                return m_data[index.row()].name;
            if(index.column()==1)
                return m_data[index.row()].number;
            if(index.column()==2)
                return m_data[index.row()].parentid;
            if(index.column()==3)
                return m_data[index.row()].id;
        }
    } else if (role == Qt::UserRole) {
        return m_data[index.row()].id;
    }
    return QVariant();
}

bool ModelForTreeView::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole)
    {
        QSqlQuery query;
        QString a = "UPDATE Numbers SET %2 WHERE id=%1";
        a=a.arg(m_data[index.row()].id);
        if(index.column()==0)
        {
            a=a.arg("name = '%1'").arg(value.toString());
            m_data[index.row()].name = value.toString();
        }
        if(index.column()==1)
        {
            a=a.arg("number = %1").arg(value.toInt());
            m_data[index.row()].number = value.toInt();
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

void ModelForTreeView::getData()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./dbdb.db");
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

    query->exec("SELECT name, number, parentid, id FROM Numbers");
    while (query->next()) {
        Data a;
        a.name=query->value(0).toString();
        a.number=query->value(1).toInt();
        a.parentid=query->value(2).toInt();
        a.id=query->value(3).toInt();
        m_data.push_back(a);
    }
}

QVariant ModelForTreeView::headerData(int section, Qt::Orientation orientation, int role) const
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
            return QString("PARENT_ID");
        if(section==3)
            return QString("ID");

    }

    if(orientation == Qt::Vertical)
    {
        return QAbstractItemModel::headerData(section,orientation,role);
    }
    return QVariant();
}

Qt::ItemFlags ModelForTreeView::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QModelIndex ModelForTreeView::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row,column,parent))
        return QModelIndex();

    if(!parent.isValid())
    return createIndex(row,column);

    QStandardItem* parentItem = static_cast<QStandardItem*>(parent.internalPointer());
    QStandardItem* childItem = parentItem->child(row);
    if(childItem)
        return createIndex(row,column,childItem);
    return QModelIndex();

}

QModelIndex ModelForTreeView::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

bool ModelForTreeView::insertRow(int parentid, int row, int count, const QModelIndex &parent)
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./dbdb.db");
    QSqlQuery* query;
    query = new QSqlQuery(db);
    if(db.open())
    {
        qDebug("open");
        beginInsertRows(parent, row, count+row-1);
        for(int i = 0; i < count; ++i){
            query->prepare("INSERT INTO Numbers (name, number, parentid) "
                           "VALUES (:name, :number, :parentid)");
            query->bindValue(":name"," ");
            query->bindValue(":number", 0);
            query->bindValue(":parentid", parentid);
            if(query->exec() && query->exec("select * FROM Numbers WHERE id = last_insert_rowid()"))
            {
                query->next();
                qDebug() << query->value(0);
                Data b;
                b.id = query->value("id").toInt();
                b.name = query->value("name").toString();
                b.number = query->value("number").toInt();
                b.parentid = query->value("parentid").toInt();
                m_data.insert(row, b);
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


bool ModelForTreeView::removeRow(int id)
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./dbdb.db");
    QSqlQuery* query;
    query = new QSqlQuery(db);
    if(db.open())
    {

        if(query->exec(QString("DELETE FROM Numbers WHERE id = %1").arg(id)))
        {
            Data temp;
            temp.id = id;
            int r = m_data.indexOf(temp);
            beginRemoveRows(QModelIndex(), r, r);
            m_data.remove(r, 1);
            endRemoveRows();
            return  true;
        }
    }
    return false;
}
