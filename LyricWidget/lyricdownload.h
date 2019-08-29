#ifndef LYRICDOWNLOAD_H
#define LYRICDOWNLOAD_H

#include <QObject>
#include <iostream>
#include <QtNetwork/QNetworkRequest>//网络请求
#include <QtNetwork/QNetworkReply>//网络响应
#include <QtNetwork/QNetworkAccessManager>//网络管理
#include <QFile>
#include <QUrl>
#include <QtDebug>
#include <QEventLoop>
#include <wchar.h>
#include <Windows.h>
using namespace std;

enum class CodeType
{
    ANSI,
    UTF8,
    UTF8_NO_BOM,
    UTF16,
    AUTO
};

class LyricDownload : public QObject
{
    Q_OBJECT
public:
    explicit LyricDownload(QObject *parent = nullptr);

   // ~LyricDownload()

    //搜索框得到歌曲id
    //结果保存到result中
    //download_translate(从界面'译'按钮得到)参数指定是否下载带翻译的歌词
    static bool DownloadLyric(const QString& song_id,bool download_translate);

    //对从网易云音乐下载的歌词进行处理，转换成正确的歌词文本
    static bool DisposeLryic(QString& lyric_str);


signals:

private:
    static string UnicodeToStr(const wstring & wstr, CodeType code_type, bool* char_cannot_convert = nullptr)
        {
            if (wstr.empty()) return string();
            if (char_cannot_convert != nullptr)
                *char_cannot_convert = false;
            bool UsedDefaultChar{ false };
            string result;
            int size{ 0 };
            if (code_type == CodeType::ANSI)//将宽字符转换为ANSI
            {
                //将wstring.c_str()转为wchar_t
                size = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
                if (size <= 0) return string();
                char* str = new char[size + 1];
                WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, str, size, NULL, NULL);
                result.assign(str);
                delete[] str;
            }
            else if (code_type == CodeType::UTF8 || code_type == CodeType::UTF8_NO_BOM)
            {
                result.clear();
                if (code_type == CodeType::UTF8)
                {
                    result.push_back(-17);
                    result.push_back(-69);
                    result.push_back(-65);
                }
                size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
                if (size <= 0) return string();
                char* str = new char[size + 1];
                WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, str, size, NULL, NULL);
                result.append(str);
                delete[] str;
            }
            else if (code_type == CodeType::UTF16)
            {
                result.clear();
                result.push_back(-1);	//在前面加上UTF16的BOM
                result.push_back(-2);
                result.append((const char*)wstr.c_str(), (const char*)wstr.c_str() + wstr.size() * 2);
                result.push_back('\0');
            }
            if (char_cannot_convert != nullptr)
                *char_cannot_convert = (UsedDefaultChar != FALSE);
            return result;
        }
public slots:
};

#endif // LYRICDOWNLOAD_H
