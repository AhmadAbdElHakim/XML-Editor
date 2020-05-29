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
    tags.clear();tagsAndLines.clear();
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
    pureTags.clear();
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
        pureTagsLinesWithoutSlash.clear();
        for(int x=0;x<tagsAndLines.size();x++){        //make tags without slash,,stored in pureTagsLinesWithoutSlash vector
            if(tagsAndLines[x][0] == '/'){
                pureTagsLinesWithoutSlash.push_back(tagsAndLines[x].substr(1,tagsAndLines[x].length()-1));
            }else{
                if(tagsAndLines[x][0] != '~'){
                    //int spacePlace = tagsAndLines[x].find(' ');
                    pureTagsLinesWithoutSlash.push_back(tagsAndLines[x]);
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

string regulateInternalData(string internalData){
  if(internalData.find('=') != -1){
  string total;
  stringstream check1(internalData);
        string s;
        while(getline(check1, s,' ')){
            stringstream check2(s);
            string l,r;
            getline(check2, l, '=');
            getline(check2, r);
            l = "\"@"+l+"\"\:";
            total+=l+r+",";
        }
  total = total.substr(0,total.length()-2);
  total=total+"\",";
  return total;
  }else{
    return internalData;
  }

}
////////////////////////////////////////////////////////////////////////////////////////

struct Node{
    string data;
    Node* parent;
    vector<Node *> children;
    string internalData;
};

Node* makeNewNode(string data){
    string mainTag,internalData;
    if(data.find('=') == -1){
        Node* newNode = new Node;
        newNode->data = data;
        return newNode;
    }else{
        int index = data.find(' ');
        mainTag = data.substr(0,index);
        internalData = data.substr(index+1,data.length()-1);
        Node* newNode = new Node;
        newNode->data = mainTag;
        newNode->internalData = internalData;
        return newNode;
    }
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
            stringstream check1(pureTagsLinesWithoutSlash[x]);
            string s;
            getline(check1, s, ' ');
            temp.push(s);

            continue;
        }

        if(pureTagsLinesWithoutSlash[x][0] != '~'){
            stringstream check1(pureTagsLinesWithoutSlash[x]);
            string s;
            getline(check1, s, ' ');
             if(s == temp.top()){
                temp.pop();
                current_root = getParent(current_root);
             }else{
                current_root = addChildren(current_root,pureTagsLinesWithoutSlash[x]);
                temp.push(s);
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
        simp->parent = root;
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
    if(root->data == "\*" && root->children.size() == 1 && root->children[0]->children.size() == 0 && root->internalData.empty()){
        return;
    }if(root->data == "\*" && root->children.size() == 1 && root->children[0]->children.size() == 0 && !root->internalData.empty()){
        json+=root->internalData;
    }else if(root->data == "\*" && root->children.size() >= 1 && root->internalData.empty()){
        json+="{";
    }else if(root->data == "\*" && root->children.size() >= 1 && !root->internalData.empty()){
        json+=root->internalData;
    }

    ////////////// print last nodes /////////////////////////////
    else if(root->children.size() == 0 && (root->data[root->data.length()-1] == '}' || root->data[root->data.length()-1] == ']') ){
        //cout<<root->data;
        if(!root->parent->internalData.empty()){
            json+="\"text\"\:"+root->data+",";
        }else{
            json+=root->data+",";
        }
    }else if(root->children.size() == 0){
        //cout<<root->data<<",";
        if(root->parent->children.size() == 1 && root->parent->internalData.empty()){
            json+=root->data+",";
        }else{
            json+=root->data+"},";
        }
    }

    ///////////print tag nodes /////////////////////////////////
    else if(root->children.size() == 1 && root->children[0]->children.size() != 0 && root->data != "\*" && root->internalData.empty() && root->parent != NULL){
        //cout<<root->data<<"\:{";
        json+=root->data+"\:{";
    }else if(root->children.size() == 1 && root->children[0]->children.size() != 0 && root->data != "\*" && !root->internalData.empty() && root->parent != NULL){
        //cout<<root->data<<"\:{";
        json+=root->data+"\:"+root->internalData;
    }else if(root->children.size() == 1 && root->data != "\*" && root->parent != NULL && root->internalData.empty()){
        //cout<<root->data<<":";
        json+=root->data+":";
    }else if(root->children.size() == 1 && root->data != "\*" && root->parent != NULL && !root->internalData.empty()){
        json+=root->data+"\:"+root->internalData;
    }

    /////////print merged nodes //////////////////////////////
    else if(root->children.size() >0 && root->children[0]->data == "\*"){
        //cout<<root->data<<"\:[";
        json+=root->data+"\:[";
    }else if(root->children.size() > 0 && root->children[0]->data != "\*" && root->internalData.empty()){
        //cout<<root->data<<":{";
        json+=root->data+"\:";
    }else if(root->children.size() > 0 && root->children[0]->data != "\*" && !root->internalData.empty()){
        //cout<<root->data<<":{";
        json+=root->data+"\:"+root->internalData;
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

///////////////////////////////Mistake Correct/////////////////////////////////////////////////
std::vector<int> mistakes;
std::vector<string> tagsMC;
std::vector<int> mistakeCase;

void findMistakesLines(){
        mistakes.clear();
        tagsMC.clear();
//////////separate tagName from < , > , id="12"///////////////////
  for(int x=0;x<lines.size();x++){

    if(lines[x].empty()){
        tagsMC.push_back(lines[x]);
        continue;
    }

    int tagCounter = std::count(lines[x].begin(), lines[x].end(), '<');
    int place1 = lines[x].find('<');
    int place2 = lines[x].find('>');

    if(tagCounter == 0){
        tagsMC.push_back("~"+lines[x]);
        continue;
    }

    for(int m=0;m<tagCounter;m++){
        //cout<<lines[x].substr(place1+1,place2-place1-1)<<"\n";
        if(m == 0){
            string s = lines[x].substr(place1+1,place2-place1-1);
            s = s.substr(0,s.find(' '));
            tagsMC.push_back(s);
        }else{
            string s = lines[x].substr(place1+1,place2-place1-1);
            s = s.substr(0,s.find(' '));
            tagsMC.back() = tagsMC.back() + "-" + s;
        }

        int place3 = lines[x].find('<',place1+1);
        int place4 = lines[x].find('>',place2+1);
        place1 = place3;
        place2 = place4;
    }
  }
/////////////////////declare mistakes lines /////////////////////////////

  vector<string> xx;
  vector<int> index;

  for(int x=1;x<tagsMC.size()+1;x++){

    if(tagsMC[x-1].empty()){
        mistakes.push_back(x);
        continue;
    }else if(tagsMC[x-1][0] == '~'){
        continue;
    }

    if(tagsMC[x-1].find('/') == string::npos){
        xx.push_back(tagsMC[x-1]);
        index.push_back(x);
    }else{
    stringstream check1(tagsMC[x-1]);

    string intermediate;

    while(getline(check1, intermediate, '-'))
    {
        if(intermediate.find('/') == string::npos){
            xx.push_back(intermediate);
            index.push_back(x);
        }else{
            string s = intermediate.substr(1,intermediate.length()-1);
            if( xx.back() == s ){
                xx.pop_back();
                index.pop_back();
            }else if( xx[xx.size()-2] == s ){
                mistakes.push_back(index.back());
                mistakeCase.push_back(1);
                xx.pop_back();
                index.pop_back();
                x--;
            }else{
                mistakes.push_back(x);
                mistakeCase.push_back(2);
                xx.pop_back();
                index.pop_back();
            }
        }
    }
   }
  }


  sort(mistakes.begin(), mistakes.end());
/*
  for(int x=0;x<mistakes.size();x++){
    cout<<mistakes[x]<<endl;
  }
*/
}

void correctMistakes(){
  if(mistakeCase.size() > 0){
   for(int x=0;x<mistakes.size();x++){

    if(mistakeCase[x] == 2 && !lines[mistakes[x]-1].empty() ){
        string s;
        stringstream check1( lines[mistakes[x]-1] );
        getline(check1, lines[mistakes[x]-1] , '/');
        string temp = lines[mistakes[x]-1].substr(0,lines[mistakes[x]-1].length()-1);
        stringstream check2( temp );
        getline(check2, s , '>');
        lines[mistakes[x]-1] = temp + "</" + s.substr(1,s.length()-1) +">";

    }else if(mistakeCase[x] == 1 && lines[mistakes[x]-1][lines[mistakes[x]-1].length()-1] != '>'){
        string s;
        stringstream check1( lines[mistakes[x]-1] );
        getline(check1, s , '>');
        lines[mistakes[x]-1] = lines[mistakes[x]-1] + "</" + s.substr(1,s.length()-1) +">";
    }else if(mistakeCase[x] == 1){
        string s;
        stringstream check1( lines[mistakes[x]-1] );
        getline(check1, s , '>');
        for(int y=0;y<lines.size();y++){
            if(lines[y].empty()){
                lines[y] = "</" + s.substr(1,s.length()-1) +">";
                break;
            }
        }
    }

   }

  }

}

string makeSpaces(int counter){

    string s;
    for(int x=0;x<counter*3;x++){
        s+=" ";
    }
    return s;
}

void indentLines(){

    int spaces=0;
  for(int x=0;x<lines.size();x++){
      int counterOpenTag  = count(lines[x].begin(),lines[x].end(),'<');
      int counterCloseTag = count(lines[x].begin(),lines[x].end(),'/');
      if(counterOpenTag == 1 && counterCloseTag == 0){
            lines[x] = makeSpaces(spaces) + lines[x];
            spaces++;
      }else if(counterOpenTag == 1 && counterCloseTag == 1){
            spaces--;
            lines[x] = makeSpaces(spaces) + lines[x];
      }else if(counterOpenTag >1 && counterCloseTag == 1){
            spaces++;
            lines[x] = makeSpaces(spaces) + lines[x];
            spaces--;
      }
  }
}

//////////////////////////search in tree ////////////////////////////////////////////////////
void getDef(Node* root){

    for(int x=0;x<root->children.size();x++){
        if(root->children[x]->data == "def"){
            if(root->children[x]->children.size() == 0){
                cout<<getLastChild(root->children[x])->data;
            }else{
                for(int y=0;y<root->children[x]->children.size();y++){
                    cout<<getLastChild(root->children[x]->children[y])->data<<endl;
                }
            }
        }
    }
}

void findName(Node* root,string name){
    for(int x=0;x<root->children.size();x++){
        if( root->children[0]->data == name ){
            if(root->data == "word"){
                getDef(root->parent);
            }else{
                getDef(root->parent->parent);
                break;
            }
        }else{
            findName(root->children[x],name);
        }
    }
}

extern int synsetCounter=0;
void countSynset(Node* root){

    for(int x=0;x<root->children.size();x++){
        if(root->children[0]->data == "synset"){
            if(root->children[0]->children[0]->data == "\*"){
                synsetCounter+=root->children[0]->children.size();
            }else{
                synsetCounter+=1;
            }
        }
        countSynset(root->children[x]);
    }

}

/////////////////////////////////////// test new json //////////////////////////////////////////////


void regulateHyberTags(Node* root){
    if(!root->internalData.empty()){
        for(int x=0;x<root->internalData.length();x++){
          if(x == 0){
            root->internalData.insert(0,"{\"");
          }
          if(root->internalData[x] == ' ' && root->internalData[x-1] == '"'){
            root->internalData[x]=',';
          }else if(root->internalData[x] == '='){
            root->internalData[x]=':';
          }
          if(root->internalData[x]==','){
            root->internalData.insert(x+1,"\"");
          }else if(root->internalData[x]==':'){
            root->internalData.insert(x,"\"");
            x++;
          }
        }
        root->internalData+=",";
    }



}


void passRoot(Node* root){

    regulateHyberTags(root);
    for(int x=0;x<root->children.size();x++){
        passRoot(root->children[x]);
    }

}





////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(){

  readFile("alaa.txt");    //text file was read line by line, stored in lines vector
/*
  findMistakesLines();          //get mistakes lines and store line has mistake in mistakes vector


  if(mistakes.size() == 0){
    cout<<"Correct XML"<<endl;
  }else{
    cout<<"Wrong XML"<<endl;
    correctMistakes();            //corrected lines stored in lines vector
    findMistakesLines();
    if(mistakes.size() == 0){
        cout<<"Correct XML"<<endl;
    }else{
        cout<<"Wrong XML"<<endl;
    }
  }


*/
  getTagsAndLines();      //sort lines according to 1-tags only stored in tags vector, 2-tags and sentences stored in tagsAndLines vector

  //makePureTags();         // separate line of tags from extra data ex. ( ahmed id="1" ) ---> ( ahmed ),stored in pureTags vector

  makePureTagsLinesWithoutSlash();               //this vector contain openTags without slash or data, closeTag without slash, data start with ~ sign


  Node* current_root = NULL;

  current_root = makeTree(pureTagsLinesWithoutSlash,current_root);      //make Tree, return main Node

  //LevelOrderTraversal(current_root);

  organizeTree(current_root);                                           //organize tree, as if repeated node get merged


  //countSynset(current_root);
  //cout<<synsetCounter;

  //LevelOrderTraversal(current_root->children[0]->children[0]->parent);

  //findName(current_root,"crowned");



    passRoot(current_root);



    //makeQutation(current_root);


  makeJson(current_root);                       //transfer xml tree to json

  cout<<json;                                   //print json


  return 0;

}
