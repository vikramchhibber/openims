#!/bin/bash
# --------------------------------------------------------------
# Include JAR Files
# --------------------------------------------------------------
export JAVA_HOME=/usr/lib/jvm/java-7-openjdk-amd64/
cd /opt/OpenIMSCore/fhoss/
CLASSPATH=$CLASSPATH:log4j.properties:.
for i in ./lib/*.jar; do CLASSPATH="$i":"$CLASSPATH"; done

# --------------------------------------------------------------
# Start-up
# --------------------------------------------------------------

$JAVA_HOME/bin/java -cp $CLASSPATH de.fhg.fokus.hss.main.HSSContainer $1 $2 $3 $4 $5 $6 $7 $8 $9
