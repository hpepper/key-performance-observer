#/usr/bin/bash

# Purpose: Manually test the Maven to Sonar interface.

#cd ..
# build the code.
make check

# Load the data to Sonar
mvn -f pom.xml -e -B sonar:sonar -Dsonar.jdbc.driver=com.mysql.jdbc.Driver -Dsonar.jdbc.url=jdbc:mysql://localhost:3306/sonar?useUnicode=true&characterEncoding=utf8

# Clean up after maven run.
rm -rf target

# clean up after ant
make clean
