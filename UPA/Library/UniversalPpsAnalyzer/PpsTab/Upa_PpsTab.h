//*****************************************************************************************
// Project:     UniversalConfigurationManager
//
// Author:      Sven Meier, NetTimeLogic GmbH
//
// License:     Copyright (c) 2017, NetTimeLogic GmbH, Switzerland, Sven Meier <contact@nettimelogic.com>
//              All rights reserved.
//
//              THIS PROGRAM IS FREE SOFTWARE: YOU CAN REDISTRIBUTE IT AND/OR MODIFY
//              IT UNDER THE TERMS OF THE GNU LESSER GENERAL PUBLIC LICENSE AS
//              PUBLISHED BY THE FREE SOFTWARE FOUNDATION, VERSION 3.
//
//              THIS PROGRAM IS DISTRIBUTED IN THE HOPE THAT IT WILL BE USEFUL, BUT
//              WITHOUT ANY WARRANTY; WITHOUT EVEN THE IMPLIED WARRANTY OF
//              MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. SEE THE GNU
//              LESSER GENERAL LESSER PUBLIC LICENSE FOR MORE DETAILS.
//
//              YOU SHOULD HAVE RECEIVED A COPY OF THE GNU LESSER GENERAL PUBLIC LICENSE
//              ALONG WITH THIS PROGRAM. IF NOT, SEE <http://www.gnu.org/licenses/>.
//
//*****************************************************************************************

#ifndef UPA_PPS_H
#define UPA_PPS_H

#include <QWidget>
#include <QTimer>
#include <QFile>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCore/QDateTime>
#include <QtCharts/QValueAxis>
#include <Upa_UniversalPpsAnalyzer.h>
#include <Upa_PpsDelayScreen.h>

class Upa_UniversalPpsAnalyzer;
class Upa_PpsDelayScreen;

#define Upa_ClkTs_ControlReg                            0x00000000
#define Upa_ClkTs_StatusReg                             0x00000004
#define Upa_ClkTs_PolarityReg                           0x00000008
#define Upa_ClkTs_VersionReg                            0x0000000C
#define Upa_ClkTs_CableDelayReg                         0x00000020
#define Upa_ClkTs_IrqReg                                0x00000030
#define Upa_ClkTs_IrqMaskReg                            0x00000034
#define Upa_ClkTs_EvtCountReg                           0x00000038
#define Upa_ClkTs_CountReg                              0x00000040
#define Upa_ClkTs_TimeValueLReg                         0x00000044
#define Upa_ClkTs_TimeValueHReg                         0x00000048
#define Upa_ClkTs_DataWidthReg                          0x0000004C
#define Upa_ClkTs_DataReg                               0x00000050

namespace Ui {
class Upa_PpsTab;
class Upa_PpsDelayScreen;
}

using namespace QtCharts;

class Upa_PpsTab : public QWidget
{
    Q_OBJECT

public:
    Upa_PpsTab(Upa_UniversalPpsAnalyzer* parent);
    ~Upa_PpsTab();

    int pps_resize(int height, int width);
    int pps_disable(void);
    int pps_enable(void);

public:
    Upa_UniversalPpsAnalyzer* upa;
    QList<QLineSeries*> pps_offset_series;
    QList<unsigned int*> pps_offset_number_of_points;
    QList<int*> pps_offset_delays;

private:
    Ui::Upa_PpsTab *ui;
    Upa_PpsDelayScreen* ui_delay;

    // PPS tab
    QTimer* pps_timer;
    unsigned int compensate_values;
    QFile log_file;
    unsigned int log_values;

    QChart* pps_offset_chart;
    QChartView* pps_offset_chart_view;


    void pps_read_values(void);

private slots:
    // PPS tab
    void pps_clear_button_clicked(void);
    void pps_save_button_clicked(void);
    void pps_log_button_clicked(void);
    void pps_compensate_values_button_clicked(void);
    void pps_read_values_timer(void);
    void pps_delay_button_clicked(void);
};

#endif // UPA_PPS_H
