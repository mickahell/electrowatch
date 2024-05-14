#include "dataFile.h"

void FSData::listDir(fs::FS &fs, const char * dirname){
	//Serial.printf("Listing directory: %s\r\n", dirname);
	File root;
	File file;

	while (!files.empty()){
		files.pop_back();
	}

	root = fs.open(dirname);
	if(!root){
		//Serial.println("- failed to open directory");
		return;
	}
	if(!root.isDirectory()){
		//Serial.println(" - not a directory");
		return;
	}

	file = root.openNextFile();
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
	File file;

	file = fs.open(path);
	if(!file || file.isDirectory()){
		//Serial.println("- failed to open file for reading");
		return;
	}

	//Serial.println("- read from file: ");
	while(file.available()){
		content = file.readString();
		//Serial.println(content);
	}
	file.close();
}

void FSData::writeFile(fs::FS &fs, const char * path, const char * message){
	//Serial.printf("Writing file: %s\r\n", path);
	File file;

	file = fs.open(path, FILE_WRITE);
	if(!file){
		//Serial.println("- failed to open file for writing");
		return;
	}
	file.print(message);
	file.close();
}

void FSData::appendFile(fs::FS &fs, const char * path, const char * message){
	File file;
	file = fs.open(path, FILE_APPEND);
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
	fs.remove(path);
}
