#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QStandardItemModel>
#include <QDebug>
#include <QStandardItemModel>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addEquipmentButton_clicked();

    void on_lineEditDeleteEquipmentID_clicked();
    void on_modifyEquipmentButton_clicked();

    void on_pushButton_5_clicked();

    void on_Mod_2_clicked();



private:
    Ui::MainWindow *ui;
    QStandardItemModel *equipmentModel; // Modèle pour stocker les informations sur les équipements
};

#endif // MAINWINDOW_H
