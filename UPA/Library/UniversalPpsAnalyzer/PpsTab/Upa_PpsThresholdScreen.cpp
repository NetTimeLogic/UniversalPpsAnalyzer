#include <Upa_PpsThresholdScreen.h>
#include <Upa_PpsTab.h>
#include <ui_Upa_PpsThresholdScreen.h>

Upa_PpsThresholdScreen::Upa_PpsThresholdScreen(Upa_PpsTab *parent) : QWidget()
{
    pps_tab = parent;

    ui = new Ui::Upa_PpsThresholdScreen();
    ui->setupUi(this);

    QPalette pal;
    pal.setColor(QPalette::Background, Qt::white);
    setPalette(pal);

    connect(ui->PpsDoneButton, SIGNAL(clicked()), this, SLOT(pps_done_button_clicked()));
    connect(ui->PpsChangeThresholdsButton, SIGNAL(clicked()), this, SLOT(pps_change_thresholds_button_clicked()));
    connect(ui->PpsAnalyzerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(pps_analyzer_changed(int)));
    connect(ui->PpsThresholdRefCheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_threshold_changed()));
    connect(ui->PpsThreshold1CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_threshold_changed()));
    connect(ui->PpsThreshold2CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_threshold_changed()));
    connect(ui->PpsThreshold3CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_threshold_changed()));
    connect(ui->PpsThreshold4CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_threshold_changed()));
    connect(ui->PpsThreshold5CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_threshold_changed()));
    connect(ui->PpsThreshold6CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_threshold_changed()));
    connect(ui->PpsThreshold7CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_threshold_changed()));
    connect(ui->PpsThreshold8CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_threshold_changed()));

    for (int i = 0; i < pps_tab->upa->ts_core_config.size(); i++)
    {
        if (0 > ui->PpsAnalyzerComboBox->findText(pps_tab->upa->ts_core_config.at(i).com_port))
        {
            ui->PpsAnalyzerComboBox->addItem(pps_tab->upa->ts_core_config.at(i).com_port);
        }
    }

    for (int i = 0; i < pps_tab->pps_offset_thresholds_high.size(); i++)
    {
        if (pps_tab->upa->ts_core_config.at(i).core_instance_nr == 1)
        {
            *(pps_tab->pps_offset_thresholds_high.at(i)) = 50;
        }
        else
        {
            *(pps_tab->pps_offset_thresholds_high.at(i)) = 1000;
        }
    }

    for (int i = 0; i < pps_tab->pps_offset_thresholds_low.size(); i++)
    {
        if (pps_tab->upa->ts_core_config.at(i).core_instance_nr == 1)
        {
            *(pps_tab->pps_offset_thresholds_low.at(i)) = -50;
        }
        else
        {
            *(pps_tab->pps_offset_thresholds_low.at(i)) = -1000;
        }
    }

    for (int i = 0; i < pps_tab->pps_offset_thresholds.size(); i++)
    {
        if (pps_tab->upa->ts_core_config.at(i).core_instance_nr == 1)
        {
            *(pps_tab->pps_offset_thresholds.at(i)) = 1;
        }
        else
        {
            *(pps_tab->pps_offset_thresholds.at(i)) = 0;
        }
    }

    pps_load_config();

    pps_analyzer_changed(0);

}

Upa_PpsThresholdScreen::~Upa_PpsThresholdScreen()
{
    pps_save_config();
    delete ui;
}

void Upa_PpsThresholdScreen::pps_done_button_clicked(void)
{
    close();
}

void Upa_PpsThresholdScreen::pps_reload(void)
{
    pps_analyzer_changed(0);
}

void Upa_PpsThresholdScreen::pps_change_thresholds_button_clicked(void)
{
    for (int i = 0; i < pps_tab->upa->ts_core_config.size(); i++)
    {
        if (ui->PpsAnalyzerComboBox->currentText() == pps_tab->upa->ts_core_config.at(i).com_port)
        {
            switch (pps_tab->upa->ts_core_config.at(i).core_instance_nr) {
                case 1:
                    *(pps_tab->pps_offset_thresholds_high.at(i)) = ui->PpsThresholdPpsHighRefValue->text().toInt(nullptr, 10);
                    *(pps_tab->pps_offset_thresholds_low.at(i)) = ui->PpsThresholdPpsLowRefValue->text().toInt(nullptr, 10);
                    break;
                case 2:
                    *(pps_tab->pps_offset_thresholds_high.at(i)) = ui->PpsThresholdPpsHigh1Value->text().toInt(nullptr, 10);
                    *(pps_tab->pps_offset_thresholds_low.at(i)) = ui->PpsThresholdPpsLow1Value->text().toInt(nullptr, 10);
                    break;
                case 3:
                    *(pps_tab->pps_offset_thresholds_high.at(i)) = ui->PpsThresholdPpsHigh2Value->text().toInt(nullptr, 10);
                    *(pps_tab->pps_offset_thresholds_low.at(i)) = ui->PpsThresholdPpsLow2Value->text().toInt(nullptr, 10);
                    break;
                case 4:
                    *(pps_tab->pps_offset_thresholds_high.at(i)) = ui->PpsThresholdPpsHigh3Value->text().toInt(nullptr, 10);
                    *(pps_tab->pps_offset_thresholds_low.at(i)) = ui->PpsThresholdPpsLow3Value->text().toInt(nullptr, 10);
                    break;
                case 5:
                    *(pps_tab->pps_offset_thresholds_high.at(i)) = ui->PpsThresholdPpsHigh4Value->text().toInt(nullptr, 10);
                    *(pps_tab->pps_offset_thresholds_low.at(i)) = ui->PpsThresholdPpsLow4Value->text().toInt(nullptr, 10);
                    break;
                case 6:
                    *(pps_tab->pps_offset_thresholds_high.at(i)) = ui->PpsThresholdPpsHigh5Value->text().toInt(nullptr, 10);
                    *(pps_tab->pps_offset_thresholds_low.at(i)) = ui->PpsThresholdPpsLow5Value->text().toInt(nullptr, 10);
                    break;
                case 7:
                    *(pps_tab->pps_offset_thresholds_high.at(i)) = ui->PpsThresholdPpsHigh6Value->text().toInt(nullptr, 10);
                    *(pps_tab->pps_offset_thresholds_low.at(i)) = ui->PpsThresholdPpsLow6Value->text().toInt(nullptr, 10);
                    break;
                case 8:
                    *(pps_tab->pps_offset_thresholds_high.at(i)) = ui->PpsThresholdPpsHigh7Value->text().toInt(nullptr, 10);
                    *(pps_tab->pps_offset_thresholds_low.at(i)) = ui->PpsThresholdPpsLow7Value->text().toInt(nullptr, 10);
                    break;
                case 9:
                    *(pps_tab->pps_offset_thresholds_high.at(i)) = ui->PpsThresholdPpsHigh8Value->text().toInt(nullptr, 10);
                    *(pps_tab->pps_offset_thresholds_low.at(i)) = ui->PpsThresholdPpsLow8Value->text().toInt(nullptr, 10);
                    break;
                default:
                    break;
            }
        }
    }
    pps_save_config();
}

void Upa_PpsThresholdScreen::pps_analyzer_changed(int index)
{
    ui->PpsThresholdPpsHighRefValue->setEnabled(false);
    ui->PpsThresholdPpsHigh1Value->setEnabled(false);
    ui->PpsThresholdPpsHigh2Value->setEnabled(false);
    ui->PpsThresholdPpsHigh3Value->setEnabled(false);
    ui->PpsThresholdPpsHigh4Value->setEnabled(false);
    ui->PpsThresholdPpsHigh5Value->setEnabled(false);
    ui->PpsThresholdPpsHigh6Value->setEnabled(false);
    ui->PpsThresholdPpsHigh7Value->setEnabled(false);
    ui->PpsThresholdPpsHigh8Value->setEnabled(false);
    ui->PpsThresholdPpsLowRefValue->setEnabled(false);
    ui->PpsThresholdPpsLow1Value->setEnabled(false);
    ui->PpsThresholdPpsLow2Value->setEnabled(false);
    ui->PpsThresholdPpsLow3Value->setEnabled(false);
    ui->PpsThresholdPpsLow4Value->setEnabled(false);
    ui->PpsThresholdPpsLow5Value->setEnabled(false);
    ui->PpsThresholdPpsLow6Value->setEnabled(false);
    ui->PpsThresholdPpsLow7Value->setEnabled(false);
    ui->PpsThresholdPpsLow8Value->setEnabled(false);
    ui->PpsThresholdRefCheckBox->setEnabled(false);
    ui->PpsThreshold1CheckBox->setEnabled(false);
    ui->PpsThreshold2CheckBox->setEnabled(false);
    ui->PpsThreshold3CheckBox->setEnabled(false);
    ui->PpsThreshold4CheckBox->setEnabled(false);
    ui->PpsThreshold5CheckBox->setEnabled(false);
    ui->PpsThreshold6CheckBox->setEnabled(false);
    ui->PpsThreshold7CheckBox->setEnabled(false);
    ui->PpsThreshold8CheckBox->setEnabled(false);
    ui->PpsThresholdRefCheckBox->setChecked(false);
    ui->PpsThreshold1CheckBox->setChecked(false);
    ui->PpsThreshold2CheckBox->setChecked(false);
    ui->PpsThreshold3CheckBox->setChecked(false);
    ui->PpsThreshold4CheckBox->setChecked(false);
    ui->PpsThreshold5CheckBox->setChecked(false);
    ui->PpsThreshold6CheckBox->setChecked(false);
    ui->PpsThreshold7CheckBox->setChecked(false);
    ui->PpsThreshold8CheckBox->setChecked(false);

    for (int i = 0; i < pps_tab->upa->ts_core_config.size(); i++)
    {
        if (ui->PpsAnalyzerComboBox->currentText() == pps_tab->upa->ts_core_config.at(i).com_port)
        {
            switch (pps_tab->upa->ts_core_config.at(i).core_instance_nr) {
                case 1:
                    ui->PpsThresholdPpsHighRefValue->setEnabled(true);
                    ui->PpsThresholdPpsHighRefValue->setText(QString::number(*(pps_tab->pps_offset_thresholds_high.at(i))));
                    ui->PpsThresholdPpsLowRefValue->setEnabled(true);
                    ui->PpsThresholdPpsLowRefValue->setText(QString::number(*(pps_tab->pps_offset_thresholds_low.at(i))));
                    ui->PpsThresholdRefCheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_thresholds.at(i)) != 0)
                    {
                        ui->PpsThresholdRefCheckBox->setChecked(true);
                    }
                    break;
                case 2:
                    ui->PpsThresholdPpsHigh1Value->setEnabled(true);
                    ui->PpsThresholdPpsHigh1Value->setText(QString::number(*(pps_tab->pps_offset_thresholds_high.at(i))));
                    ui->PpsThresholdPpsLow1Value->setEnabled(true);
                    ui->PpsThresholdPpsLow1Value->setText(QString::number(*(pps_tab->pps_offset_thresholds_low.at(i))));
                    ui->PpsThreshold1CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_thresholds.at(i)) != 0)
                    {
                        ui->PpsThreshold1CheckBox->setChecked(true);
                    }
                    break;
                case 3:
                    ui->PpsThresholdPpsHigh2Value->setEnabled(true);
                    ui->PpsThresholdPpsHigh2Value->setText(QString::number(*(pps_tab->pps_offset_thresholds_high.at(i))));
                    ui->PpsThresholdPpsLow2Value->setEnabled(true);
                    ui->PpsThresholdPpsLow2Value->setText(QString::number(*(pps_tab->pps_offset_thresholds_low.at(i))));
                    ui->PpsThreshold2CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_thresholds.at(i)) != 0)
                    {
                        ui->PpsThreshold2CheckBox->setChecked(true);
                    }
                    break;
                case 4:
                    ui->PpsThresholdPpsHigh3Value->setEnabled(true);
                    ui->PpsThresholdPpsHigh3Value->setText(QString::number(*(pps_tab->pps_offset_thresholds_high.at(i))));
                    ui->PpsThresholdPpsLow3Value->setEnabled(true);
                    ui->PpsThresholdPpsLow3Value->setText(QString::number(*(pps_tab->pps_offset_thresholds_low.at(i))));
                    ui->PpsThreshold3CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_thresholds.at(i)) != 0)
                    {
                        ui->PpsThreshold3CheckBox->setChecked(true);
                    }
                    break;
                case 5:
                    ui->PpsThresholdPpsHigh4Value->setEnabled(true);
                    ui->PpsThresholdPpsHigh4Value->setText(QString::number(*(pps_tab->pps_offset_thresholds_high.at(i))));
                    ui->PpsThresholdPpsLow4Value->setEnabled(true);
                    ui->PpsThresholdPpsLow4Value->setText(QString::number(*(pps_tab->pps_offset_thresholds_low.at(i))));
                    ui->PpsThreshold4CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_thresholds.at(i)) != 0)
                    {
                        ui->PpsThreshold4CheckBox->setChecked(true);
                    }
                    break;
                case 6:
                    ui->PpsThresholdPpsHigh5Value->setEnabled(true);
                    ui->PpsThresholdPpsHigh5Value->setText(QString::number(*(pps_tab->pps_offset_thresholds_high.at(i))));
                    ui->PpsThresholdPpsLow5Value->setEnabled(true);
                    ui->PpsThresholdPpsLow5Value->setText(QString::number(*(pps_tab->pps_offset_thresholds_low.at(i))));
                    ui->PpsThreshold5CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_thresholds.at(i)) != 0)
                    {
                        ui->PpsThreshold5CheckBox->setChecked(true);
                    }
                    break;
                case 7:
                    ui->PpsThresholdPpsHigh6Value->setEnabled(true);
                    ui->PpsThresholdPpsHigh6Value->setText(QString::number(*(pps_tab->pps_offset_thresholds_high.at(i))));
                    ui->PpsThresholdPpsLow6Value->setEnabled(true);
                    ui->PpsThresholdPpsLow6Value->setText(QString::number(*(pps_tab->pps_offset_thresholds_low.at(i))));
                    ui->PpsThreshold6CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_thresholds.at(i)) != 0)
                    {
                        ui->PpsThreshold6CheckBox->setChecked(true);
                    }
                    break;
                case 8:
                    ui->PpsThresholdPpsHigh7Value->setEnabled(true);
                    ui->PpsThresholdPpsHigh7Value->setText(QString::number(*(pps_tab->pps_offset_thresholds_high.at(i))));
                    ui->PpsThresholdPpsLow7Value->setEnabled(true);
                    ui->PpsThresholdPpsLow7Value->setText(QString::number(*(pps_tab->pps_offset_thresholds_low.at(i))));
                    ui->PpsThreshold7CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_thresholds.at(i)) != 0)
                    {
                        ui->PpsThreshold7CheckBox->setChecked(true);
                    }
                    break;
                case 9:
                    ui->PpsThresholdPpsHigh8Value->setEnabled(true);
                    ui->PpsThresholdPpsHigh8Value->setText(QString::number(*(pps_tab->pps_offset_thresholds_high.at(i))));
                    ui->PpsThresholdPpsLow8Value->setEnabled(true);
                    ui->PpsThresholdPpsLow8Value->setText(QString::number(*(pps_tab->pps_offset_thresholds_low.at(i))));
                    ui->PpsThreshold8CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_thresholds.at(i)) != 0)
                    {
                        ui->PpsThreshold8CheckBox->setChecked(true);
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void Upa_PpsThresholdScreen::pps_threshold_changed(void)
{
    for (int i = 0; i < pps_tab->upa->ts_core_config.size(); i++)
    {
        if (ui->PpsAnalyzerComboBox->currentText() == pps_tab->upa->ts_core_config.at(i).com_port)
        {
            switch (pps_tab->upa->ts_core_config.at(i).core_instance_nr) {
                case 1:
                    if (ui->PpsThresholdRefCheckBox->isChecked() == true)
                    {
                        *(pps_tab->pps_offset_thresholds.at(i)) = 1;
                    }
                    else
                    {
                        *(pps_tab->pps_offset_thresholds.at(i)) = 0;
                    }
                    break;
                case 2:
                    if (ui->PpsThreshold1CheckBox->isChecked() == true)
                    {
                        *(pps_tab->pps_offset_thresholds.at(i)) = 1;
                    }
                    else
                    {
                        *(pps_tab->pps_offset_thresholds.at(i)) = 0;
                    }
                    break;
                case 3:
                    if (ui->PpsThreshold2CheckBox->isChecked() == true)
                    {
                        *(pps_tab->pps_offset_thresholds.at(i)) = 1;
                    }
                    else
                    {
                        *(pps_tab->pps_offset_thresholds.at(i)) = 0;
                    }
                    break;
                case 4:
                    if (ui->PpsThreshold3CheckBox->isChecked() == true)
                    {
                        *(pps_tab->pps_offset_thresholds.at(i)) = 1;
                    }
                    else
                    {
                        *(pps_tab->pps_offset_thresholds.at(i)) = 0;
                    }
                    break;
                case 5:
                    if (ui->PpsThreshold4CheckBox->isChecked() == true)
                    {
                        *(pps_tab->pps_offset_thresholds.at(i)) = 1;
                    }
                    else
                    {
                        *(pps_tab->pps_offset_thresholds.at(i)) = 0;
                    }
                    break;
                case 6:
                    if (ui->PpsThreshold5CheckBox->isChecked() == true)
                    {
                        *(pps_tab->pps_offset_thresholds.at(i)) = 1;
                    }
                    else
                    {
                        *(pps_tab->pps_offset_thresholds.at(i)) = 0;
                    }
                    break;
                case 7:
                    if (ui->PpsThreshold6CheckBox->isChecked() == true)
                    {
                        *(pps_tab->pps_offset_thresholds.at(i)) = 1;
                    }
                    else
                    {
                        *(pps_tab->pps_offset_thresholds.at(i)) = 0;
                    }
                    break;
                case 8:
                    if (ui->PpsThreshold7CheckBox->isChecked() == true)
                    {
                        *(pps_tab->pps_offset_thresholds.at(i)) = 1;
                    }
                    else
                    {
                        *(pps_tab->pps_offset_thresholds.at(i)) = 0;
                    }
                    break;
                case 9:
                    if (ui->PpsThreshold8CheckBox->isChecked() == true)
                    {
                        *(pps_tab->pps_offset_thresholds.at(i)) = 1;
                    }
                    else
                    {
                        *(pps_tab->pps_offset_thresholds.at(i)) = 0;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void Upa_PpsThresholdScreen::pps_load_config(void)
{
    QFile temp_file("./UpaThresholdConfig.cfg");

    if (false == temp_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    while (false == temp_file.atEnd())
    {
        QByteArray temp_line = temp_file.readLine();
        QString temp_string = QString::fromUtf8(temp_line.data());
        QRegExp sep("\\s+");
        for (int i = 0; i < pps_tab->upa->ts_core_config.size(); i++)
        {
            if (true == temp_string.startsWith(pps_tab->upa->ts_core_config.at(i).com_port))
            {
                if (pps_tab->upa->ts_core_config.at(i).core_instance_nr == temp_string.section(sep, 1, 1).toUInt(nullptr, 10))
                {
                    *(pps_tab->pps_offset_thresholds_high.at(i)) = temp_string.section(sep, 2, 2).toInt(nullptr, 10);
                    *(pps_tab->pps_offset_thresholds_low.at(i)) = temp_string.section(sep, 3, 3).toInt(nullptr, 10);
                    *(pps_tab->pps_offset_thresholds.at(i)) = temp_string.section(sep, 4, 4).toInt(nullptr, 10);
                }
            }

        }
    }

    temp_file.close();
}

void Upa_PpsThresholdScreen::pps_save_config(void)
{
    QString temp_string;

    for (int i = 0; i < pps_tab->upa->ts_core_config.size(); i++)
    {
        temp_string.append(pps_tab->upa->ts_core_config.at(i).com_port);
        temp_string.append(" ");
        temp_string.append(QString::number(pps_tab->upa->ts_core_config.at(i).core_instance_nr));
        temp_string.append(" ");
        temp_string.append(QString::number(*(pps_tab->pps_offset_thresholds_high.at(i))));
        temp_string.append(" ");
        temp_string.append(QString::number(*(pps_tab->pps_offset_thresholds_low.at(i))));
        temp_string.append(" ");
        temp_string.append(QString::number(*(pps_tab->pps_offset_thresholds.at(i))));
        temp_string.append("\n");
    }

    QFile temp_file("./UpaThresholdConfig.cfg");

    if (false == temp_file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream temp_stream(&temp_file);

    temp_stream << temp_string << endl;

    temp_file.close();
}
