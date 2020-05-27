#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//------------------------Hossam-----------------------//

std::vector <std::string> lines;
std::vector <std::string> tags;
std::vector <std::string> tagsAndLines;
std::vector <std::string> pureTags;
std::vector <std::string> pureTagsLinesWithoutSlash;
std::string json = "{";

QFile mytempfile("out.txt");
QFile myfile("myfile.txt");
void readFile(){

    myfile.open(QIODevice::ReadOnly |QIODevice::Text);
    std::string line;
    lines.resize(0);
    while (!myfile.atEnd())
    { line = (myfile.readLine().trimmed()).toStdString();
      lines.push_back(line);
    }

    myfile.close();

}


void getTagsAndLines(){
    std::vector<std::string> TandL;
    tags.resize(0);
    tagsAndLines.resize(0);
    for(unsigned long long x=0;x<lines.size();x++){

    int tagCounter = std::count(lines[x].begin(), lines[x].end(), '<');
    int place1 = lines[x].find('<');
    int place2 = lines[x].find('>');

    for(int m=0;m<tagCounter;m++){

        tags.push_back(lines[x].substr(place1+1,place2-place1-1));
        TandL.push_back(lines[x].substr(place1+1,place2-place1-1));

        if(lines[x][place2+1] != '<'){
            int temp = lines[x].find('<',place1+1);

            TandL.push_back("~"+lines[x].substr(place2+1,temp-place2-1));
        }

        int place3 = lines[x].find('<',place1+1);
        int place4 = lines[x].find('>',place2+1);
        place1 = place3;
        place2 = place4;
    }
  }
  for(unsigned long long x=0;x<TandL.size();x++){      //to remove empty lines and store tags and lines in tagsAndLines Vector
    if(! ((TandL[x][0] == '~') && (TandL[x].length() == 1)) ){
        tagsAndLines.push_back(TandL[x]);
    }
  }
}
void makePureTags(){
    pureTags.resize(0);
  for(unsigned long long x=0;x<tags.size();x++){
    if(!tags[x].empty()){
        if(tags[x].find(' ') != std::string::npos){
            std::string s = tags[x].substr(0,tags[x].find(' '));
            pureTags.push_back(s);
        }else{
            pureTags.push_back(tags[x]);
        }
    }
  }
}
void makePureTagsLinesWithoutSlash(){
        pureTagsLinesWithoutSlash.resize(0);
        for(unsigned int x=0;x<tagsAndLines.size();x++){        //make tags without slash,,stored in pureTagsLinesWithoutSlash vector
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
}
struct Node{
    std::string data;
    Node* parent;
    std::vector<Node *> children;
};

Node* makeNewNode(std::string data){
    Node* newNode = new Node;
    newNode->data = data;
    return newNode;
}

Node* addChildren(Node* root,std::string data){
    Node* child = makeNewNode(data);
    root->children.push_back(child);
    child->parent=root;
    return child;
}

void addChildren(Node* root,Node* child){
    root->children.push_back(child);
    return;
}

void showChildren(Node* root){
    mytempfile.open(QIODevice::ReadWrite |QIODevice::Text);
    QTextStream stream(&mytempfile);
    int size = root->children.size();
    for(int x=0;x<size;x++){
        stream << QString::fromStdString(root->children[x]->data)<<"\t";
    }
    stream<<"\n";
    mytempfile.close();
    return;
}

Node* getParent(Node* root){

    Node* temp = root;
    if(temp->parent != NULL){
       root = temp->parent;
    }
    return root;
}

void LevelOrderTraversal(Node * root){
QTextStream stream(&mytempfile);
        if (root==NULL)
        return;

    // Standard level order traversal code
    // using queue
    QList<Node *> q;  // Create a queue
    q.push_back(root); // Enqueue root
    while (!q.empty())
    {
        int n = q.size();

        // If this node has children
        while (n > 0)
        {
            // Dequeue an item from queue and print it
            Node * p = q.front();
            q.pop_front();
             stream << QString::fromStdString(p->data) << " ";

            // Enqueue all children of the dequeued item
            for (unsigned long long i=0; i<p->children.size(); i++)
                q.push_back(p->children[i]);
            n--;
        }

        stream << endl; // Print new line between two levels
    }
    return;
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
    else return getLastChild(root->children[root->children.size()-1]);

}
Node* makeTree(std::vector<std::string> pureTagsLinesWithoutSlash,Node* current_root){
  std::stack<std::string> temp;

  for(unsigned int x=0;x<pureTagsLinesWithoutSlash.size();x++){     //makes tree

        if(x==0){
            current_root = makeNewNode(pureTagsLinesWithoutSlash[x]);
            current_root->parent = NULL;
            std::stringstream check1(pureTagsLinesWithoutSlash[x]);
            std::string s;
            getline(check1, s, ' ');
            temp.push(s);

            continue;
        }

        if(pureTagsLinesWithoutSlash[x][0] != '~'){
            std::stringstream check1(pureTagsLinesWithoutSlash[x]);
            std::string s;
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
      std::vector<std::string>temp;
      std::vector<std::string>names;

     for(unsigned int x=0;x<root->children.size();x++){
        if(root->children[x]->data != "*"){
            temp.push_back(root->children[x]->data);
        }
     }

     for(unsigned int x=0;x<temp.size();x++){
        if(count(temp.begin(),temp.end(),temp[x]) > 1  && count(names.begin(),names.end(),temp[x]) == 0){
            names.push_back(temp[x]);
        }
     }

     for(unsigned int x=0;x<names.size();x++){
        Node* simp = makeNewNode(names[x]);
        for(unsigned int y=0;y<root->children.size();y++){
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
return;
}

void organizeTree(Node* root){
if(root==NULL){return ;}
    makeOneNodeForRepeatedChild(root);
    for(unsigned int x=0;x<root->children.size();x++){
            organizeTree(root->children[x]);
    }
return;
}

///////////////////////////////////////////////////////////////////////////

void makeQutation(Node* root){

    for(unsigned int x=0;x<root->children.size();x++){
        if(root->children[x]->data != "*"){
            root->children[x]->data = "\"" + root->children[x]->data + "\"";
        }
        makeQutation(root->children[x]);
    }
}

void makeBrackets(Node* root){

    for(unsigned int x=0;x<root->children.size();x++){

        bool case2 = (root->children.size() >= 1) && (root->children[0]->data != "*") && (root->children[x]->children.size() != 0) && (x == root->children.size()-1);

        bool case3 = (root->children.size() > 1) && (root->children[0]->data == "*") && (x == root->children.size()-1);

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

    if((root->data == "*") && (root->children.size() == 1) && (root->children[0]->children.size() == 0))
    {return;}

    else if(root->data == "*" && root->children.size() >= 1)
    {json+="{";return;}

    ////////////// print last nodes /////////////////////////////

    else if(root->children.size() == 0 && (root->data[root->data.length()-1] == '}' || root->data[root->data.length()-1] == ']') )
    {json+=root->data+",";}

    else if(root->children.size() == 0)
    {json+=root->data+",";}

    //////////////print tag nodes /////////////////////////////////

    else if(root->children.size() == 1 && root->children[0]->children.size() != 0 && root->data != "*" && root->data.find('=') == -1)
    {json+=root->data+":{";}

    else if(root->children.size() == 1 && root->children[0]->children.size() != 0 && root->data != "*" && root->data.find('=') != -1)
    {
        std::stringstream check1(root->data);
        std::string f,s;
        std::string total;
        getline(check1, f, ' ');
        while(getline(check1, s,' ')){
            std::stringstream check2(s);
            std::string l,r;
            getline(check2, l, '=');
            getline(check2, r);
            l = "\"@" +l+ "\"\:";
            total+=l+r+",";
            }
        total = total.substr(0,total.length()-2);
        json+=f+"\"\:{"+total+",";
    }

    else if(root->children.size() == 1 && root->data != "*")
    {json+=root->data+":";}

    /////////print merged nodes //////////////////////////////

    else if(root->children.size() >0 && root->children[0]->data == "*")
    {json+=root->data+"\:[";}

    else if(root->children.size() > 0 && root->children[0]->data != "*" && root->data.find('=') == -1)
    {json+=root->data+"\:{";}

    else if(root->children.size() > 0 && root->children[0]->data != "*" && root->data.find('=') != -1)
    {
        std::stringstream check1(root->data);
        std::string f,s;
        std::string total;
        getline(check1, f, ' ');
        while(getline(check1, s,' ')){
            std::stringstream check2(s);
            std::string l,r;
            getline(check2, l, '=');
            getline(check2, r);
            l = "\"@"+l+"\"\:";
            total+=l+r+",";
        }
        total = total.substr(0,total.length()-2);
        json+=f+"\"\:{"+total+",";
    }

}

void print(Node* root){

    printNode(root);
    for(unsigned int x=0;x<root->children.size();x++){
        print(root->children[x]);
    }
    return;
}

void makeJson(Node* root){

    root->data = "\"" + root->data + "\"";

    makeQutation(root);

    makeBrackets(root);

    print(root);

    json[json.length()-1] = '}';
    return;
}

void MainWindow::on_JSON_Button_clicked()
{    mytempfile.resize(0);
     mytempfile.open(QIODevice::ReadWrite |QIODevice::Text);
     QTextStream st(& mytempfile);

      readFile();    //text file was read line by line, stored in lines vector

      getTagsAndLines();      //sort lines according to 1-tags only stored in tags vector, 2-tags and sentences stored in tagsAndLines vector

      makePureTags();         // separate line of tags from extra data ex. ( ahmed id="1" ) ---> ( ahmed ),stored in pureTags vector

      makePureTagsLinesWithoutSlash();               //this vector contain openTags without slash or data, closeTag without slash, data start with ~ sign

      Node* current_root = NULL;

      current_root = makeTree(pureTagsLinesWithoutSlash,current_root);      //make Tree, return main Node

      organizeTree(current_root);                                           //organize tree, as if repeated node get merged

      makeJson(current_root);                       //transfer xml tree to json

      st<<QString::fromStdString(json);         //print json
      mytempfile.close();
      mytempfile.open(QIODevice::ReadWrite |QIODevice::Text);
      QTextStream strq(&mytempfile);
      ui->output_text->setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
      ui->output_text->clear();
      QString out=strq.readAll();
      QChar prevchar;
       int level=0;
       for(auto i:out)
      {
          if(i=='{'){
              ui->output_text->insertPlainText(i);
              ui->output_text->insertPlainText("\n");
              for(int j=-1;j<level;j++){ui->output_text->insertPlainText("    ");}
              level=level+1;
          }
          else if(i=='}'){

              ui->output_text->insertPlainText("\n");
              for(int j=0;j<level;j++){ui->output_text->insertPlainText("    ");}
              ui->output_text->insertPlainText(i);
              level=level-1;}
          else if(i=='['){
              ui->output_text->insertPlainText(i);
              ui->output_text->insertPlainText("\n");
              for(int j=-1;j<level;j++){ui->output_text->insertPlainText("    ");}
              level=level+1;}
          else if(i==']'){

              ui->output_text->insertPlainText("\n");
              for(int j=0;j<level;j++){ui->output_text->insertPlainText("    ");}
               ui->output_text->insertPlainText(i);
               level=level-1;}
          else if(i==',')
          {
              ui->output_text->insertPlainText(i);
              ui->output_text->insertPlainText("\n");
              for(int j=0;j<level;j++){ui->output_text->insertPlainText("    ");}
          }
          else {ui->output_text->insertPlainText(i);}
          prevchar=i;
      }
      mytempfile.close();
      json = "{";
      lines.resize(0);
       tags.resize(0);
       tagsAndLines.resize(0);
       pureTags.resize(0);
      pureTagsLinesWithoutSlash.resize(0);
       return;
}

std::vector<unsigned int> mistakes;
std::vector<std::string> tagsMC;
std::vector<int> mistakeCase;

void findMistakesLines(){
        mistakes.resize(0);
        tagsMC.resize(0);
        mistakeCase.resize(0);
//////////separate tagName from < , > , id="12"///////////////////
  for(unsigned int x=0;x<lines.size();x++){

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
            std::string s = lines[x].substr(place1+1,place2-place1-1);
            s = s.substr(0,s.find(' '));
            tagsMC.push_back(s);
        }else{
            std::string s = lines[x].substr(place1+1,place2-place1-1);
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

  std::vector<std::string> xx;
  std::vector<int> index;

  for(unsigned int x=1;x<tagsMC.size()+1;x++){

    if(tagsMC[x-1].empty()){
        mistakes.push_back(x);
        continue;
    }else if(tagsMC[x-1][0] == '~'){
        continue;
    }

    if(tagsMC[x-1].find('/') == std::string::npos){
        xx.push_back(tagsMC[x-1]);
        index.push_back(x);
    }else{
    std::stringstream check1(tagsMC[x-1]);

    std::string intermediate;

    while(getline(check1, intermediate, '-'))
    {
        if(intermediate.find('/') == std::string::npos){
            xx.push_back(intermediate);
            index.push_back(x);
        }else{
            std::string s = intermediate.substr(1,intermediate.length()-1);
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
return;
}

void correctMistakes(){
  if(mistakeCase.size() > 0){
   for(unsigned int x=0;x<mistakes.size();x++){

    if(mistakeCase[x] == 2){
        std::string s;
        std::stringstream check1( lines[mistakes[x]-1] );
        getline(check1, lines[mistakes[x]-1] , '/');
        std::string temp = lines[mistakes[x]-1].substr(0,lines[mistakes[x]-1].length()-1);
        std::stringstream check2( temp );
        getline(check2, s , '>');
        lines[mistakes[x]-1] = temp + "</" + s.substr(1,s.length()-1) +">";

    }else if(mistakeCase[x] == 1 && lines[mistakes[x]-1][lines[mistakes[x]-1].length()-1] != '>'){
        std::string s;
        std::stringstream check1( lines[mistakes[x]-1] );
        getline(check1, s , '>');
        lines[mistakes[x]-1] = lines[mistakes[x]-1] + "</" + s.substr(1,s.length()-1) +">";
    }else if(mistakeCase[x] == 1){
        std::string s;
        std::stringstream check1( lines[mistakes[x]-1] );
        getline(check1, s , '>');
        for(unsigned int y=0;y<lines.size();y++){
            if(lines[y].empty()){
                lines[y] = "</" + s.substr(1,s.length()-1) +">";
            }
        }
    }

   }
  }
  return;
}

//-----------------------Alaa--------------------------//
void makef(QFile *input,QFile *output){
    (*input).open(QIODevice::ReadWrite |QIODevice::Text);
    (*output).open(QIODevice::ReadWrite |QIODevice::Text);
    QTextStream strm(output);
    QString line;
    QChar temp;

    while (!myfile.atEnd())
    { line = myfile.readLine();

        for (auto i:line)
        {
           if(i=='\n'||i=='\t'){temp=i;continue;}
           if((i=='<')&&(temp=='\n')){strm<<i;temp=i;continue;}
           if((i=='<')&&(temp=='>')){strm<<'\n'<<i;temp=i;continue;}
           if(i=='>'){strm<<i<<'\n';temp=i;continue;}
           strm<<i;
        }
    }

    (*input).close();
    (*output).close();
}
int classify_word(QString word) {//1:opening 2:closing 3:value  4:lone tag  5:comment  6:prolog
   if(word.isEmpty()){return 0;}
    if((word[0]=='<')&&word[word.length()-1]=='>')
    {
       if(word[1]=='/'){return 2;}
       if(word[word.length()-2]=='/'){return 4;}
       if(word[1]=='?'){return 6;}
       if(word[1]=='!'){return 5;}
       return 1;
    }
    return 3;
}

void MainWindow::on_Prettify_Button_clicked()
{
    QTextCursor cursor( ui->output_text->textCursor() );
    QTextCharFormat format;
    format.setFontWeight( QFont::TypeWriter );



       ui->output_text->clear();
       ui->output_text->setLineWrapMode(QPlainTextEdit::NoWrap);
       QFile tagsfile("mytags.txt");
        tagsfile.resize(0);
          makef(&myfile,&tagsfile);
        mytempfile.resize(0);
        mytempfile.open(QIODevice::ReadWrite |QIODevice::Text);
        QTextStream str(&mytempfile);
        tagsfile.open(QIODevice::ReadOnly |QIODevice::Text);

             QString word,wordpre;
             int level = 0;
             int x,xpre;
             int q=ui->input_text->blockCount();
             if(q<8000){
             while (!tagsfile.atEnd())
          {
            format.setForeground( QBrush( QColor(Qt::blue) ) );
             word = tagsfile.readLine().trimmed();
             if(word.isEmpty()){continue;}
             x=classify_word(word);
             xpre=classify_word(wordpre);

          //opening
              if(x==1){
                  if(xpre==0)
                  {
                  cursor.setCharFormat( format );
                  cursor.insertText( word );
                  level=level+1;
                  }
                  else if(xpre==1)
                  {
                  cursor.insertText("\n");
                  for(int i=0;i<level;i++){cursor.insertText("  ");}
                  cursor.setCharFormat( format );
                  cursor.insertText( word );
                  level=level+1;
                  }
                  else
                  {
                  cursor.insertText("\n");
                  for(int i=0;i<level;i++){cursor.insertText("  ");}
                  cursor.setCharFormat( format );
                  cursor.insertText( word );
                  level=level+1;
                  }
              }
          //closing
              else if (x==2){
                  if(xpre==1)
                  {
                      cursor.setCharFormat( format );
                      cursor.insertText( word );
                      level=level-1;
                  }
                  if(xpre==2)
                  {
                      level=level-1;
                      cursor.insertText("\n");
                      for(int i=0;i<level;i++){cursor.insertText("  ");}
                      cursor.setCharFormat( format );
                      cursor.insertText( word );
                  }
                  else
                  {

                      cursor.setCharFormat( format );
                      cursor.insertText( word );
                      level=level-1;
                  }
              }
          //value
              else if(x==3){

                  format.setForeground( QBrush( QColor(Qt::black) ) );
                  cursor.setCharFormat(format);
                  cursor.insertText( word );
               }
          //lone tag
              else if(x==4){

                  cursor.insertText("\n");
                  for(int i=0;i<level;i++){cursor.insertText("  ");}
                  format.setForeground( QBrush( QColor(Qt::darkMagenta) ) );
                  cursor.setCharFormat( format );
                  cursor.insertText( word );
              }
          //comment or prolog
              else {
                  cursor.insertText("\n");
                  format.setForeground( QBrush( QColor(Qt::darkGreen) ) );
                  cursor.setCharFormat( format );
                  cursor.insertText( word );
              }
          wordpre=word;
             }
             cursor.~QTextCursor();
             format.setForeground( QBrush( QColor(Qt::black) ) );
             ui->output_text->setCurrentCharFormat(format);
             }

         else{
                format.setForeground( QBrush( QColor(Qt::black) ) );
                 while (!tagsfile.atEnd())
                           {

                     word = tagsfile.readLine().trimmed();
                     if(word.isEmpty()){continue;}
                     x=classify_word(word);
                     xpre=classify_word(wordpre);

                           //opening
                               if(x==1){
                                   if(xpre==0)
                                   {
                                   str<<word;
                                   level=level+1;
                                   }
                                   else if(xpre==1)
                                   {
                                   str<<"\n";
                                   for(int i=0;i<level;i++){str<<"  ";}
                                   str<<word;
                                   level=level+1;
                                   }
                                   else
                                   {
                                   str<<"\n";
                                   for(int i=0;i<level;i++){str<<"  ";}
                                   str<<word;
                                   level=level+1;
                                   }
                               }
                           //closing
                               else if (x==2){
                                   if(xpre==1)
                                   {
                                       str<<word;
                                       level=level-1;
                                   }
                                   if(xpre==2)
                                   {
                                       level=level-1;
                                       str<<"\n";
                                       for(int i=0;i<level;i++){str<<"  ";}
                                       str<<word;
                                   }
                                   else
                                   {
                                       str<<word;
                                       level=level-1;
                                   }
                               }
                           //value
                               else if(x==3){
                                   str<<word;
                                }
                           //lone tag
                               else if(x==4){
                                   str<<"\n";
                                   for(int i=0;i<level;i++){str<<"  ";}
                                   str<<word;
                               }
                           //comment or prolog
                               else {
                                   str<<"\n";
                                   str<<word;
                               }
                           wordpre=word;
                           }
                 mytempfile.close();
                    mytempfile.open(QIODevice::ReadWrite |QIODevice::Text);
                    QTextStream strq(&mytempfile);
                    ui->output_text->clear();
                    QString t=strq.readAll();
                    ui->output_text->setPlainText(t);

         }

             tagsfile.close();
             mytempfile.close();

}



void MainWindow::on_OpenFileButton_clicked()
{
    ui->input_text->clear();
    QFile input_file(QFileDialog::getOpenFileName(this,tr("Open File"),"",tr("XML File (*.xml) ;;TextFile (*.txt)")));
    input_file.open(QIODevice::ReadOnly |QIODevice::Text);
    QTextStream stream(&input_file);
    QString text= stream.readAll();
    myfile.remove();
    mytempfile.resize(0);
    input_file.copy("myfile.txt");
    QFile myfile("myfile.txt");
    ui->input_text->setPlainText(text);
    ui->input_text->setLineWrapMode(QPlainTextEdit::NoWrap);
   input_file.close();
}


void MainWindow::on_Save_Button_clicked()
{
 QFile output_file(QFileDialog::getSaveFileName(this,tr("Save File"),"",tr("Text File ()*.txt;;XML File ()*.xml")));
 output_file.open(QIODevice::ReadWrite|QIODevice::Text);
 QString text=ui->output_text->toPlainText();
     output_file.write(text.toUtf8());
     output_file.close();
}



void MainWindow::on_Remove_Spaces_clicked()
{
    ui->output_text->clear();
    ui->output_text->setLineWrapMode(QPlainTextEdit::LineWrapMode::WidgetWidth);
          QFile tagsfile("mytags.txt");
          tagsfile.resize(0);
          mytempfile.resize(0);
             makef(&myfile,&tagsfile);


             tagsfile.open(QIODevice::ReadWrite |QIODevice::Text);
             mytempfile.open(QIODevice::ReadWrite |QIODevice::Text);
             QTextStream str(&mytempfile);
             QString word;

             while (!tagsfile.atEnd())
             { word = tagsfile.readLine().trimmed();
                if(word.isEmpty()){continue;}
                  str<<word;
             }
             mytempfile.close();
             mytempfile.open(QIODevice::ReadWrite |QIODevice::Text);
             QTextStream strq(&mytempfile);
             ui->output_text->setPlainText(strq.readAll());
             mytempfile.close();
             tagsfile.close();

}

void MainWindow::on_Check_Button_clicked()
{   lines.resize(0);
    ui->output_text->clear();
    std::string line;
    QTextCharFormat format;
    QTextCursor cursor( ui->output_text->textCursor() );
     readFile();    //text file was read line by line, stored in lines vector

     findMistakesLines();          //get mistakes lines and store line has mistake in mistakes vector

     if(mistakes.size() == 0)
     {
      QMessageBox enteredString;
      enteredString.setText("Correct XML File");
      enteredString.exec();
     }
     else
     {int j =0;
       for (unsigned int i=1;i<lines.size()+1;i++)
       {line=lines[i-1];
           if(i == mistakes[j])
           {
               format.setFontWeight( QFont::TypeWriter );
               format.setForeground( QBrush( QColor(Qt::red) ) );
               cursor.setCharFormat( format );
               cursor.insertText(QString::fromStdString(line));
               if(cursor.PreviousCharacter != '\n'){cursor.insertText("\n");}
               j++;
           }
           else
           {
               format.setFontWeight( QFont::TypeWriter );
               format.setForeground( QBrush( QColor(Qt::black) ) );
               cursor.setCharFormat( format );
               cursor.insertText(QString::fromStdString(line));
               if(cursor.PreviousCharacter != '\n'){cursor.insertText("\n");}
           }
       }
     }

return;
}
void MainWindow::on_Correct_Button_clicked()
{
        correctMistakes();      //corrected lines stored in lines vector
        ui->output_text->clear();
        std::string line;
        QTextCharFormat format;
        QTextCursor cursor( ui->output_text->textCursor() );
           int j =0;
           for (unsigned int i=1;i<lines.size()+1;i++)
           {line=lines[i-1];
               if(i == mistakes[j])
               {
                   format.setFontWeight( QFont::TypeWriter );
                   format.setForeground( QBrush( QColor(Qt::darkGreen) ) );
                   cursor.setCharFormat( format );
                   cursor.insertText(QString::fromStdString(line));
                   cursor.insertText("\n");
                   j++;
               }
               else
               {
                   format.setFontWeight( QFont::TypeWriter );
                   format.setForeground( QBrush( QColor(Qt::black) ) );
                   cursor.setCharFormat( format );
                   cursor.insertText(QString::fromStdString(line));
                   cursor.insertText("\n");
               }
           }
         }
void MainWindow::on_Reset_button_clicked()
{
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void MainWindow::on_Exit_Button_clicked()
{
    qApp->quit();
}

