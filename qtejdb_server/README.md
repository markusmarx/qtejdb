#README

##What is QtEjdb Server?
A simple server implementation with small footprint based on Qt and QtEjdb.

##Requirements

* Qt Framework Version 5

##Installation

##Documentation

###Commandline Args

confFile        path to a configuration file

###Configuration XML

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

##Licence

LGPL v3

