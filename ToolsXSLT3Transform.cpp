#include "StdAfx.h"
#include "XSLT3TransformDlg.h"
#include "Debug.h"

XSLT3TransformDlg* pXSLTransformDlg = NULL;
void performXSLT3Transform() {
    dbgln("performXSLTransform()");

    if (pXSLTransformDlg == NULL) {
        pXSLTransformDlg = new XSLT3TransformDlg(NULL);
        pXSLTransformDlg->Create(XSLT3TransformDlg::IDD);
    }
    pXSLTransformDlg->ShowWindow(SW_SHOW);
}