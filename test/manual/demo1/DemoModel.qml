import QtQuick 2.0
import QEjdb 1.0 as QEjdb

QEjdb.CollectionModel {
    id:demoModel
    collection: 'testcollection'

    function saveItem(name) {
        var item = {'name': name};
        demoModel.insert(item, 0);
    }

}
