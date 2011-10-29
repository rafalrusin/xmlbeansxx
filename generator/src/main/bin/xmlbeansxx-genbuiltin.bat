

set XMLBEANSXX_HOME=%~dp0..


java -Dgenbuiltin -DinputPath=%1 -jar %XMLBEANSXX_HOME%\xmlbeansxx-gen.jar
