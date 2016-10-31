
// SerialTestMFCDialogDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#include "SerialMFC.h"

// CSerialTestMFCDialogDlg 对话框
class CSerialTestMFCDialogDlg : public CDialogEx
{
// 构造
public:
	CSerialTestMFCDialogDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SERIALTESTMFCDIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void	setDefault();

public:
	CComboBox m_comboPort;
	// 端口索引，0对应COM1
	int m_nIndexPort;
	// 波特率类型索引
	int m_nIndexBaudRate;
	// 数据位类型索引
	int m_nIndexByteType;
	// 校验类型索引
	int m_nIndexParityType;
	// 停止位类型索引
	int m_nIndexStopBits;

	CSerialMFC		m_serial;
	CString			m_strPort;		// com port string, eg. COM3

	afx_msg void OnBnClickedButtonComOpen();
	afx_msg void OnBnClickedButtonComSet();
};
