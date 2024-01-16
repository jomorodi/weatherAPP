#ifndef WEATHERAPP_H
#define WEATHERAPP_H

#include <QWidget>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QLabel>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class WeatherApp; }
QT_END_NAMESPACE

class WeatherApp : public QWidget
{
    Q_OBJECT

public:
    WeatherApp(QWidget *parent = nullptr);
    ~WeatherApp();

signals:
    void geocodeDataRecieved (const double & , const double & );

private slots:
    void fetchGeocodeData();
    void fetchWeatherData(const double & , const double & );
    void handleNetworkResponse();
    void parseGeocodeData(const QByteArray &data);
    void handleWeatherNetworkResponse();
    void parseWeatherData(const QByteArray &data);



    void on_lineEdit_returnPressed();

private:
    void setupUI();
    void connectSignals();

private:
    Ui::WeatherApp *ui;
    QNetworkAccessManager networkManager;
    //QPushButton *fetchButton;
    //QLabel *weatherLabel;
    QString cityNameFormatted;
};
double roundTotwoDecimalPlace (double );
#endif // WEATHERAPP_H
