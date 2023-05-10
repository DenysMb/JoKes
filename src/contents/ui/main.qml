// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Denys Madureira <>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.JoKes 1.0

Kirigami.ApplicationWindow {
    id: root

    title: i18n("JoKes")

    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 11

    onClosing: App.saveWindowGeometry(root)

    onWidthChanged: saveWindowGeometryTimer.restart()
    onHeightChanged: saveWindowGeometryTimer.restart()
    onXChanged: saveWindowGeometryTimer.restart()
    onYChanged: saveWindowGeometryTimer.restart()

    Component.onCompleted: {
        App.restoreWindowGeometry(root)
        App.request(jokeText)
    }

    // This timer allows to batch update the window size change to reduce
    // the io load and also work around the fact that x/y/width/height are
    // changed when loading the page and overwrite the saved geometry from
    // the previous session.
    Timer {
        id: saveWindowGeometryTimer
        interval: 1000
        onTriggered: App.saveWindowGeometry(root)
    }

    globalDrawer: Kirigami.GlobalDrawer {
        title: i18n("JoKes")
        titleIcon: "applications-graphics"
        isMenu: !root.isMobile
        actions: [
            Kirigami.Action {
                text: i18n("About JoKes")
                icon.name: "help-about"
                onTriggered: pageStack.layers.push('qrc:About.qml')
            },
            Kirigami.Action {
                text: i18n("Quit")
                icon.name: "application-exit"
                onTriggered: Qt.quit()
            }
        ]
    }

    contextDrawer: Kirigami.ContextDrawer {
        id: contextDrawer
    }

    pageStack.initialPage: page

    Kirigami.ScrollablePage {
        id: page

        Layout.fillWidth: true

        title: i18n("JoKes")

        actions.main: Kirigami.Action {
            text: i18n("Tell me a new JoKe")
            icon.name: "list-add"
            tooltip: i18n("Get a new joke")
            onTriggered: {
                App.request(jokeText)
            }
        }

        Kirigami.FormLayout {
            anchors.fill: parent

            Layout.fillWidth: true

            Kirigami.Heading {
                Layout.alignment: Qt.AlignCenter
                text: i18n("Here's your JoKe!")
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Controls.Label {
                id: jokeText

                Layout.fillWidth: true
                wrapMode: Text.Wrap
                text: ""
                color: "grey"
            }

            Kirigami.Separator {
                Kirigami.FormData.isSection: true
            }

            Controls.Button {
                Layout.alignment: Qt.AlignHCenter

                Layout.fillWidth: true

                text: "Tell me a new JoKe"
                onClicked: App.request(jokeText)
            }
        }
    }
}
