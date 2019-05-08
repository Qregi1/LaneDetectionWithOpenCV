#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <Windows.h>
#include <stdio.h>
#include <string>

namespace LOGGER
{
	//��־�������ʾ��Ϣ
	static const std::string strErrorPrefix = "Error\t";
	static const std::string strWarningPrefix = "Warning\t";
	static const std::string strInfoPrefix = "Info\t";

	class CLogger
	{
	public:
		//nLogLevel����־��¼�ĵȼ����ɿ�
		//strLogPath����־Ŀ¼���ɿ�
		//strLogName����־���ƣ��ɿ�
		CLogger(const std::string strLogPath = "", const std::string strLogName = "");
		//��������
		virtual ~CLogger();
	public:
		//д������Ϣ
		void TraceError(const char *lpcszFormat, ...);
		//д������Ϣ
		void TraceWarn(const char *lpcszFormat, ...);
		//д��ʾ��Ϣ
		void TraceInfo(const char *lpcszFormat, ...);
		//��ȡ��������·��
		static std::string GetAppPathA();
	private:
		//д�ļ�����
		void Trace(const std::string &strLog);
		//��ȡ��ǰϵͳʱ��
		std::string GetTime();
		//�ļ�ȫ·���õ��ļ���
		const char *path_file(const char *path, char splitter);
		//��ʽ���ַ���
		static std::string FormatString(const char *lpcszFormat, ...);
	private:
		//д��־�ļ���
		FILE * m_pFileStream;
		//��־Ŀ¼
		std::string m_strLogPath;
		//��־������
		std::string m_strLogName;
		//��־�ļ�ȫ·��
		std::string m_strLogFilePath;
		//�߳�ͬ�����ٽ�������
		CRITICAL_SECTION m_cs;
	};
}

#endif // !_LOGGER_H_