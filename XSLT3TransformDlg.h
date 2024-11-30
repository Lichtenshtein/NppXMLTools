#include "PluginInterface.h"
#include "resource.h"

#include <string.h>


// XSLT3TransformDlg-Dialog

class XSLT3TransformDlg : public CDialog
{
	DECLARE_DYNAMIC(XSLT3TransformDlg)

public:
	XSLT3TransformDlg(CWnd* pParent = nullptr);   // Standardkonstruktor
	virtual ~XSLT3TransformDlg();

	// Dialog Data
	  //{{AFX_DATA(CXSLTransformDlg)
	enum { IDD = IDD_XSLT3DLG };
	int m_input_type;
	CStringW m_xslt30file_selected;


	//}}AFX_DATA


	CStringW ShowOpenFileDlg(CStringW filetypes);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	HWND getCurrentHScintilla(int which);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedBtnXslt3fileselect();
	afx_msg void OnBnClickedRadioInputXml();
	afx_msg void OnBnClickedRadioInputJson();
	afx_msg void OnBnClickedRadioInputNone();
	afx_msg void OnBnClickedBtnXslt3Transform();
	int m_input_type2;
};
