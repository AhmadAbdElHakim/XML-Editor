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
int classify_word(QString word) {//1:opening 2:closing 3:value
    return (word[0] == '<') && (word[word.length() - 1] == '>') ? (word[1] == '/') ? 2 : 1 : 3;
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



void MainWindow::on_pushButton_clicked()
{
    QString text=ui->input_text->toPlainText();

         QString temp = "";
         QString temp2 = "";
         int x;
         int level =0;

         ui->output_text->setLineWrapMode(QPlainTextEdit::NoWrap);

            for (auto i : text) {

                 if ((i == '>') && (temp.length() > 1))
                 {
                     temp = temp + i; temp2 = temp; temp = "";
                     x=classify_word(temp2);                     {
                      if(x==1){//opening
                        for(int j=0;j<level;j++){ui->output_text->insertPlainText("   ");}
                        ui->output_text->moveCursor(QTextCursor::NextRow);
                        ui->output_text->insertPlainText(temp2);
                        level++;
                        }

                     else if (x==2){//closing
                          level--;
                        ui->output_text->insertPlainText(temp2);
                        ui->output_text->moveCursor(QTextCursor::NextRow);}
                     else if(x==3){//value
                        ui->output_text->moveCursor(QTextCursor::PreviousRow);
                        ui->output_text->insertPlainText(temp2);}

                     }
                 }

                 else if ((i == '<') && (temp.length() > 1))
                 {
                     temp2 = temp; temp = ""; temp = temp + i;
                     x=classify_word(temp2);                     {
                         if(x==1){//opening
                           for(int j=0;j<level;j++){ui->output_text->insertPlainText("   ");}
                           ui->output_text->moveCursor(QTextCursor::NextRow);
                           ui->output_text->insertPlainText(temp2);
                           level++;
                           }

                        else if (x==2){//closing
                             level--;
                           ui->output_text->insertPlainText(temp2);
                           ui->output_text->moveCursor(QTextCursor::NextRow);}
                        else if(x==3){//value
                           ui->output_text->moveCursor(QTextCursor::PreviousRow);
                           ui->output_text->insertPlainText(temp2);}


                     }
                 }

                 else temp = temp + i;

             }

}
