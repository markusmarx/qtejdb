import QtQuick 2.0
import QEjdb 1.0 as QEjdb

QEjdb.CollectionModel {
    id:demoModel
    collection: 'testcollection'

    hints: {
        '$orderby': {'_createdAt': -1}
    }

    function saveItem(name) {
        var item = {
            '_createdAt': new Date(),
            'name': name
        }

        item = demoModel.insert(item, 0);

        console.log(item);
    }

}
