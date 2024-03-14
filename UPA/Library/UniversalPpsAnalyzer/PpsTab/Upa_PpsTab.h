//*****************************************************************************************
// Project:     UniversalConfigurationManager
//
// Author:      Sven Meier, NetTimeLogic GmbH
//
// License:     Copyright (c) 2022, NetTimeLogic GmbH, Switzerland, Sven Meier <contact@nettimelogic.com>
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

#define Upa_MaxSamples                                  100000 // min 10000
#define Upa_PpsPerBoard                                 9 // 1 ref + 8 inputs

namespace Ui {
class Upa_PpsTab;
class Upa_PpsConfigScreen;
class Upa_PpsThresholdScreen;
}

#if QT_MAJOR_VERSION < 6
    using namespace QtCharts;
#endif

class Upa_PpsTs;
class Upa_PpsCtrl;
class Upa_PpsLog;
class Upa_PpsGui;
class Upa_PpsBoard;

typedef struct Upa_PpsRawSample
{
    int pps_active;
    int pps_offset;
    QDateTime pps_fetch_time;
} Upa_PpsRawSample;

typedef struct Upa_PpsLogSample
{
    int pps_active;
    int pps_offset;
    QDateTime pps_fetch_time;
    int pps_threshold_enabled;
    int pps_threshold_high;
    int pps_threshold_low;
    int pps_threshold_high_exceeded;
    int pps_threshold_low_exceeded;
} Upa_PpsLogSample;

typedef struct Upa_PpsGuiSample
{
    int pps_active;
    int pps_offset;
} Upa_PpsGuiSample;

class Upa_PpsTs
{
public:
    Upa_PpsBoard* pps_board;
    unsigned int pps_index;
    Upa_CoreConfig* ts_core_config;
    Upa_PpsRawSample pps_sample;

    Upa_PpsTs();
    ~Upa_PpsTs();
    int pps_disable(void);
    int pps_enable(void);
    int pps_get_sample(void);
};

class Upa_PpsCtrl
{
public:
    Upa_PpsBoard* pps_board;
    unsigned int pps_index;
    int pps_delay;
    int pps_threshold_enabled;
    int pps_threshold_high;
    int pps_threshold_low;
    int pps_threshold_high_exceeded;
    int pps_threshold_low_exceeded;
    QList<Upa_PpsLogSample> pps_samples; // store the last Upa_MaxSamples here

    Upa_PpsCtrl();
    ~Upa_PpsCtrl();
    int pps_disable(void);
    int pps_enable(void);
};

class Upa_PpsLog
{
public:
    Upa_PpsBoard* pps_board;
    unsigned int pps_index;
    QList<Upa_PpsLogSample> pps_samples;

    Upa_PpsLog();
    ~Upa_PpsLog();
    int pps_disable(void);
    int pps_enable(void);
};

class Upa_PpsGui
{
public:
    Upa_PpsBoard* pps_board;
    unsigned int pps_index;
    QString pps_name;
    int pps_show;
    QList<Upa_PpsGuiSample> pps_samples;
    QLineSeries* pps_offset_series;

    Upa_PpsGui();
    ~Upa_PpsGui();
    int pps_disable(void);
    int pps_enable(void);
};

class Upa_PpsBoard
{
public:
    QString com_port;
    unsigned int pps_ref_channel;
    Upa_PpsTs pps_ts[Upa_PpsPerBoard];
    Upa_PpsCtrl pps_ctrl[Upa_PpsPerBoard];
    QMutex mutex_ctrl;
    QMutex mutex_ctrl_threshold;
    QMutex mutex_ctrl_log;
    Upa_PpsLog pps_log[Upa_PpsPerBoard];
    QMutex mutex_ctrl_gui;
    QMutex mutex_gui;
    Upa_PpsGui pps_gui[Upa_PpsPerBoard];

    Upa_PpsBoard();
    ~Upa_PpsBoard();
    int pps_disable(void);
    int pps_enable(void);
};

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
    QList<Upa_PpsBoard*> pps_boards;

private:
    Ui::Upa_PpsTab *ui;
    Upa_PpsConfigScreen* ui_config;
    Upa_PpsThresholdScreen* ui_threshold;

    // PPS tab
    QTimer* pps_ctrl_timer;
    QTimer* pps_log_timer;
    QTimer* pps_gui_timer;
    QTimer* pps_threshold_timer;

    QFile pps_log_file;
    unsigned int pps_log_values;
    unsigned int pps_compensate_values;
    unsigned int pps_zoom_factor;

    QChart* pps_offset_chart;
    QChartView* pps_offset_chart_view;
    QValueAxis* pps_offset_chart_x_axis;
    QValueAxis* pps_offset_chart_y_axis;

    int pps_fetch(Upa_PpsTs* pps_ts);

private slots:
    // PPS tab
    void pps_clear_button_clicked(void);
    void pps_save_view_button_clicked(void);
    void pps_save_values_button_clicked(void);
    void pps_log_button_clicked(void);
    void pps_compensate_values_button_clicked(void);
    void pps_ctrl_timer_run(void);
    void pps_log_timer_run(void);
    void pps_gui_timer_run(void);
    void pps_threshold_timer_run(void);
    void pps_delay_button_clicked(void);
    void pps_threshold_button_clicked(void);
    void pps_zoom_in_button_clicked(void);
    void pps_zoom_out_button_clicked(void);
};

#endif // UPA_PPS_H
