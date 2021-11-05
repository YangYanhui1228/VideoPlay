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

	//����ý��
	//����һ
	//wstring�ǿ��ַ���ռ��2���ֽڵĴ�С�����UNICODE�����ʽ�����ڶ����ĺ��ֵĶ���͸�ֵ��
	//std::string path = Unicode2Utf8(L"��������.mp4");
	//���ֽ�תunicode����תutf-8
	//libvlc_media_t* media = libvlc_media_new_path(vlc_ins, path.c_str());

	//������
	std::string path = Unicode2Utf8(L"file:///E:\\Project\\VideoPlay\\VideoPlay\\��������.mp4");
	libvlc_media_t* media = libvlc_media_new_location(vlc_ins, path.c_str());

	//����������
	libvlc_media_player_t* player = libvlc_media_player_new_from_media(media);
	do {
		//��ʼ����
		int ret = libvlc_media_player_play(player);
		if (ret == -1) {
			printf("error found!\r\n");
			break;
		}
		int vol = -1;
		while (vol == -1) {
			Sleep(10);//ֻ��ý�����������ɣ��Ż�������Ĳ���
			//��ȡ��������
			vol = libvlc_audio_get_volume(player);//����Ĭ��Ϊ100
		}
		printf("volume is %d\r\n", vol);
		getchar();
		//���ò��ŵ�����
		libvlc_audio_set_volume(player, 10);
		vol = libvlc_audio_get_volume(player);
		printf("volume is %d\r\n", vol);
		//��ȡ���ų��� ms
		libvlc_time_t tm = libvlc_media_player_get_length(player);//����ֵ��ms
		printf("%02d:%02d:%02d.%03d\r\n", int(tm / 3600000), int((tm / 60000) % 60), int((tm / 1000) % 60), INT(tm % 1000));
		//��ȡ����ý��Ŀ�/��
		int width = libvlc_video_get_width(player);
		int height = libvlc_video_get_height(player);
		printf("width=%d height=%d\r\n", width, height);
		while (!_kbhit()) {
			//��ȡ����λ��
			printf("%f%%\r", 100.0 * libvlc_media_player_get_position(player));
			Sleep(500);
		}
		getchar();
		// ���ò���λ��
		libvlc_media_player_set_position(player, 0.15);
		printf("%f%%\r", 100.0 * libvlc_media_player_get_position(player));
		getchar();
		//��ͣ����
		libvlc_media_player_pause(player);
		getchar();
		libvlc_media_player_play(player);
		getchar();
		//ֹͣ����
		libvlc_media_player_stop(player);
	} while (0);
	libvlc_media_player_release(player);
	libvlc_media_release(media);
	libvlc_release(vlc_ins);
	return 0;
}
