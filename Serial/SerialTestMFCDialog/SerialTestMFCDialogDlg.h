
// SerialTestMFCDialogDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include "SerialMFC.h"

// CSerialTestMFCDialogDlg �Ի���
class CSerialTestMFCDialogDlg : public CDialogEx
{
// ����
public:
	CSerialTestMFCDialogDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERIALTESTMFCDIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void	setDefault();

public:
	CComboBox m_comboPort;
	// �˿�������0��ӦCOM1
	int m_nIndexPort;
	// ��������������
	int m_nIndexBaudRate;
	// ����λ��������
	int m_nIndexByteType;
	// У����������
	int m_nIndexParityType;
	// ֹͣλ��������
	int m_nIndexStopBits;

	CSerialMFC		m_serial;
	CString			m_strPort;		// com port string, eg. COM3

	afx_msg void OnBnClickedButtonComOpen();
	afx_msg void OnBnClickedButtonComSet();
};
