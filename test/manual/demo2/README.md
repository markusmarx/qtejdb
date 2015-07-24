# Demo1

Shows a bson in a ListView and connect in a collection with CollectionModel.

## demo1.qml

import QEjdb

```qml
import QEjdb 1.0 as QEjdb
```


Create a client, connect the database and create the collection.
```qml
QEjdb.Client {
    id: client
    uri: "file:demo1.db"
    connectionName: "test"
    Component.onCompleted: {
        connect()
        createCollection('testcollection')
    }
    Component.onDestruction: {
        disconnect();
    }
}
```

Creates the demomodel from DemoModel.qml
```qml
DemoModel {
    id: model
    client: client
}
```

## DemoModel.qml

Creates a model to store bson and order by _createdAt descending.

```qml
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
```


