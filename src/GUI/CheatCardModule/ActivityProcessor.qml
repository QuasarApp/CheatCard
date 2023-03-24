import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

StackView {
    id: root
    property var model: null
    property string cuurentActivityTitle: (currentItem && currentItem.toolBarTitle)?
                                              currentItem.toolBarTitle: ""

    Connections {
        target: model
        function onSigFreeItem(card, userId, freeItemsCount) {

            var activity = root.newActivity("qrc:/CheatCardModule/FreeItemsView.qml",
                                                         card);
            activity.freeItems = freeItemsCount
            activity.userId = userId
        }

        function onSigNewActivity(view, model) {
            root.newActivity(view, model);
        }
    }

    function newActivityFromComponent(component, activityModel) {
        var activity = component.createObject(root);
        if (activity === null) {
            // Error Handling
            console.error("Error creating Activity object. " + component.errorString());
            return;
        }

        if (activityModel) {
            activity.model = activityModel;
        }

        push(activity);

        return activity;
    }

    function newActivity(viewFile, activityModel) {

        if (!viewFile || !viewFile.length) {
            console.error("Failed to create activity. view object is invalid");
            return;
        }

        var component = Qt.createComponent(viewFile);

        if (component.status === Component.Ready) {
            var activity = component.createObject(root);
            if (activity === null) {
                // Error Handling
                console.error("Error creating Activity object");
                return;
            }

            if (activityModel) {
                activity.model = activityModel;
            }

            push(activity);

            return activity;

        } else if (component.status === Component.Error) {
            // Error Handling
            console.log("Error loading component:", component.errorString());
        }


    }

    function popItem() {
        var item = pop();
        item.destroy();
    }
}
