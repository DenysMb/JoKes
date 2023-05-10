/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2023 Denys Madureira <>
*/

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QtQml>

#include "about.h"
#include "app.h"
#include "version-jokes.h"
#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "jokesconfig.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setApplicationName(QStringLiteral("JoKes"));

    KAboutData aboutData(
                         // The program name used internally.
                         QStringLiteral("JoKes"),
                         // A displayable program name string.
                         i18nc("@title", "JoKes"),
                         // The program version string.
                         QStringLiteral(JOKES_VERSION_STRING),
                         // Short description of what the app does.
                         i18n("Just a small application that tells you a jokes fetched from JokesAPI"),
                         // The license this code is released under.
                         KAboutLicense::GPL,
                         // Copyright Statement.
                         i18n("(c) 2023"));
    aboutData.addAuthor(i18nc("@info:credit", "Denys Madureira"),
                        i18nc("@info:credit", "Developer"),
                        QStringLiteral(""),
                        QStringLiteral("https://github.com/DenysMb"));
    KAboutData::setApplicationData(aboutData);

    QQmlApplicationEngine engine;

    auto config = JoKesConfig::self();

    qmlRegisterSingletonInstance("org.kde.JoKes", 1, 0, "Config", config);

    AboutType about;
    qmlRegisterSingletonInstance("org.kde.JoKes", 1, 0, "AboutType", &about);

    App application;
    qmlRegisterSingletonInstance("org.kde.JoKes", 1, 0, "App", &application);

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
