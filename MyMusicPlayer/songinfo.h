#pragma once
#ifndef SONGINFO_H
#define SONGINFO_H
#include<QString>
#include<windows.h>
using namespace std;

//编码信息
enum class CodeType
{
    ANSI,
    UTF8,
    UTF8_NO_BOM,
    UTF16,
    AUTO
};

//一首歌曲的信息
struct SongInfo
{
    wstring file_path{};        //歌曲的文件名
    wstring lyric_file{};	//匹配的歌词文件的路径
    int bitrate{};		//比特率
    wstring title;		//标题
    wstring artist;		//艺术家
    wstring album;		//唱片集
    wstring year;		//年份

    int tag_type{};		//标签的类型（0：其他；1：ID3v1；2：ID3v2）
    bool info_acquired{ false };		//如果已经获取到了信息，则为ture
    wstring album_cover{};		//专辑封面保存的路径
    wstring song_id{};			//歌曲对应的网易云音乐中的歌曲ID
    int listen_time{};			//歌曲累计听的时间（单位为秒）
    bool online_lyric{ false };         //如果为true，则不在线下载歌词
    bool online_cover{ false };   //如果为true，则不在线下载专辑封面
    bool is_favourite{ false };

};




#endif // SONGINFO_H
