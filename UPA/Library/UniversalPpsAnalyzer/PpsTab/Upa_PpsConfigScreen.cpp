#include <Upa_PpsConfigScreen.h>
#include <Upa_PpsTab.h>
#include <ui_Upa_PpsConfigScreen.h>

Upa_PpsConfigScreen::Upa_PpsConfigScreen(Upa_PpsTab *parent) : QDialog()
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

    // add the boards found to the drop down
    for (int i = 0; i < pps_tab->pps_boards.size(); i++)
    {
        ui->PpsAnalyzerComboBox->addItem(pps_tab->pps_boards.at(i)->com_port);
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
    for (int i = 0; i < pps_tab->pps_boards.size(); i++)
    {
        if (ui->PpsAnalyzerComboBox->currentText() == pps_tab->pps_boards.at(i)->com_port)
        {
            pps_tab->pps_boards.at(i)->pps_ctrl[0].pps_delay = ui->PpsDelayPpsRefValue->text().toInt(nullptr, 10);
            pps_tab->pps_boards.at(i)->pps_ctrl[1].pps_delay = ui->PpsDelayPps1Value->text().toInt(nullptr, 10);
            pps_tab->pps_boards.at(i)->pps_ctrl[2].pps_delay = ui->PpsDelayPps2Value->text().toInt(nullptr, 10);
            pps_tab->pps_boards.at(i)->pps_ctrl[3].pps_delay = ui->PpsDelayPps3Value->text().toInt(nullptr, 10);
            pps_tab->pps_boards.at(i)->pps_ctrl[4].pps_delay = ui->PpsDelayPps4Value->text().toInt(nullptr, 10);
            pps_tab->pps_boards.at(i)->pps_ctrl[5].pps_delay = ui->PpsDelayPps5Value->text().toInt(nullptr, 10);
            pps_tab->pps_boards.at(i)->pps_ctrl[6].pps_delay = ui->PpsDelayPps6Value->text().toInt(nullptr, 10);
            pps_tab->pps_boards.at(i)->pps_ctrl[7].pps_delay = ui->PpsDelayPps7Value->text().toInt(nullptr, 10);
            pps_tab->pps_boards.at(i)->pps_ctrl[8].pps_delay = ui->PpsDelayPps8Value->text().toInt(nullptr, 10);
        }
    }
}

void Upa_PpsConfigScreen::pps_change_names(void)
{
    for (int i = 0; i < pps_tab->pps_boards.size(); i++)
    {
        if (ui->PpsAnalyzerComboBox->currentText() == pps_tab->pps_boards.at(i)->com_port)
        {
            pps_tab->pps_boards.at(i)->pps_gui[1].pps_name.clear();
            pps_tab->pps_boards.at(i)->pps_gui[2].pps_name.clear();
            pps_tab->pps_boards.at(i)->pps_gui[3].pps_name.clear();
            pps_tab->pps_boards.at(i)->pps_gui[4].pps_name.clear();
            pps_tab->pps_boards.at(i)->pps_gui[5].pps_name.clear();
            pps_tab->pps_boards.at(i)->pps_gui[6].pps_name.clear();
            pps_tab->pps_boards.at(i)->pps_gui[7].pps_name.clear();
            pps_tab->pps_boards.at(i)->pps_gui[8].pps_name.clear();
            pps_tab->pps_boards.at(i)->pps_gui[1].pps_name.append(ui->PpsNamePps1Value->text());
            pps_tab->pps_boards.at(i)->pps_gui[2].pps_name.append(ui->PpsNamePps2Value->text());
            pps_tab->pps_boards.at(i)->pps_gui[3].pps_name.append(ui->PpsNamePps3Value->text());
            pps_tab->pps_boards.at(i)->pps_gui[4].pps_name.append(ui->PpsNamePps4Value->text());
            pps_tab->pps_boards.at(i)->pps_gui[5].pps_name.append(ui->PpsNamePps5Value->text());
            pps_tab->pps_boards.at(i)->pps_gui[6].pps_name.append(ui->PpsNamePps6Value->text());
            pps_tab->pps_boards.at(i)->pps_gui[7].pps_name.append(ui->PpsNamePps7Value->text());
            pps_tab->pps_boards.at(i)->pps_gui[8].pps_name.append(ui->PpsNamePps8Value->text());
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

    ui->PpsNamePps1Value->setEnabled(false);
    ui->PpsNamePps2Value->setEnabled(false);
    ui->PpsNamePps3Value->setEnabled(false);
    ui->PpsNamePps4Value->setEnabled(false);
    ui->PpsNamePps5Value->setEnabled(false);
    ui->PpsNamePps6Value->setEnabled(false);
    ui->PpsNamePps7Value->setEnabled(false);
    ui->PpsNamePps8Value->setEnabled(false);

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

    for (int i = 0; i < pps_tab->pps_boards.size(); i++)
    {
        if (ui->PpsAnalyzerComboBox->currentText() == pps_tab->pps_boards.at(i)->com_port)
        {
            ui->PpsDelayPpsRefValue->setEnabled(true);
            ui->PpsDelayPps1Value->setEnabled(true);
            ui->PpsDelayPps2Value->setEnabled(true);
            ui->PpsDelayPps3Value->setEnabled(true);
            ui->PpsDelayPps4Value->setEnabled(true);
            ui->PpsDelayPps5Value->setEnabled(true);
            ui->PpsDelayPps6Value->setEnabled(true);
            ui->PpsDelayPps7Value->setEnabled(true);
            ui->PpsDelayPps8Value->setEnabled(true);
            ui->PpsDelayPpsRefValue->setText(QString::number(pps_tab->pps_boards.at(i)->pps_ctrl[0].pps_delay));
            ui->PpsDelayPps1Value->setText(QString::number(pps_tab->pps_boards.at(i)->pps_ctrl[1].pps_delay));
            ui->PpsDelayPps2Value->setText(QString::number(pps_tab->pps_boards.at(i)->pps_ctrl[2].pps_delay));
            ui->PpsDelayPps3Value->setText(QString::number(pps_tab->pps_boards.at(i)->pps_ctrl[3].pps_delay));
            ui->PpsDelayPps4Value->setText(QString::number(pps_tab->pps_boards.at(i)->pps_ctrl[4].pps_delay));
            ui->PpsDelayPps5Value->setText(QString::number(pps_tab->pps_boards.at(i)->pps_ctrl[5].pps_delay));
            ui->PpsDelayPps6Value->setText(QString::number(pps_tab->pps_boards.at(i)->pps_ctrl[6].pps_delay));
            ui->PpsDelayPps7Value->setText(QString::number(pps_tab->pps_boards.at(i)->pps_ctrl[7].pps_delay));
            ui->PpsDelayPps8Value->setText(QString::number(pps_tab->pps_boards.at(i)->pps_ctrl[8].pps_delay));

            ui->PpsNamePps1Value->setEnabled(true);
            ui->PpsNamePps2Value->setEnabled(true);
            ui->PpsNamePps3Value->setEnabled(true);
            ui->PpsNamePps4Value->setEnabled(true);
            ui->PpsNamePps5Value->setEnabled(true);
            ui->PpsNamePps6Value->setEnabled(true);
            ui->PpsNamePps7Value->setEnabled(true);
            ui->PpsNamePps8Value->setEnabled(true);
            ui->PpsNamePps1Value->setText(pps_tab->pps_boards.at(i)->pps_gui[1].pps_name);
            ui->PpsNamePps2Value->setText(pps_tab->pps_boards.at(i)->pps_gui[2].pps_name);
            ui->PpsNamePps3Value->setText(pps_tab->pps_boards.at(i)->pps_gui[3].pps_name);
            ui->PpsNamePps4Value->setText(pps_tab->pps_boards.at(i)->pps_gui[4].pps_name);
            ui->PpsNamePps5Value->setText(pps_tab->pps_boards.at(i)->pps_gui[5].pps_name);
            ui->PpsNamePps6Value->setText(pps_tab->pps_boards.at(i)->pps_gui[6].pps_name);
            ui->PpsNamePps7Value->setText(pps_tab->pps_boards.at(i)->pps_gui[7].pps_name);
            ui->PpsNamePps8Value->setText(pps_tab->pps_boards.at(i)->pps_gui[8].pps_name);

            ui->PpsGraph1CheckBox->setEnabled(true);
            ui->PpsGraph2CheckBox->setEnabled(true);
            ui->PpsGraph3CheckBox->setEnabled(true);
            ui->PpsGraph4CheckBox->setEnabled(true);
            ui->PpsGraph5CheckBox->setEnabled(true);
            ui->PpsGraph6CheckBox->setEnabled(true);
            ui->PpsGraph7CheckBox->setEnabled(true);
            ui->PpsGraph8CheckBox->setEnabled(true);
            if (pps_tab->pps_boards.at(i)->pps_gui[1].pps_show != 0)
            {
                ui->PpsGraph1CheckBox->setChecked(true);
            }
            if (pps_tab->pps_boards.at(i)->pps_gui[2].pps_show != 0)
            {
                ui->PpsGraph2CheckBox->setChecked(true);
            }
            if (pps_tab->pps_boards.at(i)->pps_gui[3].pps_show != 0)
            {
                ui->PpsGraph3CheckBox->setChecked(true);
            }
            if (pps_tab->pps_boards.at(i)->pps_gui[4].pps_show != 0)
            {
                ui->PpsGraph4CheckBox->setChecked(true);
            }
            if (pps_tab->pps_boards.at(i)->pps_gui[5].pps_show != 0)
            {
                ui->PpsGraph5CheckBox->setChecked(true);
            }
            if (pps_tab->pps_boards.at(i)->pps_gui[6].pps_show != 0)
            {
                ui->PpsGraph6CheckBox->setChecked(true);
            }
            if (pps_tab->pps_boards.at(i)->pps_gui[7].pps_show != 0)
            {
                ui->PpsGraph7CheckBox->setChecked(true);
            }
            if (pps_tab->pps_boards.at(i)->pps_gui[8].pps_show != 0)
            {
                ui->PpsGraph8CheckBox->setChecked(true);
            }
        }
    }
}

void Upa_PpsConfigScreen::pps_graph_changed(void)
{
    for (int i = 0; i < pps_tab->pps_boards.size(); i++)
    {
        if (ui->PpsAnalyzerComboBox->currentText() == pps_tab->pps_boards.at(i)->com_port)
        {
            if (ui->PpsGraph1CheckBox->isChecked() == true)
            {
                pps_tab->pps_boards.at(i)->pps_gui[1].pps_show = 1;
            }
            else
            {
                pps_tab->pps_boards.at(i)->pps_gui[1].pps_show = 0;
            }
            if (ui->PpsGraph2CheckBox->isChecked() == true)
            {
                pps_tab->pps_boards.at(i)->pps_gui[2].pps_show = 1;
            }
            else
            {
                pps_tab->pps_boards.at(i)->pps_gui[2].pps_show = 0;
            }
            if (ui->PpsGraph3CheckBox->isChecked() == true)
            {
                pps_tab->pps_boards.at(i)->pps_gui[3].pps_show = 1;
            }
            else
            {
                pps_tab->pps_boards.at(i)->pps_gui[3].pps_show = 0;
            }
            if (ui->PpsGraph4CheckBox->isChecked() == true)
            {
                pps_tab->pps_boards.at(i)->pps_gui[4].pps_show = 1;
            }
            else
            {
                pps_tab->pps_boards.at(i)->pps_gui[4].pps_show = 0;
            }
            if (ui->PpsGraph5CheckBox->isChecked() == true)
            {
                pps_tab->pps_boards.at(i)->pps_gui[5].pps_show = 1;
            }
            else
            {
                pps_tab->pps_boards.at(i)->pps_gui[5].pps_show = 0;
            }
            if (ui->PpsGraph6CheckBox->isChecked() == true)
            {
                pps_tab->pps_boards.at(i)->pps_gui[6].pps_show = 1;
            }
            else
            {
                pps_tab->pps_boards.at(i)->pps_gui[6].pps_show = 0;
            }
            if (ui->PpsGraph7CheckBox->isChecked() == true)
            {
                pps_tab->pps_boards.at(i)->pps_gui[7].pps_show = 1;
            }
            else
            {
                pps_tab->pps_boards.at(i)->pps_gui[7].pps_show = 0;
            }
            if (ui->PpsGraph8CheckBox->isChecked() == true)
            {
                pps_tab->pps_boards.at(i)->pps_gui[8].pps_show = 1;
            }
            else
            {
                pps_tab->pps_boards.at(i)->pps_gui[8].pps_show = 0;
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
        for (int i = 0; i < pps_tab->pps_boards.size(); i++)
        {
            if (true == temp_string.startsWith(pps_tab->pps_boards.at(i)->com_port))
            {
                for (int j = 0; j < Upa_PpsPerBoard; j++)
                {
                    if (pps_tab->pps_boards.at(i)->pps_ts[j].ts_core_config->core_instance_nr == temp_string.section(sep, 1, 1).toUInt(nullptr, 10))
                    {
                        pps_tab->pps_boards.at(i)->pps_ctrl[j].pps_delay = temp_string.section(sep, 2, 2).toInt(nullptr, 10);
                        if (j != 0)
                        {
                            pps_tab->pps_boards.at(i)->pps_gui[j].pps_show = temp_string.section(sep, 3, 3).toInt(nullptr, 10);
                            if ("" != temp_string.section(sep, 4, 4))
                            {
                                pps_tab->pps_boards.at(i)->pps_gui[j].pps_name.clear();
                                pps_tab->pps_boards.at(i)->pps_gui[j].pps_name.append(temp_string.section(sep, 4, 4));
                            }
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

    for (int i = 0; i < pps_tab->pps_boards.size(); i++)
    {
        for (int j = 0; j < Upa_PpsPerBoard; j++)
        {
            temp_string.append(pps_tab->pps_boards.at(i)->com_port);
            temp_string.append(" ");
            temp_string.append(QString::number(pps_tab->pps_boards.at(i)->pps_ts[j].ts_core_config->core_instance_nr));
            temp_string.append(" ");
            temp_string.append(QString::number(pps_tab->pps_boards.at(i)->pps_ctrl[j].pps_delay));
            temp_string.append(" ");
            temp_string.append(QString::number(pps_tab->pps_boards.at(i)->pps_gui[j].pps_show));
            temp_string.append(" ");
            temp_string.append(pps_tab->pps_boards.at(i)->pps_gui[j].pps_name);
            temp_string.append("\n");
        }
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
