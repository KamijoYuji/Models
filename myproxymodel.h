#ifndef MYPROXYMODEL_H
#define MYPROXYMODEL_H
#include <QSortFilterProxyModel>

class MyProxyModel : public QSortFilterProxyModel
{
public:
    MyProxyModel(QObject *parent = Q_NULLPTR);
    void setCurrentColumn(int column);
private:
    int m_column;
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    int CurrentColumn();

};

#endif // MYPROXYMODEL_H
