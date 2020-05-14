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
    QFile input_file(QFileDialog::getOpenFileName(this,tr("Open File"),"",tr("XML File (*.XML) ;;TextFile (*.txt)")));
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
