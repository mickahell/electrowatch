#include "dataFile.h"

// You only need to format LittleFS the first time you run a
// test or else use the LITTLEFS plugin to create a partition 
// https://github.com/lorol/arduino-esp32littlefs-plugin

void FSData::listDir(fs::FS &fs, const char * dirname){
    //Serial.printf("Listing directory: %s\r\n", dirname);

    while (!files.empty()){
        files.pop_back();
    }

    File root = fs.open(dirname);
    if(!root){
        //Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        //Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(!file.isDirectory()){
            //Serial.print("  FILE : ");
            //Serial.println(file.name());
            files.push_back(String(file.name()));
        }
        file = root.openNextFile();
    }
}

void FSData::createDir(fs::FS &fs, const char * path){
    fs.mkdir(path);
}

void FSData::removeDir(fs::FS &fs, const char * path){
    fs.rmdir(path);
}

void FSData::readFile(fs::FS &fs, const char * path){
    //Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file: ");
    while(file.available()){
        content = file.readString();
        Serial.println(content);
    }
    file.close();
}

void FSData::writeFile(fs::FS &fs, const char * path, const char * message){
    //Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        //Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        //Serial.println("- file written");
    } else {
        //Serial.println("- write failed");
    }
    file.close();
}

void FSData::appendFile(fs::FS &fs, const char * path, const char * message){
    File file = fs.open(path, FILE_APPEND);
    if(!file){
        return;
    }
    file.print(message);
    file.close();
}

void FSData::renameFile(fs::FS &fs, const char * path1, const char * path2){
    fs.rename(path1, path2);
}

void FSData::deleteFile(fs::FS &fs, const char * path){
    //Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        //Serial.println("- file deleted");
    } else {
        //Serial.println("- delete failed");
    }
}
