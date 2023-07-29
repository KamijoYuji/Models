#include "myproxymodel.h"
#include <QDebug>

MyProxyModel::MyProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

bool MyProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(filterRegExp().pattern().length()>0)
    {
        int search_index = 0;
        QString search_text = filterRegExp().pattern();
        Qt::CaseSensitivity search_case_sensitivity = filterCaseSensitivity();

        QVariant search_value = sourceModel()->index(source_row, search_index+m_column, source_parent).data();
        if(search_value.isValid())
        {
            QString search_string = search_value.toString();
            if(search_case_sensitivity == Qt::CaseSensitive)
            {
                return search_string.contains(search_text);
            }
            else
            {
                return search_string.toLower().contains(search_text.toLower());
            }
        }
        else {
            return false;
        }
    }
    return true;
}

QVariant MyProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical && role == Qt::DisplayRole)
    {
        QModelIndex sourceModelIndex = mapToSource(index(section,0));
        if(sourceModelIndex.isValid()){
            return section+1;
        }
    }
    return QSortFilterProxyModel::headerData(section,orientation,role);
}

void MyProxyModel::setCurrentColumn(int column)
{
    m_column = column;
}
