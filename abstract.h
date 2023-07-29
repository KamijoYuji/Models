#ifndef ABSTRACT_H
#define ABSTRACT_H
#include <QAbstractTableModel>

class Abstract : public QAbstractTableModel
{
public:
    struct Data {
        QString name;
        int number;
        int id;

        bool operator==(const Data& a) const {return a.id == id;}
    };
    Abstract(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void getData();
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVector<Data> _data;

public:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRow(int id);
};

#endif // ABSTRACT_H
