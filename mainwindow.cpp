
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectToDatabase();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::connectToDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("radar_db");
    db.setUserName("admin");
    db.setPassword("admin");
    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", db.lastError().text());
    }
}

void MainWindow::on_saveButton_clicked() {
    double freq = ui->frequencySpinBox->value();
    double amp = ui->amplitudeSpinBox->value();
    double pw = ui->pulseWidthSpinBox->value();

    QSqlQuery query;
    query.prepare("INSERT INTO radar_signals (frequency, amplitude, pulse_width) VALUES (?, ?, ?)");
    query.addBindValue(freq);
    query.addBindValue(amp);
    query.addBindValue(pw);

    if (!query.exec()) {
        QMessageBox::warning(this, "Error", query.lastError().text());
    } else {
        QMessageBox::information(this, "Success", "Signal data saved.");
    }
}

void MainWindow::on_loadButton_clicked() {
    QSqlQuery query("SELECT * FROM radar_signals");
    while (query.next()) {
        double freq = query.value("frequency").toDouble();
        double amp = query.value("amplitude").toDouble();
        double pw = query.value("pulse_width").toDouble();

        // Display logic placeholder
        qDebug("Loaded: %.2f GHz, %.2f dB, %.2f µs", freq, amp, pw);
    }
}

void MainWindow::on_clearButton_clicked() {
    ui->frequencySpinBox->setValue(0.5);
    ui->amplitudeSpinBox->setValue(0.0);
    ui->pulseWidthSpinBox->setValue(0.0);
}
