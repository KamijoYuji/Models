#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>
#include "abstract.h"
#include "modelfortreeview.h"
#include "myproxymodel.h"
#include <QStandardItemModel>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_lineEdit_editingFinished();
    void hSortChanged(int logicalIndex, Qt::SortOrder order);

private:

    int m_logicalIndex = 2;
    Qt::SortOrder m_order = Qt::AscendingOrder;
    Ui::MainWindow *ui;

    QSqlDatabase db;
    QSqlQuery *query;

    QSqlDatabase db2;
    QSqlQuery *query2;

    ModelForTreeView *t_model;
    Abstract *model;
};

#endif // MAINWINDOW_H
