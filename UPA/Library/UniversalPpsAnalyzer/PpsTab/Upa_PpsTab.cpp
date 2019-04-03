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
#include <ui_Upa_PpsDelayScreen.h>

Upa_PpsTab::Upa_PpsTab(Upa_UniversalPpsAnalyzer *parent) : QWidget()
{
    QString temp_string;
    unsigned int* temp_number_of_points;
    int* temp_delay;
    int* temp_show;
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
    connect(ui->PpsZoomInButton, SIGNAL(clicked()), this, SLOT(pps_zoom_in_button_clicked()));
    connect(ui->PpsZoomOutButton, SIGNAL(clicked()), this, SLOT(pps_zoom_out_button_clicked()));
    connect(pps_timer, SIGNAL(timeout()), this, SLOT(pps_read_values_timer()));

    for (int i = 0; i < upa->core_config.size(); i++)
    {
        int* temp_offsets = new int[100000];
        memset(temp_offsets, 0, sizeof(int[100000]));
        pps_offsets.append(temp_offsets);
    }

    for (int i = 0; i < upa->core_config.size(); i++)
    {        
        temp_string.clear();
        temp_string.append(upa->core_config.at(i).com_port);
        temp_string.append("_PPS_");
        QLineSeries* temp_line_series = new QLineSeries();
        if (upa->core_config.at(i).core_instance_nr == 1)
        {
            temp_string.append("REF");
            QColor temp_color;
            temp_color.setRgb(230, 230, 230);
            temp_line_series->setColor(temp_color);
        }
        else
        {
            temp_string.append(QString("%1").arg((upa->core_config.at(i).core_instance_nr-1), 2, 10, QLatin1Char('0')).toUpper());
            QColor temp_color;
            switch (upa->core_config.at(i).core_instance_nr-2) // only 8 PPS per analyzer
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

        temp_addr =  upa->core_config.at(i).address_range_low;
        temp_data = 0x00000001;
        if (0 == upa->core_config.at(i).com_lib->write_reg(temp_addr + Upa_ClkTs_ControlReg, temp_data))
        {
            cout << "VERBOSE: " << "enabled " << temp_string.toLatin1().constData() << endl;
        }
        else
        {
            cout << "ERROR: " << "could not enable" << temp_string.toLatin1().constData() << endl;
        }
        if (0 == upa->core_config.at(i).com_lib->write_reg(temp_addr + Upa_ClkTs_IrqMaskReg, temp_data))
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
        temp_delay = new(int);
        *temp_delay = 0;
        temp_show = new(int);
        *temp_show = 1;
        pps_offset_show.append(temp_show);
        pps_offset_delays.append(temp_delay);
        pps_offset_number_of_points.append(temp_number_of_points);
        pps_offset_series.append(temp_line_series);
    }

    pps_offset_chart = new QChart();
    pps_offset_chart->setContentsMargins(0, 0, 0, 0);
    pps_offset_chart->setBackgroundRoundness(0);
    pps_offset_chart->setBackgroundBrush(Qt::white);
    for (int i = 0; i < upa->core_config.size(); i++)
    {
        pps_offset_chart->addSeries(pps_offset_series.at(i));
    }

    pps_offset_chart_x_axis = new QValueAxis;
    pps_offset_chart_x_axis->setLabelFormat("%i");
    pps_offset_chart_x_axis->setTickCount(21);
    pps_offset_chart_x_axis->setMin(0);
    pps_offset_chart_x_axis->setMax(100);
    pps_offset_chart->addAxis(pps_offset_chart_x_axis, Qt::AlignBottom);
    for (int i = 0; i < upa->core_config.size(); i++)
    {
        pps_offset_series.at(i)->attachAxis(pps_offset_chart_x_axis);
    }

    pps_offset_chart_y_axis = new QValueAxis;
    pps_offset_chart_y_axis->setLabelFormat("%i");
    pps_offset_chart_y_axis->setTickCount(11);
    pps_offset_chart_y_axis->setMin(-100);
    pps_offset_chart_y_axis->setMax(100);
    pps_offset_chart->addAxis(pps_offset_chart_y_axis, Qt::AlignLeft);
    for (int i = 0; i < upa->core_config.size(); i++)
    {
        pps_offset_series.at(i)->attachAxis(pps_offset_chart_y_axis);
    }
    pps_offset_chart_view = new QChartView(pps_offset_chart);
    pps_offset_chart_view->setRenderHint(QPainter::Antialiasing);

    ui->PpsOffsetChartLayout->addWidget(pps_offset_chart_view, 0, 0);

    ui_delay = new Upa_PpsDelayScreen(this);
    ui_delay->pps_reload();

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


    for (int i = 0; i < upa->core_config.size(); i++)
    {        
        temp_string.clear();
        temp_string.append(upa->core_config.at(i).com_port);
        temp_string.append("_PPS_");
        temp_string.append(QString("%1").arg((upa->core_config.at(i).core_instance_nr-1), 2, 10, QLatin1Char('0')).toUpper());

        temp_addr =  upa->core_config.at(i).address_range_low;
        temp_data = 0x00000001;
        if (0 == upa->core_config.at(i).com_lib->write_reg(temp_addr + Upa_ClkTs_ControlReg, temp_data))
        {
            cout << "VERBOSE: " << "disabled " << temp_string.toLatin1().constData() << endl;
        }
        else
        {
            cout << "ERROR: " << "could not disable" << temp_string.toLatin1().constData() << endl;
        }
        if (0 == upa->core_config.at(i).com_lib->write_reg(temp_addr + Upa_ClkTs_IrqMaskReg, temp_data))
        {
            cout << "VERBOSE: " << "disabled irq on " << temp_string.toLatin1().constData() << endl;
        }
        else
        {
            cout << "ERROR: " << "could not disable irq on" << temp_string.toLatin1().constData() << endl;
        }
    }

    delete ui;
    ui_delay->close();
    delete ui_delay;
    delete pps_timer;
    for (int i = 0; i < upa->core_config.size(); i++)
    {
        delete pps_offsets.at(i);
        delete pps_offset_show.at(i);
        delete pps_offset_delays.at(i);
        delete pps_offset_number_of_points.at(i);
        delete pps_offset_series.at(i);
    }
    pps_offsets.clear();
    pps_offset_show.clear();
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
    for (int i = 0; i < upa->core_config.size(); i++)
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

    for (int i = 0; i < upa->core_config.size(); i++)
    {
        *pps_offset_number_of_points.at(i) = 0;
        pps_offset_series.at(i)->clear();
        memset(pps_offsets.at(i), 0, sizeof(int[100000]));
    }

    return 0;
}

void Upa_PpsTab::pps_read_values(void)
{
    unsigned int temp_number_of_points;
    unsigned int temp_nanoseconds = 0;
    long long temp_signed_offset;
    int temp_min = 0;
    int temp_max = 0;
    unsigned int temp_data = 0;
    unsigned int temp_addr = 0;

    for (int k = 0; k < upa->core_config.size(); k++)
    {
        temp_addr =  upa->core_config.at(k).address_range_low;

        //********************************
        // get timestamp
        //********************************
        if (0 == upa->core_config.at(k).com_lib->read_reg(temp_addr + Upa_ClkTs_IrqReg, temp_data))
        {
            if ((temp_data & 0x00000001) != 0)
            {
                // nanoseconds
                if (0 == upa->core_config.at(k).com_lib->read_reg(temp_addr + Upa_ClkTs_TimeValueLReg, temp_data))
                {
                    temp_nanoseconds = temp_data;
                }

                // clear irq
                temp_data = 0x00000001;
                if (0 == upa->core_config.at(k).com_lib->write_reg(temp_addr + Upa_ClkTs_IrqReg, temp_data))
                {
                    // nothing irq cleared
                }
                else
                {
                    cout << "ERROR: " << "irq cleaning didn't work" << endl;
                }
                
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

                if (upa->core_config.at(k).core_instance_nr == 1)
                {
                    *(upa->core_config.at(k).ref_offset) = temp_signed_offset + *(pps_offset_delays.at(k));
                    temp_signed_offset = temp_signed_offset - *(pps_offset_delays.at(k));
                }
                else if (pps_compensate_values == 1)
                {
                    temp_signed_offset = temp_signed_offset - *(upa->core_config.at(k).ref_offset);
                    temp_signed_offset = temp_signed_offset - *(pps_offset_delays.at(k));
                }

                if (*pps_offset_number_of_points.at(k) < 100000)
                {
                    pps_offsets.at(k)[*pps_offset_number_of_points.at(k)] = (int)temp_signed_offset;
                    *pps_offset_number_of_points.at(k) = *pps_offset_number_of_points.at(k) + 1;
                }
                else
                {
                    for (int i = 1; i < (100000-1); i++)
                    {
                       pps_offsets.at(k)[i-1] = pps_offsets.at(k)[i];
                    }
                    pps_offsets.at(k)[(100000-1)] = (int)temp_signed_offset;
                }
            }
        }
    }

    // find max number
    temp_number_of_points = 0;
    for (int k = 0; k < upa->core_config.size(); k++)
    {
        if (*pps_offset_number_of_points.at(k) > temp_number_of_points)
        {
            temp_number_of_points = *pps_offset_number_of_points.at(k);
        }
    }

    // fill with old
    for (int k = 0; k < upa->core_config.size(); k++)
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

            for (int k = 0; k < upa->core_config.size(); k++)
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

    // fill values
    unsigned int temp_range = (100*pps_zoom_factor)+1;
    for (int k = 0; k < upa->core_config.size(); k++)
    {
        pps_offset_chart->removeSeries(pps_offset_series.at(k));
        pps_offset_series.at(k)->clear();

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
        pps_offset_chart->addSeries(pps_offset_series.at(k));
    }

    // axis
    pps_offset_chart->removeAxis(pps_offset_chart_x_axis);
    pps_offset_chart_x_axis->setLabelFormat("%i");
    pps_offset_chart_x_axis->setTickCount(21);
    pps_offset_chart_x_axis->setMin(0);
    pps_offset_chart_x_axis->setMax((100*pps_zoom_factor));
    pps_offset_chart->addAxis(pps_offset_chart_x_axis, Qt::AlignBottom);
    for (int i = 0; i < upa->core_config.size(); i++)
    {
        pps_offset_series.at(i)->attachAxis(pps_offset_chart_x_axis);
    }

    if (ui->PpsFixedScaleCheckBox->isChecked() == false)
    {
        temp_min = 0;
        temp_max = 0;
        for (int k = 0; k < upa->core_config.size(); k++)
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
    for (int i = 0; i < upa->core_config.size(); i++)
    {
        pps_offset_series.at(i)->attachAxis(pps_offset_chart_y_axis);
    }

    for (int k = 0; k < upa->core_config.size(); k++)
    {
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
    for (int i = 0; i < upa->core_config.size(); i++)
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
                for (int k = 0; k < upa->core_config.size(); k++)
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
    ui_delay->pps_reload();
    ui_delay->show();
    ui_delay->activateWindow();
    ui_delay->raise();}


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
