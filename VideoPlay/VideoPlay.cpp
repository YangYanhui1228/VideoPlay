#include <Windows.h>
#include <iostream>
#include "vlc.h"
#include <conio.h>

std::string Unicode2Utf8(const std::wstring& strIn)
{
	std::string str;
	int length = ::WideCharToMultiByte(CP_UTF8, 0, strIn.c_str(), strIn.size(), NULL, 0, NULL, NULL);
	str.resize(length + 1);
	::WideCharToMultiByte(CP_UTF8, 0, strIn.c_str(), strIn.size(), (LPSTR)str.c_str(), length, NULL, NULL);
	return str;
}

int main()
{
	int argc = 1;
	char* argv[2];
	argv[0] = (char*)("--ignore-config");
	libvlc_instance_t* vlc_ins = libvlc_new(argc, argv);

	//加载媒体
	//方法一
	//wstring是宽字符，占用2个字节的大小，针对UNICODE编码格式，用于对中文汉字的定义和赋值。
	//std::string path = Unicode2Utf8(L"股市讨论.mp4");
	//多字节转unicode，再转utf-8
	//libvlc_media_t* media = libvlc_media_new_path(vlc_ins, path.c_str());

	//方法二
	std::string path = Unicode2Utf8(L"file:///E:\\Project\\VideoPlay\\VideoPlay\\股市讨论.mp4");
	libvlc_media_t* media = libvlc_media_new_location(vlc_ins, path.c_str());

	//创建播放器
	libvlc_media_player_t* player = libvlc_media_player_new_from_media(media);
	do {
		//开始播放
		int ret = libvlc_media_player_play(player);
		if (ret == -1) {
			printf("error found!\r\n");
			break;
		}
		int vol = -1;
		while (vol == -1) {
			Sleep(10);//只有媒体解析加载完成，才会有下面的参数
			//获取播放音量
			vol = libvlc_audio_get_volume(player);//音量默认为100
		}
		printf("volume is %d\r\n", vol);
		getchar();
		//设置播放的音量
		libvlc_audio_set_volume(player, 10);
		vol = libvlc_audio_get_volume(player);
		printf("volume is %d\r\n", vol);
		//获取播放长度 ms
		libvlc_time_t tm = libvlc_media_player_get_length(player);//返回值是ms
		printf("%02d:%02d:%02d.%03d\r\n", int(tm / 3600000), int((tm / 60000) % 60), int((tm / 1000) % 60), INT(tm % 1000));
		//获取播放媒体的宽/高
		int width = libvlc_video_get_width(player);
		int height = libvlc_video_get_height(player);
		printf("width=%d height=%d\r\n", width, height);
		while (!_kbhit()) {
			//获取播放位置
			printf("%f%%\r", 100.0 * libvlc_media_player_get_position(player));
			Sleep(500);
		}
		getchar();
		// 设置播放位置
		libvlc_media_player_set_position(player, 0.15);
		printf("%f%%\r", 100.0 * libvlc_media_player_get_position(player));
		getchar();
		//暂停播放
		libvlc_media_player_pause(player);
		getchar();
		libvlc_media_player_play(player);
		getchar();
		//停止播放
		libvlc_media_player_stop(player);
	} while (0);
	libvlc_media_player_release(player);
	libvlc_media_release(media);
	libvlc_release(vlc_ins);
	return 0;
}
