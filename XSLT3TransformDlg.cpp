// XSLT3TransformDlg.cpp: Implementierungsdatei
//
#include "StdAfx.h"
#include "Scintilla.h"
#include "XMLTools.h"
#include "XSLT3TransformDlg.h"
#include "Report.h"
#include "menuCmdID.h"
#include <string>
#include <codecvt>
#include <locale>

std::string utf16_to_utf8(const std::wstring& utf16_str) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(utf16_str);
}

std::string filePathToUrl(const std::wstring& filePath) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::string utf8Path = converter.to_bytes(filePath);

	std::string url = "file:///";
	for (char c : utf8Path) {
		if (c == '\\') {
			url += '/';
		}
		else {
			url += c;
		}
	}
	return url;
}
// XSLT3TransformDlg-Dialog

IMPLEMENT_DYNAMIC(XSLT3TransformDlg, CDialog)

XSLT3TransformDlg::XSLT3TransformDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_XSLT3DLG, pParent)
	, m_xslt30file_selected(_T(""))
	, m_input_type(0)
	, m_input_type2(0)
{

}

XSLT3TransformDlg::~XSLT3TransformDlg()
{
}

void XSLT3TransformDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_XSLT3FILE_EDIT, m_xslt30file_selected);
	//DDX_Text(pDX, IDC_RADIO_INPUT_XML, m_input_type);
	DDX_Radio(pDX, IDC_RADIO_INPUT_XML, m_input_type2);
}


BEGIN_MESSAGE_MAP(XSLT3TransformDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_XSLT3FILESELECT, &XSLT3TransformDlg::OnBnClickedBtnXslt3fileselect)
	ON_BN_CLICKED(IDC_RADIO_INPUT_XML, &XSLT3TransformDlg::OnBnClickedRadioInputXml)
	ON_BN_CLICKED(IDC_RADIO_INPUT_JSON, &XSLT3TransformDlg::OnBnClickedRadioInputJson)
	ON_BN_CLICKED(IDC_RADIO_INPUT_NONE, &XSLT3TransformDlg::OnBnClickedRadioInputNone)
	ON_BN_CLICKED(IDC_BTN_XSLT3_TRANSFORM, &XSLT3TransformDlg::OnBnClickedBtnXslt3Transform)
END_MESSAGE_MAP()


// XSLT3TransformDlg-Meldungshandler

HWND XSLT3TransformDlg::getCurrentHScintilla(int which) {
	return (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;
}

BOOL XSLT3TransformDlg::OnInitDialog() {
	CDialog::OnInitDialog();

	CRect myRect;
	GetWindowRect(&myRect);

	MoveWindow(myRect.left + 100, myRect.top + 100, myRect.Width(), myRect.Height());

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void XSLT3TransformDlg::OnBnClickedBtnXslt3fileselect()
{
	CStringW ret = ShowOpenFileDlg("XML/XSL Files|*.xml;*.xsl|JSON Files|*.json|All files|*.*|");
	if (ret.GetLength()) GetDlgItem(IDC_XSLT3FILE_EDIT)->SetWindowText(ret);
}


void XSLT3TransformDlg::OnBnClickedRadioInputXml()
{
	//m_input_type = 0;
	UpdateData(TRUE);
}


void XSLT3TransformDlg::OnBnClickedRadioInputJson()
{
	//m_input_type = 1;
	UpdateData(TRUE);

}


void XSLT3TransformDlg::OnBnClickedRadioInputNone()
{
	//m_input_type = 2;
	UpdateData(TRUE);

}


void XSLT3TransformDlg::OnBnClickedBtnXslt3Transform()
{
	this->UpdateData();

	int msg = NPPM_GETFULLCURRENTPATH;

	int currentEdit;
	TCHAR path[MAX_PATH];

	// A message to Notepad++ to get a multibyte string (if ANSI mode) or a wide char string (if Unicode mode)
	::SendMessage(nppData._nppHandle, msg, 0, (LPARAM)path);

	if (m_xslt30file_selected.GetLength() <= 0  && m_input_type2 != 2) {
		Report::_printf_err(L"Cannot continue, missing parameters. Please select an input file or set input type to None.");
		return;
	}

	//if (path == L".xsl" && (m_input_type == 0 || m_input_type == 1)) {
	//	Report::_printf_err(L"Cannot continue, missing parameters. Please select an input file or set input type to None.");
	//	return;
	//}

	std::string fileURI = filePathToUrl(m_xslt30file_selected.GetString());

	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&currentEdit);
	HWND hCurrentEditView = getCurrentHScintilla(currentEdit);

	size_t currentLength = (size_t) ::SendMessage(hCurrentEditView, SCI_GETLENGTH, 0, 0);

	char* data = new char[currentLength + sizeof(char)];
	if (!data) return;  // allocation error, abort check
	memset(data, '\0', currentLength + sizeof(char));

	::SendMessage(hCurrentEditView, SCI_GETTEXT, currentLength + sizeof(char), reinterpret_cast<LPARAM>(data));

	auto xslt30Processor = saxoncHelper->getXslt30Processor();

	std::wstring utf16_str = path;
	std::string utf8_str = utf16_to_utf8(utf16_str);
	utf8_str = utf8_str.substr(0, utf8_str.find_last_of('\\'));
	const char* utf8_cstr = utf8_str.c_str();
	xslt30Processor->setcwd(utf8_cstr);

	XsltExecutable* xsltExecutable = nullptr;

	try {
		//xsltExecutable
		xsltExecutable = xslt30Processor->compileFromString(data, "utf-8");
	}
	catch (SaxonApiException ex) {
		std::string msg = ex.getMessage();
		std::wstring wmsg = std::wstring(msg.begin(), msg.end());
		displayXMLError(wmsg);
		return;
	}

	if (m_input_type2 == 0) {
		std::wstring utf16_str = m_xslt30file_selected.GetString();
		std::string utf8_str = utf16_to_utf8(utf16_str);
		const char* utf8_cstr = utf8_str.c_str();

		try {
			DocumentBuilder* docBuilder = saxoncHelper->getSaxonProcessor()->newDocumentBuilder();
			docBuilder->setBaseUri(filePathToUrl(utf16_str).c_str());
			auto xdmNode = docBuilder->parseXmlFromFile(utf8_cstr);
			try {
				const char* result = xsltExecutable->transformToString(xdmNode);

				::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);
				::SendMessage(hCurrentEditView, SCI_SETTEXT, 0, reinterpret_cast<LPARAM>(result));

				delete result;
			}
			catch (SaxonApiException ex) {
				std::string msg = ex.getMessage();
				std::wstring wmsg = std::wstring(msg.begin(), msg.end());
				displayXMLError(wmsg);
			}
			delete docBuilder;
		}
		catch (SaxonApiException ex) {
			std::string msg = ex.getMessage();
			std::wstring wmsg = std::wstring(msg.begin(), msg.end());
			displayXMLError(wmsg);
		}

	}
	else if (m_input_type2 == 1) {
		std::wstring utf16_str = m_xslt30file_selected.GetString();//.Left(m_xslt30file_selected.ReverseFind(L'/'));
		std::string utf8_str = utf16_to_utf8(utf16_str);
		const char* utf8_cstr = utf8_str.c_str();

		try {
			XdmValue* xdmValue = saxoncHelper->getSaxonProcessor()->parseJsonFromFile(utf8_cstr);

			try {
				xsltExecutable->setInitialMatchSelection(xdmValue);

				XdmItem* xdmItem = xdmValue->getHead();

				xsltExecutable->setGlobalContextItem(xdmItem);

				const char* result = xsltExecutable->applyTemplatesReturningString();

				::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);
				::SendMessage(hCurrentEditView, SCI_SETTEXT, 0, reinterpret_cast<LPARAM>(result));

				delete result;
				delete xdmItem;
			}
			catch (SaxonApiException ex) {
				std::string msg = ex.getMessage();
				std::wstring wmsg = std::wstring(msg.begin(), msg.end());
				displayXMLError(wmsg);
			}

			delete xdmValue;
		}
		catch (SaxonApiException ex) {
			std::string msg = ex.getMessage();
			std::wstring wmsg = std::wstring(msg.begin(), msg.end());
			displayXMLError(wmsg);
		}
	}
	else if (m_input_type2 == 2) {
		std::wstring utf16_str = m_xslt30file_selected.GetString();
		std::string utf8_str = utf16_to_utf8(utf16_str);
		const char* utf8_cstr = utf8_str.c_str();

		xsltExecutable->setcwd(utf8_cstr);

		try {
			const char* result = xsltExecutable->callTemplateReturningString();

			::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);
			::SendMessage(hCurrentEditView, SCI_SETTEXT, 0, reinterpret_cast<LPARAM>(result));

			delete result;
		}
		catch (SaxonApiException ex) {
			std::string msg = ex.getMessage();
			std::wstring wmsg = std::wstring(msg.begin(), msg.end());
			displayXMLError(wmsg);
		}
	}
	delete[] data;
	delete xsltExecutable;

}

CStringW XSLT3TransformDlg::ShowOpenFileDlg(CStringW filetypes) {
	CFileDialog dlg(TRUE, NULL, NULL, NULL, filetypes);
	INT_PTR ret = dlg.DoModal();
	if (ret == IDOK) {
		return dlg.GetPathName();
	}
	return "";
}
