#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstring>
using namespace std;
const unsigned short int MAX_BUF_SIZE = 4096;

// $ diff file1 file2       to compare two files
int copy_func(string fileName) {

  ifstream readingFile(fileName, ifstream::binary);
  ofstream copyFile(fileName + ".copy", ifstream::binary);

  if ( readingFile && copyFile ) {

    char* buffer = new char [MAX_BUF_SIZE];

    // Get length of file in characters
    readingFile.seekg(0, readingFile.end);
    int length = readingFile.tellg();
    readingFile.seekg(0, readingFile.beg);

    // Write big chunks
    for (int i=0; i<(length/MAX_BUF_SIZE); i++){
      readingFile.read(buffer, MAX_BUF_SIZE);
      copyFile.write(buffer, MAX_BUF_SIZE);
    }
    // Wrtite reminders
    readingFile.read(buffer, length % MAX_BUF_SIZE );
    copyFile.write(buffer, length % MAX_BUF_SIZE);

    delete[] buffer;
    readingFile.close();
    copyFile.close();
  }
  return 0;
}

int main(int argc, char* argv[]) {
  if( argc >1 && strlen(argv[1]) > 0)
    copy_func(argv[1]);
	else
		cout << "No file to copy specified.\n./a.out fimeName";
	return 0;
}
