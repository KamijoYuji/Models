#ifndef MODELFORTREEVIEW_H
#define MODELFORTREEVIEW_H
#include <QAbstractItemModel>

class ModelForTreeView : public QAbstractItemModel
{
public:
    struct Data {
        QString name;
        int number;
        int id;
        int parentid;

        QVector<Data>* _children;

        bool operator==(const Data& a) const {return a.id == id;}
    };
    ModelForTreeView(QObject *parent = nullptr);

    bool hasId(int id);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void getData();
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;

public:
    bool insertRow(int parentid, int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRow(int id);

private:
    QVector<Data> m_data;
};

#endif // MODELFORTREEVIEW_H
