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
#include <QtConcurrent/QtConcurrentRun>
#include <QFuture>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCore/QDateTime>
#include <QtCharts/QValueAxis>
#include <Upa_UniversalPpsAnalyzer.h>
#include <Upa_PpsConfigScreen.h>
#include <Upa_PpsThresholdScreen.h>

class Upa_UniversalPpsAnalyzer;
class Upa_PpsConfigScreen;
class Upa_PpsThresholdScreen;

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

#define Upa_IoConf_OutputDataReg                        0x00000000
#define Upa_IoConf_OutputEnableReg                      0x00000004
#define Upa_IoConf_InputDataReg                         0x00000008


namespace Ui {
class Upa_PpsTab;
class Upa_PpsConfigScreen;
class Upa_PpsThresholdScreen;
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
    QList<int*> pps_offsets;
    QList<unsigned int*> pps_offset_number_of_points;
    QList<QString*> pps_offset_names;
    QList<int*> pps_offset_delays;
    QList<int*> pps_offset_show;
    QList<int*> pps_offset_thresholds_high;
    QList<int*> pps_offset_thresholds_low;
    QList<int*> pps_offset_thresholds;
    QList<int*> pps_offset_thresholds_high_exceeded;
    QList<int*> pps_offset_thresholds_low_exceeded;
    unsigned int pps_compensate_values;


private:
    Ui::Upa_PpsTab *ui;
    Upa_PpsConfigScreen* ui_config;
    Upa_PpsThresholdScreen* ui_threshold;

    // PPS tab
    QTimer* pps_timer;
    QFile pps_log_file;
    unsigned int pps_log_values;
    unsigned int pps_zoom_factor;

    QChart* pps_offset_chart;
    QChartView* pps_offset_chart_view;
    QValueAxis* pps_offset_chart_x_axis;
    QValueAxis* pps_offset_chart_y_axis;

    void pps_read_ts(QString com_port);
    void pps_read_values(void);

private slots:
    // PPS tab
    void pps_clear_button_clicked(void);
    void pps_save_button_clicked(void);
    void pps_log_button_clicked(void);
    void pps_compensate_values_button_clicked(void);
    void pps_read_values_timer(void);
    void pps_delay_button_clicked(void);
    void pps_threshold_button_clicked(void);
    void pps_zoom_in_button_clicked(void);
    void pps_zoom_out_button_clicked(void);
};

#endif // UPA_PPS_H
