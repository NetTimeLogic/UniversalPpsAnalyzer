//*****************************************************************************************
// Project:     UniversalPpsAnalyzer
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

#ifndef UPA_UNIVERSALCONFIGURATIONMANAGER_H
#define UPA_UNIVERSALCONFIGURATIONMANAGER_H

#include <QScreen>
#include <QCloseEvent>
#include <QWidget>
#include <QTimer>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCore/QDateTime>
#include <QtCharts/QValueAxis>
#include <ui_Upa_UniversalPpsAnalyzer.h>
#include <CommunicationLib/Upa_CommunicationLib.h>
#include <CoreConfig/Upa_CoreConfig.h>
#include <DebugStream/Upa_DebugStream.h>

#include <ConfigTab/Upa_ConfigTab.h>
#include <AdvancedTab/Upa_AdvancedTab.h>
#include <PpsTab/Upa_PpsTab.h>

using namespace QtCharts;

class Upa_ConfigTab;
class Upa_AdvancedTab;
class Upa_PpsTab;

#define Upa_MainHeight               820
#define Upa_MainWidth                1380

class Upa_UniversalPpsAnalyzer : public QMainWindow, public Ui::Upa_UniversalPpsAnalyzer
{
    Q_OBJECT

    public:
        Upa_UniversalPpsAnalyzer(QMainWindow *parent = 0);
        ~Upa_UniversalPpsAnalyzer();

        int upa_resize(int height, int width);

        QList<Upa_CommunicationLib*> com_lib;
        QList<Upa_CoreConfig> core_config;

        // Config tab
        Upa_ConfigTab* config_tab;

        // Advcanced tab
        Upa_AdvancedTab* advanced_tab;

        // Pps tab
        Upa_PpsTab* pps_tab;

private:
        void resizeEvent(QResizeEvent* event);
        void closeEvent(QCloseEvent *event);

private slots:
        // File menu
        void upa_file_exit_clicked(void);

};

#endif // UPA_UNIVERSALCONFIGURATIONMANAGER_H
