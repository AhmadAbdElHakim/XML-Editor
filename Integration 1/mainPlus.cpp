#include<iostream>
#include <fstream>
#include<vector>
#include<string>
#include<algorithm>
#include <bits/stdc++.h>

using namespace std;

std::vector <string> lines;
std::vector <string> tags;
std::vector <string> tagsAndLines;
std::vector<string> pureTags;

void readFile(string fileName){
  string line;
  ifstream myfile (fileName);
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      lines.push_back(line);
    }
    myfile.close();
  }

/*
    for(int x=0;x<lines.size();x++){
    cout<<lines[x]<<"\n";
  }
*/
}

void getTagsAndLines(){
for(int x=0;x<lines.size();x++){

    int tagCounter = std::count(lines[x].begin(), lines[x].end(), '<');
    int place1 = lines[x].find('<');
    int place2 = lines[x].find('>');

    for(int m=0;m<tagCounter;m++){
        //cout<<lines[x].substr(place1+1,place2-place1-1)<<"\n";
        tags.push_back(lines[x].substr(place1+1,place2-place1-1));
        tagsAndLines.push_back(lines[x].substr(place1+1,place2-place1-1));

        if(lines[x][place2+1] != '<'){
            int temp = lines[x].find('<',place1+1);
            //cout<<lines[x].substr(place2+1,temp-place2-1)<<"\n";
            tagsAndLines.push_back(lines[x].substr(place2+1,temp-place2-1));
        }

        int place3 = lines[x].find('<',place1+1);
        int place4 = lines[x].find('>',place2+1);
        place1 = place3;
        place2 = place4;
    }
  }
}

void makePureTags(){
  for(int x=0;x<tags.size();x++){
    if(!tags[x].empty()){
        if(tags[x].find(' ') != string::npos){
            string s = tags[x].substr(0,tags[x].find(' '));
            pureTags.push_back(s);
        }else{
            pureTags.push_back(tags[x]);
        }
    }
  }
  /*
  for(int x=0;x<pureTag.size();x++){
    cout<<pureTag[x]<<"\n";
  }
*/
}

bool checkCorrectXml(){

  stack<string> tagChecker;

  for(int x=0;x<pureTags.size();x++){
    if(pureTags[x].find('/') == string::npos){
        tagChecker.push(pureTags[x]);
    }else{
        string temp = tagChecker.top();                                //last string saved in stack
        string s    = pureTags[x].substr(1,pureTags[x].length()-1);   //string without /
        //cout<<temp<<"\t"<<s<<"\n";
        if(s == tagChecker.top()){
            tagChecker.pop();
            continue;
        }else{
            tagChecker.push(temp);
            tagChecker.push(s);
        }
    }
  }
  if(tagChecker.size() == 0){return true;}
  else{ return false;}
}
int main(){

  readFile("text.txt");    //text file was read line by line, stored in lines vector

  getTagsAndLines();      //sort lines according to 1-tags only stored in tags vector, 2-tags and sentences stored in tagsAndLines vector

  makePureTags();         // separate line of tags from extra data ex. ( ahmed id="1" ) ---> ( ahmed ),stored in pureTags vector

  if(checkCorrectXml() == true){  //check if xml file is correct or no
    cout<<"Correct XML"<<"\n";
  }else{
    cout<<"Wrong XML"<<"\n";
  }


    return 0;
}
