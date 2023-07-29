#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlError>
#include <QItemSelectionModel>
#include <QSortFilterProxyModel>
#include <QTreeView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./2mydb.db");

    db2 = QSqlDatabase::addDatabase("QSQLITE");
    db2.setDatabaseName("./dbdb.db");

    if(db.open())
    {
        qDebug("open");
    }
    else
    {
        qDebug("no open");
    }

    if(db2.open())
    {
        qDebug("open");
    }
    else
    {
        qDebug("no open");
    }

    query = new QSqlQuery(db);

    query2 = new QSqlQuery(db2);

    if(!query->exec("CREATE TABLE Numbers(name TEXT, number INT, id INTEGER PRIMARY KEY AUTOINCREMENT)"))
        qDebug()<<query->lastError().text();

    if(!query2->exec("CREATE TABLE Numbers(name TEXT, number INT, parentid INT, id INTEGER PRIMARY KEY AUTOINCREMENT)"))
        qDebug()<<query2->lastError().text();

    model = new Abstract(this);
    t_model = new ModelForTreeView(this);
    ui->comboBox->addItem("<Фильтрация>");
    for(int i = 0; i<model->columnCount(); i++)
    {
        ui->comboBox->addItem(model->headerData(i,Qt::Horizontal, Qt::DisplayRole).toString());
    }
    ui->treeView->setModel(t_model);
    ui->tableView->setModel(model);
    ui->tableView->setSortingEnabled(true);
    connect(ui->tableView->horizontalHeader(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)),this,SLOT(hSortChanged(int, Qt::SortOrder)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(t_model->hasId(ui->lineEdit_2->text().toInt()) or ui->lineEdit_2->text().toInt() == 0)
        t_model->insertRow(ui->lineEdit_2->text().toInt(),t_model->rowCount(), ui->spinBox->value());
}

void MainWindow::on_pushButton_2_clicked()
{
    if(!ui->treeView->selectionModel()->selectedRows(0).isEmpty())
        t_model->removeRow(ui->treeView->selectionModel()->selectedRows(0).first().data(Qt::UserRole).toInt());
}

void MainWindow::on_lineEdit_editingFinished()
{
    QString line = ui->lineEdit->text();
    int column;
    if(ui->comboBox->currentText()=="<Фильтрация>"){
        column = -1;
        line = "";}
    if(ui->comboBox->currentText()=="Name"){
        column = 0;}
    if(ui->comboBox->currentText()=="Number"){
        column = 1;}
    if(ui->comboBox->currentText()=="ID"){
        column = 2;}
    MyProxyModel *proxyModel = new MyProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setCurrentColumn(column);
    proxyModel->setFilterRegExp(line);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setSortRole(Qt::DisplayRole);
    proxyModel->setDynamicSortFilter(true);
    proxyModel->sort(m_logicalIndex, m_order);
    ui->tableView->setModel(proxyModel);
}

void MainWindow::hSortChanged(int logicalIndex, Qt::SortOrder order)
{
    m_logicalIndex = logicalIndex;
    m_order = order;
    on_lineEdit_editingFinished();
}
