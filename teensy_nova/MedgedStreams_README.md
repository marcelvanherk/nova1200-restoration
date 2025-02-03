# MergedStreams

Make an Arduino stream which writes reads and writes from two streams. The first stream will get read and write preference.

## Note
This libarary is very much in beta. It hasn't been fully tested nor are all Serial class methods implemented yet.

## Install
As usual, download zip, unzip and rename to remove the dash character and place in your Arduino Libraries folder, on MacOSX ~/Documents/Arduino/libraries/

## Use
First stream argument will get priority.
```cpp
MergedStreams mergedStreams(Serial, Serial1);
```

Then use it just like a stream.
```cpp
mergedStreams.write('a');
Serial.println((char)mergedStreams.read());
```
