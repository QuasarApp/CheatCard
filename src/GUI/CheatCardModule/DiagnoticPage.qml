import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import DoctorPillModule 1.0

DoctorView {

    onContactWithDevsPressed: {
        activityProcessor.newActivity("qrc:/CheatCardModule/Contacts.qml");

    }

}
