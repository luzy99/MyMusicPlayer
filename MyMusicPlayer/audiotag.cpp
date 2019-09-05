#include "audiotag.h"
#include"songinfo.h"
#include<QDebug>

CodeType JudgeCodeType(const string & str, CodeType default_code);
wstring StrToUnicode(const string & str, CodeType code_type);

AudioTag::AudioTag(QString f_url, SongInfo* &si)
{
    fp = _wfopen(f_url.toStdWString().c_str(),L"rb");
    if (NULL == fp)
    {
        printf("open read file error!!");
    }
    qDebug()<<f_url;
    m_song_info = si;
    m_song_info->file_path=f_url;
}

AudioTag::~AudioTag()
{
}

bool AudioTag::GetAlbumCover(string &tag_content, int tag_index, int tag_size)
{
    string img_type;
    //到存储封面的文件里读取
    QString default_path ="./CoverImages/";
    img_type = tag_content.substr(tag_index + 25, 2);
    qDebug()<<QString::fromStdString(img_type);
    if(!img_type.compare("PN"))    //png
    {
        default_path +=m_song_info->title;
        default_path +=QString(".png");
        m_song_info->album_cover = default_path;
    }
    else    //jpg
    {
        default_path +=m_song_info->title;
        default_path +=QString(".jpg");
        m_song_info->album_cover = default_path;
    }

    fseek(fp,tag_index+24,SEEK_SET);//定位图片信息
    FILE *wfp = _wfopen( m_song_info->album_cover.toStdWString().c_str(), L"wb" );
    wchar_t* temp_str = new wchar_t[tag_size];
    fread(temp_str,1,tag_size-14,fp);
    fwrite(temp_str,1,tag_size-14,wfp);
    fclose(wfp);
    m_song_info->has_cover=true;
    return 1;
}

bool AudioTag::getAllinfo()
{
    bool success;
    fseek(fp,0,SEEK_SET);
    char Header[3];
    fread(&Header,1,3,fp);//id3v2格式前三位为“ID3”
    if(Header[0]=='I'&&Header[1]=='D'&&Header[2]=='3')
    {
        qDebug()<<"open ID3 correct!";
        m_song_info->tag_type=1;
        char size[4];
        fseek(fp,6,SEEK_SET);
        fread(&size,1,4,fp);//标签头开始往后偏移6个字节开始的4个字节是整个标签的大小
        const int tag_size=size[0]* 0x1000000 +size[1] * 0x10000 + size[2]* 0x100 + size[3];	//获取标签区域的总大小
        //const int tag_size = (size[0] & 0x7F) * 0x200000 + (size[1] & 0x7F) * 0x4000 + (size[2] & 0x7F) * 0x80 + (size[3] & 0x7F);
        //string tag_content;
        char* buffer=new char[tag_size];
        fseek(fp,0,SEEK_SET);
        fread(buffer,tag_size,1,fp);
        //tag_content =buffer;
        string tag_content(buffer, buffer + tag_size);
        //tag_content.assign(id3v2, tag_size);	//将标签区域的内容保存到一个string对象里

        const int TAG_NUM=5;
        //要查找的标签标识字符串（标题、艺术家、唱片集、年份、图片）
        const string tag_identify[TAG_NUM]{ "TIT2","TPE1","TALB","TYER","APIC"};
        for (int i=0; i < TAG_NUM; i++)
        {
            size_t tag_index;
            tag_index = tag_content.find(tag_identify[i]);	//查找一个标签标识字符串
            if (i == 1 && tag_index == string::npos)	//如果在查找艺术家时找不到TPE1标签，尝试查找TPE2标签
            {
                tag_index = tag_content.find("TPE2");
            }
            if (tag_index != string::npos)
            {
                string size = tag_content.substr(tag_index + 4, 4);
                const size_t tag_size = (BYTE)size[0] * 0x1000000 + (BYTE)size[1] * 0x10000 + (BYTE)size[2] * 0x100 + (BYTE)size[3];	//获取当前标签的大小
                if (tag_size <= 0)
                {
                    continue;
                 }
                if (tag_index + 11 >= tag_content.size())
                 {
                    continue;
                 }
                //判断标签的编码格式
                CodeType default_code, code_type;
                switch (tag_content[tag_index + 10])
                {
                case 1: case 2:
                    default_code = CodeType::UTF16;
                    break;
                case 3:
                    default_code = CodeType::UTF8;
                    break;
                default:
                    default_code = CodeType::ANSI;
                    break;
                }
                string tag_info_str;

                tag_info_str = tag_content.substr(tag_index + 11, tag_size - 1);

                code_type = JudgeCodeType(tag_info_str, default_code);
                wstring tag_info;
                tag_info = StrToUnicode(tag_info_str, code_type);

                //标签不为空则写入数据
                if (!tag_info.empty())
                {
                    switch (i)
                    {
                    case 0:
                        m_song_info->title = QString::fromStdWString(tag_info);
                        m_song_info->info_acquired=true;//有标题就算信息已获取
                        break;
                    case 1:
                        m_song_info->artist = QString::fromStdWString(tag_info);
                        break;
                    case 2:
                        m_song_info->album = QString::fromStdWString(tag_info);
                        break;
                    case 3:
                        m_song_info->year = QString::fromStdWString(tag_info);
                        break;
                    case 4: //获取专辑图片
                        GetAlbumCover(tag_content, tag_index, tag_size);
                        break;
                    }
                }
            }
        }
    }
    else
    {
        success = false;
    }
    return success;
}

bool AudioTag::idMatch()
{
    QString url = "http://music.163.com/api/search/pc/?s="
            +m_song_info->title
            +"&limit=20&type=1&offset=0";
    //构造请求
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute,true);
    //构造网络管理
    QNetworkAccessManager* manager = new QNetworkAccessManager;
    // 发送请求
    QNetworkReply *pReply = manager->get(request);
    //设置事件循环，等待资源下载完毕
    QEventLoop eventLoop;
    QObject::connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    //检测http请求的状态码
    int nHttpCode = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();//http返回码
    qDebug()<<"id匹配："<<nHttpCode;

    QByteArray bytes = pReply->readAll();
    QString Qresult=bytes;
    //qDebug()<<Qresult;
    QJsonParseError error;
    QJsonDocument jsonResult =QJsonDocument::fromJson(Qresult.toUtf8(),&error);


    if (error.error == QJsonParseError::NoError) // 解析未发生错误
    {
        if(jsonResult.isObject())
        {
            qDebug()<<"toJson SUCCESS";
            //            QVariantMap jsonMap = jsonResult.toVariant().toMap();
            //            QVariantMap =jsonMap["result"].toVariant().toMap()["songs"].toArray();
            QJsonObject temp =jsonResult.object().value("result").toObject();
            QJsonArray songsArray =temp["songs"].toArray();
            qDebug()<<songsArray;
            int i=0;
            for(;i<songsArray.count();i++)
            {
                QString tempStr =songsArray[i].toString();
                if(tempStr.contains(m_song_info->title)
                        &&temp.contains(m_song_info->artist))
                {
                    QJsonObject jjj =songsArray[i].toObject();
                    QString idstr=QString::number(jjj.value("id").toInt());//解析songID
                    qDebug()<<idstr;
                    m_song_info->song_id=idstr;
                    QString picUrl =jjj.value("album").toObject().value("picUrl").toString();//解析封面地址
                    qDebug()<<"picUrl: "<<picUrl;
                    m_song_info->pic_url=picUrl;
                    break;
                }
            }
            if(i == songsArray.count())//如果没有匹配的，取第一条
            {
                QJsonObject jjj = songsArray[0].toObject();
                QString idstr = QString::number(jjj.value("id").toInt());//解析songID
                qDebug()<<idstr;
                m_song_info->song_id=idstr;
                QString picUrl = jjj.value("album").toObject().value("picUrl").toString();//解析封面地址
                qDebug()<<"picUrl: "<<picUrl;
                m_song_info->pic_url=picUrl;
            }

        }
        else return 0;
    }
    return 1;
}

bool AudioTag::downloadPic()
{
    if(m_song_info->has_cover == false && !m_song_info->pic_url.isEmpty())//若没有封面，且链接获取正常
    {
        //构造请求
        QNetworkRequest request;
        request.setUrl(QUrl(m_song_info->pic_url));
        request.setAttribute(QNetworkRequest::FollowRedirectsAttribute,true);
        //构造网络管理
        QNetworkAccessManager* manager = new QNetworkAccessManager;
        // 发送请求
        QNetworkReply *pReply = manager->get(request);
        //设置事件循环，等待资源下载完毕
        QEventLoop eventLoop;
        QObject::connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec();
        //检测http请求的状态码
        int nHttpCode = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();//http返回码
        qDebug()<<"封面下载："<<nHttpCode;
        QByteArray bytes = pReply->readAll();
        //默认将封面存储在路径为"项目根目录/CoverImages"
        QString default_path = "./CoverImages/";
        QString path = default_path+m_song_info->title
                +m_song_info->pic_url.mid(m_song_info->pic_url.length()-4,4);
        qDebug()<<path;
        m_song_info->album_cover=path;
        QFile file(path);
        file.open(QIODevice::WriteOnly);
        file.write(bytes);
        file.close();
        return true;
    }

    return false;
}

bool AudioTag::mvIdMatch()
{
    if(m_song_info->song_id.isEmpty())//若id不存在
    {
        qDebug()<<"id不存在";
        return 0;
    }
    else
    {
        QString url = "https://music.163.com/m/song?id="
                +m_song_info->song_id;
        //构造请求
        QNetworkRequest request;
        request.setUrl(QUrl(url));
        //构造网络管理
        QNetworkAccessManager* manager = new QNetworkAccessManager;
        // 发送请求
        QNetworkReply *pReply = manager->get(request);
        //设置事件循环，等待资源下载完毕
        QEventLoop eventLoop;
        QObject::connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec();

         //检测http请求的状态码
        int nHttpCode = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();//http返回码
        qDebug()<<"mv查找："<<nHttpCode;

         QByteArray response = pReply->readAll();
        QString responseStr=response;
        qDebug()<<responseStr;
        QRegExp re("\"mv\":(\\d+),");//正则匹配数字
        int pos=responseStr.indexOf(re);
        qDebug()<<pos;
        if(pos>=0)
        {
            QString matchedId =re.cap(1);
            qDebug()<<"mv_id "<<matchedId;
            if(matchedId.length()>3)
            {
                m_song_info->mv_id=matchedId;
            }
            else
            {
                return 0;
            }
            return 1;
        }
        else//未匹配到
        {
            return 0;
        }
    }
}

//判断编码
CodeType JudgeCodeType(const string & str, CodeType default_code)
{
    //如果前面有UTF8的BOM，则编码类型为UTF8
    if (str.size() >= 3 && str[0] == -17 && str[1] == -69 && str[2] == -65)
    {
        return CodeType::UTF8;
    }
    //如果前面有UTF16的BOM，则编码类型为UTF16
    else if (str.size() >= 2 && str[0] == -1 && str[1] == -2)
    {
        //else if (IsUTF8Bytes(str.c_str()))		//如果没有找到UTF8和UTF16的BOM，则判断字符串是否有UTF8编码的特性
        return CodeType::UTF16;
    }
    //	return CodeType::UTF8_NO_BOM;
    else
    {
        return default_code;
    }
}

wstring StrToUnicode(const string & str, CodeType code_type)
{
    if (str.empty())
    {
        return wstring();
    }

    if (code_type == CodeType::AUTO)
    {
        code_type = JudgeCodeType(str,CodeType::ANSI);
    }

    wstring result;
    int size;

    if (code_type == CodeType::ANSI)
    {
        size = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
        if (size <= 0)
        {
            return wstring();
        }
        wchar_t* str_unicode = new wchar_t[size + 1];
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, str_unicode, size);
        result.assign(str_unicode);
        delete[] str_unicode;
    }
    else if (code_type == CodeType::UTF8 || code_type == CodeType::UTF8_NO_BOM)
    {
        string temp;
        //如果前面有BOM，则去掉BOM
        if (str.size() >= 3 && str[0] == -17 && str[1] == -69 && str[2] == -65)
        {
            temp = str.substr(3);
        }
        else
        {
            temp = str;
        }
        size = MultiByteToWideChar(CP_UTF8, 0, temp.c_str(), -1, NULL, 0);
        if (size <= 0)
        {
            return wstring();
        }
        wchar_t* str_unicode = new wchar_t[size + 1];
        MultiByteToWideChar(CP_UTF8, 0, temp.c_str(), -1, str_unicode, size);
        result.assign(str_unicode);
        delete[] str_unicode;
    }
    else if (code_type == CodeType::UTF16)
    {
        string temp;
        //如果前面有BOM，则去掉BOM
        if (str.size() >= 2 && str[0] == -1 && str[1] == -2)
        {
            temp = str.substr(2);
        }
        else
        {
            temp = str;
        }
        if (temp.size() % 2 == 1)
        {
            temp.pop_back();
        }
        temp.push_back('\0');
        result = (const wchar_t*)temp.c_str();
    }
    return result;
}
