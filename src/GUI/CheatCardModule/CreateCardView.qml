import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15


Page {
    id: root;
    property var model: null

    Connections {
        target: model

        function onSigCardCreated() {
            activityProcessor.popItem()
        }
    }

    contentItem: ColumnLayout {
        anchors.fill: parent
        EditCardView {
            Layout.preferredWidth: Math.min(root.width, root.height / 0.7)
            Layout.preferredHeight: width * 0.7
            Layout.alignment: Qt.AlignCenter

            model: (root.model)? root.model.currentCard: null
            editable: true
            creating: true
            cardInteractive: false
        }
    }
}
