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

#include <QStandardPaths>

#include <Upa_PpsTab.h>
#include <ui_Upa_PpsTab.h>
#include <ui_Upa_PpsConfigScreen.h>
#include <ui_Upa_PpsThresholdScreen.h>

Upa_PpsTs::Upa_PpsTs()
{
    pps_board = NULL;
    ts_core_config = NULL;
    pps_sample.pps_active = 0;
    pps_sample.pps_offset = 0;
    pps_sample.pps_fetch_time = QDateTime::currentDateTime();
}

Upa_PpsTs::~Upa_PpsTs()
{
    pps_board = NULL;
    ts_core_config = NULL;
}

int Upa_PpsTs::pps_disable(void)
{
    //unsigned int temp_data = 0;
    //unsigned int temp_addr = 0;
    //QString temp_string;

    pps_sample.pps_active = 0;
    pps_sample.pps_offset = 0;
    pps_sample.pps_fetch_time = QDateTime::currentDateTime();

    // since we support multiuser we can not disable
    //temp_addr =  ts_core_config->address_range_low;
    //temp_string = pps_board->pps_gui[pps_index].pps_name;
    //temp_data = 0x00000000;
    //if (0 == ts_core_config->com_lib->write_reg(temp_addr + Upa_ClkTs_ControlReg, temp_data))
    //{
    //    cout << "VERBOSE: " << "enabled " << temp_string.toLatin1().constData() << endl;
    //}
    //else
    //{
    //    cout << "ERROR: " << "could not enable" << temp_string.toLatin1().constData() << endl;
    //    return -1;
    //}
    //temp_data = 0x00000000;
    //if (0 == ts_core_config->com_lib->write_reg(temp_addr + Upa_ClkTs_IrqMaskReg, temp_data))
    //{
    //    cout << "VERBOSE: " << "enabled irq on " << temp_string.toLatin1().constData() << endl;
    //}
    //else
    //{
    //    cout << "ERROR: " << "could not enable irq on" << temp_string.toLatin1().constData() << endl;
    //    return -1;
    //}

    return 0;
}

int Upa_PpsTs::pps_enable(void)
{
    unsigned int temp_data = 0;
    unsigned int temp_addr = 0;
    QString temp_string;

    pps_sample.pps_active = 0;
    pps_sample.pps_offset = 0;
    pps_sample.pps_fetch_time = QDateTime::currentDateTime();

    temp_addr =  ts_core_config->address_range_low;
    temp_string = pps_board->pps_gui[pps_index].pps_name;
    temp_data = 0x00000001;
    if (0 == ts_core_config->com_lib->write_reg(temp_addr + Upa_ClkTs_ControlReg, temp_data))
    {
        cout << "VERBOSE: " << "enabled " << temp_string.toLatin1().constData() << endl;
    }
    else
    {
        cout << "ERROR: " << "could not enable" << temp_string.toLatin1().constData() << endl;
        return -1;
    }
    temp_data = 0x00000001;
    if (0 == ts_core_config->com_lib->write_reg(temp_addr + Upa_ClkTs_IrqMaskReg, temp_data))
    {
        cout << "VERBOSE: " << "enabled irq on " << temp_string.toLatin1().constData() << endl;
    }
    else
    {
        cout << "ERROR: " << "could not enable irq on" << temp_string.toLatin1().constData() << endl;
        return -1;
    }

    return 0;
}


Upa_PpsCtrl::Upa_PpsCtrl()
{
    pps_board = NULL;
    pps_index = 0;
    pps_delay = 0;
    pps_threshold_enabled = 0;
    pps_threshold_high = 0;
    pps_threshold_low = 0;
    pps_threshold_high_exceeded = 0;
    pps_threshold_low_exceeded = 0;
    pps_samples.clear();
}

Upa_PpsCtrl::~Upa_PpsCtrl()
{
    pps_board = NULL;
    pps_index = 0;
    pps_samples.clear();
}

int Upa_PpsCtrl::pps_disable(void)
{
    pps_samples.clear();
    pps_threshold_high_exceeded = 0;
    pps_threshold_low_exceeded = 0;
    return 0;
}

int Upa_PpsCtrl::pps_enable(void)
{
    pps_samples.clear();
    pps_threshold_high_exceeded = 0;
    pps_threshold_low_exceeded = 0;
    return 0;
}


Upa_PpsLog::Upa_PpsLog()
{
    pps_board = NULL;
    pps_index = 0;
    pps_samples.clear();
}

Upa_PpsLog::~Upa_PpsLog()
{
    pps_board = NULL;
    pps_index = 0;
    pps_samples.clear();
}

int Upa_PpsLog::pps_disable(void)
{
    pps_samples.clear();
    return 0;
}

int Upa_PpsLog::pps_enable(void)
{
    pps_samples.clear();
    return 0;
}


Upa_PpsGui::Upa_PpsGui()
{
    pps_board = NULL;
    pps_index = 0;
    pps_name.clear();
    pps_show = 0;
    pps_samples.clear();
    pps_offset_series = NULL;
}

Upa_PpsGui::~Upa_PpsGui()
{
    pps_board = NULL;
    pps_index = 0;
    pps_name.clear();
    pps_samples.clear();
    pps_offset_series = NULL;
}

int Upa_PpsGui::pps_disable(void)
{
    pps_samples.clear();
    pps_offset_series->clear();
    return 0;
}

int Upa_PpsGui::pps_enable(void)
{
    pps_samples.clear();
    pps_offset_series->clear();
    return 0;
}


Upa_PpsBoard::Upa_PpsBoard()
{
    com_port.clear();
    for (int i = 0; i < Upa_PpsPerBoard; i++)
    {
        pps_ts[i].pps_board = this;
        pps_ctrl[i].pps_board = this;
        pps_log[i].pps_board = this;
        pps_gui[i].pps_board = this;
        pps_ts[i].pps_index = i;
        pps_ctrl[i].pps_index = i;
        pps_log[i].pps_index = i;
        pps_gui[i].pps_index = i;
    }
}

Upa_PpsBoard::~Upa_PpsBoard()
{
    com_port.clear();
    for (int i = 0; i < Upa_PpsPerBoard; i++)
    {
        pps_ts[i].pps_board = NULL;
        pps_ctrl[i].pps_board = NULL;
        pps_log[i].pps_board = NULL;
        pps_gui[i].pps_board = NULL;
    }
}

int Upa_PpsBoard::pps_disable(void)
{
    mutex_ctrl.lock();
    mutex_ctrl_threshold.lock();
    mutex_ctrl_log.lock();
    mutex_ctrl_gui.lock();
    mutex_gui.lock();
    for (int i = 0; i < Upa_PpsPerBoard; i++)
    {
        pps_ts[i].pps_disable();
        pps_ctrl[i].pps_disable();
        pps_log[i].pps_disable();
        pps_gui[i].pps_disable();
    }
    mutex_gui.unlock();
    mutex_ctrl_gui.unlock();
    mutex_ctrl_log.unlock();
    mutex_ctrl_threshold.unlock();
    mutex_ctrl.unlock();
    return 0;
}

int Upa_PpsBoard::pps_enable(void)
{
    mutex_ctrl.lock();
    mutex_ctrl_threshold.lock();
    mutex_ctrl_log.lock();
    mutex_ctrl_gui.lock();
    mutex_gui.lock();
    for (int i = 0; i < Upa_PpsPerBoard; i++)
    {
        pps_ts[i].pps_enable();
        pps_ctrl[i].pps_enable();
        pps_log[i].pps_enable();
        pps_gui[i].pps_enable();
    }
    mutex_gui.unlock();
    mutex_ctrl_gui.unlock();
    mutex_ctrl_log.unlock();
    mutex_ctrl_threshold.unlock();
    mutex_ctrl.unlock();
    return 0;
}


Upa_PpsTab::Upa_PpsTab(Upa_UniversalPpsAnalyzer *parent) : QWidget()
{
    QList<QString> temp_com_ports;
    unsigned int temp_data = 0;
    unsigned int temp_addr = 0;

    upa = parent;

    ui = new Ui::Upa_PpsTab();
    ui->setupUi(this);

    // timer and slots
    pps_ctrl_timer = new QTimer(this);
    pps_ctrl_timer->stop();
    pps_log_timer = new QTimer(this);
    pps_log_timer->setSingleShot(true);
    pps_log_timer->stop();
    pps_gui_timer = new QTimer(this);
    pps_gui_timer->setSingleShot(true);
    pps_gui_timer->stop();
    pps_threshold_timer = new QTimer(this);
    pps_threshold_timer->setSingleShot(true);
    pps_threshold_timer->stop();

    connect(ui->PpsClearButton, SIGNAL(clicked()), this, SLOT(pps_clear_button_clicked()));
    connect(ui->PpsSaveViewButton, SIGNAL(clicked()), this, SLOT(pps_save_view_button_clicked()));
    connect(ui->PpsSaveValuesButton, SIGNAL(clicked()), this, SLOT(pps_save_values_button_clicked()));
    connect(ui->PpsLogButton, SIGNAL(clicked()), this, SLOT(pps_log_button_clicked()));
    connect(ui->PpsCompensateValuesButton, SIGNAL(clicked()), this, SLOT(pps_compensate_values_button_clicked()));
    connect(ui->PpsChangeDelaysButton, SIGNAL(clicked()), this, SLOT(pps_delay_button_clicked()));
    connect(ui->PpsSetThresholdsButton, SIGNAL(clicked()), this, SLOT(pps_threshold_button_clicked()));
    connect(ui->PpsZoomInButton, SIGNAL(clicked()), this, SLOT(pps_zoom_in_button_clicked()));
    connect(ui->PpsZoomOutButton, SIGNAL(clicked()), this, SLOT(pps_zoom_out_button_clicked()));
    connect(pps_ctrl_timer, SIGNAL(timeout()), this, SLOT(pps_ctrl_timer_run()));
    connect(pps_log_timer, SIGNAL(timeout()), this, SLOT(pps_log_timer_run()));
    connect(pps_gui_timer, SIGNAL(timeout()), this, SLOT(pps_gui_timer_run()));
    connect(pps_threshold_timer, SIGNAL(timeout()), this, SLOT(pps_threshold_timer_run()));

    // search for all different com ports
    for (int i = 0; i < upa->ts_core_config.size(); i++)
    {
        int j = 0;
        for (j = 0; j < temp_com_ports.size(); j++)
        {
            if (0 == temp_com_ports.at(j).compare(upa->ts_core_config.at(i).com_port))
            {
                break;
            }
        }
        if (j == temp_com_ports.size())
        {
            temp_com_ports.append(upa->ts_core_config.at(i).com_port);
        }
    }

    // create the boards and assign the com port name
    for (int i = 0; i < temp_com_ports.size(); i++)
    {
        Upa_PpsBoard* temp_board = new Upa_PpsBoard();
        temp_board->com_port.clear();
        temp_board->com_port = temp_com_ports.at(i);

        pps_boards.append(temp_board);
    }

    // connect the right timestampers of that board to the modules
    for (int i = 0; i < pps_boards.size(); i++)
    {
        for (int j = 0; j < Upa_PpsPerBoard; j++)
        {
            for (int k = 0; k < upa->ts_core_config.size(); k++)
            {
                if ((0 == pps_boards.at(i)->com_port.compare(upa->ts_core_config.at(k).com_port)) &&
                    ((j+1) == upa->ts_core_config.at(k).core_instance_nr))
                {
                    pps_boards.at(i)->pps_ts[j].ts_core_config = (Upa_CoreConfig*)&(upa->ts_core_config.at(k));
                }
            }
        }
    }

    // set the control part to 0
    for (int i = 0; i < pps_boards.size(); i++)
    {
        for (int j = 0; j < Upa_PpsPerBoard; j++)
        {
            if (j == 0) // this is the reference
            {
                pps_boards.at(i)->pps_ctrl[j].pps_delay = 0;
                pps_boards.at(i)->pps_ctrl[j].pps_threshold_enabled = 1;
                pps_boards.at(i)->pps_ctrl[j].pps_threshold_high = 50;
                pps_boards.at(i)->pps_ctrl[j].pps_threshold_low = -50;
            }
            else
            {
                pps_boards.at(i)->pps_ctrl[j].pps_delay = 0;
                pps_boards.at(i)->pps_ctrl[j].pps_threshold_enabled = 0;
                pps_boards.at(i)->pps_ctrl[j].pps_threshold_high = 1000;
                pps_boards.at(i)->pps_ctrl[j].pps_threshold_low = -1000;
            }
        }
    }

    //general part
    pps_compensate_values = 1;

    //log part
    pps_log_values = 0;

    // gui parts
    pps_zoom_factor = 100;
    ui->PpsZoomInButton->setEnabled(false);
    ui->PpsZoomOutButton->setEnabled(true); // now we can zoom out for sure

    pps_offset_chart = new QChart();
    pps_offset_chart->setContentsMargins(0, 0, 0, 0);
    pps_offset_chart->setBackgroundRoundness(0);
    pps_offset_chart->setBackgroundBrush(Qt::white);

    pps_offset_chart_x_axis = new QValueAxis;
    pps_offset_chart_x_axis->setLabelFormat("%i");
    pps_offset_chart_x_axis->setTickCount(21);
    pps_offset_chart_x_axis->setMin(0);
    pps_offset_chart_x_axis->setMax(100);
    pps_offset_chart->addAxis(pps_offset_chart_x_axis, Qt::AlignBottom);

    pps_offset_chart_y_axis = new QValueAxis;
    pps_offset_chart_y_axis->setLabelFormat("%i");
    pps_offset_chart_y_axis->setTickCount(11);
    pps_offset_chart_y_axis->setMin(-100);
    pps_offset_chart_y_axis->setMax(100);
    pps_offset_chart->addAxis(pps_offset_chart_y_axis, Qt::AlignLeft);

    // create the line series and initialize them
    for (int i = 0; i < pps_boards.size(); i++)
    {
        for (int j = 0; j < Upa_PpsPerBoard; j++)
        {
            QString temp_name;
            QLineSeries* temp_line_series = new QLineSeries();
            if (upa->use_open_gl == 1)
            {
                temp_line_series->setUseOpenGL(true);
            }
            QPen temp_pen;
            temp_pen.setWidth(2);
            temp_line_series->setPen(temp_pen);

            temp_name.clear();
            temp_name.append(pps_boards.at(i)->com_port);
            temp_name.append("_PPS_");
            if (j == 0) // this is the reference
            {
                temp_name.append("REF");
                QColor temp_color;
                temp_color.setRgb(230, 230, 230);
                temp_line_series->setColor(temp_color);
            }
            else
            {
                temp_name.append(QString::number(j));
                QColor temp_color;
                switch (j) // only 8 Input PPS per analyzer
                {
                    case 1:
                        temp_color.setRgb(255, 64, 64);
                        temp_line_series->setColor(temp_color);
                        break;
                    case 2:
                        temp_color.setRgb(64, 255, 64);
                        temp_line_series->setColor(temp_color);
                        break;
                    case 3:
                        temp_color.setRgb(64, 64, 255);
                        temp_line_series->setColor(temp_color);
                        break;
                    case 4:
                        temp_color.setRgb(255, 64, 255);
                        temp_line_series->setColor(temp_color);
                        break;
                    case 5:
                        temp_color.setRgb(160, 64, 64);
                        temp_line_series->setColor(temp_color);
                        break;
                    case 6:
                        temp_color.setRgb(64, 160, 64);
                        temp_line_series->setColor(temp_color);
                        break;
                    case 7:
                        temp_color.setRgb(64, 64, 160);
                        temp_line_series->setColor(temp_color);
                        break;
                    case 8:
                        temp_color.setRgb(160, 64, 160);
                        temp_line_series->setColor(temp_color);
                        break;
                    default:
                        temp_line_series->setColor(Qt::black);
                        break;
                }
            }

            temp_line_series->setName(temp_name);

            pps_offset_chart->addSeries(temp_line_series);
            temp_line_series->attachAxis(pps_offset_chart_x_axis);
            temp_line_series->attachAxis(pps_offset_chart_y_axis);

            pps_boards.at(i)->pps_gui[j].pps_show = 1; // default show all
            pps_boards.at(i)->pps_gui[j].pps_name = temp_name;
            pps_boards.at(i)->pps_gui[j].pps_offset_series = temp_line_series;
        }
    }

    pps_offset_chart_view = new QChartView(pps_offset_chart);
    pps_offset_chart_view->setRenderHint(QPainter::Antialiasing);

    ui->PpsOffsetChartLayout->addWidget(pps_offset_chart_view, 0, 0);

    // disable all of the parts (set to a defined value)
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->pps_disable();
    }

    // get the configs
    ui_config = new Upa_PpsConfigScreen(this);
    ui_config->pps_reload();

    ui_threshold = new Upa_PpsThresholdScreen(this);
    ui_threshold->pps_reload();

    pps_offset_chart->legend()->setVisible(true);
}

Upa_PpsTab::~Upa_PpsTab()
{
    pps_ctrl_timer->stop();
    pps_log_timer->stop();
    pps_gui_timer->stop();
    pps_threshold_timer->stop();

    // stop loging
    if (pps_log_values != 0)
    {
        pps_log_values = 0;
        pps_log_file.close();
        ui->PpsLogButton->setText("Start Log");
    }

    // disable
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->pps_disable();
    }

    // remove gui parts
    for (int i = 0; i < pps_boards.size(); i++)
    {
        for (int j = 0; j < Upa_PpsPerBoard; j++)
        {
            delete pps_boards.at(i)->pps_gui[j].pps_offset_series;
        }
    }
    delete pps_offset_chart;
    delete pps_offset_chart_x_axis;
    delete pps_offset_chart_y_axis;
    delete ui;
    ui_config->close();
    delete ui_config;
    ui_threshold->close();
    delete ui_threshold;

    // remove timers
    delete pps_ctrl_timer;
    delete pps_log_timer;
    delete pps_gui_timer;
    delete pps_threshold_timer;

    for (int i = 0; i < pps_boards.size(); i++)
    {
        delete pps_boards.at(i);
    }
    pps_boards.clear();

    delete pps_offset_chart_view;
}

int Upa_PpsTab::pps_resize(int height, int width)
{
    int height_delta = 0;
    int width_delta = 0;

    height_delta = (height-Upa_MainHeight);
    width_delta = (width-Upa_MainWidth);

    ui->PpsOffsetChartValue->setFixedHeight(700+height_delta);
    ui->PpsOffsetChartValue->setFixedWidth(1340+width_delta);
    ui->PpsOffsetChartLabel->setFixedWidth(1340+width_delta);

    updateGeometry();

    return 0;
}

int Upa_PpsTab::pps_enable(void)
{
    // enable
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->pps_enable();
    }

    pps_ctrl_timer->start(1000);

    return 0;
}

int Upa_PpsTab::pps_disable(void)
{
    pps_ctrl_timer->stop();
    pps_log_timer->stop();
    pps_gui_timer->stop();
    pps_threshold_timer->stop();

    if (pps_log_values != 0)
    {
        pps_log_values = 0;
        pps_log_file.close();
        ui->PpsLogButton->setText("Start Log");
    }

    // disable
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->pps_disable();
    }

    return 0;
}

int Upa_PpsTab::pps_fetch(Upa_PpsTs* pps_ts)
{
    unsigned int temp_data = 0;
    unsigned int temp_addr = 0;

    //********************************
    // get timestamp
    //********************************
    temp_addr = pps_ts->ts_core_config->address_range_low;

    pps_ts->pps_sample.pps_fetch_time = QDateTime::currentDateTime();

    if (0 == pps_ts->ts_core_config->com_lib->read_reg(temp_addr + Upa_ClkTs_StatusReg, temp_data)) // check if the PPS has activity at all (within the last 1.5s)
    {
        if ((temp_data & 0x00000002) != 0)
        {
            pps_ts->pps_sample.pps_active = 1;

            // nanoseconds
            if (0 == pps_ts->ts_core_config->com_lib->read_reg(temp_addr + Upa_ClkTs_TimeValueLReg, temp_data)) // check if the PPS has activity at all (within the last 1.5s)
            {
                if(temp_data > 500000000)
                {
                    pps_ts->pps_sample.pps_offset = (int)temp_data - 1000000000;
                }
                else
                {
                    pps_ts->pps_sample.pps_offset = (int)temp_data;
                }
                return 0;
            }
            else
            {
                pps_ts->pps_sample.pps_active = 0;
                pps_ts->pps_sample.pps_offset = 0;
                cout << "ERROR: " << "could not read sample" << endl;
                return -1;
            }
        }
        else
        {
            pps_ts->pps_sample.pps_active = 0;
            pps_ts->pps_sample.pps_offset = 0;
        }
    }
    else
    {
        pps_ts->pps_sample.pps_active = 0;
        pps_ts->pps_sample.pps_offset = 0;
        cout << "ERROR: " << "could not read sample" << endl;
        return -1;
    }
    return -1;
}


void Upa_PpsTab::pps_clear_button_clicked(void)
{
    // lock all passing of samples
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_gui.lock();
    }
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_ctrl_gui.lock();
    }

    for (int i = 0; i < pps_boards.size(); i++)
    {
        for (int j = 0; j < Upa_PpsPerBoard; j++)
        {
            pps_boards.at(i)->pps_gui[j].pps_samples.clear();
            pps_boards.at(i)->pps_gui[j].pps_offset_series->clear();
        }
    }

    pps_offset_chart->axisY()->setMin(-100);
    pps_offset_chart->axisY()->setMax(100);

    // unlock all passing of samples
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_ctrl_gui.unlock();
    }
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_gui.unlock();
    }

    pps_offset_chart->show();
}

void Upa_PpsTab::pps_save_view_button_clicked(void)
{
    QDateTime temp_time = QDateTime::currentDateTime();
    QString temp_string = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0);
    temp_string.append("/pps_chart_");
    temp_string.append(temp_time.toString("dd-MM-yyyy_hh-mm-ss"));

    QString temp_filename = QFileDialog::getSaveFileName(this, "save chart", temp_string, "PNG(*.png);; TIFF(*.tiff *.tif);; JPEG(*.jpg *.jpeg)");

    ui->PpsOffsetChartValue->grab().save(temp_filename);
}

void Upa_PpsTab::pps_log_button_clicked(void)
{
    QDateTime temp_time = QDateTime::currentDateTime();
    QString temp_string = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0);
    temp_string.append("/pps_log_");
    temp_string.append(temp_time.toString("dd-MM-yyyy_hh-mm-ss"));

    if (ui->PpsLogButton->text() == "Start Log")
    {
        QString temp_filename = QFileDialog::getSaveFileName(this, "save log", temp_string, "CSV(*.csv)");

        if (temp_filename != "")
        {
            pps_log_file.setFileName(temp_filename);
            if (true == pps_log_file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream temp_stream(&pps_log_file);

                temp_stream << "sep=;\n";
                temp_stream << "Timestamp";
                for (int i = 0; i < pps_boards.size(); i++)
                {
                    for (int j = 0; j < Upa_PpsPerBoard; j++)
                    {
                        temp_stream << ";" << pps_boards.at(i)->pps_gui[j].pps_name;
                    }
                }
                for (int i = 0; i < pps_boards.size(); i++)
                {
                    for (int j = 0; j < Upa_PpsPerBoard; j++)
                    {
                        temp_stream << ";" << pps_boards.at(i)->pps_gui[j].pps_name << " threshold high ("  << QString::number(pps_boards.at(i)->pps_ctrl[j].pps_threshold_high) << ") exceeded";
                        temp_stream << ";" << pps_boards.at(i)->pps_gui[j].pps_name << " threshold low ("  << QString::number(pps_boards.at(i)->pps_ctrl[j].pps_threshold_low) << ") exceeded";
                    }
                }
                temp_stream << "\n";

                pps_log_values = 1;
                ui->PpsLogButton->setText("Stop Log");
            }
            else
            {
                pps_log_values = 0;
                pps_log_file.setFileName("");
                cout << "ERROR: " << "could not open log file" << endl;
            }
        }
        else
        {
            pps_log_values = 0;
            cout << "ERROR: " << "empty log file name" << endl;
        }
    }
    else
    {
        QMessageBox temp_msg_box;
        temp_msg_box.setText("Do you realy want to stop logging?");
        temp_msg_box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        temp_msg_box.setDefaultButton(QMessageBox::Cancel);

        int ret = temp_msg_box.exec();
        switch (ret)
        {
            case QMessageBox::Ok:
                pps_log_values = 0;
                pps_log_file.close();
                ui->PpsLogButton->setText("Start Log");
                break;

            default:
                break;
        }
    }
}

void Upa_PpsTab::pps_save_values_button_clicked(void)
{
    unsigned int temp_number_of_points;
    int temp_index;
    QFile temp_file;
    Upa_PpsLogSample temp_log_sample;
    QDateTime temp_time = QDateTime::currentDateTime();
    QString temp_string = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0);
    temp_string.append("/pps_values_");
    temp_string.append(temp_time.toString("dd-MM-yyyy_hh-mm-ss"));

    QString temp_filename = QFileDialog::getSaveFileName(this, "save values", temp_string, "CSV(*.csv)");

    if (temp_filename != "")
    {
        temp_file.setFileName(temp_filename);
        if (true == temp_file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream temp_stream(&temp_file);

            temp_stream << "sep=;\n";
            temp_stream << "Timestamp";
            for (int i = 0; i < pps_boards.size(); i++)
            {
                for (int j = 0; j < Upa_PpsPerBoard; j++)
                {
                    temp_stream << ";" << pps_boards.at(i)->pps_gui[j].pps_name;
                }
            }
            for (int i = 0; i < pps_boards.size(); i++)
            {
                for (int j = 0; j < Upa_PpsPerBoard; j++)
                {
                    temp_stream << ";" << pps_boards.at(i)->pps_gui[j].pps_name << " threshold high ("  << QString::number(pps_boards.at(i)->pps_ctrl[j].pps_threshold_high) << ") exceeded";
                    temp_stream << ";" << pps_boards.at(i)->pps_gui[j].pps_name << " threshold low ("  << QString::number(pps_boards.at(i)->pps_ctrl[j].pps_threshold_low) << ") exceeded";
                }
            }
            temp_stream << "\n";

            // lock log
            for (int i = 0; i < pps_boards.size(); i++)
            {
                pps_boards.at(i)->mutex_ctrl.lock();
            }

            temp_string.clear();
            for (int k = 0; k < pps_boards.at(0)->pps_ctrl[0].pps_samples.size(); k++)
            {
                for (int i = 0; i < pps_boards.size(); i++)
                {
                    for (int j = 0; j < Upa_PpsPerBoard; j++)
                    {
                        temp_log_sample = pps_boards.at(i)->pps_ctrl[j].pps_samples.at(k);
                        if ((i==0) && (j==0))
                        {
                            temp_string.append(temp_log_sample.pps_fetch_time.toString("dd-MM-yyyy hh:mm:ss"));
                        }
                        temp_string.append(";");
                        if (temp_log_sample.pps_active != 0)
                        {
                            temp_string.append(QString::number(temp_log_sample.pps_offset));
                        }
                        else
                        {
                            temp_string.append("-");
                        }
                    }
                }

                for (int i = 0; i < pps_boards.size(); i++)
                {
                    for (int j = 0; j < Upa_PpsPerBoard; j++)
                    {
                        temp_log_sample = pps_boards.at(i)->pps_ctrl[j].pps_samples.at(k);
                        if ((temp_log_sample.pps_active != 0) &&
                            (temp_log_sample.pps_threshold_enabled != 0))
                        {
                            temp_string.append(";");
                            if (temp_log_sample.pps_threshold_high_exceeded == 0)
                            {
                                temp_string.append("n");
                            }
                            else
                            {
                                temp_string.append("y");
                            }
                            temp_string.append(";");
                            if (temp_log_sample.pps_threshold_low_exceeded == 0)
                            {
                                temp_string.append("n");
                            }
                            else
                            {
                                temp_string.append("y");
                            }
                        }
                        else
                        {
                            temp_string.append(";");
                            temp_string.append("-");
                            temp_string.append(";");
                            temp_string.append("-");
                        }
                    }
                }

                temp_string.append("\n");
            }

            // lock log
            for (int i = 0; i < pps_boards.size(); i++)
            {
                pps_boards.at(i)->mutex_ctrl.unlock();
            }

            // now write out the file
            temp_stream << temp_string;


            temp_file.close();

        }
        else
        {
            temp_file.setFileName("");
            cout << "ERROR: " << "could not open values file" << endl;
        }
    }
    else
    {
        cout << "ERROR: " << "empty values file name" << endl;
    }
}

void Upa_PpsTab::pps_compensate_values_button_clicked(void)
{
    if (ui->PpsCompensateValuesButton->text() == "Raw Values")
    {
        pps_compensate_values = 0;
        ui->PpsCompensateValuesButton->setText("Compensate Values");
    }
    else
    {
        pps_compensate_values = 1;
        ui->PpsCompensateValuesButton->setText("Raw Values");
    }
}

void Upa_PpsTab::pps_delay_button_clicked(void)
{
    ui_config->pps_reload();
    ui_config->setModal(true);
    ui_config->exec();
    //ui_config->show();
    //ui_config->activateWindow();
    //ui_config->raise();
}

void Upa_PpsTab::pps_threshold_button_clicked(void)
{
    ui_threshold->pps_reload();
    ui_threshold->setModal(true);
    ui_threshold->exec();
    //ui_threshold->pps_reload();
    //ui_threshold->show();
    //ui_threshold->activateWindow();
    //ui_threshold->raise();
}


void Upa_PpsTab::pps_ctrl_timer_run(void)
{
    QDateTime temp_time;
    int temp_offset_ref;
    int temp_offset;
    int temp_active;
    int temp_threshold_high_exceeded;
    int temp_threshold_low_exceeded;
    Upa_PpsGuiSample temp_gui_sample;
    Upa_PpsLogSample temp_log_sample;

    // just take a snapshot
    temp_time = QDateTime::currentDateTime();

    //********************************
    // fetch all timestamps from all boards
    //********************************
    for (int i = 0; i < pps_boards.size(); i++)
    {
        for (int j = 0; j < Upa_PpsPerBoard; j++)
        {
            pps_fetch(&(pps_boards.at(i)->pps_ts[j]));
        }
    }

    //********************************
    // calculations and forwarding
    //********************************
    // lock all passing of samples
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_ctrl.lock();
    }
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_ctrl_threshold.lock();
    }
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_ctrl_gui.lock();
    }
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_ctrl_log.lock();
    }


    // calculate and push things
    for (int i = 0; i < pps_boards.size(); i++)
    {
        if (pps_boards.at(i)->pps_ts[0].pps_sample.pps_active != 0)
        {
            temp_offset_ref = pps_boards.at(i)->pps_ts[0].pps_sample.pps_offset;
            temp_offset_ref += pps_boards.at(i)->pps_ctrl[0].pps_delay;
        }
        else
        {
            temp_offset_ref = 0;
        }
        for (int j = 0; j < Upa_PpsPerBoard; j++)
        {
            if (pps_boards.at(i)->pps_ts[j].pps_sample.pps_active != 0)
            {
                temp_active = 1;
                if (j == 0)
                {
                    temp_offset = pps_boards.at(i)->pps_ts[j].pps_sample.pps_offset;
                    temp_offset -= pps_boards.at(i)->pps_ctrl[j].pps_delay;
                }
                else
                {
                    temp_offset = pps_boards.at(i)->pps_ts[j].pps_sample.pps_offset;
                    temp_offset -= pps_boards.at(i)->pps_ctrl[j].pps_delay;
                    if (pps_compensate_values != 0)
                    {
                        temp_offset -= temp_offset_ref;
                    }
                }

                if (pps_boards.at(i)->pps_ctrl[j].pps_threshold_enabled != 0)
                {
                    if (temp_offset > pps_boards.at(i)->pps_ctrl[j].pps_threshold_high)
                    {
                        temp_threshold_high_exceeded = 1;
                    }
                    else
                    {
                        temp_threshold_high_exceeded = 0;
                    }
                    if (temp_offset < pps_boards.at(i)->pps_ctrl[j].pps_threshold_low)
                    {
                        temp_threshold_low_exceeded = 1;
                    }
                    else
                    {
                        temp_threshold_low_exceeded = 0;
                    }
                }
                else
                {
                    temp_threshold_high_exceeded = 0;
                    temp_threshold_low_exceeded = 0;
                }

            }
            else
            {
                temp_active = 0;
                temp_offset = 0;
                temp_threshold_high_exceeded = 0;
                temp_threshold_low_exceeded = 0;
            }

            // fill
            temp_gui_sample.pps_active = temp_active;
            temp_gui_sample.pps_offset = temp_offset;


            // fill the array
            while (pps_boards.at(i)->pps_gui[j].pps_samples.size() >= Upa_MaxSamples) // only the last N samples
            {
                pps_boards.at(i)->pps_gui[j].pps_samples.removeFirst();
            }
            pps_boards.at(i)->pps_gui[j].pps_samples.append(temp_gui_sample);

            // fill
            temp_log_sample.pps_active = temp_active;
            temp_log_sample.pps_offset = temp_offset;
            temp_log_sample.pps_fetch_time = temp_time; // just take when we started
            temp_log_sample.pps_threshold_enabled = pps_boards.at(i)->pps_ctrl[j].pps_threshold_enabled;
            temp_log_sample.pps_threshold_high = pps_boards.at(i)->pps_ctrl[j].pps_threshold_high;
            temp_log_sample.pps_threshold_low = pps_boards.at(i)->pps_ctrl[j].pps_threshold_low;
            temp_log_sample.pps_threshold_high_exceeded = temp_threshold_high_exceeded;
            temp_log_sample.pps_threshold_low_exceeded  = temp_threshold_low_exceeded;

            if (pps_log_values != 0)
            {
                // push the offset to the Log
                pps_boards.at(i)->pps_log[j].pps_samples.append(temp_log_sample);
            }
            else
            {
                pps_boards.at(i)->pps_log[j].pps_samples.clear();
            }

            // fill the array
            while (pps_boards.at(i)->pps_ctrl[j].pps_samples.size() >= Upa_MaxSamples) // only the last N samples
            {
                pps_boards.at(i)->pps_ctrl[j].pps_samples.removeFirst();
            }
            // save
            pps_boards.at(i)->pps_ctrl[j].pps_samples.append(temp_log_sample);

            // push the threshold
            pps_boards.at(i)->pps_ctrl[j].pps_threshold_high_exceeded = temp_threshold_high_exceeded;
            pps_boards.at(i)->pps_ctrl[j].pps_threshold_low_exceeded = temp_threshold_low_exceeded;
        }
    }

    // unlock all passing of samples
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_ctrl_log.unlock();
    }
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_ctrl_gui.unlock();
    }
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_ctrl_threshold.unlock();
    }
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_ctrl.unlock();
    }

    //********************************
    // wake up others
    //********************************
    if (pps_log_values != 0)
    {
        if (false == pps_log_timer->isActive())
        {
            pps_log_timer->start(1);
        }
    }
    if (false == pps_gui_timer->isActive())
    {
        pps_gui_timer->start(1);
    }
    if (false == pps_threshold_timer->isActive())
    {
        pps_threshold_timer->start(1);
    }
}


void Upa_PpsTab::pps_log_timer_run(void)
{
    QString temp_string;
    Upa_PpsLogSample temp_log_sample;

    // lock log
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_ctrl_log.lock();
    }

    if (pps_log_values != 0)
    {
        temp_string.clear();
        while (pps_boards.at(0)->pps_log[0].pps_samples.size() > 0)
        {
            for (int i = 0; i < pps_boards.size(); i++)
            {
                for (int j = 0; j < Upa_PpsPerBoard; j++)
                {
                    temp_log_sample = pps_boards.at(i)->pps_log[j].pps_samples.first();
                    if ((i==0) && (j==0))
                    {
                        temp_string.append(temp_log_sample.pps_fetch_time.toString("dd-MM-yyyy hh:mm:ss"));
                    }
                    temp_string.append(";");
                    if (temp_log_sample.pps_active != 0)
                    {
                        temp_string.append(QString::number(temp_log_sample.pps_offset));
                    }
                    else
                    {
                        temp_string.append("-");
                    }
                }
            }

            for (int i = 0; i < pps_boards.size(); i++)
            {
                for (int j = 0; j < Upa_PpsPerBoard; j++)
                {
                    temp_log_sample = pps_boards.at(i)->pps_log[j].pps_samples.first();
                    if ((temp_log_sample.pps_active != 0) &&
                        (temp_log_sample.pps_threshold_enabled != 0))
                    {
                        temp_string.append(";");
                        if (temp_log_sample.pps_threshold_high_exceeded == 0)
                        {
                            temp_string.append("n");
                        }
                        else
                        {
                            temp_string.append("y");
                        }
                        temp_string.append(";");
                        if (temp_log_sample.pps_threshold_low_exceeded == 0)
                        {
                            temp_string.append("n");
                        }
                        else
                        {
                            temp_string.append("y");
                        }
                    }
                    else
                    {
                        temp_string.append(";");
                        temp_string.append("-");
                        temp_string.append(";");
                        temp_string.append("-");
                    }
                }
            }

            temp_string.append("\n");

            for (int i = 0; i < pps_boards.size(); i++)
            {
                for (int j = 0; j < Upa_PpsPerBoard; j++)
                {
                    pps_boards.at(i)->pps_log[j].pps_samples.removeFirst();
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < pps_boards.size(); i++)
        {
            for (int j = 0; j < Upa_PpsPerBoard; j++)
            {
                pps_boards.at(i)->pps_log[j].pps_samples.clear();
            }
        }
    }

    // lock log
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_ctrl_log.unlock();
    }

    if ((pps_log_values != 0) && (temp_string.size() != 0))
    {
        QTextStream temp_stream(&pps_log_file);
        temp_stream << temp_string;
    }

}

void Upa_PpsTab::pps_gui_timer_run(void)
{
    int temp_size;
    int temp_range;
    QColor temp_color;
    int temp_min;
    int temp_max;

    // lock gui
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_gui.lock();
    }

    // disable updates
    pps_offset_chart_view->setUpdatesEnabled(false);

    //remove all Series
    foreach (QAbstractSeries *i, pps_offset_chart->series())
    {
        pps_offset_chart->removeSeries(i);
    }

    for (int i = 0; i < pps_boards.size(); i++)
    {
        for (int j = 0; j < Upa_PpsPerBoard; j++)
        {
            pps_boards.at(i)->pps_gui[j].pps_offset_series->clear();
        }
    }

    // lock all passing of samples
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_ctrl_gui.lock();
    }

    temp_range = pps_zoom_factor+1;
    for (int i = 0; i < pps_boards.size(); i++)
    {
        for (int j = 0; j < Upa_PpsPerBoard; j++)
        {
            if (pps_boards.at(i)->pps_gui[j].pps_show != 0)
            {
                temp_size = pps_boards.at(i)->pps_gui[j].pps_samples.size();

                if (temp_range > temp_size)
                {
                    for (int k = 0; k < temp_size; k++)
                    {
                        pps_boards.at(i)->pps_gui[j].pps_offset_series->append(k, pps_boards.at(i)->pps_gui[j].pps_samples.at(k).pps_offset);
                    }
                }
                else
                {
                    for (int k = 0; k < temp_range; k++)
                    {
                        pps_boards.at(i)->pps_gui[j].pps_offset_series->append(k, pps_boards.at(i)->pps_gui[j].pps_samples.at(temp_size-temp_range+k).pps_offset);
                    }
                }

                pps_offset_chart->addSeries(pps_boards.at(i)->pps_gui[j].pps_offset_series);
            }

            if (j != 0)
            {
                if (pps_boards.at(i)->pps_gui[j].pps_samples.last().pps_active == 0)
                {
                    temp_color.setRgb(128, 128, 128);
                    pps_boards.at(i)->pps_gui[j].pps_offset_series->setColor(temp_color);
                }
                else
                {
                    switch (j) // only 8 Input PPS per analyzer
                    {
                        case 1:
                            temp_color.setRgb(255, 64, 64);
                            pps_boards.at(i)->pps_gui[j].pps_offset_series->setColor(temp_color);
                            break;
                        case 2:
                            temp_color.setRgb(64, 255, 64);
                            pps_boards.at(i)->pps_gui[j].pps_offset_series->setColor(temp_color);
                            break;
                        case 3:
                            temp_color.setRgb(64, 64, 255);
                            pps_boards.at(i)->pps_gui[j].pps_offset_series->setColor(temp_color);
                            break;
                        case 4:
                            temp_color.setRgb(255, 64, 255);
                            pps_boards.at(i)->pps_gui[j].pps_offset_series->setColor(temp_color);
                            break;
                        case 5:
                            temp_color.setRgb(160, 64, 64);
                            pps_boards.at(i)->pps_gui[j].pps_offset_series->setColor(temp_color);
                            break;
                        case 6:
                            temp_color.setRgb(64, 160, 64);
                            pps_boards.at(i)->pps_gui[j].pps_offset_series->setColor(temp_color);
                            break;
                        case 7:
                            temp_color.setRgb(64, 64, 160);
                            pps_boards.at(i)->pps_gui[j].pps_offset_series->setColor(temp_color);
                            break;
                        case 8:
                            temp_color.setRgb(160, 64, 160);
                            pps_boards.at(i)->pps_gui[j].pps_offset_series->setColor(temp_color);
                            break;
                        default:
                            pps_boards.at(i)->pps_gui[j].pps_offset_series->setColor(Qt::black);
                            break;
                    }
                }
            }
        }
    }

    // unlock all passing of samples
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_ctrl_gui.unlock();
    }

    // axis
    pps_offset_chart->removeAxis(pps_offset_chart_x_axis);
    pps_offset_chart_x_axis->setLabelFormat("%i");
    pps_offset_chart_x_axis->setTickCount(21);
    pps_offset_chart_x_axis->setMin(0);
    pps_offset_chart_x_axis->setMax(pps_zoom_factor);
    pps_offset_chart->addAxis(pps_offset_chart_x_axis, Qt::AlignBottom);
    foreach (QAbstractSeries *i, pps_offset_chart->series())
    {
        i->attachAxis(pps_offset_chart_x_axis);
    }

    if (ui->PpsFixedScaleCheckBox->isChecked() == false)
    {
        temp_min = 0;
        temp_max = 0;

        for (int i = 0; i < pps_boards.size(); i++)
        {
            for (int j = 0; j < Upa_PpsPerBoard; j++)
            {
                if (pps_boards.at(i)->pps_gui[j].pps_show != 0)
                {
                    for (int k = 0; k < pps_boards.at(i)->pps_gui[j].pps_offset_series->count(); k++)
                    {
                        QPointF temp_point = pps_boards.at(i)->pps_gui[j].pps_offset_series->at(k);
                        if (temp_min > temp_point.y())
                        {
                            temp_min = temp_point.y();
                        }
                        if (temp_max < temp_point.y())
                        {
                            temp_max = temp_point.y();
                        }
                    }
                }
            }
        }
        temp_max = ((temp_max/100)+1)*100;
        temp_min = ((temp_min/100)-1)*100;
        if (temp_max > 1000000000)
        {
            temp_max = 1000000000;
        }
        if (temp_min < -1000000000)
        {
            temp_min = -1000000000;
        }
    }
    else
    {
        temp_max = ui->PpsMaxScaleValue->text().toInt(nullptr, 10);
        temp_min = ui->PpsMinScaleValue->text().toInt(nullptr, 10);
        if (temp_max > 1000000000)
        {
            temp_max = 1000000000;
            ui->PpsMaxScaleValue->setText(QString::number(temp_max));
        }
        if (temp_min < -1000000000)
        {
            temp_min = -1000000000;
            ui->PpsMinScaleValue->setText(QString::number(temp_min));
        }
    }

    pps_offset_chart->removeAxis(pps_offset_chart_y_axis);
    pps_offset_chart_y_axis->setLabelFormat("%i");
    pps_offset_chart_y_axis->setTickCount(11);
    pps_offset_chart_y_axis->setMin(temp_min);
    pps_offset_chart_y_axis->setMax(temp_max);
    pps_offset_chart->addAxis(pps_offset_chart_y_axis, Qt::AlignLeft);
    foreach (QAbstractSeries *i, pps_offset_chart->series())
    {
        i->attachAxis(pps_offset_chart_y_axis);
    }

    for (int i = 0; i < pps_boards.size(); i++)
    {
        for (int j = 0; j < Upa_PpsPerBoard; j++)
        {
            pps_boards.at(i)->pps_gui[j].pps_offset_series->setName(pps_boards.at(i)->pps_gui[j].pps_name);
            if (pps_boards.at(i)->pps_gui[j].pps_name != 0)
            {
                pps_boards.at(i)->pps_gui[j].pps_offset_series->show();
            }
            else
            {
                pps_boards.at(i)->pps_gui[j].pps_offset_series->hide();
            }
        }
    }

    // enable updates
    pps_offset_chart_view->setUpdatesEnabled(true);

    // unlock gui
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_gui.unlock();
    }

    pps_offset_chart->show();

}

void Upa_PpsTab::pps_threshold_timer_run(void)
{
    unsigned int temp_data = 0;
    unsigned int temp_addr = 0;
    int temp_threshold_high_exceeded;
    int temp_threshold_low_exceeded;
    QPixmap temp_pix;

    //********************************
    // calculations and forwarding
    //********************************
    // lock all passing of samples
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_ctrl_threshold.lock();
    }

    temp_threshold_high_exceeded = 0;
    temp_threshold_low_exceeded = 0;
    for (int i = 0; i < pps_boards.size(); i++)
    {
        for (int j = 0; j < Upa_PpsPerBoard; j++)
        {
            temp_threshold_high_exceeded |= pps_boards.at(i)->pps_ctrl[j].pps_threshold_high_exceeded;
            temp_threshold_low_exceeded |= pps_boards.at(i)->pps_ctrl[j].pps_threshold_low_exceeded;
        }
    }

    // unlock all passing of samples
    for (int i = 0; i < pps_boards.size(); i++)
    {
        pps_boards.at(i)->mutex_ctrl_threshold.unlock();
    }

    // set Ok/Nok logo

    if ((temp_threshold_high_exceeded == 0) and (temp_threshold_low_exceeded == 0))
    {
        temp_pix.load(":/PpsTab/Ok.png");
    }
    else
    {
        temp_pix.load(":/PpsTab/Nok.png");
    }
    temp_pix = temp_pix.scaled( ui->PpsOkNokLabel->size(),Qt::KeepAspectRatio);
    ui->PpsOkNokLabel->setPixmap(temp_pix);

    // set threshold signals on all PPS shields (if available)
    for (int i = 0; i < upa->io_core_config.size(); i++)
    {
        temp_addr = upa->io_core_config.at(i).address_range_low;
        temp_data = 0;
        if (temp_threshold_low_exceeded != 0)
        {
            temp_data |= 0x00000001;
        }
        if (temp_threshold_high_exceeded != 0)
        {
            temp_data |= 0x00000002;
        }
        if (0 == upa->io_core_config.at(i).com_lib->write_reg(temp_addr + Upa_IoConf_OutputDataReg, temp_data))
        {
            // nothing threshold values written
        }
        else
        {
            cout << "ERROR: " << "could not write threshold values to PPS Analyzer" << endl;
        }
    }
}

void Upa_PpsTab::pps_zoom_in_button_clicked(void)
{
    unsigned int temp_pps_zoom_factor;

    temp_pps_zoom_factor = pps_zoom_factor;

    if ((Upa_MaxSamples > 10000) &&
        (temp_pps_zoom_factor > 10000))
    {
        temp_pps_zoom_factor /= 10;
        if (temp_pps_zoom_factor <= 10000)
        {
            temp_pps_zoom_factor = 10000;
        }
    }
    else
    {
        switch (temp_pps_zoom_factor)
        {
        case 10000:
            temp_pps_zoom_factor = 8000;
            break;

        case 8000:
            temp_pps_zoom_factor = 6000;
            break;

        case 6000:
            temp_pps_zoom_factor = 4000;
            break;

        case 4000:
            temp_pps_zoom_factor = 2000;
            break;

        case 2000:
            temp_pps_zoom_factor = 1000;
            break;

        case 1000:
            temp_pps_zoom_factor = 800;
            break;

        case 800:
            temp_pps_zoom_factor = 600;
            break;

        case 600:
            temp_pps_zoom_factor = 400;
            break;

        case 400:
            temp_pps_zoom_factor = 200;
            break;

        case 200:
            temp_pps_zoom_factor = 100;
            ui->PpsZoomInButton->setEnabled(false);
            break;

        case 100: // we should not get here
            temp_pps_zoom_factor = 100;
            ui->PpsZoomInButton->setEnabled(false);
            break;

        default:
            temp_pps_zoom_factor = Upa_MaxSamples;
            ui->PpsZoomInButton->setEnabled(false);
            break;
        }
    }

    pps_zoom_factor = temp_pps_zoom_factor;

    ui->PpsZoomOutButton->setEnabled(true); // now we can zoom out for sure

}

void Upa_PpsTab::pps_zoom_out_button_clicked(void)
{
    unsigned int temp_pps_zoom_factor;

    temp_pps_zoom_factor = pps_zoom_factor;

    if ((Upa_MaxSamples > 10000) &&
        (temp_pps_zoom_factor >= 10000))
    {
        temp_pps_zoom_factor *= 10;
        if (temp_pps_zoom_factor >= Upa_MaxSamples)
        {
            temp_pps_zoom_factor = Upa_MaxSamples;
            ui->PpsZoomOutButton->setEnabled(false);
        }
    }
    else
    {
        switch (temp_pps_zoom_factor)
        {
        case 10000: // we should not get here
            temp_pps_zoom_factor = 10000;
            ui->PpsZoomOutButton->setEnabled(false);
            break;

        case 8000:
            temp_pps_zoom_factor = 10000;
            if (Upa_MaxSamples <= 10000)
            {
                ui->PpsZoomOutButton->setEnabled(false);
            }
            break;

        case 6000:
            temp_pps_zoom_factor = 8000;
            break;

        case 4000:
            temp_pps_zoom_factor = 6000;
            break;

        case 2000:
            temp_pps_zoom_factor = 4000;
            break;

        case 1000:
            temp_pps_zoom_factor = 2000;
            break;

        case 800:
            temp_pps_zoom_factor = 1000;
            break;

        case 600:
            temp_pps_zoom_factor = 800;
            break;

        case 400:
            temp_pps_zoom_factor = 600;
            break;

        case 200:
            temp_pps_zoom_factor = 400;
            break;

        case 100:
            temp_pps_zoom_factor = 200;
            break;

        default:
            temp_pps_zoom_factor = Upa_MaxSamples;
            ui->PpsZoomOutButton->setEnabled(false);
            break;
        }
    }

    pps_zoom_factor = temp_pps_zoom_factor;

    ui->PpsZoomInButton->setEnabled(true); // now we can zoom in for sure

    //pps_read_values(); // realigne
}
