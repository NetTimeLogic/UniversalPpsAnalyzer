#include <Upa_PpsConfigScreen.h>
#include <Upa_PpsTab.h>
#include <ui_Upa_PpsConfigScreen.h>

Upa_PpsConfigScreen::Upa_PpsConfigScreen(Upa_PpsTab *parent) : QWidget()
{
    QString temp_string;
    pps_tab = parent;

    ui = new Ui::Upa_PpsConfigScreen();
    ui->setupUi(this);

    QPalette pal;
    pal.setColor(QPalette::Background, Qt::white);
    setPalette(pal);

    connect(ui->PpsDoneButton, SIGNAL(clicked()), this, SLOT(pps_done_button_clicked()));
    connect(ui->PpsChangeDelaysAndNamesButton, SIGNAL(clicked()), this, SLOT(pps_change_delays_and_names_button_clicked()));
    connect(ui->PpsAnalyzerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(pps_analyzer_changed(int)));
    connect(ui->PpsGraph1CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_graph_changed()));
    connect(ui->PpsGraph2CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_graph_changed()));
    connect(ui->PpsGraph3CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_graph_changed()));
    connect(ui->PpsGraph4CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_graph_changed()));
    connect(ui->PpsGraph5CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_graph_changed()));
    connect(ui->PpsGraph6CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_graph_changed()));
    connect(ui->PpsGraph7CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_graph_changed()));
    connect(ui->PpsGraph8CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_graph_changed()));

    for (int i = 0; i < pps_tab->upa->ts_core_config.size(); i++)
    {
        if (0 > ui->PpsAnalyzerComboBox->findText(pps_tab->upa->ts_core_config.at(i).com_port))
        {
            ui->PpsAnalyzerComboBox->addItem(pps_tab->upa->ts_core_config.at(i).com_port);
        }
    }

    for (int i = 0; i < pps_tab->pps_offset_delays.size(); i++)
    {
        *(pps_tab->pps_offset_delays.at(i)) = 0;
    }

    for (int i = 0; i < pps_tab->pps_offset_names.size(); i++)
    {

        temp_string.clear();
        temp_string.append("PPS_");
        if (pps_tab->upa->ts_core_config.at(i).core_instance_nr == 1)
        {
            temp_string.append("REF");
        }
        else
        {
            temp_string.append(QString("%1").arg((pps_tab->upa->ts_core_config.at(i).core_instance_nr-1), 2, 10, QLatin1Char('0')).toUpper());
        }
        (pps_tab->pps_offset_names.at(i))->clear();
        (pps_tab->pps_offset_names.at(i))->append(temp_string);

    }

    for (int i = 0; i < pps_tab->pps_offset_show.size(); i++)
    {
        *(pps_tab->pps_offset_show.at(i)) = 1;
    }

    pps_load_config();

    pps_analyzer_changed(0);

}

Upa_PpsConfigScreen::~Upa_PpsConfigScreen()
{
    pps_save_config();
    delete ui;
}

void Upa_PpsConfigScreen::pps_done_button_clicked(void)
{
    close();
}

void Upa_PpsConfigScreen::pps_reload(void)
{
    pps_analyzer_changed(0);
}

void Upa_PpsConfigScreen::pps_change_delays(void)
{
    for (int i = 0; i < pps_tab->upa->ts_core_config.size(); i++)
    {
        if (ui->PpsAnalyzerComboBox->currentText() == pps_tab->upa->ts_core_config.at(i).com_port)
        {
            switch (pps_tab->upa->ts_core_config.at(i).core_instance_nr) {
                case 1:
                    *(pps_tab->pps_offset_delays.at(i)) = ui->PpsDelayPpsRefValue->text().toInt(nullptr, 10);
                    break;
                case 2:
                    *(pps_tab->pps_offset_delays.at(i)) = ui->PpsDelayPps1Value->text().toInt(nullptr, 10);
                    break;
                case 3:
                    *(pps_tab->pps_offset_delays.at(i)) = ui->PpsDelayPps2Value->text().toInt(nullptr, 10);
                    break;
                case 4:
                    *(pps_tab->pps_offset_delays.at(i)) = ui->PpsDelayPps3Value->text().toInt(nullptr, 10);
                    break;
                case 5:
                    *(pps_tab->pps_offset_delays.at(i)) = ui->PpsDelayPps4Value->text().toInt(nullptr, 10);
                    break;
                case 6:
                    *(pps_tab->pps_offset_delays.at(i)) = ui->PpsDelayPps5Value->text().toInt(nullptr, 10);
                    break;
                case 7:
                    *(pps_tab->pps_offset_delays.at(i)) = ui->PpsDelayPps6Value->text().toInt(nullptr, 10);
                    break;
                case 8:
                    *(pps_tab->pps_offset_delays.at(i)) = ui->PpsDelayPps7Value->text().toInt(nullptr, 10);
                    break;
                case 9:
                    *(pps_tab->pps_offset_delays.at(i)) = ui->PpsDelayPps8Value->text().toInt(nullptr, 10);
                    break;
                default:
                    break;
            }
        }
    }
}

void Upa_PpsConfigScreen::pps_change_names(void)
{
    for (int i = 0; i < pps_tab->upa->ts_core_config.size(); i++)
    {
        if (ui->PpsAnalyzerComboBox->currentText() == pps_tab->upa->ts_core_config.at(i).com_port)
        {
            switch (pps_tab->upa->ts_core_config.at(i).core_instance_nr) {
                case 1:
                    break;
                case 2:
                    pps_tab->pps_offset_names.at(i)->clear();
                    pps_tab->pps_offset_names.at(i)->append(ui->PpsNamePps1Value->text());
                    break;
                case 3:
                    pps_tab->pps_offset_names.at(i)->clear();
                    pps_tab->pps_offset_names.at(i)->append(ui->PpsNamePps2Value->text());
                    break;
                case 4:
                    pps_tab->pps_offset_names.at(i)->clear();
                    pps_tab->pps_offset_names.at(i)->append(ui->PpsNamePps3Value->text());
                    break;
                case 5:
                    pps_tab->pps_offset_names.at(i)->clear();
                    pps_tab->pps_offset_names.at(i)->append(ui->PpsNamePps4Value->text());
                    break;
                case 6:
                    pps_tab->pps_offset_names.at(i)->clear();
                    pps_tab->pps_offset_names.at(i)->append(ui->PpsNamePps5Value->text());
                    break;
                case 7:
                    pps_tab->pps_offset_names.at(i)->clear();
                    pps_tab->pps_offset_names.at(i)->append(ui->PpsNamePps6Value->text());
                    break;
                case 8:
                    pps_tab->pps_offset_names.at(i)->clear();
                    pps_tab->pps_offset_names.at(i)->append(ui->PpsNamePps7Value->text());
                    break;
                case 9:
                    pps_tab->pps_offset_names.at(i)->clear();
                    pps_tab->pps_offset_names.at(i)->append(ui->PpsNamePps8Value->text());
                    break;
                default:
                    break;
            }
        }
    }
}

void Upa_PpsConfigScreen::pps_change_delays_and_names_button_clicked(void)
{
    pps_change_names();
    pps_change_delays();
    pps_save_config();
}

void Upa_PpsConfigScreen::pps_analyzer_changed(int index)
{
    ui->PpsDelayPpsRefValue->setEnabled(false);
    ui->PpsDelayPps1Value->setEnabled(false);
    ui->PpsDelayPps2Value->setEnabled(false);
    ui->PpsDelayPps3Value->setEnabled(false);
    ui->PpsDelayPps4Value->setEnabled(false);
    ui->PpsDelayPps5Value->setEnabled(false);
    ui->PpsDelayPps6Value->setEnabled(false);
    ui->PpsDelayPps7Value->setEnabled(false);
    ui->PpsDelayPps8Value->setEnabled(false);
    ui->PpsGraph1CheckBox->setEnabled(false);
    ui->PpsGraph2CheckBox->setEnabled(false);
    ui->PpsGraph3CheckBox->setEnabled(false);
    ui->PpsGraph4CheckBox->setEnabled(false);
    ui->PpsGraph5CheckBox->setEnabled(false);
    ui->PpsGraph6CheckBox->setEnabled(false);
    ui->PpsGraph7CheckBox->setEnabled(false);
    ui->PpsGraph8CheckBox->setEnabled(false);
    ui->PpsGraph1CheckBox->setChecked(false);
    ui->PpsGraph2CheckBox->setChecked(false);
    ui->PpsGraph3CheckBox->setChecked(false);
    ui->PpsGraph4CheckBox->setChecked(false);
    ui->PpsGraph5CheckBox->setChecked(false);
    ui->PpsGraph6CheckBox->setChecked(false);
    ui->PpsGraph7CheckBox->setChecked(false);
    ui->PpsGraph8CheckBox->setChecked(false);

    for (int i = 0; i < pps_tab->upa->ts_core_config.size(); i++)
    {
        if (ui->PpsAnalyzerComboBox->currentText() == pps_tab->upa->ts_core_config.at(i).com_port)
        {
            switch (pps_tab->upa->ts_core_config.at(i).core_instance_nr) {
                case 1:
                    ui->PpsDelayPpsRefValue->setEnabled(true);
                    ui->PpsDelayPpsRefValue->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    break;
                case 2:
                    ui->PpsDelayPps1Value->setEnabled(true);
                    ui->PpsGraph1CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_show.at(i)) != 0)
                    {
                        ui->PpsGraph1CheckBox->setChecked(true);
                    }
                    ui->PpsDelayPps1Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    ui->PpsNamePps1Value->setText(*(pps_tab->pps_offset_names.at(i)));
                    break;
                case 3:
                    ui->PpsDelayPps2Value->setEnabled(true);
                    ui->PpsGraph2CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_show.at(i)) != 0)
                    {
                        ui->PpsGraph2CheckBox->setChecked(true);
                    }
                    ui->PpsDelayPps2Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    ui->PpsNamePps2Value->setText(*(pps_tab->pps_offset_names.at(i)));
                    break;
                case 4:
                    ui->PpsDelayPps3Value->setEnabled(true);
                    ui->PpsGraph3CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_show.at(i)) != 0)
                    {
                        ui->PpsGraph3CheckBox->setChecked(true);
                    }
                    ui->PpsDelayPps3Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    ui->PpsNamePps3Value->setText(*(pps_tab->pps_offset_names.at(i)));
                    break;
                case 5:
                    ui->PpsDelayPps4Value->setEnabled(true);
                    ui->PpsGraph4CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_show.at(i)) != 0)
                    {
                        ui->PpsGraph4CheckBox->setChecked(true);
                    }
                    ui->PpsDelayPps4Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    ui->PpsNamePps4Value->setText(*(pps_tab->pps_offset_names.at(i)));
                    break;
                case 6:
                    ui->PpsDelayPps5Value->setEnabled(true);
                    ui->PpsGraph5CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_show.at(i)) != 0)
                    {
                        ui->PpsGraph5CheckBox->setChecked(true);
                    }
                    ui->PpsDelayPps5Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    ui->PpsNamePps5Value->setText(*(pps_tab->pps_offset_names.at(i)));
                    break;
                case 7:
                    ui->PpsDelayPps6Value->setEnabled(true);
                    ui->PpsGraph6CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_show.at(i)) != 0)
                    {
                        ui->PpsGraph6CheckBox->setChecked(true);
                    }
                    ui->PpsDelayPps6Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    ui->PpsNamePps6Value->setText(*(pps_tab->pps_offset_names.at(i)));
                    break;
                case 8:
                    ui->PpsDelayPps7Value->setEnabled(true);
                    ui->PpsGraph7CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_show.at(i)) != 0)
                    {
                        ui->PpsGraph7CheckBox->setChecked(true);
                    }
                    ui->PpsDelayPps7Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    ui->PpsNamePps7Value->setText(*(pps_tab->pps_offset_names.at(i)));
                    break;
                case 9:
                    ui->PpsDelayPps8Value->setEnabled(true);
                    ui->PpsGraph8CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_show.at(i)) != 0)
                    {
                        ui->PpsGraph8CheckBox->setChecked(true);
                    }
                    ui->PpsDelayPps8Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    ui->PpsNamePps8Value->setText(*(pps_tab->pps_offset_names.at(i)));
                    break;
                default:
                    break;
            }
        }
    }
}

void Upa_PpsConfigScreen::pps_graph_changed(void)
{
    for (int i = 0; i < pps_tab->upa->ts_core_config.size(); i++)
    {
        if (ui->PpsAnalyzerComboBox->currentText() == pps_tab->upa->ts_core_config.at(i).com_port)
        {
            switch (pps_tab->upa->ts_core_config.at(i).core_instance_nr) {
                case 1:
                    break;
                case 2:
                    if (ui->PpsGraph1CheckBox->isChecked() == true)
                    {
                        *(pps_tab->pps_offset_show.at(i)) = 1;
                    }
                    else
                    {
                        *(pps_tab->pps_offset_show.at(i)) = 0;
                    }
                    break;
                case 3:
                    if (ui->PpsGraph2CheckBox->isChecked() == true)
                    {
                        *(pps_tab->pps_offset_show.at(i)) = 1;
                    }
                    else
                    {
                        *(pps_tab->pps_offset_show.at(i)) = 0;
                    }
                    break;
                case 4:
                    if (ui->PpsGraph3CheckBox->isChecked() == true)
                    {
                        *(pps_tab->pps_offset_show.at(i)) = 1;
                    }
                    else
                    {
                        *(pps_tab->pps_offset_show.at(i)) = 0;
                    }
                    break;
                case 5:
                    if (ui->PpsGraph4CheckBox->isChecked() == true)
                    {
                        *(pps_tab->pps_offset_show.at(i)) = 1;
                    }
                    else
                    {
                        *(pps_tab->pps_offset_show.at(i)) = 0;
                    }
                    break;
                case 6:
                    if (ui->PpsGraph5CheckBox->isChecked() == true)
                    {
                        *(pps_tab->pps_offset_show.at(i)) = 1;
                    }
                    else
                    {
                        *(pps_tab->pps_offset_show.at(i)) = 0;
                    }
                    break;
                case 7:
                    if (ui->PpsGraph6CheckBox->isChecked() == true)
                    {
                        *(pps_tab->pps_offset_show.at(i)) = 1;
                    }
                    else
                    {
                        *(pps_tab->pps_offset_show.at(i)) = 0;
                    }
                    break;
                case 8:
                    if (ui->PpsGraph7CheckBox->isChecked() == true)
                    {
                        *(pps_tab->pps_offset_show.at(i)) = 1;
                    }
                    else
                    {
                        *(pps_tab->pps_offset_show.at(i)) = 0;
                    }
                    break;
                case 9:
                    if (ui->PpsGraph8CheckBox->isChecked() == true)
                    {
                        *(pps_tab->pps_offset_show.at(i)) = 1;
                    }
                    else
                    {
                        *(pps_tab->pps_offset_show.at(i)) = 0;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void Upa_PpsConfigScreen::pps_load_config(void)
{
    QFile temp_file("./UpaDelayConfig.cfg");

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
                    *(pps_tab->pps_offset_delays.at(i)) = temp_string.section(sep, 2, 2).toInt(nullptr, 10);
                    *(pps_tab->pps_offset_show.at(i)) = temp_string.section(sep, 3, 3).toInt(nullptr, 10);
                    if (pps_tab->upa->ts_core_config.at(i).core_instance_nr != 1)
                    {
                        if ("" != temp_string.section(sep, 4, 4))
                        {
                            (pps_tab->pps_offset_names.at(i))->clear();
                            (pps_tab->pps_offset_names.at(i))->append(temp_string.section(sep, 4, 4));
                        }
                    }
                }
            }

        }
    }

    temp_file.close();
}

void Upa_PpsConfigScreen::pps_save_config(void)
{
    QString temp_string;

    for (int i = 0; i < pps_tab->upa->ts_core_config.size(); i++)
    {
        temp_string.append(pps_tab->upa->ts_core_config.at(i).com_port);
        temp_string.append(" ");
        temp_string.append(QString::number(pps_tab->upa->ts_core_config.at(i).core_instance_nr));
        temp_string.append(" ");
        temp_string.append(QString::number(*(pps_tab->pps_offset_delays.at(i))));
        temp_string.append(" ");
        temp_string.append(QString::number(*(pps_tab->pps_offset_show.at(i))));
        temp_string.append(" ");
        temp_string.append(*pps_tab->pps_offset_names.at(i));
        temp_string.append("\n");
    }

    QFile temp_file("./UpaDelayConfig.cfg");

    if (false == temp_file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream temp_stream(&temp_file);

    temp_stream << temp_string << endl;

    temp_file.close();
}
