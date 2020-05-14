#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

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
int classify_word(QString word) {//1:opening 2:closing 3:value  4:lone tag  5:comment  6:prolog
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

void MainWindow::on_OpenFileButton_clicked()
{
    //For Linux
    QFile input_file(QFileDialog::getOpenFileName(this,tr("Open File"),"",tr("XML File (*.xml) ;;TextFile (*.txt)")));
    //QFile input_file(QFileDialog::getOpenFileName(this,tr("Open File"),"",tr("XML File (*.XML) ;;TextFile (*.txt)")));
    input_file.open(QIODevice::ReadOnly |QIODevice::Text);
    QTextStream stream(&input_file);
    QString text= stream.readAll();
    ui->input_text->setPlainText(text);
    ui->input_text->setLineWrapMode(QPlainTextEdit::NoWrap);
    input_file.close();
}

void MainWindow::on_Prettify_Button_clicked()
{
    QString text=ui->input_text->toPlainText();

         QString temp = "";
         QString temp2 = "";
         int x;
         int flag = 0;
         int level = 0;

         ui->output_text->setLineWrapMode(QPlainTextEdit::NoWrap);

            for (auto i : text) {

                 if ((i == '>') && (temp.length() > 1))
                 {
                     temp = temp + i; temp2 = temp; temp = "";
                     x=classify_word(temp2);                     {
                      if(x==1){//opening
                        ui->output_text->moveCursor(QTextCursor::NextRow);
                        for(int j=0;j<level;j++){ui->output_text->insertPlainText(" ");}

                        ui->output_text->insertPlainText(temp2);
                        level++;
                        }

                     else if (x==2){//closing
                          if (flag==1){
                        ui->output_text->insertPlainText(temp2);
                        ui->output_text->moveCursor(QTextCursor::NextRow);
                        level--;
                        flag=0;
                          }
                      else if(flag==0) {
                              for(int j=0;j<level;j++){ui->output_text->insertPlainText(" ");}
                              ui->output_text->insertPlainText(temp2);
                              ui->output_text->moveCursor(QTextCursor::NextRow);
                              level--;
                          }
                      }
                     else if(x==3){//value
                        ui->output_text->moveCursor(QTextCursor::PreviousRow);
                        ui->output_text->insertPlainText(temp2);
                        flag=1;}
                      else if(x==4){for(int j=0;j<level;j++){ui->output_text->insertPlainText(" ");}
                          ui->output_text->insertPlainText(temp2);}

                     }
                 }

                 else if ((i == '<') && (temp.length() > 1))
                 {
                     temp2 = temp; temp = ""; temp = temp + i;
                     x=classify_word(temp2);                     {
                         if(x==1){//opening
                           ui->output_text->moveCursor(QTextCursor::NextRow);
                           for(int j=0;j<level;j++){ui->output_text->insertPlainText(" ");}

                           ui->output_text->insertPlainText(temp2);
                           level++;
                           }

                         else if (x==2){//closing

                              if (flag==1){
                            ui->output_text->insertPlainText(temp2);
                            ui->output_text->moveCursor(QTextCursor::NextRow);
                            level--;
                            flag=0;
                            }
                          else if (flag==0) {
                                  for(int j=0;j<level;j++){ui->output_text->insertPlainText(" ");}
                                  ui->output_text->insertPlainText(temp2);
                                  ui->output_text->moveCursor(QTextCursor::NextRow);
                                  level--;
                               }
                          }
                         else if(x==3){//value
                            ui->output_text->moveCursor(QTextCursor::PreviousRow);
                            ui->output_text->insertPlainText(temp2);
                            flag=1;}
                         else if(x==4){for(int j=0;j<level;j++){ui->output_text->insertPlainText(" ");}
                           ui->output_text->insertPlainText(temp2);}


                     }
                 }

                 else temp = temp + i;

             }

}

void MainWindow::on_Save_Button_clicked()
{
 QFile output_file(QFileDialog::getSaveFileName(this,tr("Save File"),"",tr("Text File ()*.txt")));
 output_file.open(QIODevice::ReadWrite|QIODevice::Text);
 QString text=ui->output_text->toPlainText();
     output_file.write(text.toUtf8());
     output_file.close();
}

std::vector <string> lines;
std::vector <string> tags;
std::vector <string> tagsAndLines;
std::vector <string> pureTags;

void getTagsAndLines(){
for(unsigned int x=0;x<lines.size();x++){

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
  for(unsigned int x=0;x<tags.size();x++){
    if(!tags[x].empty()){
        if(tags[x].find(' ') != string::npos){
            string s = tags[x].substr(0,tags[x].find(' '));
            pureTags.push_back(s);
        }else{
            pureTags.push_back(tags[x]);
        }
    }
  }
}

void MainWindow::on_check_button_clicked()
{
    stack<string> tagChecker;

    for(unsigned int x=0;x<pureTags.size();x++){
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
    if(tagChecker.size() == 0)
    {
        QMessageBox enteredStirng;
        enteredStirng.setText("Correct XML");
        enteredStirng.exec();
    }
    else
    {
        QMessageBox enteredStirng;
        enteredStirng.setText("Wrong XML");
        enteredStirng.exec();
    }
}
