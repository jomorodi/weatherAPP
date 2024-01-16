#include "weatherapp.h"
#include "./ui_weatherapp.h"
#include <cmath>

WeatherApp::WeatherApp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WeatherApp)
{
    ui->setupUi(this);
    connect (this, &WeatherApp::geocodeDataRecieved, this, &WeatherApp::fetchWeatherData );
}

WeatherApp::~WeatherApp()
{
    delete ui;
}





void WeatherApp::fetchGeocodeData() {
    QUrl apiUrl(cityNameFormatted);

    QNetworkRequest request(apiUrl);
    QNetworkReply* reply = networkManager.get(request);

    connect(reply, &QNetworkReply::finished, this, &WeatherApp::handleNetworkResponse);
}


void WeatherApp::fetchWeatherData(const double & latitude , const double & longitude ){

         QString apiUrlFormated = QString("https://api.open-meteo.com/v1/forecast?latitude=%1&longitude=%2&current=temperature_2m,relative_humidity_2m,apparent_temperature,rain,snowfall,wind_speed_10m").arg(
            QString::number(latitude), QString::number(longitude) );
        QUrl apiUrl (apiUrlFormated );
        QNetworkRequest request_weather(apiUrl);
        QNetworkReply* reply_weather = networkManager.get(request_weather);
        connect(reply_weather, &QNetworkReply::finished, this, &WeatherApp::handleWeatherNetworkResponse);

}

void WeatherApp::handleWeatherNetworkResponse(){


    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if (reply && reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        qDebug() << data ;
        parseWeatherData(data);
    } else {
        qDebug() << "Network Error: " << reply->errorString();
    }

    reply->deleteLater();

}

void  WeatherApp::parseWeatherData(const QByteArray &data){

    QJsonDocument jsonDocument = QJsonDocument::fromJson(data);

    if (!jsonDocument.isNull()) {
        QJsonObject rootObject = jsonDocument.object();

        if (rootObject.contains("current")) {
            QJsonObject currentWeather = rootObject.value("current").toObject();
            QJsonValue temperature_2m = currentWeather ["temperature_2m"];
            QJsonValue relative_humidity_2m = currentWeather ["relative_humidity_2m"] ;
            QJsonValue apparent_temperature  = currentWeather ["apparent_temperature"] ;
            QJsonValue rain = currentWeather ["rain"] ;
            QJsonValue snowfall = currentWeather ["snowfall"] ;
            QJsonValue wind_speed_10m = currentWeather ["wind_speed_10m"] ;
            ui->lineEdit_4->setText(QString::number(temperature_2m.toDouble()));
            ui->lineEdit_5->setText(QString::number(relative_humidity_2m.toDouble()));
            ui->lineEdit_6->setText(QString::number(apparent_temperature.toDouble()));
            ui->lineEdit_7->setText(QString::number(snowfall.toDouble()));
            ui->lineEdit_8->setText(QString::number(rain.toDouble()));
            ui->lineEdit_9->setText(QString::number(wind_speed_10m.toDouble()));

            return;


        }
    }

    qDebug() << "Error parsing weather data" ;



}

void WeatherApp::handleNetworkResponse() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if (reply && reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        //qDebug() << data ;
        parseGeocodeData(data);
    } else {
        qDebug() << "Network Error: " << reply->errorString();
    }

    reply->deleteLater();
}

void WeatherApp::parseGeocodeData(const QByteArray &data) {

    QJsonDocument jsonDocument = QJsonDocument::fromJson(data);

    if (!jsonDocument.isNull()) {
        QJsonObject rootObject = jsonDocument.object();

        if (rootObject.contains("results")) {
            QJsonArray mainArray = rootObject.value("results").toArray();
            QJsonObject mainObject = mainArray[0].toObject();

            if (mainObject.contains("latitude")) {
                double latitude = mainObject.value("latitude").toDouble();
                double longitude = mainObject.value("longitude").toDouble();
                emit geocodeDataRecieved (roundTotwoDecimalPlace (latitude) , roundTotwoDecimalPlace (longitude)) ;
                ui->lineEdit_2->setText(QString::number(longitude));
                ui->lineEdit_3->setText(QString::number(latitude)) ;

                return;
            }
        }
    }

        qDebug() << "Error parsing weather data" ;
}






void WeatherApp::on_lineEdit_returnPressed()
{
    QString	 cityName = ui->lineEdit->text();

    cityNameFormatted = QString("https://geocoding-api.open-meteo.com/v1/search?name=%1&count=1").arg(cityName);
    WeatherApp::fetchGeocodeData();

}

double roundTotwoDecimalPlace (double value ){
    return std::ceil(value * 100.0) / 100.0;
}
