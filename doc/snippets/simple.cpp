//! [create]
QEjdbDatabase::database("file:test_db");
//! [create]
//! [createcol]
QEjdbCollection col = db.collection(QLatin1String("COLL"), true)
//! [createcol]
//! [save]
QBsonObject obj("name", "Alfred Kabuschke")
col.save(obj);
//! [save]