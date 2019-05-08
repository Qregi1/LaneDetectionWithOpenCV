#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <Windows.h>
#include <stdio.h>
#include <string>

namespace LOGGER
{
	//日志级别的提示信息
	static const std::string strErrorPrefix = "Error\t";
	static const std::string strWarningPrefix = "Warning\t";
	static const std::string strInfoPrefix = "Info\t";

	class CLogger
	{
	public:
		//nLogLevel：日志记录的等级，可空
		//strLogPath：日志目录，可空
		//strLogName：日志名称，可空
		CLogger(const std::string strLogPath = "", const std::string strLogName = "");
		//析构函数
		virtual ~CLogger();
	public:
		//写错误信息
		void TraceError(const char *lpcszFormat, ...);
		//写警告信息
		void TraceWarn(const char *lpcszFormat, ...);
		//写提示信息
		void TraceInfo(const char *lpcszFormat, ...);
		//获取程序运行路径
		static std::string GetAppPathA();
	private:
		//写文件操作
		void Trace(const std::string &strLog);
		//获取当前系统时间
		std::string GetTime();
		//文件全路径得到文件名
		const char *path_file(const char *path, char splitter);
		//格式化字符串
		static std::string FormatString(const char *lpcszFormat, ...);
	private:
		//写日志文件流
		FILE * m_pFileStream;
		//日志目录
		std::string m_strLogPath;
		//日志的名称
		std::string m_strLogName;
		//日志文件全路径
		std::string m_strLogFilePath;
		//线程同步的临界区变量
		CRITICAL_SECTION m_cs;
	};
}

#endif // !_LOGGER_H_