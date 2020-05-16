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
std::vector<string> pureTagsLinesWithoutSlash;
extern string json = "{";


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
    vector<string> TandL;
for(int x=0;x<lines.size();x++){

    int tagCounter = std::count(lines[x].begin(), lines[x].end(), '<');
    int place1 = lines[x].find('<');
    int place2 = lines[x].find('>');

    for(int m=0;m<tagCounter;m++){
        //cout<<lines[x].substr(place1+1,place2-place1-1)<<"\n";
        tags.push_back(lines[x].substr(place1+1,place2-place1-1));
        TandL.push_back(lines[x].substr(place1+1,place2-place1-1));

        if(lines[x][place2+1] != '<'){
            int temp = lines[x].find('<',place1+1);
            //cout<<lines[x].substr(place2+1,temp-place2-1)<<"\n";
            TandL.push_back("~"+lines[x].substr(place2+1,temp-place2-1));
        }

        int place3 = lines[x].find('<',place1+1);
        int place4 = lines[x].find('>',place2+1);
        place1 = place3;
        place2 = place4;
    }
  }
  for(int x=0;x<TandL.size();x++){      //to remove empty lines and store tags and lines in tagsAndLines Vector
    if(! ((TandL[x][0] == '~') && (TandL[x].length() == 1)) ){
        tagsAndLines.push_back(TandL[x]);
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
  for(int x=0;x<pureTags.size();x++){
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

void makePureTagsLinesWithoutSlash(){

               for(int x=0;x<tagsAndLines.size();x++){        //make tags without slash,,stored in pureTagsLinesWithoutSlash vector
            if(tagsAndLines[x][0] == '/'){
                pureTagsLinesWithoutSlash.push_back(tagsAndLines[x].substr(1,tagsAndLines[x].length()-1));
            }else{
                if(tagsAndLines[x][0] != '~'){
                    int spacePlace = tagsAndLines[x].find(' ');
                    pureTagsLinesWithoutSlash.push_back(tagsAndLines[x].substr(0,spacePlace));
                }else{
                    pureTagsLinesWithoutSlash.push_back(tagsAndLines[x]);
                }
            }
    }

/*
    for(int x=0;x<pureTagsLinesWithoutSlash.size();x++){
        cout<<pureTagsLinesWithoutSlash[x]<<"\n";
    }
*/
}
////////////////////////////////////////////////////////////////////////////////////////

struct Node{
    string data;
    Node* parent;
    vector<Node *> children;
};

Node* makeNewNode(string data){
    Node* newNode = new Node;
    newNode->data = data;
    return newNode;
}

Node* addChildren(Node* root,string data){
    Node* child = makeNewNode(data);
    root->children.push_back(child);
    child->parent=root;
    return child;
}

void addChildren(Node* root,Node* child){
    root->children.push_back(child);
}

void showChildren(Node* root){
    int size = root->children.size();
    for(int x=0;x<size;x++){
        cout<<root->children[x]->data<<"\t";
    }
    cout<<"\n";
}

Node* getParent(Node* root){

    Node* temp = root;
    if(temp->parent != NULL){
       root = temp->parent;
    }
    return root;
}

void LevelOrderTraversal(Node * root){

    if (root==NULL)
        return;

    // Standard level order traversal code
    // using queue
    queue<Node *> q;  // Create a queue
    q.push(root); // Enqueue root
    while (!q.empty())
    {
        int n = q.size();

        // If this node has children
        while (n > 0)
        {
            // Dequeue an item from queue and print it
            Node * p = q.front();
            q.pop();
            cout << p->data << " ";

            // Enqueue all children of the dequeued item
            for (int i=0; i<p->children.size(); i++)
                q.push(p->children[i]);
            n--;
        }

        cout << endl; // Print new line between two levels
    }
}

Node* getMainParent(Node* root){

    Node* temp = root;
    while(1){
        if(temp->parent == NULL){
            break;
        }else{
            temp = temp->parent;
        }
    }
    return temp;
}

Node* getLastChild(Node* root){

    if(root->children.size() == 0){
        return root;
    }
    int last = root->children.size()-1;
    getLastChild(root->children[last]);

}
////////////////////////////////////////////////////////////////////////////

Node* makeTree(vector<string> pureTagsLinesWithoutSlash,Node* current_root){
  stack<string> temp;
  for(int x=0;x<pureTagsLinesWithoutSlash.size();x++){     //makes tree

        if(x==0){
            current_root = makeNewNode(pureTagsLinesWithoutSlash[x]);
            current_root->parent = NULL;
            temp.push(pureTagsLinesWithoutSlash[x]);
            continue;
        }

        if(pureTagsLinesWithoutSlash[x][0] != '~'){
             if(pureTagsLinesWithoutSlash[x] == temp.top()){
                temp.pop();
                current_root = getParent(current_root);
             }else{
                current_root = addChildren(current_root,pureTagsLinesWithoutSlash[x]);
                temp.push(pureTagsLinesWithoutSlash[x]);
             }
        }else{
             current_root = addChildren(current_root,pureTagsLinesWithoutSlash[x].substr(1,pureTagsLinesWithoutSlash[x].length()-1));
             current_root = getParent(current_root);
        }
  }
  return getMainParent(current_root);
}

void makeOneNodeForRepeatedChild(Node* root){

      if(root->children.size() < 1){
            return;
      }
      vector<string>temp;
      vector<string>names;

     for(int x=0;x<root->children.size();x++){
        if(root->children[x]->data != "\*"){
            temp.push_back(root->children[x]->data);
        }
     }

     for(int x=0;x<temp.size();x++){
        if(count(temp.begin(),temp.end(),temp[x]) > 1  && count(names.begin(),names.end(),temp[x]) == 0){
            names.push_back(temp[x]);
        }
     }

     for(int x=0;x<names.size();x++){
        Node* simp = makeNewNode(names[x]);
        for(int y=0;y<root->children.size();y++){
            if(root->children[y]->data == names[x]){
                root->children[y]->data = '*';
                root->children[y]->parent = simp;
                addChildren(simp,root->children[y]);
                root->children.erase(root->children.begin()+y);
                y--;
            }

        }
        addChildren(root,simp);
     }

     //LevelOrderTraversal(root);

}

void organizeTree(Node* root){

    makeOneNodeForRepeatedChild(root);
    for(int x=0;x<root->children.size();x++){
            organizeTree(root->children[x]);
    }

}
///////////////////////////////////////////////////////////////////////////

void makeQutation(Node* root){

    for(int x=0;x<root->children.size();x++){
        if(root->children[x]->data != "\*"){
            root->children[x]->data = "\"" + root->children[x]->data + "\"";
        }
        makeQutation(root->children[x]);
    }
}

void makeBrackets(Node* root){

    for(int x=0;x<root->children.size();x++){

        bool case2 = (root->children.size() >= 1) && (root->children[0]->data != "\*") && (root->children[x]->children.size() != 0) && (x == root->children.size()-1);

        bool case3 = (root->children.size() > 1) && (root->children[0]->data == "\*") && (x == root->children.size()-1);

        if( case2 ){
            Node* temp = getLastChild(root);
            if(temp->data[temp->data.length()-1] == ']' || (temp->data[temp->data.length()-1] == '}') ){
                int s1 = count(temp->data.begin(),temp->data.end(),']');
                int s2 = count(temp->data.begin(),temp->data.end(),'}');
                if(s1<0){s1=0;}
                if(s2<0){s2=0;}
                int sum = s1+s2;
                temp->data.insert(temp->data.length()-sum,"}");
            }else{
                temp->data = temp->data + "}";
            }
        }else if( case3 ){
            Node* temp = getLastChild(root->children[x]);
            if(temp->data[temp->data.length()-1] == ']' || (temp->data[temp->data.length()-1] == '}') ){
                int s1 = count(temp->data.begin(),temp->data.end(),']');
                int s2 = count(temp->data.begin(),temp->data.end(),'}');
                if(s1<0){s1=0;}
                if(s2<0){s2=0;}
                int sum = s1+s2;
                temp->data.insert(temp->data.length()-sum,"]");
            }else{
                temp->data = temp->data + "]";
            }
        }

       makeBrackets(root->children[x]);
    }
}

void printNode(Node* root){

    //////////////// print * nodes  /////////////////////////////
    if(root->data == "\*" && root->children.size() == 1 && root->children[0]->children.size() == 0){
        return;
    }else if(root->data == "\*" && root->children.size() >= 1){
        //cout<<"{";
        json+="{";
    }

    ////////////// print last nodes /////////////////////////////
    else if(root->children.size() == 0 && (root->data[root->data.length()-1] == '}' || root->data[root->data.length()-1] == ']') ){
        //cout<<root->data;
        json+=root->data+",";
    }else if(root->children.size() == 0){
        //cout<<root->data<<",";
        json+=root->data+",";
    }

    ///////////print tag nodes /////////////////////////////////
    else if(root->children.size() == 1 && root->children[0]->children.size() != 0 && root->data != "\*"){
        //cout<<root->data<<"\:{";
        json+=root->data+"\:{";
    }else if(root->children.size() == 1 && root->data != "\*"){
        //cout<<root->data<<":";
        json+=root->data+":";
    }

    /////////print merged nodes //////////////////////////////
    else if(root->children.size() >0 && root->children[0]->data == "\*"){
        //cout<<root->data<<"\:[";
        json+=root->data+"\:[";
    }else if(root->children.size() > 0 && root->children[0]->data != "\*"){
        //cout<<root->data<<":{";
        json+=root->data+"\:{";
    }

}

void print(Node* root){

    printNode(root);
    for(int x=0;x<root->children.size();x++){
        print(root->children[x]);
    }
}

void makeJson(Node* root){

    root->data = "\"" + root->data + "\"";

    makeQutation(root);

    makeBrackets(root);

    //LevelOrderTraversal(current_root);

    print(root);

    json[json.length()-1] = '}';
}


int main(){

  readFile("simpleText.txt");    //text file was read line by line, stored in lines vector

  getTagsAndLines();      //sort lines according to 1-tags only stored in tags vector, 2-tags and sentences stored in tagsAndLines vector

  makePureTags();         // separate line of tags from extra data ex. ( ahmed id="1" ) ---> ( ahmed ),stored in pureTags vector

  if(checkCorrectXml() == true){  //check if xml file is correct or no
    cout<<"Correct XML"<<"\n";
  }else{
    cout<<"Wrong XML"<<"\n";
  }

  makePureTagsLinesWithoutSlash();               //this vector contain openTags without slash or data, closeTag without slash, data start with ~ sign


  Node* current_root = NULL;

  current_root = makeTree(pureTagsLinesWithoutSlash,current_root);      //make Tree, return main Node

  organizeTree(current_root);                                           //organize tree, as if repeated node get merged

  //LevelOrderTraversal(current_root);

  makeJson(current_root);                       //transfer xml tree to json

  cout<<json;                                   //print json

  return 0;

}
