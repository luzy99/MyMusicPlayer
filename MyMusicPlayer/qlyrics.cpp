#include "qlyrics.h"
#include "common.h"
#include <QDataStream>
#include <QFile>
#include <iomanip>
#include<vector>
#include<string>
#include<QDebug>
#include<iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include<ios>
using namespace std;

void QLyrics::SaveLyric2()
{
    if (m_lyrics.size() == 0) return;	//没有歌词时直接返回
    QString Qm_file = QString::fromStdWString(m_file);
    QFile p_file(Qm_file);
    p_file.open(QIODevice::WriteOnly);
    //QTextStream 关联文件
    QDataStream data_stram(&p_file);

    //如果歌词编码是UTF8，先在前面输出BOM
    if (m_code_type == CodeType::UTF8)
    {
         char buff[4];
         buff[0] = -17;
         buff[1] = -69;
         buff[2] = -65;
         buff[3] = 0;
         data_stram << buff;
    }
    //输出标识标签
    if(m_id_tag) data_stram <<"[id:" << QString::fromStdString(Common::UnicodeToStr(m_id, m_code_type))<<QString("]") << "\r\n";
    if(m_ti_tag) data_stram <<"[ti:" << QString::fromStdString(Common::UnicodeToStr(m_ti, m_code_type))<< QString("]") << "\r\n";
    if (m_ar_tag) data_stram << "[ar:" << QString::fromStdString(Common::UnicodeToStr(m_ar, m_code_type)) <<QString("]") << "\r\n";
    if (m_al_tag) data_stram << "[al:" << QString::fromStdString(Common::UnicodeToStr(m_al, m_code_type)) << QString("]") << "\r\n";
    if (m_by_tag) data_stram << "[by:" << QString::fromStdString(Common::UnicodeToStr(m_by, m_code_type))<< QString("]")<< "\r\n";
    if (m_offset_tag) data_stram << "[offset:0]" << "\r\n";		//由于偏移量被保存到时间标签中，所以offset标签中的偏移量为0
    char time_buff[16];
    for (auto a_lyric : m_lyrics)
    {
         Time a_time{ a_lyric.GetTime(m_offset) };
         sprintf_s(time_buff, "[%.2d:%.2d.%.2d]", a_time.min, a_time.sec, a_time.msec / 10);
         data_stram << time_buff << QString::fromStdString(Common::UnicodeToStr(a_lyric.text, m_code_type));
         if (!a_lyric.translate.empty())
             data_stram << " / " << QString::fromStdString(Common::UnicodeToStr(a_lyric.translate, m_code_type));
         data_stram <<"\r\n";
    }
    p_file.close();
    m_modified = false;
    m_chinese_converted = false;
}

void QLyrics::CombineSameTimeLyric()
{
    for (int i=0; i < static_cast<int>(m_lyrics.size() - 1); i++)
        {
            if (m_lyrics[i].time == m_lyrics[i + 1].time)	//找到相同时间标签的歌词
            {
                if (!m_lyrics[i].text.empty() && !m_lyrics[i + 1].text.empty())		//只有两句相同时间标签的歌词都有文本时，才需要插入一个斜杠
                {
                    m_lyrics[i].text += L" / ";
                }
                m_lyrics[i].text += m_lyrics[i + 1].text;	//合并两句歌词的文本
                m_lyrics.erase(m_lyrics.begin() + i + 1);	//删除后面一句歌词
            }
        }
}

void QLyrics::DeleteRedundantLyric()
{
    for (size_t i{}; i < m_lyrics.size(); i++)
       {
           if (m_lyrics[i].time > Time{ 100, 0, 0 })		//找到一句歌词的时间标签大于100分钟
           {
               m_lyrics.erase(m_lyrics.begin() + i, m_lyrics.end());		//删除该句歌词及其后面的所有歌词
               break;
           }
       }
}

void QLyrics::AdjustLyric(int offset)
{
    if (m_lyrics.size() == 0) return;	//没有歌词时直接返回
    m_offset += offset;
    m_modified = true;
}
