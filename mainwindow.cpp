#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>
#include <QVBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QtCore>
#include <QInputDialog>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix(":/img/converter.png");
    ui->label_4->setPixmap(pix.scaled(300,300,Qt::KeepAspectRatio));
}

QString Shift(QString data, int num)
{
    if(data.toInt()>=(num-5000))   //x>5000, y=x-10000; x>15000, y=x-20000
    {
        int temp=data.toInt()-num;
        data = QString::number(temp);
    }
    return data;
}


static QString Writefile(QString Input, QString Output){
    QFile data(Output);
    if ((!data.open(QFile::WriteOnly | QFile::Text))|(!Output.endsWith(".csv")))
    {
        return "Write failed!";
    }
    QTextStream out(&data);
    out << Input;
    data.flush();
    data.close();
    return "Write Suceess!";
}

MainWindow::~MainWindow()
{
    delete ui;

}
void MainWindow::on_pushButton_clicked()
{
    QString type_of_mech = ui->comboBox->currentText();
    QString InputFile =  ui->lineEdit_2->text();
    QString OutputFile =  ui->lineEdit->text();
    QString out;
    QString type = type_of_mech;
    QFile inputfile(InputFile);
    if (!inputfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        inputfile.close();
        out = "failed";
    }
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(1);
    QTextStream in(&inputfile);
    in.setCodec("UTF-8");

    QString line = in.readLine();
    out = "Time,";  // First write time

    //==============================
    //=======Choose Type============
    int number = 50;        //type-corresonding parameter number
    if(type == "COP65")  //load in COP65 parameters
    {
        number = 13;
        out.append("Group1,ELWT,EEWT,CLWT,CEWT,SuctP,DishP,OilP,DiscT,RLA,Group2,LqdLevel,EEV \n");
    }
    if(type == "WMT")
    {
        number = 15;
        out.append("Group1,ACTSetpiont,EvapLWT,EvapEWT,CondLWT,CondEWT,LiqLevel,EXV,CondP,EvapP,"
                   "Group2,LiqLineT,SuctP,DiscP,SuctT,DiscT,IGV,MBCSpd,VFDSpd,OptCurrent,"
                   "Group3,EconP,ActMinSpd,GapT,StatorT1,StatorT2,StatorT3,StatorT4,StatorT5,StatorT6,\n");
    }
    if(type == "F7")  //load in F7 parameters
    {
        number = 17;
        out.append("Group1,ELWT,EEWT,CLWT,CEWT,SuctP,DishP,1#OilP,2#OilP,1#DiscT,"
                   "Group2,2#DiscT,1#RLA,2#RLA,LqdLevel,EEV,EEV1 \n");
    }
    if(type == "WTC")  //load in WTC parameters
    {
        number = 20;
        out.append("EvapLWT,EvapEWT,CondLWT,CondEWT,OilSumpTemp,OilFeedTemp,SuctTemp,DischTemp,PctRLA,VfdSpeed,"
                   "OilNetPress,IGV_Pos,DDC_Pos,EconPress,KiloWatts,CondPress,EvapPress,EXV%PosCmd,LiqLineTemp,LiquidLevel \n");
    }
    if(type == "WMEGEN2-2")  //load in WMEGEN2-2 parameters
    {
        number = 7;
        out.append("Group1,ActiveSp,ELWT,EEWT,CLWT,CEWT,LiqLevel,EXV,CondP,LiquidT,"
                   "Group2,DischP,SuctP,DischT,SuctT,IGV,SpeedCmd,VFDSpd,MBCSpd,CalMinSpd,"
                   "Group3,StatorT1,StatorT2,StatorT3,MtrRotorT,MtrCaseT,MtrGapT,RLA,OutCurrent,OutCurrent2,"
                   "Group4,MBCFaultMap1,MBCFaultMap2,MBCFaultMap3,MBCFaultMap4,MBCFaultMap5,MBCFaultMap6,MBCFaultMap7,MBCFaultMap8, ,"
                   "Group5,DischP2,SuctP2,DischT2,SuctT2,IGV2,SpeedCmd2,VFDSpd2,MBCSpd2,CalMinSpd2,"
                   "Group6,StatorT1_2,StatorT2_2,StatorT3_2,MtrRotorT2,MtrCaseT2,MtrGapT2,RLA2,OutCurrent2,OutCurrent2_2,"
                   "Group7,MBCFaultMap1_2,MBCFaultMap2_2,MBCFaultMap3_2,MBCFaultMap4_2,MBCFaultMap5_2,MBCFaultMap6_2,MBCFaultMap7_2,MBCFaultMap8_2, ,\n");
    }
    if(type == "WMEGEN2-1")  //load in WMEGEN2-1 parameters
    {
        number = 6;
        out.append("Group1,ActiveSp,ELWT,EEWT,CLWT,CEWT,LiqLevel,EXV,CondP,LiquidT,"
                   "Group2,DischP,SuctP,DischT,SuctT,IGV,SpeedCmd,VFDSpd,MBCSpd,CalMinSpd,"
                   "Group3,StatorT1,StatorT2,StatorT3,MtrRotorT,MtrCaseT,MtrGapT,RLA,OutCurrent,OutCurrent2,"
                   "Group4,MBCFaultMap1,MBCFaultMap2,MBCFaultMap3,MBCFaultMap4,MBCFaultMap5,MBCFaultMap6,MBCFaultMap7,MBCFaultMap8, ,\n");
    }
    if(type == "WXE-1")  //load in WXE parameters
    {
        number = 2;
        out.append("Unit,Actsetpoint,ELWT,EEWT,CLWT,CEWT,LqdLevel,EXV,EconT,EconP,"
                   "1#Comp,SucT1,DiscT1,SuctP1,DiscP1,RLA1,IGV1,ActRPM1,MaxRPM1,MinRPM1,\n");
    }
    if(type == "WXE-2")  //load in WXE parameters
    {
        number = 3;
        out.append("Unit,Actsetpoint,ELWT,EEWT,CLWT,CEWT,LqdLevel,EXV,EconT,EconP,"
                   "1#Comp,SucT1,DiscT1,SuctP1,DiscP1,RLA1,IGV1,ActRPM1,MaxRPM1,MinRPM1,"
                   "2#Comp,SucT2,DiscT2,SuctP2,DiscP2,RLA2,IGV2,ActRPM2,MaxRPM2,MinRPM2,"
                   "\n");
    }
    if(type == "WXE-3")  //load in WXE parameters
    {
        number = 4;
        out.append("Unit,Actsetpoint,ELWT,EEWT,CLWT,CEWT,LqdLevel,EXV,EconT,EconP,"
                   "1#Comp,SucT1,DiscT1,SuctP1,DiscP1,RLA1,IGV1,ActRPM1,MaxRPM1,MinRPM1,"
                   "2#Comp,SucT2,DiscT2,SuctP2,DiscP2,RLA2,IGV2,ActRPM2,MaxRPM2,MinRPM2,"
                   "3#Comp,SucT3,DiscT3,SuctP3,DiscP3,RLA3,IGV3,ActRPM3,MaxRPM3,MinRPM3,"
                   "\n");
    }
    if(type == "WXE-4")  //load in WXE parameters
    {
        number = 5;
        out.append("Unit,Actsetpoint,ELWT,EEWT,CLWT,CEWT,LqdLevel,EXV,EconT,EconP,"
                   "1#Comp,SucT1,DiscT1,SuctP1,DiscP1,RLA1,IGV1,ActRPM1,MaxRPM1,MinRPM1,"
                   "2#Comp,SucT2,DiscT2,SuctP2,DiscP2,RLA2,IGV2,ActRPM2,MaxRPM2,MinRPM2,"
                   "3#Comp,SucT3,DiscT3,SuctP3,DiscP3,RLA3,IGV3,ActRPM3,MaxRPM3,MinRPM3,"
                   "4#Comp,SucT4,DiscT4,SuctP4,DiscP4,RLA4,IGV4,ActRPM4,MaxRPM4,MinRPM4 \n");
    }
    if(type == "WSC/WCC")
    {
        number = 30;
        out.append("CondEWT,CondLWT,EvapEWT,EvapLWT,"
                   "LiqLineTemp1,LiqLineTemp2,LiquidLevel1,LiquidLevel2,EXV%PosCmd1,EXV%PosCmd2,PctRLA1,SuctTemp1,DischTemp1,EvapPress1,CondPress1,OilFeedTemp1,OilSumpTemp1,OilSumpPress1,"
                   "PctRLA2,SuctTemp2,DischTemp2,EvapPress2,CondPress2,OilFeedTemp2,OilSumpTemp2,OilSumpPress2,NetOilPress1,NetOilPress2,IGV1,IGV2\n");
    }
    //==============================
    QString time;         // define the time mark arrays
    QString data;       // define the data arrays
    QStringList lineout;
    QString med;        //a medium between lineout and out
    int i=1;    // marker for data acquriment.
    int groupnum = 0; // group number marker.
    int x = 0;      //marker for data position in the list. when x = 9, data10 will be read.




    line = in.readLine();    //2nd read line
    int sumline = 0;

    switch (number) {
    case 13:
        for(int listini = 0; listini <= 20; listini++)
        {
            lineout <<" ";
        }
        do{
            if(sumline++ >= 500000)
            {
                out = "format";
                break;
            }
            //===================================================
            //================= Read 50 datas ===================

            if((line.contains("TRE_0", Qt::CaseInsensitive))&&(line.contains("\t0\t0\t", Qt::CaseInsensitive))&&(!in.atEnd()))
            {
                line = in.readLine();//修复数据无效问题
            }

            if((line.contains("TRE_0", Qt::CaseInsensitive))&&(line.contains("\t0\t1\t", Qt::CaseInsensitive))&&(!in.atEnd()))
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;


                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,7);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,29);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[1+x]= data;
                    x++;
                    line = in.readLine(); // read again (3rd line), at end it will point to another data01
                } while((!line.contains("TRE_0", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[1+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 2)
            {

                for(int times = 0; times<=6; times++){
                    lineout.removeLast();
                }
                for(int times = 0; times<=6; times++){
                    lineout << "";
                }
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 20; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("TRE_0", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }

            if((line.contains("TRE_0", Qt::CaseInsensitive))&&(line.contains("\t1\t1\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(28);
                time.remove(1,7);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,29);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[11+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("TRE_0", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[11+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if
            if(groupnum == 2)
            {
                for(int times = 0; times<=6; times++){
                    lineout.removeLast();
                }
                for(int times = 0; times<=6; times++){
                    lineout << "";
                }
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 20; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("TRE_0", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }
        }
        while (!in.atEnd());
        break;

    case 15:
        for(int listini = 0; listini <= 30; listini++)
        {
            lineout <<" ";
        }
        do{
            if(sumline++ >= 500000)
            {
                out = "format";
                break;
            }
            //===================================================
            //================= Read 50 datas ===================
            if((line.contains("Data01", Qt::CaseInsensitive))
                    &&((line.contains("\t5\t0\t", Qt::CaseInsensitive))
                       ||(line.contains("\t1\t0\t", Qt::CaseInsensitive))
                       ||(line.contains("\t2\t0\t", Qt::CaseInsensitive)))
                    &&(!in.atEnd()))
            {
                line = in.readLine();//修复数据无效问题
            }

            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t5\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;
                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[1+x]= data;
                    x++;
                    line = in.readLine(); // read again (3rd line), at end it will point to another data01
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[1+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 3)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 30; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }




            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t1\t1\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[11+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[11+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if
            if(groupnum == 3)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 30; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }
            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t2\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[21+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[21+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 3)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 30; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }

        }
        while (!in.atEnd());
        break;
    case 17:
        for(int listini = 0; listini <= 20; listini++)
        {
            lineout <<" ";
        }
        do{
            if(sumline++ >= 500000)
            {
                out = "format";
                break;
            }
            //===================================================
            //================= Read 50 datas ===================


            if((line.contains("TRE_0", Qt::CaseInsensitive))&&(line.contains("\t0\t0\t", Qt::CaseInsensitive))&&(!in.atEnd()))
            {
                line = in.readLine();//修复数据无效问题
            }
            if((line.contains("TRE_0", Qt::CaseInsensitive))&&(line.contains("\t0\t1\t", Qt::CaseInsensitive))&&(!in.atEnd()))
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;


                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,7);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,29);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[1+x]= data;
                    x++;
                    line = in.readLine(); // read again (3rd line), at end it will point to another data01
                } while((!line.contains("TRE_0", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[1+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 2)
            {
                for(int times = 0; times<=2; times++){
                    lineout.removeLast();
                }
                for(int times = 0; times<=2; times++){
                    lineout << "";
                }
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 20; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("TRE_0", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }

            if((line.contains("TRE_0", Qt::CaseInsensitive))&&(line.contains("\t1\t1\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(28);
                time.remove(1,7);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,29);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[11+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("TRE_0", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[11+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if
            if(groupnum == 2)
            {
                for(int times = 0; times<=2; times++){
                    lineout.removeLast();
                }
                for(int times = 0; times<=2; times++){
                    lineout << "";
                }
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 20; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("TRE_0", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }
        }
        while (!in.atEnd());
        break;
    case 20: // WTC 15000>x>5000 => y = x - 10000;
        for(int listini = 0; listini <= 20; listini++)
        {
            lineout <<" ";
        }
        do{
            if(sumline++ >= 500000)
            {
                out = "format";
                break;
            }
            //===================================================
            //================= Read 50 datas ===================

            data = line;
            data.remove(1,29);
            i = data.indexOf("\t");
            data.truncate(i);
            data.remove(QChar('\"'), Qt::CaseInsensitive);
            int numdata = data.toInt();
            if((line.contains("TRE_0", Qt::CaseInsensitive))
                    &&(numdata>=15000)&&(numdata<25000)
                    &&(!in.atEnd()))
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;


                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,7);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,29);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    data = Shift(data,20000);
                    lineout[11+x]= data;
                    x++;
                    line = in.readLine(); // read again (3rd line), at end it will point to another data01
                } while((!line.contains("TRE_0", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[1+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 2)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 20; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("TRE_0", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }
            data = line;
            data.remove(1,29);
            i = data.indexOf("\t");
            data.truncate(i);
            data.remove(QChar('\"'), Qt::CaseInsensitive);
            numdata = data.toInt();

            if((line.contains("TRE_0", Qt::CaseInsensitive))
                    &&(numdata>=5000)&&(numdata<15000)
                    &&(!in.atEnd()))
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,7);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,29);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    data = Shift(data,10000);
                    lineout[1+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("TRE_0", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[11+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if
            if(groupnum == 2)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 20; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("TRE_0", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }
        }
        while (!in.atEnd());
        break;
    case 30: // WCS/WCC 15000>x>5000 => y = x - 10000;
        for(int listini = 0; listini <= 30; listini++)
        {
            lineout <<" ";
        }
        do{
            if(sumline++ >= 500000)
            {
                out = "format";
                break;
            }
            //===================================================
            //================= Read 50 datas ===================




            data = line;
            data.remove(1,29);
            i = data.indexOf("\t");
            data.truncate(i);
            data.remove(QChar('\"'), Qt::CaseInsensitive);
            int numdata = data.toInt();

            if((line.contains("TRE_0", Qt::CaseInsensitive))
                    &&(numdata>=5000)&&(numdata<15000)
                    &&(!in.atEnd()))
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;


                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,7);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,29);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    data = Shift(data,10000);
                    lineout[1+x]= data;

                    x++;
                    line = in.readLine(); // read again (3rd line), at end it will point to another data01

                } while((!line.contains("TRE_0", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[1+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 3)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 30; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("TRE_0", Qt::CaseInsensitive))
                {
                    line = in.readLine();

                }
            }


            data = line;
            data.remove(1,29);
            i = data.indexOf("\t");
            data.truncate(i);
            data.remove(QChar('\"'), Qt::CaseInsensitive);
            numdata = data.toInt();

            if((line.contains("TRE_0", Qt::CaseInsensitive))
                    &&(numdata>=15000)&&(numdata<25000)
                    &&(!in.atEnd()))
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(28);
                time.remove(1,7);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,29);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    data = Shift(data,20000);
                    lineout[11+x]= data;
                    x++;
                    line = in.readLine();

                } while((!line.contains("TRE_0", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[11+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if
            if(groupnum == 3)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 30; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("TRE_0", Qt::CaseInsensitive))
                {
                    line = in.readLine();

                }
            }



            data = line;
            data.remove(1,29);
            i = data.indexOf("\t");
            data.truncate(i);
            data.remove(QChar('\"'), Qt::CaseInsensitive);
            numdata = data.toInt();

            if((line.contains("TRE_0", Qt::CaseInsensitive))
                    &&(numdata>=25000)&&(numdata<35000)
                    &&(!in.atEnd()))
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(28);
                time.remove(1,7);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,29);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    data = Shift(data,30000);
                    lineout[21+x]= data;
                    x++;
                    line = in.readLine();

                } while((!line.contains("TRE_0", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[21+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if
            if(groupnum == 3)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 30; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("TRE_0", Qt::CaseInsensitive))
                {
                    line = in.readLine();

                }
            }
        }
        while (!in.atEnd());
        break;
    case 7:
        for(int listini = 0; listini <= 70; listini++)
        {
            lineout <<" ";
        }
        do{
            if(sumline++ >= 500000)
            {
                out = "format";
                break;
            }
            //===================================================
            //================= Read 50 datas ===================


            if((line.contains("Data01", Qt::CaseInsensitive))
                    &&((line.contains("\t5\t0\t", Qt::CaseInsensitive))
                       ||(line.contains("\t11\t0\t", Qt::CaseInsensitive))
                       ||(line.contains("\t12\t0\t", Qt::CaseInsensitive))
                       ||(line.contains("\t13\t0\t", Qt::CaseInsensitive))
                       ||(line.contains("\t21\t0\t", Qt::CaseInsensitive))
                       ||(line.contains("\t22\t0\t", Qt::CaseInsensitive))
                       ||(line.contains("\t23\t0\t", Qt::CaseInsensitive)))
                    &&(!in.atEnd()))
            {
                line = in.readLine();//修复数据无效问题
            }

            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t5\t1", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;


                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[1+x]= data;
                    x++;
                    line = in.readLine(); // read again (3rd line), at end it will point to another data01
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[1+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 7)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 70; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }

            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t11\t1\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[11+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[11+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if
            if(groupnum == 7)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 70; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }
            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t12\t1", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[21+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[21+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 7)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 70; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }
            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t13\t1", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[31+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[31+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 7)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 70; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }

            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t21\t1", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[41+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[41+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 7)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 50; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }

            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t22\t1", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;


                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[51+x]= data;
                    x++;
                    line = in.readLine(); // read again (3rd line), at end it will point to another data01
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[1+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 7)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 70; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }
            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t23\t1", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;


                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[61+x]= data;
                    x++;
                    line = in.readLine(); // read again (3rd line), at end it will point to another data01
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[1+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 7)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 70; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }
        }
        while (!in.atEnd());
        break;
    case 6:
        for(int listini = 0; listini <= 40; listini++)
        {
            lineout <<" ";
        }
        do{
            if(sumline++ >= 500000)
            {
                out = "format";
                break;
            }
            //===================================================
            //================= Read 50 datas ===================


            if((line.contains("Data01", Qt::CaseInsensitive))
                    &&((line.contains("\t5\t0\t", Qt::CaseInsensitive))
                       ||(line.contains("\t11\t0\t", Qt::CaseInsensitive))
                       ||(line.contains("\t12\t0\t", Qt::CaseInsensitive))
                       ||(line.contains("\t13\t0\t", Qt::CaseInsensitive)))
                    &&(!in.atEnd()))
            {
                line = in.readLine();//修复数据无效问题
            }

            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t5\t1", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;


                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[1+x]= data;
                    x++;
                    line = in.readLine(); // read again (3rd line), at end it will point to another data01
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[1+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 4)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 40; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }

            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t11\t1\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[11+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[11+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if
            if(groupnum == 4)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 40; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }
            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t12\t1", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[21+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[21+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 4)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 40; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }
            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t13\t1", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[31+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[31+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 4)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 40; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }}
            while (!in.atEnd());
            break;
    case 5:
        for(int listini = 0; listini <= 50; listini++)
        {
            lineout <<" ";
        }
        do{
            if(sumline++ >= 500000)
            {
                out = "format";
                break;
            }
            //===================================================
            //================= Read 50 datas ===================


            if((line.contains("Data01", Qt::CaseInsensitive))
                    &&((line.contains("\t5\t0\t", Qt::CaseInsensitive))
                       ||(line.contains("\t4\t0\t", Qt::CaseInsensitive))
                       ||(line.contains("\t3\t0\t", Qt::CaseInsensitive))
                       ||(line.contains("\t2\t0\t", Qt::CaseInsensitive))
                       ||(line.contains("\t1\t0\t", Qt::CaseInsensitive)))
                    &&(!in.atEnd()))
            {
                line = in.readLine();//修复数据无效问题
            }

            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t5\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;


                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[1+x]= data;
                    x++;
                    line = in.readLine(); // read again (3rd line), at end it will point to another data01
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[1+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 5)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 50; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }

            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t1\t1\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[11+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[11+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if
            if(groupnum == 5)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 50; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }
            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t2\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[21+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[21+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 5)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 50; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }
            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t3\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[31+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[31+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 5)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 50; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }

            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t4\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[41+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[41+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 5)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 50; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }
        }
        while (!in.atEnd());
        break;
    case 4:
        for(int listini = 0; listini <= 40; listini++)
        {
            lineout <<" ";
        }
        do{
            if(sumline++ >= 500000)
            {
                out = "format";
                break;
            }

            //===================================================
            //================= Read 50 datas ===================
            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t5\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;


                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[1+x]= data;
                    x++;
                    line = in.readLine(); // read again (3rd line), at end it will point to another data01
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[1+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 4)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 40; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t4\t1\t", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }

            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t1\t1\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[11+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[11+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if
            if(groupnum == 4)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 40; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t4\t1\t", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }
            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t2\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[21+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[21+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 4)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 40; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t4\t1\t", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }
            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t3\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[31+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[31+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 4)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 40; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t4\t1\t", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }

        }
        while (!in.atEnd());
        break;
    case 3:
        for(int listini = 0; listini <= 30; listini++)
        {
            lineout <<" ";
        }
        do{
            if(sumline++ >= 500000)
            {
                out = "format";
                break;
            }
            //===================================================
            //================= Read 50 datas ===================



            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t5\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;


                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[1+x]= data;
                    x++;
                    line = in.readLine(); // read again (3rd line), at end it will point to another data01
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[1+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 3)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 30; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t3\t1\t", Qt::CaseInsensitive)||line.contains("\t4\t1\t", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }

            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t1\t1\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[11+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[11+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if
            if(groupnum == 3)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 30; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t3\t1\t", Qt::CaseInsensitive)||line.contains("\t4\t1\t", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }
            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t2\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[21+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[21+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 3)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 30; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t3\t1\t", Qt::CaseInsensitive)||line.contains("\t4\t1\t", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }

        }
        while (!in.atEnd());

        break;
    case 2:
        for(int listini = 0; listini <= 20; listini++)
        {
            lineout <<" ";
        }
        do{
            if(sumline++ >= 500000)
            {
                out = "format";
                break;
            }
            //===================================================
            //================= Read 50 datas ===================



            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t5\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;


                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[1+x]= data;
                    x++;
                    line = in.readLine(); // read again (3rd line), at end it will point to another data01
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[1+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if

            if(groupnum == 2)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 20; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t3\t1\t", Qt::CaseInsensitive)||line.contains("\t4\t1\t", Qt::CaseInsensitive)||line.contains("\t2\t1\t", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }

            if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t1\t1\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
            {
                //read 10 datas and obtain the time
                //if error happens, null should be written into out.

                x = 0;

                //=======================================
                //========== obtain the time ============
                time= line;
                time.truncate(29);
                time.remove(1,8);
                time.remove(QChar('\"'), Qt::CaseInsensitive);
                lineout[0] = time;
                //========== obtain the time ============
                //=======================================

                //================ start ================
                //========== obtain the data ============
                do{
                    data = line;
                    data.remove(1,30);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    lineout[11+x]= data;
                    x++;
                    line = in.readLine();
                } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                if(x == 10)
                {
                    groupnum++;
                }
                else{
                    for(int listini = 0; listini < 10; listini++)
                    {
                        lineout[11+listini]=" ";
                    }
                }
                //========== obtain the data ============
                //===============  end ==================
            }
            //end of if
            if(groupnum == 2)
            {
                med = lineout.join(",");
                out.append(med);
                out.append("\n");
                groupnum = 0;
                //==============================================
                //============== list initial ==================
                for(int listini = 0; listini <= 20; listini++)
                {
                    lineout[listini]=" ";
                }
                //============== list initial ==================
                //==============================================
            }
            else
            {
                if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t3\t1\t", Qt::CaseInsensitive)||line.contains("\t4\t1\t", Qt::CaseInsensitive)||line.contains("\t2\t1\t", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                {
                    line = in.readLine();
                }
            }

        }
        while (!in.atEnd());
        break;

    }
    if(out=="failed")
    {
        QMessageBox::warning(this,
                             "Load File",
                             "Can't read file!\n\n"
                             "Please check the input directory!"
                             );
        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(0);
        return;
    }
    if(out=="format")
    {
        QMessageBox::warning(this,
                             "Load File",
                             "Can't convert!\n\n"
                             "Please check the input file format!"
                             );
        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(0);
        return;
    }

    if(Writefile(out,OutputFile)=="Write failed!")
    {
        QMessageBox::warning(this,
                             "Write File",
                             "Can't Write file!\n\n"
                             "Please check the output directory!"
                             );
        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(0);
        return;
    }
    ui->progressBar->setValue(1);

}

void MainWindow::on_pushButton_3_clicked()
{
    QString inputfile = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                qApp->applicationDirPath(),
                "Text File (*.txt);;All files (*.*)"
                );
    ui->lineEdit_2->setText(inputfile);
}
void MainWindow::on_pushButton_4_clicked()
{
    QString outputfile = QFileDialog::getSaveFileName(
                this,
                tr("Save File"),
                qApp->applicationDirPath(),
                tr("Comma Seperate Files (*.csv);;All files (*.*)")
                );
    ui->lineEdit->setText(outputfile);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString path = ui->lineEdit->text();
    QDesktopServices::openUrl( QUrl::fromLocalFile(path));
}

void MainWindow::on_pushButton_5_clicked()
{
    QString path = ui->lineEdit->text();
    int posi = path.lastIndexOf('/');
    path.truncate(posi);
    // qDebug() << path;
    QDesktopServices::openUrl( QUrl::fromLocalFile(path));
}

void MainWindow::on_pushButton_6_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    qApp->applicationDirPath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->lineEdit->setText(dir);
}

void MainWindow::on_Multifile_clicked()
{
    QStringList filenames = QFileDialog::getOpenFileNames(
                this,
                tr("Select Multiple Files"),
                qApp->applicationDirPath(),
                "Text File (*.txt);;All files (*.*)"
                );
    if(!filenames.isEmpty())
    {
        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(filenames.count());
        for (int iter = 0;iter<filenames.count();iter++)
        {

            QString type_of_mech = ui->comboBox->currentText();
            QString InputFile =  filenames[iter];
            QString DirOfOutput = ui->lineEdit->text();
            QFileInfo fi(InputFile);
            QString Onlyfilename = fi.fileName();
            Onlyfilename.chop(4);

            QString out;
            QString type = type_of_mech;
            QFile inputfile(InputFile);
            if (!inputfile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                inputfile.close();
                out = "failed";
            }
            QTextStream in(&inputfile);
            in.setCodec("UTF-8");

            QString line = in.readLine();
            out = "Time,";  // First write time

            //==============================
            //=======Choose Type============
            int number = 50;        //type-corresonding parameter number
            if(type == "COP65")  //load in COP65 parameters
            {
                number = 13;
                out.append("Group1,ELWT,EEWT,CLWT,CEWT,SuctP,DishP,OilP,DiscT,RLA,Group2,LqdLevel,EEV \n");
            }
            if(type == "WMT")
            {
                number = 15;
                out.append("Group1,ACTSetpiont,EvapLWT,EvapEWT,CondLWT,CondEWT,LiqLevel,EXV,CondP,EvapP,"
                           "Group2,LiqLineT,SuctP,DiscP,SuctT,DiscT,IGV,MBCSpd,VFDSpd,OptCurrent,"
                           "Group3,EconP,ActMinSpd,GapT,StatorT1,StatorT2,StatorT3,StatorT4,StatorT5,StatorT6,\n");
            }
            if(type == "F7")  //load in F7 parameters
            {
                number = 17;
                out.append("Group1,ELWT,EEWT,CLWT,CEWT,SuctP,DishP,1#OilP,2#OilP,1#DiscT,"
                           "Group2,2#DiscT,1#RLA,2#RLA,LqdLevel,EEV,EEV1 \n");
            }
            if(type == "WTC")  //load in WTC parameters
            {
                number = 20;
                out.append("EvapLWT,EvapEWT,CondLWT,CondEWT,OilSumpTemp,OilFeedTemp,SuctTemp,DischTemp,PctRLA,VfdSpeed,"
                           "OilNetPress,IGV_Pos,DDC_Pos,EconPress,KiloWatts,CondPress,EvapPress,EXV%PosCmd,LiqLineTemp,LiquidLevel \n");
            }
            if(type == "WMEGEN2-2")  //load in WMEGEN2-2 parameters
            {
                number = 7;
                out.append("Group1,ActiveSp,ELWT,EEWT,CLWT,CEWT,LiqLevel,EXV,CondP,LiquidT,"
                           "Group2,DischP,SuctP,DischT,SuctT,IGV,SpeedCmd,VFDSpd,MBCSpd,CalMinSpd,"
                           "Group3,StatorT1,StatorT2,StatorT3,MtrRotorT,MtrCaseT,MtrGapT,RLA,OutCurrent,OutCurrent2,"
                           "Group4,MBCFaultMap1,MBCFaultMap2,MBCFaultMap3,MBCFaultMap4,MBCFaultMap5,MBCFaultMap6,MBCFaultMap7,MBCFaultMap8, ,"
                           "Group5,DischP2,SuctP2,DischT2,SuctT2,IGV2,SpeedCmd2,VFDSpd2,MBCSpd2,CalMinSpd2,"
                           "Group6,StatorT1_2,StatorT2_2,StatorT3_2,MtrRotorT2,MtrCaseT2,MtrGapT2,RLA2,OutCurrent2,OutCurrent2_2,"
                           "Group7,MBCFaultMap1_2,MBCFaultMap2_2,MBCFaultMap3_2,MBCFaultMap4_2,MBCFaultMap5_2,MBCFaultMap6_2,MBCFaultMap7_2,MBCFaultMap8_2, ,\n");
            }
            if(type == "WMEGEN2-1")  //load in WMEGEN2-1 parameters
            {
                number = 6;
                out.append("Group1,ActiveSp,ELWT,EEWT,CLWT,CEWT,LiqLevel,EXV,CondP,LiquidT,"
                           "Group2,DischP,SuctP,DischT,SuctT,IGV,SpeedCmd,VFDSpd,MBCSpd,CalMinSpd,"
                           "Group3,StatorT1,StatorT2,StatorT3,MtrRotorT,MtrCaseT,MtrGapT,RLA,OutCurrent,OutCurrent2,"
                           "Group4,MBCFaultMap1,MBCFaultMap2,MBCFaultMap3,MBCFaultMap4,MBCFaultMap5,MBCFaultMap6,MBCFaultMap7,MBCFaultMap8, ,\n");
            }
            if(type == "WXE-1")  //load in WXE parameters
            {
                number = 2;
                out.append("Unit,Actsetpoint,ELWT,EEWT,CLWT,CEWT,LqdLevel,EXV,EconT,EconP,"
                           "1#Comp,SucT1,DiscT1,SuctP1,DiscP1,RLA1,IGV1,ActRPM1,MaxRPM1,MinRPM1,\n");
            }
            if(type == "WXE-2")  //load in WXE parameters
            {
                number = 3;
                out.append("Unit,Actsetpoint,ELWT,EEWT,CLWT,CEWT,LqdLevel,EXV,EconT,EconP,"
                           "1#Comp,SucT1,DiscT1,SuctP1,DiscP1,RLA1,IGV1,ActRPM1,MaxRPM1,MinRPM1,"
                           "2#Comp,SucT2,DiscT2,SuctP2,DiscP2,RLA2,IGV2,ActRPM2,MaxRPM2,MinRPM2,"
                           "\n");
            }
            if(type == "WXE-3")  //load in WXE parameters
            {
                number = 4;
                out.append("Unit,Actsetpoint,ELWT,EEWT,CLWT,CEWT,LqdLevel,EXV,EconT,EconP,"
                           "1#Comp,SucT1,DiscT1,SuctP1,DiscP1,RLA1,IGV1,ActRPM1,MaxRPM1,MinRPM1,"
                           "2#Comp,SucT2,DiscT2,SuctP2,DiscP2,RLA2,IGV2,ActRPM2,MaxRPM2,MinRPM2,"
                           "3#Comp,SucT3,DiscT3,SuctP3,DiscP3,RLA3,IGV3,ActRPM3,MaxRPM3,MinRPM3,"
                           "\n");
            }
            if(type == "WXE-4")  //load in WXE parameters
            {
                number = 5;
                out.append("Unit,Actsetpoint,ELWT,EEWT,CLWT,CEWT,LqdLevel,EXV,EconT,EconP,"
                           "1#Comp,SucT1,DiscT1,SuctP1,DiscP1,RLA1,IGV1,ActRPM1,MaxRPM1,MinRPM1,"
                           "2#Comp,SucT2,DiscT2,SuctP2,DiscP2,RLA2,IGV2,ActRPM2,MaxRPM2,MinRPM2,"
                           "3#Comp,SucT3,DiscT3,SuctP3,DiscP3,RLA3,IGV3,ActRPM3,MaxRPM3,MinRPM3,"
                           "4#Comp,SucT4,DiscT4,SuctP4,DiscP4,RLA4,IGV4,ActRPM4,MaxRPM4,MinRPM4 \n");
            }
            if(type == "WSC/WCC")
            {
                number = 30;
                out.append("CondEWT,CondLWT,EvapEWT,EvapLWT,"
                           "LiqLineTemp1,LiqLineTemp2,LiquidLevel1,LiquidLevel2,EXV%PosCmd1,EXV%PosCmd2,PctRLA1,SuctTemp1,DischTemp1,EvapPress1,CondPress1,OilFeedTemp1,OilSumpTemp1,OilSumpPress1,"
                           "PctRLA2,SuctTemp2,DischTemp2,EvapPress2,CondPress2,OilFeedTemp2,OilSumpTemp2,OilSumpPress2,NetOilPress1,NetOilPress2,IGV1,IGV2\n");
            }
            //==============================
            QString time;         // define the time mark arrays
            QString filetime;       //store the file time stamp
            QString data;       // define the data arrays
            QStringList lineout;
            QString med;        //a medium between lineout and out
            int validlines = 0;     //define the total valid length of the file
            int i=1;    // marker for data acquriment.
            int groupnum = 0; // group number marker.
            int x = 0;      //marker for data position in the list. when x = 9, data10 will be read.

            line = in.readLine();    //2nd read line
            int sumline = 0;

            switch (number) {
            case 13:
                for(int listini = 0; listini <= 20; listini++)
                {
                    lineout <<" ";
                }
                do{
                    if(sumline++ >= 500000)
                    {
                        out = "format";
                        break;
                    }
                    //===================================================
                    //================= Read 50 datas ===================

                    if((line.contains("TRE_0", Qt::CaseInsensitive))&&(line.contains("\t0\t0\t", Qt::CaseInsensitive))&&(!in.atEnd()))
                    {
                        line = in.readLine();//修复数据无效问题
                    }

                    if((line.contains("TRE_0", Qt::CaseInsensitive))&&(line.contains("\t0\t1\t", Qt::CaseInsensitive))&&(!in.atEnd()))
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;


                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,7);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,29);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[1+x]= data;
                            x++;
                            line = in.readLine(); // read again (3rd line), at end it will point to another data01
                        } while((!line.contains("TRE_0", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[1+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 2)
                    {

                        for(int times = 0; times<=6; times++){
                            lineout.removeLast();
                        }
                        for(int times = 0; times<=6; times++){
                            lineout << "";
                        }
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 20; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("TRE_0", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }

                    if((line.contains("TRE_0", Qt::CaseInsensitive))&&(line.contains("\t1\t1\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(28);
                        time.remove(1,7);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,29);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[11+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("TRE_0", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[11+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if
                    if(groupnum == 2)
                    {
                        for(int times = 0; times<=6; times++){
                            lineout.removeLast();
                        }
                        for(int times = 0; times<=6; times++){
                            lineout << "";
                        }
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 20; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("TRE_0", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }
                }
                while (!in.atEnd());

                break;
            case 15:
                for(int listini = 0; listini <= 30; listini++)
                {
                    lineout <<" ";
                }
                do{
                    if(sumline++ >= 500000)
                    {
                        out = "format";
                        break;
                    }
                    //===================================================
                    //================= Read 50 datas ===================
                    if((line.contains("Data01", Qt::CaseInsensitive))
                            &&((line.contains("\t5\t0\t", Qt::CaseInsensitive))
                               ||(line.contains("\t1\t0\t", Qt::CaseInsensitive))
                               ||(line.contains("\t2\t0\t", Qt::CaseInsensitive)))
                            &&(!in.atEnd()))
                    {
                        line = in.readLine();//修复数据无效问题
                    }

                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t5\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;
                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[1+x]= data;
                            x++;
                            line = in.readLine(); // read again (3rd line), at end it will point to another data01
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[1+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 3)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 30; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }




                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t1\t1\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[11+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[11+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if
                    if(groupnum == 3)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 30; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }
                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t2\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[21+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[21+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 3)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 30; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }

                }
                while (!in.atEnd());
                break;

            case 17:
                for(int listini = 0; listini <= 20; listini++)
                {
                    lineout <<" ";
                }
                do{
                    if(sumline++ >= 500000)
                    {
                        out = "format";
                        break;
                    }
                    //===================================================
                    //================= Read 50 datas ===================

                    if((line.contains("TRE_0", Qt::CaseInsensitive))&&(line.contains("\t0\t0\t", Qt::CaseInsensitive))&&(!in.atEnd()))
                    {
                        line = in.readLine();//修复数据无效问题
                    }

                    if((line.contains("TRE_0", Qt::CaseInsensitive))&&(line.contains("\t0\t1\t", Qt::CaseInsensitive))&&(!in.atEnd()))
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;


                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,7);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,29);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[1+x]= data;
                            x++;
                            line = in.readLine(); // read again (3rd line), at end it will point to another data01
                        } while((!line.contains("TRE_0", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[1+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 2)
                    {
                        for(int times = 0; times<=2; times++){
                            lineout.removeLast();
                        }
                        for(int times = 0; times<=2; times++){
                            lineout << "";
                        }
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 20; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("TRE_0", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }

                    if((line.contains("TRE_0", Qt::CaseInsensitive))&&(line.contains("\t1\t1\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(28);
                        time.remove(1,7);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,29);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[11+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("TRE_0", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[11+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if
                    if(groupnum == 2)
                    {
                        for(int times = 0; times<=2; times++){
                            lineout.removeLast();
                        }
                        for(int times = 0; times<=2; times++){
                            lineout << "";
                        }
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 20; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("TRE_0", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }
                }
                while (!in.atEnd());
                break;
            case 20: // WTC 15000>x>5000 => y = x - 10000;
                for(int listini = 0; listini <= 20; listini++)
                {
                    lineout <<" ";
                }
                do{
                    if(sumline++ >= 500000)
                    {
                        out = "format";
                        break;
                    }
                    //===================================================
                    //================= Read 50 datas ===================

                    data = line;
                    data.remove(1,29);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    int numdata = data.toInt();

                    if((line.contains("TRE_0", Qt::CaseInsensitive))
                            &&(numdata>=15000)&&(numdata<25000)
                            &&(!in.atEnd()))
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;


                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,7);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,29);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            data = Shift(data,20000);
                            lineout[11+x]= data;
                            x++;
                            line = in.readLine(); // read again (3rd line), at end it will point to another data01
                        } while((!line.contains("TRE_0", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[1+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 2)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 20; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("TRE_0", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }
                    data = line;
                    data.remove(1,29);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    numdata = data.toInt();

                    if((line.contains("TRE_0", Qt::CaseInsensitive))
                            &&(numdata>=5000)&&(numdata<15000)
                            &&(!in.atEnd()))
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,7);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,29);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            data = Shift(data,10000);
                            lineout[1+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("TRE_0", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[11+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if
                    if(groupnum == 2)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 20; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("TRE_0", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }
                }
                while (!in.atEnd());
                break;
            case 30: // WCS/WCC 15000>x>5000 => y = x - 10000;
                for(int listini = 0; listini <= 30; listini++)
                {
                    lineout <<" ";
                }
                do{
                    if(sumline++ >= 500000)
                    {
                        out = "format";
                        break;
                    }
                    //===================================================
                    //================= Read 50 datas ===================




                    data = line;
                    data.remove(1,29);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    int numdata = data.toInt();

                    if((line.contains("TRE_0", Qt::CaseInsensitive))
                            &&(numdata>=5000)&&(numdata<15000)
                            &&(!in.atEnd()))
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;


                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,7);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,29);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            data = Shift(data,10000);
                            lineout[1+x]= data;

                            x++;
                            line = in.readLine(); // read again (3rd line), at end it will point to another data01

                        } while((!line.contains("TRE_0", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[1+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 3)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 30; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("TRE_0", Qt::CaseInsensitive))
                        {
                            line = in.readLine();

                        }
                    }


                    data = line;
                    data.remove(1,29);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    numdata = data.toInt();

                    if((line.contains("TRE_0", Qt::CaseInsensitive))
                            &&(numdata>=15000)&&(numdata<25000)
                            &&(!in.atEnd()))
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(28);
                        time.remove(1,7);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,29);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            data = Shift(data,20000);
                            lineout[11+x]= data;
                            x++;
                            line = in.readLine();

                        } while((!line.contains("TRE_0", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[11+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if
                    if(groupnum == 3)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 30; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("TRE_0", Qt::CaseInsensitive))
                        {
                            line = in.readLine();

                        }
                    }



                    data = line;
                    data.remove(1,29);
                    i = data.indexOf("\t");
                    data.truncate(i);
                    data.remove(QChar('\"'), Qt::CaseInsensitive);
                    numdata = data.toInt();

                    if((line.contains("TRE_0", Qt::CaseInsensitive))
                            &&(numdata>=25000)&&(numdata<35000)
                            &&(!in.atEnd()))
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(28);
                        time.remove(1,7);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,29);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            data = Shift(data,30000);
                            lineout[21+x]= data;
                            x++;
                            line = in.readLine();

                        } while((!line.contains("TRE_0", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[21+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if
                    if(groupnum == 3)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 30; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("TRE_0", Qt::CaseInsensitive))
                        {
                            line = in.readLine();

                        }
                    }
                }
                while (!in.atEnd());
                break;
            case 7:
                for(int listini = 0; listini <= 70; listini++)
                {
                    lineout <<" ";
                }
                do{
                    if(sumline++ >= 500000)
                    {
                        out = "format";
                        break;
                    }
                    //===================================================
                    //================= Read 50 datas ===================


                    if((line.contains("Data01", Qt::CaseInsensitive))
                            &&((line.contains("\t5\t0\t", Qt::CaseInsensitive))
                               ||(line.contains("\t11\t0\t", Qt::CaseInsensitive))
                               ||(line.contains("\t12\t0\t", Qt::CaseInsensitive))
                               ||(line.contains("\t13\t0\t", Qt::CaseInsensitive))
                               ||(line.contains("\t21\t0\t", Qt::CaseInsensitive))
                               ||(line.contains("\t22\t0\t", Qt::CaseInsensitive))
                               ||(line.contains("\t23\t0\t", Qt::CaseInsensitive)))
                            &&(!in.atEnd()))
                    {
                        line = in.readLine();//修复数据无效问题
                    }

                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t5\t1", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;


                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[1+x]= data;
                            x++;
                            line = in.readLine(); // read again (3rd line), at end it will point to another data01
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[1+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 7)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 70; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }

                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t11\t1\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[11+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[11+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if
                    if(groupnum == 7)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 70; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }
                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t12\t1", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[21+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[21+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 7)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 70; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }
                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t13\t1", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[31+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[31+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 7)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        groupnum = 0;
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 70; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }

                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t21\t1", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[41+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[41+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 7)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 50; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }

                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t22\t1", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;


                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[51+x]= data;
                            x++;
                            line = in.readLine(); // read again (3rd line), at end it will point to another data01
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[1+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 7)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 70; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }
                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t23\t1", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;


                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[61+x]= data;
                            x++;
                            line = in.readLine(); // read again (3rd line), at end it will point to another data01
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[1+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 7)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 70; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }
                }
                while (!in.atEnd());
                break;

            case 6:
                for(int listini = 0; listini <= 40; listini++)
                {
                    lineout <<" ";
                }
                do{
                    if(sumline++ >= 500000)
                    {
                        out = "format";
                        break;
                    }
                    //===================================================
                    //================= Read 50 datas ===================


                    if((line.contains("Data01", Qt::CaseInsensitive))
                            &&((line.contains("\t5\t0\t", Qt::CaseInsensitive))
                               ||(line.contains("\t11\t0\t", Qt::CaseInsensitive))
                               ||(line.contains("\t12\t0\t", Qt::CaseInsensitive))
                               ||(line.contains("\t13\t0\t", Qt::CaseInsensitive)))
                            &&(!in.atEnd()))
                    {
                        line = in.readLine();//修复数据无效问题
                    }

                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t5\t1", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;


                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[1+x]= data;
                            x++;
                            line = in.readLine(); // read again (3rd line), at end it will point to another data01
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[1+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 4)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 40; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }

                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t11\t1\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[11+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[11+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if
                    if(groupnum == 4)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 40; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }
                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t12\t1", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[21+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[21+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 4)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 40; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }
                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t13\t1", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[31+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[31+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 4)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 40; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }}
                    while (!in.atEnd());
                    break;
            case 5:
                for(int listini = 0; listini <= 50; listini++)
                {
                    lineout <<" ";
                }
                do{
                    if(sumline++ >= 500000)
                    {
                        out = "format";
                        break;
                    }
                    //===================================================
                    //================= Read 50 datas ===================



                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t5\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;


                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[1+x]= data;
                            x++;
                            line = in.readLine(); // read again (3rd line), at end it will point to another data01
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[1+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 5)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 50; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }

                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t1\t1\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[11+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[11+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if
                    if(groupnum == 5)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 50; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }
                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t2\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[21+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[21+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 5)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 50; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }
                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t3\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[31+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[31+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 5)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 50; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }

                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t4\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[41+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[41+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 5)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 50; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }
                }
                while (!in.atEnd());

                break;
            case 4:
                for(int listini = 0; listini <= 40; listini++)
                {
                    lineout <<" ";
                }
                do{
                    if(sumline++ >= 500000)
                    {
                        out = "format";
                        break;
                    }

                    //===================================================
                    //================= Read 50 datas ===================



                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t5\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;


                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[1+x]= data;
                            x++;
                            line = in.readLine(); // read again (3rd line), at end it will point to another data01
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[1+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 4)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 40; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t4\t1\t", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }

                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t1\t1\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[11+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[11+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if
                    if(groupnum == 4)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 40; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t4\t1\t", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }
                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t2\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[21+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[21+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 4)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 40; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t4\t1\t", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }
                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t3\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[31+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[31+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 4)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 40; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t4\t1\t", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }

                }
                while (!in.atEnd());
                break;
            case 3:
                for(int listini = 0; listini <= 30; listini++)
                {
                    lineout <<" ";
                }
                do{
                    if(sumline++ >= 500000)
                    {
                        out = "format";
                        break;
                    }
                    //===================================================
                    //================= Read 50 datas ===================



                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t5\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;


                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[1+x]= data;
                            x++;
                            line = in.readLine(); // read again (3rd line), at end it will point to another data01
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[1+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 3)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 30; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t3\t1\t", Qt::CaseInsensitive)||line.contains("\t4\t1\t", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }

                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t1\t1\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[11+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[11+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if
                    if(groupnum == 3)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 30; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t3\t1\t", Qt::CaseInsensitive)||line.contains("\t4\t1\t", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }
                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t2\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[21+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[21+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 3)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 30; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t3\t1\t", Qt::CaseInsensitive)||line.contains("\t4\t1\t", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }

                }
                while (!in.atEnd());

                break;
            case 2:
                for(int listini = 0; listini <= 20; listini++)
                {
                    lineout <<" ";
                }
                do{
                    if(sumline++ >= 500000)
                    {
                        out = "format";
                        break;
                    }
                    //===================================================
                    //================= Read 50 datas ===================



                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t5\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;


                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[1+x]= data;
                            x++;
                            line = in.readLine(); // read again (3rd line), at end it will point to another data01
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[1+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if

                    if(groupnum == 2)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 20; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t3\t1\t", Qt::CaseInsensitive)||line.contains("\t4\t1\t", Qt::CaseInsensitive)||line.contains("\t2\t1\t", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }

                    if((line.contains("Data01", Qt::CaseInsensitive))&&(line.contains("\t1\t1\t", Qt::CaseInsensitive))&&(!in.atEnd())) //if marked as 5
                    {
                        //read 10 datas and obtain the time
                        //if error happens, null should be written into out.

                        x = 0;

                        //=======================================
                        //========== obtain the time ============
                        time= line;
                        time.truncate(29);
                        time.remove(1,8);
                        time.remove(QChar('\"'), Qt::CaseInsensitive);
                        lineout[0] = time;
                        //========== obtain the time ============
                        //=======================================

                        //================ start ================
                        //========== obtain the data ============
                        do{
                            data = line;
                            data.remove(1,30);
                            i = data.indexOf("\t");
                            data.truncate(i);
                            data.remove(QChar('\"'), Qt::CaseInsensitive);
                            lineout[11+x]= data;
                            x++;
                            line = in.readLine();
                        } while((!line.contains("Data01", Qt::CaseInsensitive))&&(!in.atEnd())&&(x<10));
                        if(x == 10)
                        {
                            groupnum++;
                        }
                        else{
                            for(int listini = 0; listini < 10; listini++)
                            {
                                lineout[11+listini]=" ";
                            }
                        }
                        //========== obtain the data ============
                        //===============  end ==================
                    }
                    //end of if
                    if(groupnum == 2)
                    {
                        med = lineout.join(",");
                        out.append(med);
                        out.append("\n");
                        validlines++;
                        if(validlines==1)
                        {
                            filetime = time;
                        }
                        groupnum = 0;
                        //==============================================
                        //============== list initial ==================
                        for(int listini = 0; listini <= 20; listini++)
                        {
                            lineout[listini]=" ";
                        }
                        //============== list initial ==================
                        //==============================================
                    }
                    else
                    {
                        if(!line.contains("Data01", Qt::CaseInsensitive)||line.contains("\t3\t1\t", Qt::CaseInsensitive)||line.contains("\t4\t1\t", Qt::CaseInsensitive)||line.contains("\t2\t1\t", Qt::CaseInsensitive)||line.contains("\t0\t1\t", Qt::CaseInsensitive))
                        {
                            line = in.readLine();
                        }
                    }

                }
                while (!in.atEnd());
                break;

            }




            time.truncate(10);
            time.remove(4,1).remove(6,1);
            filetime.truncate(10);
            filetime.remove(4,1).remove(6,1);
            QString OutputFile = DirOfOutput.append("/").append(filetime).append("_").append(time).append("_").append(type_of_mech).append("_").append(Onlyfilename).append(".csv");
            qDebug()<<OutputFile;
            if(out=="failed")
            {
                QMessageBox::warning(this,
                                     "Load File",
                                     "Can't read file!\n\n"
                                     "Please check the input directory!"
                                     );
                ui->progressBar->setMinimum(0);
                ui->progressBar->setMaximum(0);
                return;
            }
            if(out=="format")
            {
                QMessageBox::warning(this,
                                     "Load File",
                                     "Can't convert!\n\n"
                                     "Please check the input file format!"
                                     );
                ui->progressBar->setMinimum(0);
                ui->progressBar->setMaximum(0);
                return;
            }

            if(Writefile(out,OutputFile)=="Write failed!")
            {
                QMessageBox::warning(this,
                                     "Write File",
                                     "Can't Write file!\n\n"
                                     "Please check the output directory!"
                                     );
                ui->progressBar->setMinimum(0);
                ui->progressBar->setMaximum(0);
                return;
            }

            ui->progressBar->setValue(iter+1);

        }
    }
}

