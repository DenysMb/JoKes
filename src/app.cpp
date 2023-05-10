// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Denys Madureira <>

#include "app.h"
#include <iostream>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QQuickWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>

void App::restoreWindowGeometry(QQuickWindow *window, const QString &group) const
{
    KConfig dataResource(QStringLiteral("data"), KConfig::SimpleConfig, QStandardPaths::AppDataLocation);
    KConfigGroup windowGroup(&dataResource, QStringLiteral("Window-") + group);
    KWindowConfig::restoreWindowSize(window, windowGroup);
    KWindowConfig::restoreWindowPosition(window, windowGroup);
}

void App::saveWindowGeometry(QQuickWindow *window, const QString &group) const
{
    KConfig dataResource(QStringLiteral("data"), KConfig::SimpleConfig, QStandardPaths::AppDataLocation);
    KConfigGroup windowGroup(&dataResource, QStringLiteral("Window-") + group);
    KWindowConfig::saveWindowPosition(window, windowGroup);
    KWindowConfig::saveWindowSize(window, windowGroup);
    dataResource.sync();
}

void App::request(QObject *element) const
{
  element->setProperty("text", "Wait a moment, I am thinking...");
  element->setProperty("color", "grey");

  QNetworkAccessManager *manager = new QNetworkAccessManager();

  QUrl url("https://v2.jokeapi.dev/joke/Any");
  QUrlQuery query;
  query.addQueryItem("blacklistFlags", "nsfw,religious,political,racist,sexist,explicit");
  query.addQueryItem("type", "single");
  url.setQuery(query);

  QNetworkRequest request(url);

  QNetworkReply *reply = manager->get(request);

  connect(reply, &QNetworkReply::finished, [=]() {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response);
        QString joke = json.object().value("joke").toString();

        qDebug() << "Joke:" << joke;

        element->setProperty("text", joke);
        element->setProperty("color", "black");

        reply->deleteLater();
    } else {
      // Tratar erros
      qDebug() << reply->errorString();

      element->setProperty("text", reply->errorString());
      element->setProperty("color", "red");
    }
    reply->deleteLater();
  });
}
