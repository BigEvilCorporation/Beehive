#include "MainWindowTemplate.h"

class MainWindow : public MainWindowTemplate 
{
public:
	MainWindow();
	virtual ~MainWindow();

protected:
	virtual void OnBtnToolsMapEdit( wxRibbonButtonBarEvent& event );

private:
	ToolboxMapEditTemplate* m_toolboxMapEdit;
};
