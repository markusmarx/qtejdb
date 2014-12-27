# A Documentstore for Qt / QML

QtEjdb is a simple to use document store. It's based on Qt and ejdb.


- Open database. If the database file not exist it will be created.
```c++
QEjdbDatabase::addDatabase("file:testdb").open();
```

- Create a collection. A collection is a container for one documenttype.
```c++
QEjdbDatabase qejdb = QEjdbDatabase::database();
qejdb.createCollection("testcollection");
```

- Build and save a bson
``` c++
QBsonObject person;
person.insert("name", "Frodo Baggins");
person.insert("age", 21);

qejdb.save("testcollection", person);
```

- Get the bson _id and load it
``` c++
QBsonOid oid = obj.value("_id").toId();
QBsonObject frodo = qejdb.load( "testcollection", oid);
```

- Or load bson by query
``` c++
QEjdbQuery query("testcollection", qejdb);
QEjdbResult result = query.exec(
      QBsonObject(
          "name",QBsonObject("$begin", "Frod")
      )
);

```
- voilà!






## Development

Buildsystem: CMake
