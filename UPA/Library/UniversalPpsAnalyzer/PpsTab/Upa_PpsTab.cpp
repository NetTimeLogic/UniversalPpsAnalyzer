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

#include <QStandardPaths>

#include <Upa_PpsTab.h>
#include <ui_Upa_PpsTab.h>
#include <ui_Upa_PpsConfigScreen.h>
#include <ui_Upa_PpsThresholdScreen.h>

Upa_PpsTab::Upa_PpsTab(Upa_UniversalPpsAnalyzer *parent) : QWidget()
{
    QString temp_string;
    unsigned int* temp_number_of_points;
    QString* temp_name;
    int* temp_delay;
    int* temp_show;
    int* temp_threshold_high;
    int* temp_threshold_low;
    int* temp_threshold;
    int* temp_threshold_high_exceeded;
    int* temp_threshold_low_exceeded;
    upa = parent;
    unsigned int temp_data = 0;
    unsigned int temp_addr = 0;

    ui = new Ui::Upa_PpsTab();
    ui->setupUi(this);

    pps_timer = new QTimer(this);
    pps_timer->stop();

    pps_compensate_values = 1;
    pps_log_values = 0;

    pps_zoom_factor = 1;
    ui->PpsZoomInButton->setEnabled(false);
    ui->PpsZoomOutButton->setEnabled(true); // now we can zoom out for sure

    connect(ui->PpsClearButton, SIGNAL(clicked()), this, SLOT(pps_clear_button_clicked()));
    connect(ui->PpsSaveButton, SIGNAL(clicked()), this, SLOT(pps_save_button_clicked()));
    connect(ui->PpsLogButton, SIGNAL(clicked()), this, SLOT(pps_log_button_clicked()));
    connect(ui->PpsCompensateValuesButton, SIGNAL(clicked()), this, SLOT(pps_compensate_values_button_clicked()));
    connect(ui->PpsChangeDelaysButton, SIGNAL(clicked()), this, SLOT(pps_delay_button_clicked()));
    connect(ui->PpsSetThresholdsButton, SIGNAL(clicked()), this, SLOT(pps_threshold_button_clicked()));
    connect(ui->PpsZoomInButton, SIGNAL(clicked()), this, SLOT(pps_zoom_in_button_clicked()));
    connect(ui->PpsZoomOutButton, SIGNAL(clicked()), this, SLOT(pps_zoom_out_button_clicked()));
    connect(pps_timer, SIGNAL(timeout()), this, SLOT(pps_read_values_timer()));

    for (int i = 0; i < upa->ts_core_config.size(); i++)
    {
        int* temp_offsets = new int[100000];
        memset(temp_offsets, 0, sizeof(int[100000]));
        pps_offsets.append(temp_offsets);
    }

    for (int i = 0; i < upa->ts_core_config.size(); i++)
    {        
        temp_string.clear();
        temp_string.append(upa->ts_core_config.at(i).com_port);
        temp_string.append("_PPS_");
        QLineSeries* temp_line_series = new QLineSeries();
        if (upa->ts_core_config.at(i).core_instance_nr == 1)
        {
            temp_string.append("REF");
            QColor temp_color;
            temp_color.setRgb(230, 230, 230);
            temp_line_series->setColor(temp_color);
        }
        else
        {
            temp_string.append(QString("%1").arg((upa->ts_core_config.at(i).core_instance_nr-1), 2, 10, QLatin1Char('0')).toUpper());
            QColor temp_color;
            switch (upa->ts_core_config.at(i).core_instance_nr-2) // only 8 PPS per analyzer
            {
                case 0:
                    temp_color.setRgb(255, 64, 64);
                    temp_line_series->setColor(temp_color);
                    break;
                case 1:
                    temp_color.setRgb(64, 255, 64);
                    temp_line_series->setColor(temp_color);
                    break;
                case 2:
                    temp_color.setRgb(64, 64, 255);
                    temp_line_series->setColor(temp_color);
                    break;
                case 3:
                    temp_color.setRgb(255, 64, 255);
                    temp_line_series->setColor(temp_color);
                    break;
                case 4:
                    temp_color.setRgb(160, 64, 64);
                    temp_line_series->setColor(temp_color);
                    break;
                case 5:
                    temp_color.setRgb(64, 160, 64);
                    temp_line_series->setColor(temp_color);
                    break;
                case 6:
                    temp_color.setRgb(64, 64, 160);
                    temp_line_series->setColor(temp_color);
                    break;
                case 7:
                    temp_color.setRgb(160, 64, 160);
                    temp_line_series->setColor(temp_color);
                    break;
                default:
                    temp_line_series->setColor(Qt::black);
                    break;
            }
        }

        temp_addr =  upa->ts_core_config.at(i).address_range_low;
        temp_data = 0x00000001;
        if (0 == upa->ts_core_config.at(i).com_lib->write_reg(temp_addr + Upa_ClkTs_ControlReg, temp_data))
        {
            cout << "VERBOSE: " << "enabled " << temp_string.toLatin1().constData() << endl;
        }
        else
        {
            cout << "ERROR: " << "could not enable" << temp_string.toLatin1().constData() << endl;
        }
        if (0 == upa->ts_core_config.at(i).com_lib->write_reg(temp_addr + Upa_ClkTs_IrqMaskReg, temp_data))
        {
            cout << "VERBOSE: " << "enabled irq on " << temp_string.toLatin1().constData() << endl;
        }
        else
        {
            cout << "ERROR: " << "could not enable irq on" << temp_string.toLatin1().constData() << endl;
        }

        temp_line_series->setName(temp_string);
        temp_number_of_points = new(unsigned int);
        *temp_number_of_points = 0;
        temp_name = new(QString);
        temp_name->clear();
        temp_name->append("PPS_");
        if (upa->ts_core_config.at(i).core_instance_nr == 1)
        {
            temp_name->append("REF");
        }
        else
        {
            temp_name->append(QString("%1").arg((upa->ts_core_config.at(i).core_instance_nr-1), 2, 10, QLatin1Char('0')).toUpper());
        }
        temp_delay = new(int);
        *temp_delay = 0;
        temp_show = new(int);
        *temp_show = 1;
        temp_threshold_high_exceeded = new(int);
        *temp_threshold_high_exceeded = 0;
        temp_threshold_low_exceeded = new(int);
        *temp_threshold_low_exceeded = 0;
        temp_threshold_high = new(int);
        temp_threshold_low = new(int);
        temp_threshold = new(int);
        if (upa->ts_core_config.at(i).core_instance_nr == 1)
        {
            *temp_threshold_high = 50;
            *temp_threshold_low = -50;
            *temp_threshold = 1;
        }
        else
        {
            *temp_threshold_high = 1000;
            *temp_threshold_low = -1000;
            *temp_threshold = 0;
        }
        pps_offset_thresholds_high_exceeded.append(temp_threshold_high_exceeded);
        pps_offset_thresholds_low_exceeded.append(temp_threshold_low_exceeded);
        pps_offset_thresholds_high.append(temp_threshold_high);
        pps_offset_thresholds_low.append(temp_threshold_low);
        pps_offset_thresholds.append(temp_threshold);
        pps_offset_show.append(temp_show);
        pps_offset_names.append(temp_name);
        pps_offset_delays.append(temp_delay);
        pps_offset_number_of_points.append(temp_number_of_points);
        pps_offset_series.append(temp_line_series);
    }

    pps_offset_chart = new QChart();
    pps_offset_chart->setContentsMargins(0, 0, 0, 0);
    pps_offset_chart->setBackgroundRoundness(0);
    pps_offset_chart->setBackgroundBrush(Qt::white);
    for (int i = 0; i < upa->ts_core_config.size(); i++)
    {
        pps_offset_chart->addSeries(pps_offset_series.at(i));
    }

    pps_offset_chart_x_axis = new QValueAxis;
    pps_offset_chart_x_axis->setLabelFormat("%i");
    pps_offset_chart_x_axis->setTickCount(21);
    pps_offset_chart_x_axis->setMin(0);
    pps_offset_chart_x_axis->setMax(100);
    pps_offset_chart->addAxis(pps_offset_chart_x_axis, Qt::AlignBottom);
    for (int i = 0; i < upa->ts_core_config.size(); i++)
    {
        pps_offset_series.at(i)->attachAxis(pps_offset_chart_x_axis);
    }

    pps_offset_chart_y_axis = new QValueAxis;
    pps_offset_chart_y_axis->setLabelFormat("%i");
    pps_offset_chart_y_axis->setTickCount(11);
    pps_offset_chart_y_axis->setMin(-100);
    pps_offset_chart_y_axis->setMax(100);
    pps_offset_chart->addAxis(pps_offset_chart_y_axis, Qt::AlignLeft);
    for (int i = 0; i < upa->ts_core_config.size(); i++)
    {
        pps_offset_series.at(i)->attachAxis(pps_offset_chart_y_axis);
    }
    pps_offset_chart_view = new QChartView(pps_offset_chart);
    pps_offset_chart_view->setRenderHint(QPainter::Antialiasing);

    ui->PpsOffsetChartLayout->addWidget(pps_offset_chart_view, 0, 0);

    ui_config = new Upa_PpsConfigScreen(this);
    ui_config->pps_reload();

    ui_threshold = new Upa_PpsThresholdScreen(this);
    ui_threshold->pps_reload();

    pps_offset_chart->legend()->setVisible(true);
}

Upa_PpsTab::~Upa_PpsTab()
{
    QString temp_string;
    unsigned int temp_data = 0;
    unsigned int temp_addr = 0;

    pps_timer->stop();

    if (pps_log_values != 0)
    {
        pps_log_values = 0;
        pps_log_file.close();
        ui->PpsLogButton->setText("Start Log");
    }


    for (int i = 0; i < upa->ts_core_config.size(); i++)
    {        
        temp_string.clear();
        temp_string.append(upa->ts_core_config.at(i).com_port);
        temp_string.append("_PPS_");
        temp_string.append(QString("%1").arg((upa->ts_core_config.at(i).core_instance_nr-1), 2, 10, QLatin1Char('0')).toUpper());

        // do not disable
        //temp_addr =  upa->ts_core_config.at(i).address_range_low;
        //temp_data = 0x00000000;
        //if (0 == upa->ts_core_config.at(i).com_lib->write_reg(temp_addr + Upa_ClkTs_ControlReg, temp_data))
        //{
        //    cout << "VERBOSE: " << "disabled " << temp_string.toLatin1().constData() << endl;
        //}
        //else
        //{
        //    cout << "ERROR: " << "could not disable" << temp_string.toLatin1().constData() << endl;
        //}
        //if (0 == upa->ts_core_config.at(i).com_lib->write_reg(temp_addr + Upa_ClkTs_IrqMaskReg, temp_data))
        //{
        //    cout << "VERBOSE: " << "disabled irq on " << temp_string.toLatin1().constData() << endl;
        //}
        //else
        //{
        //    cout << "ERROR: " << "could not disable irq on" << temp_string.toLatin1().constData() << endl;
        //}
    }

    delete ui;
    ui_config->close();
    delete ui_config;
    ui_threshold->close();
    delete ui_threshold;
    delete pps_timer;
    for (int i = 0; i < upa->ts_core_config.size(); i++)
    {
        delete pps_offsets.at(i);
        delete pps_offset_thresholds_high_exceeded.at(i);
        delete pps_offset_thresholds_low_exceeded.at(i);
        delete pps_offset_thresholds_high.at(i);
        delete pps_offset_thresholds_low.at(i);
        delete pps_offset_thresholds.at(i);
        delete pps_offset_show.at(i);
        pps_offset_names.at(i)->clear();
        delete pps_offset_names.at(i);
        delete pps_offset_delays.at(i);
        delete pps_offset_number_of_points.at(i);
        delete pps_offset_series.at(i);
    }
    pps_offsets.clear();
    pps_offset_thresholds_high_exceeded.clear();
    pps_offset_thresholds_low_exceeded.clear();
    pps_offset_thresholds_high.clear();
    pps_offset_thresholds_low.clear();
    pps_offset_thresholds.clear();
    pps_offset_show.clear();
    pps_offset_names.clear();
    pps_offset_delays.clear();
    pps_offset_number_of_points.clear();
    pps_offset_series.clear();

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
    for (int i = 0; i < upa->ts_core_config.size(); i++)
    {
        *pps_offset_number_of_points.at(i) = 0;
        pps_offset_series.at(i)->clear();
        memset(pps_offsets.at(i), 0, sizeof(int[100000]));
    }

    pps_timer->start(1000);
    return 0;
}

int Upa_PpsTab::pps_disable(void)
{
    pps_timer->stop();

    if (pps_log_values != 0)
    {
        pps_log_values = 0;
        pps_log_file.close();
        ui->PpsLogButton->setText("Start Log");
    }

    for (int i = 0; i < upa->ts_core_config.size(); i++)
    {
        *pps_offset_number_of_points.at(i) = 0;
        pps_offset_series.at(i)->clear();
        memset(pps_offsets.at(i), 0, sizeof(int[100000]));
    }

    return 0;
}

void Upa_PpsTab::pps_read_ts(QString com_port)
{
    unsigned int temp_nanoseconds = 0;
    long long temp_signed_offset;
    unsigned int temp_irq = 0;
    unsigned int temp_data = 0;
    unsigned int temp_addr = 0;
    QColor temp_color;

    for (int k = 0; k < upa->ts_core_config.size(); k++)
    {
        if (0 == com_port.compare(upa->ts_core_config.at(k).com_port))
        {
            temp_addr =  upa->ts_core_config.at(k).address_range_low;

            //********************************
            // get timestamp
            //********************************
            //if (0 == upa->ts_core_config.at(k).com_lib->read_reg(temp_addr + Upa_ClkTs_IrqReg, temp_irq))
            //{
                //if ((temp_irq & 0x00000001) != 0)
                //{
            if (0 == upa->ts_core_config.at(k).com_lib->read_reg(temp_addr + Upa_ClkTs_StatusReg, temp_data)) // check if the PPS has activity at all (within the last 1.5s)
            {
                if ((temp_data & 0x00000002) != 0)
                {
                    // change color to the one we had before disconnecting
                    if (upa->ts_core_config.at(k).core_instance_nr > 1)
                    {
                        switch (upa->ts_core_config.at(k).core_instance_nr-2) // only 8 PPS per analyzer
                        {
                            case 0:
                                temp_color.setRgb(255, 64, 64);
                                pps_offset_series.at(k)->setColor(temp_color);
                                break;
                            case 1:
                                temp_color.setRgb(64, 255, 64);
                                pps_offset_series.at(k)->setColor(temp_color);
                                break;
                            case 2:
                                temp_color.setRgb(64, 64, 255);
                                pps_offset_series.at(k)->setColor(temp_color);
                                break;
                            case 3:
                                temp_color.setRgb(255, 64, 255);
                                pps_offset_series.at(k)->setColor(temp_color);
                                break;
                            case 4:
                                temp_color.setRgb(160, 64, 64);
                                pps_offset_series.at(k)->setColor(temp_color);
                                break;
                            case 5:
                                temp_color.setRgb(64, 160, 64);
                                pps_offset_series.at(k)->setColor(temp_color);
                                break;
                            case 6:
                                temp_color.setRgb(64, 64, 160);
                                pps_offset_series.at(k)->setColor(temp_color);
                                break;
                            case 7:
                                temp_color.setRgb(160, 64, 160);
                                pps_offset_series.at(k)->setColor(temp_color);
                                break;
                            default:
                                pps_offset_series.at(k)->setColor(Qt::black);
                                break;
                        }
                    }

                    // nanoseconds
                    if (0 == upa->ts_core_config.at(k).com_lib->read_reg(temp_addr + Upa_ClkTs_TimeValueLReg, temp_data))
                    {
                        temp_nanoseconds = temp_data;
                    }

                    // clear irq
                    //temp_data = temp_irq;
                    //if (0 == upa->ts_core_config.at(k).com_lib->write_reg(temp_addr + Upa_ClkTs_IrqReg, temp_data))
                    //{
                    //    // nothing irq cleared
                    //}
                    //else
                    //{
                    //    cout << "ERROR: " << "irq cleaning didn't work" << endl;
                    //}

                    if(temp_nanoseconds > 500000000)
                    {
                        temp_signed_offset = (long long)temp_nanoseconds - 1000000000;
                    }
                    else
                    {
                        temp_signed_offset = (long long)temp_nanoseconds;
                    }

                    // limit to 100000 in display
                    if (temp_signed_offset >= 100000)
                    {
                        temp_signed_offset = 100000;
                    }
                    else if (temp_signed_offset <= -100000)
                    {
                        temp_signed_offset = -100000;
                    }

                    // compensation
                    if (upa->ts_core_config.at(k).core_instance_nr == 1)
                    {
                        *(upa->ts_core_config.at(k).ref_offset) = temp_signed_offset + *(pps_offset_delays.at(k));
                        temp_signed_offset = temp_signed_offset - *(pps_offset_delays.at(k));
                    }
                    else if (pps_compensate_values == 1)
                    {
                        temp_signed_offset = temp_signed_offset - *(upa->ts_core_config.at(k).ref_offset);
                        temp_signed_offset = temp_signed_offset - *(pps_offset_delays.at(k));
                    }

                    // threshold high check
                    if ((*(pps_offset_thresholds.at(k)) != 0) && (*(pps_offset_show.at(k)) != 0) &&
                        (*(pps_offset_thresholds_high.at(k)) < temp_signed_offset))
                    {
                        *(pps_offset_thresholds_high_exceeded.at(k)) = 1;
                    }
                    else
                    {
                        *(pps_offset_thresholds_high_exceeded.at(k)) = 0;
                    }

                    // threshold low check
                    if ((*(pps_offset_thresholds.at(k)) != 0) && (*(pps_offset_show.at(k)) != 0) &&
                        (*(pps_offset_thresholds_low.at(k)) > temp_signed_offset))
                    {
                        *(pps_offset_thresholds_low_exceeded.at(k)) = 1;
                    }
                    else
                    {
                        *(pps_offset_thresholds_low_exceeded.at(k)) = 0;
                    }

                    // shift
                    if (*(pps_offset_number_of_points.at(k)) < 100000)
                    {
                        pps_offsets.at(k)[*(pps_offset_number_of_points.at(k))] = (int)temp_signed_offset;
                        *(pps_offset_number_of_points.at(k)) = *(pps_offset_number_of_points.at(k)) + 1;
                    }
                    else
                    {
                        for (int i = 1; i <= (100000-1); i++)
                        {
                           pps_offsets.at(k)[i-1] = pps_offsets.at(k)[i];
                        }
                        pps_offsets.at(k)[(100000-1)] = (int)temp_signed_offset;
                    }
                }
                else
                {
                    temp_signed_offset = 0; // just say 0
                    // shift
                    if (*(pps_offset_number_of_points.at(k)) < 100000)
                    {
                        pps_offsets.at(k)[*(pps_offset_number_of_points.at(k))] = (int)temp_signed_offset;
                        *(pps_offset_number_of_points.at(k)) = *(pps_offset_number_of_points.at(k)) + 1;
                    }
                    else
                    {
                        for (int i = 1; i <= (100000-1); i++)
                        {
                           pps_offsets.at(k)[i-1] = pps_offsets.at(k)[i];
                        }
                        pps_offsets.at(k)[(100000-1)] = (int)temp_signed_offset;
                    }

                    // we don't care on thresholds if we don't have a PPS
                    *(pps_offset_thresholds_high_exceeded.at(k)) = 0;
                    *(pps_offset_thresholds_low_exceeded.at(k)) = 0;

                    // change color to a mid grey
                    if (upa->ts_core_config.at(k).core_instance_nr > 1)
                    {
                        temp_color.setRgb(128, 128, 128);
                        pps_offset_series.at(k)->setColor(temp_color);
                    }

                }
            }
        }
    }
}

void Upa_PpsTab::pps_read_values(void)
{
    QList<QString> temp_com_ports;
    QString temp_string;
    unsigned int temp_number_of_points;
    int temp_min = 0;
    int temp_max = 0;
    unsigned int temp_data = 0;
    unsigned int temp_addr = 0;
    int temp_threshold_high_exceeded = 0;
    int temp_threshold_low_exceeded = 0;

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

    for (int i = 0; i < temp_com_ports.size(); i++)
    {
        pps_read_ts(temp_com_ports.at(i));
    }

    // set Ok/Nok logo
    temp_threshold_high_exceeded = 0;
    for (int i = 0; i < pps_offset_thresholds_high_exceeded.size(); i++)
    {
        temp_threshold_high_exceeded |= *(pps_offset_thresholds_high_exceeded.at(i));
    }
    temp_threshold_low_exceeded = 0;
    for (int i = 0; i < pps_offset_thresholds_low_exceeded.size(); i++)
    {
        temp_threshold_low_exceeded |= *(pps_offset_thresholds_low_exceeded.at(i));
    }
    QPixmap temp_pix;
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

    // set threshold signals on PPS shield (if available)
    for (int i = 0; i < upa->io_core_config.size(); i++)
    {
        temp_threshold_high_exceeded = 0;
        temp_threshold_low_exceeded = 0;
        for (int j = 0; j < upa->ts_core_config.size(); j++)
        {
            if (upa->ts_core_config.at(j).com_port == upa->io_core_config.at(i).com_port)
            {
                temp_threshold_high_exceeded |= *(pps_offset_thresholds_high_exceeded.at(j));
                temp_threshold_low_exceeded |= *(pps_offset_thresholds_low_exceeded.at(j));
            }
        }

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

    // find max number
    temp_number_of_points = 0;
    for (int k = 0; k < upa->ts_core_config.size(); k++)
    {
        if (*pps_offset_number_of_points.at(k) > temp_number_of_points)
        {
            temp_number_of_points = *pps_offset_number_of_points.at(k);
        }
    }

    // fill with old
    for (int k = 0; k < upa->ts_core_config.size(); k++)
    {
        while (*pps_offset_number_of_points.at(k) < temp_number_of_points)
        {
            if (*pps_offset_number_of_points.at(k) > 0)
            {
                pps_offsets.at(k)[*pps_offset_number_of_points.at(k)] = pps_offsets.at(k)[(*pps_offset_number_of_points.at(k)-1)];
            }
            else
            {
                pps_offsets.at(k)[*pps_offset_number_of_points.at(k)] = 0;
            }
            if (*pps_offset_number_of_points.at(k) < 100000)
            {
                *pps_offset_number_of_points.at(k) = *pps_offset_number_of_points.at(k) + 1;
            }
        }
    }

    // log
    if (pps_log_values != 0)
    {
        QDateTime temp_time = QDateTime::currentDateTime();
        QString temp_timestamp = temp_time.toString("dd-MM-yyyy hh:mm:ss.zzz");
        QTextStream temp_stream(&pps_log_file);

        if (temp_number_of_points > 0)
        {
            temp_stream << temp_timestamp;

            for (int k = 0; k < upa->ts_core_config.size(); k++)
            {
                int temp_index;
                temp_index = *pps_offset_number_of_points.at(k) - 1;
                if (temp_index >= 0)
                {
                    temp_stream << ";" << QString::number(pps_offsets.at(k)[temp_index]);
                }
                else
                {
                    temp_stream << ";" << "0";
                }
            }
            temp_stream << "\n";
        }
    }

    // disable updates
    pps_offset_chart_view->setUpdatesEnabled(false);

    //remove all Series
    foreach (QAbstractSeries *i, pps_offset_chart->series())
    {
        pps_offset_chart->removeSeries(i);
    }

    // fill values
    unsigned int temp_range = (100*pps_zoom_factor)+1;
    for (int k = 0; k < upa->ts_core_config.size(); k++)
    {
        pps_offset_series.at(k)->clear();

        if (*(pps_offset_show.at(k)) != 0) // only for those who are shown we care for the filling
        {
            if (temp_range > *pps_offset_number_of_points.at(k))
            {
                for (int i = 0; i < *pps_offset_number_of_points.at(k); i++)
                {
                    pps_offset_series.at(k)->append(i, pps_offsets.at(k)[i]);
                }
            }
            else
            {
                for (int i = 0; i < temp_range; i++)
                {
                    pps_offset_series.at(k)->append(i, pps_offsets.at(k)[(*pps_offset_number_of_points.at(k)-temp_range+i)]);
                }
            }
        }
        pps_offset_chart->addSeries(pps_offset_series.at(k));
    }

    // axis
    pps_offset_chart->removeAxis(pps_offset_chart_x_axis);
    pps_offset_chart_x_axis->setLabelFormat("%i");
    pps_offset_chart_x_axis->setTickCount(21);
    pps_offset_chart_x_axis->setMin(0);
    pps_offset_chart_x_axis->setMax((100*pps_zoom_factor));
    pps_offset_chart->addAxis(pps_offset_chart_x_axis, Qt::AlignBottom);
    foreach (QAbstractSeries *i, pps_offset_chart->series())
    {
        i->attachAxis(pps_offset_chart_x_axis);
    }

    if (ui->PpsFixedScaleCheckBox->isChecked() == false)
    {
        temp_min = 0;
        temp_max = 0;
        for (int k = 0; k < upa->ts_core_config.size(); k++)
        {
            for (int j = 0; j < pps_offset_series.at(k)->count(); j++)
            {
                if (*(pps_offset_show.at(k)) != 0) // only for those who are shown we care for the scaling
                {
                    QPointF temp_point = pps_offset_series.at(k)->at(j);
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
        temp_max = ((temp_max/100)+1)*100;
        temp_min = ((temp_min/100)-1)*100;
        if (temp_max > 100000)
        {
            temp_max = 100000;
        }
        if (temp_min < -100000)
        {
            temp_min = -100000;
        }
    }
    else
    {
        temp_max = ui->PpsMaxScaleValue->text().toInt(nullptr, 10);
        temp_min = ui->PpsMinScaleValue->text().toInt(nullptr, 10);
        if (temp_max > 100000)
        {
            temp_max = 100000;
            ui->PpsMaxScaleValue->setText(QString::number(temp_max));
        }
        if (temp_min < -100000)
        {
            temp_min = -100000;
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

    for (int k = 0; k < upa->ts_core_config.size(); k++)
    {
        temp_string.clear();
        //temp_string.append(upa->ts_core_config.at(k).com_port);
        //temp_string.append("_");
        temp_string.append(*(pps_offset_names.at(k)));
        pps_offset_series.at(k)->setName(temp_string);
        if (*(pps_offset_show.at(k)) == 0)
        {
            pps_offset_series.at(k)->hide();
        }
        else
        {
            pps_offset_series.at(k)->show();
        }
    }

    // enable updates
    pps_offset_chart_view->setUpdatesEnabled(true);

    pps_offset_chart->show();
}

void Upa_PpsTab::pps_clear_button_clicked(void)
{
    for (int i = 0; i < upa->ts_core_config.size(); i++)
    {
        *pps_offset_number_of_points.at(i) = 0;
        pps_offset_series.at(i)->clear();
        memset(pps_offsets.at(i), 0, sizeof(int[100000]));
    }
    pps_offset_chart->axisY()->setMin(-100);
    pps_offset_chart->axisY()->setMax(100);

    pps_offset_chart->show();
}

void Upa_PpsTab::pps_save_button_clicked(void)
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
                for (int k = 0; k < upa->ts_core_config.size(); k++)
                {
                    temp_stream << ";" << pps_offset_series.at(k)->name();
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
    ui_config->show();
    ui_config->activateWindow();
    ui_config->raise();
}

void Upa_PpsTab::pps_threshold_button_clicked(void)
{
    ui_threshold->pps_reload();
    ui_threshold->show();
    ui_threshold->activateWindow();
    ui_threshold->raise();
}

void Upa_PpsTab::pps_read_values_timer(void)
{
    pps_read_values();
}

void Upa_PpsTab::pps_zoom_in_button_clicked(void)
{
    switch (pps_zoom_factor)
    {
    case 1000:
        pps_zoom_factor = 100;
        break;

    case 100:
        pps_zoom_factor = 80;
        break;

    case 80:
        pps_zoom_factor = 60;
        break;

    case 60:
        pps_zoom_factor = 40;
        break;

    case 40:
        pps_zoom_factor = 20;
        break;

    case 20:
        pps_zoom_factor = 10;
        break;

    case 10:
        pps_zoom_factor = 8;
        break;

    case 8:
        pps_zoom_factor = 6;
        break;

    case 6:
        pps_zoom_factor = 4;
        break;

    case 4:
        pps_zoom_factor = 2;
        break;

    case 2:
        pps_zoom_factor = 1;
        ui->PpsZoomInButton->setEnabled(false);
        break;

    case 1:
        pps_zoom_factor = 1;
        ui->PpsZoomInButton->setEnabled(false);
        break;

    default:
        pps_zoom_factor = 1;
        ui->PpsZoomInButton->setEnabled(false);
        break;
    }

    ui->PpsZoomOutButton->setEnabled(true); // now we can zoom out for sure

    pps_read_values(); // realigne
}

void Upa_PpsTab::pps_zoom_out_button_clicked(void)
{
    switch (pps_zoom_factor)
    {
    case 1000:
        pps_zoom_factor = 1000;
        ui->PpsZoomOutButton->setEnabled(false);
        break;

    case 100:
        pps_zoom_factor = 1000;
        ui->PpsZoomOutButton->setEnabled(false);
        break;

    case 80:
        pps_zoom_factor = 100;
        break;

    case 60:
        pps_zoom_factor = 80;
        break;

    case 40:
        pps_zoom_factor = 60;
        break;

    case 20:
        pps_zoom_factor = 40;
        break;

    case 10:
        pps_zoom_factor = 20;
        break;

    case 8:
        pps_zoom_factor = 10;
        break;

    case 6:
        pps_zoom_factor = 8;
        break;

    case 4:
        pps_zoom_factor = 6;
        break;

    case 2:
        pps_zoom_factor = 4;
        break;

    case 1:
        pps_zoom_factor = 2;
        break;

    default:
        pps_zoom_factor = 100;
        ui->PpsZoomOutButton->setEnabled(false);
        break;
    }

    ui->PpsZoomInButton->setEnabled(true); // now we can zoom in for sure

    pps_read_values(); // realigne
}
