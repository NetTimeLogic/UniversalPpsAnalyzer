#ifndef UCM_PPSDELAYSCREEN_H
#define UCM_PPSDELAYSCREEN_H

#include <QWidget>
#include <ui_Upa_PpsTab.h>

class Upa_PpsTab;

namespace Ui {
class Upa_PpsDelayScreen;
}

class Upa_PpsDelayScreen : public QWidget
{
    Q_OBJECT
private:
    // Pps tab
    Upa_PpsTab* pps_tab;

public:
    Upa_PpsDelayScreen(Upa_PpsTab* parent);
    ~Upa_PpsDelayScreen();
    void pps_reload(void);

    Ui::Upa_PpsDelayScreen *ui;

private slots:
    // PPS Delay screen
    void pps_done_button_clicked(void);
    void pps_change_delays_button_clicked(void);
    void pps_analyzer_changed(int index);
    void pps_graph_changed(void);
};

#endif // UCM_PPSDELAYSCREEN_H
