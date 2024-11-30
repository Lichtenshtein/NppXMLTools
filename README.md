XMLTools
--------
This plugin is a small set of useful tools for editing XML with Notepad++. The plugin is based on MSXML for XSLT 1.0, XPath 1.0 and DTD and XSD based validation. For XSLT 3.0 it uses SaxonC HE 12. The plugin features are:
- XML syntax Check
- XML Schema (XSD) + DTD Validation
- XML tag autoclose
- Pretty print
- Linarize XML
- Current XML Path
- Conversion XML &amp;lt;-&amp;gt; Text
- Comment / Uncomment
- XPath expression evaluation
- XSLT 3.0 transformation with SaxonC HE 12

Authors: Nicolas Crittin (original author of XMLTools based on MSXML), Martin Honnen (SaxonC HE 12 integration for XSLT 3.0)

Homepage: [https://github.com/martin-honnen/xmltools](https://github.com/martin-honnen/xmltools)

Plugin Usage
------------
- You can download the latest release from [https://github.com/martin-honnen/xmltools/releases](https://github.com/martin-honnen/xmltools/releases) (for 64Bit Windows)
- Create a folder XMLTools within Notepad++\Plugins (Typically C:\Program Files\Notepad++\plugins\) and copy the XMLTools.dll and the SaxonC dlls saxonc-core-he.dll and saxonc-he.dll there
- Restart Notepad ++. Now you should be able to see the XMLTools menu within the Plugins section 
- If you encounter this issue when building XMLTools:
```
4>C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Microsoft\VC\v170\Microsoft.CppCommon.targets(159,5): error MSB3073: The command "if EXIST "C:\Program Files (x86)\Notepad++\plugins\" ( xcopy /y "C:\xmltools\Win32\Debug\XMLTools.dll" "C:\Program Files (x86)\Notepad++\plugins\XMLTools\" )
```
Please restart Visual Studio in Administrator mode

