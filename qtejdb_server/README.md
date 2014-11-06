#QtEjdb Server Documentation

A simple server implementation with small footprint based on Qt and QtEjdb.

##Commandline Args

confFile        path to a configuration file

##Configuration XML

```xml
<QtEjdbDatabase name="string" vendor="vendor" app="applicationname">
  <Connectors>
     <WebSocket port="portvalue" />
  </Connectors>
  <Collections>
     <Collection compressed="true" large="true" itemsCount="xxx" />
     <Collection compressed="true" large="true" itemsCount="xxx" />
     <Collection compressed="true" large="true" itemsCount="xxx" />
 </Collections>

</QtEjdbConfiguration>
```
