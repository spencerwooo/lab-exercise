#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString parseURL(QString url)
{
    /*通过url解析出域名*/
    QString domain;
    int start = 0;

    if (url.contains("https://"))
    {   start = 8;
    }
    else if (url.contains("http://"))
    {   start = 7;
    }
    else
    {   start = 0;
    }

    domain = url.mid(start);    //解析域名

    int index = url.indexOf("/", start);
    domain = url.mid(start, index - start);

    return domain;
}

QString getFilename(QString url, QString filename)
{
    //获取下载文件名
    int index = url.lastIndexOf("/");
    if (index != url.size() - 1)    //得到文件名
    {
        filename = url.mid(index + 1);
    }
    return filename;
}

void MainWindow::on_pathButton_clicked()
{
    //选择文件保存地址
    QString dir = QFileDialog::getExistingDirectory(NULL, tr("Open Directory"),
                                        ".",QFileDialog::ShowDirsOnly
                                        | QFileDialog::DontResolveSymlinks);
    ui->path->setText(dir);
}

QByteArray readOneByte(QTcpSocket *client)
{
    QByteArray ba;
    qint64 size = 1;
    while (true)
    {
        client->waitForReadyRead(500);
        ba = client->read(size);
        if (ba.size() == 0)
            continue;
        else break;
    }
    return ba;
}

int getLenth(QByteArray ba)
{
    QString str = ba.data();
    QStringList len_temp = str.split("\r\n");//Content-Length: 40831\r\n
    len_temp = len_temp.filter("Content-Length: ");
    QString len_str = len_temp.join("");
    len_str = len_str.mid(16);
    bool ok;
    int len = len_str.toInt(&ok);
    return len;
}

//参数为socket,文件长度，路径，文件名
bool MainWindow::download(QTcpSocket *client, int lenth, QString path, QString fileName)
{
    //下载文件函数
    int len = 0;    //len表示已写入长度

    //创建文件描述符
    path = path.append("/");
    QString str = path.append(fileName);

    QFile file(str);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,"Warnning","can't open the file",
                             QMessageBox::Yes);
        return false;
    }

    while(len < lenth)
    {
        QByteArray ba = client->readAll();
        client->waitForReadyRead(100000);
        file.write(ba);
        len+=ba.size();
    }


    file.close();
    if (len == lenth)
    {
        return true;
    }
    else return false;
}

void MainWindow::on_pushButton_clicked()
{
    /*
    test url:
    http://images.sohu.com/cs/button/lianxiang/483-24/dkt2760480.jpg
    */
    QString url = ui->urlEdit->text();
    QString domain;
    QString fileName = ui->name->text();
    QString path = ui->path->text();

    if (path.size() == 0)   //未指定存储位置
    {
        QMessageBox::warning(this,"Warnning","请指定存储位置",
                             QMessageBox::Yes);
        return;
    }

    domain = parseURL(url); //解析域名

    if (fileName.size() == 0)   //未命名文件
    {
        fileName = getFilename(url, fileName);
    }

    QTcpSocket *client;        //创建套接字
    client = new QTcpSocket(this);
    QHostInfo info = QHostInfo::fromName(domain);
    QHostAddress address = info.addresses().first();

    client->connectToHost(address, 80); //连接服务器
    client->waitForConnected(5000);
    qDebug() <<  client->state();

    QString header =  QString("GET %1 HTTP/1.1\r\n").arg(url)
                        + QString("Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n")
                        + QString("User-Agent:Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537(KHTML, like Gecko) Chrome/47.0.2526Safari/537.36\r\n")
                        + QString("Host:%1\r\n").arg(domain)
                        + QString("Connection:close\r\n\r\n");

    client->write(header.toLatin1());
    client->waitForBytesWritten(3000);

    QByteArray ba, rbuf = "";
    int flag;
    while (true)
    {
        ba = readOneByte(client);   //一个字节一个字节读取
        rbuf.append(ba);
        if(ba.contains("\n") || ba.contains("\r"))//分割响应头
        {
            flag++;
            if (flag == 4) break;
        }
        else flag = 0;
    }
    qDebug() << rbuf;

    int lenth = getLenth(rbuf);
    if(download(client, lenth, path, fileName))
    {
        QMessageBox::warning(this,"","Download Compeleted!",
                             QMessageBox::Yes);
    }
    else
    {
        QMessageBox::warning(this,"Warnning!","Ah oh! Download failed",
                             QMessageBox::Yes);
    }

     client->close();

}
