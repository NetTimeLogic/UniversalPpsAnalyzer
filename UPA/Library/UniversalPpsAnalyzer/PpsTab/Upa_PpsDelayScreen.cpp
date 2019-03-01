#include <Upa_PpsDelayScreen.h>
#include <Upa_PpsTab.h>
#include <ui_Upa_PpsDelayScreen.h>

Upa_PpsDelayScreen::Upa_PpsDelayScreen(Upa_PpsTab *parent) : QWidget()
{
    pps_tab = parent;

    ui = new Ui::Upa_PpsDelayScreen();
    ui->setupUi(this);

    connect(ui->PpsDoneButton, SIGNAL(clicked()), this, SLOT(pps_done_button_clicked()));
    connect(ui->PpsChangeDelaysButton, SIGNAL(clicked()), this, SLOT(pps_change_delays_button_clicked()));
    connect(ui->PpsAnalyzerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(pps_analyzer_changed(int)));
    connect(ui->PpsGraph1CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_graph_changed()));
    connect(ui->PpsGraph2CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_graph_changed()));
    connect(ui->PpsGraph3CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_graph_changed()));
    connect(ui->PpsGraph4CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_graph_changed()));
    connect(ui->PpsGraph5CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_graph_changed()));
    connect(ui->PpsGraph6CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_graph_changed()));
    connect(ui->PpsGraph7CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_graph_changed()));
    connect(ui->PpsGraph8CheckBox, SIGNAL(clicked(bool)), this, SLOT(pps_graph_changed()));

    for (int i = 0; i < pps_tab->upa->core_config.size(); i++)
    {
        if (0 > ui->PpsAnalyzerComboBox->findText(pps_tab->upa->core_config.at(i).com_port))
        {
            ui->PpsAnalyzerComboBox->addItem(pps_tab->upa->core_config.at(i).com_port);
        }
    }

    for (int i = 0; i < pps_tab->pps_offset_delays.size(); i++)
    {
        *(pps_tab->pps_offset_delays.at(i)) = 0;
    }

    for (int i = 0; i < pps_tab->pps_offset_show.size(); i++)
    {
        *(pps_tab->pps_offset_show.at(i)) = 1;
    }

    pps_analyzer_changed(0);

}

Upa_PpsDelayScreen::~Upa_PpsDelayScreen()
{
    delete ui;
}

void Upa_PpsDelayScreen::pps_done_button_clicked(void)
{
    close();
}

void Upa_PpsDelayScreen::pps_reload(void)
{
    pps_analyzer_changed(0);
}

void Upa_PpsDelayScreen::pps_change_delays_button_clicked(void)
{
    for (int i = 0; i < pps_tab->upa->core_config.size(); i++)
    {
        if (ui->PpsAnalyzerComboBox->currentText() == pps_tab->upa->core_config.at(i).com_port)
        {
            switch (pps_tab->upa->core_config.at(i).core_instance_nr) {
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

void Upa_PpsDelayScreen::pps_analyzer_changed(int index)
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

    for (int i = 0; i < pps_tab->upa->core_config.size(); i++)
    {
        if (ui->PpsAnalyzerComboBox->currentText() == pps_tab->upa->core_config.at(i).com_port)
        {
            switch (pps_tab->upa->core_config.at(i).core_instance_nr) {
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
                    break;
                case 3:
                    ui->PpsDelayPps2Value->setEnabled(true);
                    ui->PpsGraph2CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_show.at(i)) != 0)
                    {
                        ui->PpsGraph2CheckBox->setChecked(true);
                    }
                    ui->PpsDelayPps2Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    break;
                case 4:
                    ui->PpsDelayPps3Value->setEnabled(true);
                    ui->PpsGraph3CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_show.at(i)) != 0)
                    {
                        ui->PpsGraph3CheckBox->setChecked(true);
                    }
                    ui->PpsDelayPps3Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    break;
                case 5:
                    ui->PpsDelayPps4Value->setEnabled(true);
                    ui->PpsGraph4CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_show.at(i)) != 0)
                    {
                        ui->PpsGraph4CheckBox->setChecked(true);
                    }
                    ui->PpsDelayPps4Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    break;
                case 6:
                    ui->PpsDelayPps5Value->setEnabled(true);
                    ui->PpsGraph5CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_show.at(i)) != 0)
                    {
                        ui->PpsGraph5CheckBox->setChecked(true);
                    }
                    ui->PpsDelayPps5Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    break;
                case 7:
                    ui->PpsDelayPps6Value->setEnabled(true);
                    ui->PpsGraph6CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_show.at(i)) != 0)
                    {
                        ui->PpsGraph6CheckBox->setChecked(true);
                    }
                    ui->PpsDelayPps6Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    break;
                case 8:
                    ui->PpsDelayPps7Value->setEnabled(true);
                    ui->PpsGraph7CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_show.at(i)) != 0)
                    {
                        ui->PpsGraph7CheckBox->setChecked(true);
                    }
                    ui->PpsDelayPps7Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    break;
                case 9:
                    ui->PpsDelayPps8Value->setEnabled(true);
                    ui->PpsGraph8CheckBox->setEnabled(true);
                    if (*(pps_tab->pps_offset_show.at(i)) != 0)
                    {
                        ui->PpsGraph8CheckBox->setChecked(true);
                    }
                    ui->PpsDelayPps8Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    break;
                default:
                    break;
            }
        }
    }
}

void Upa_PpsDelayScreen::pps_graph_changed(void)
{
    for (int i = 0; i < pps_tab->upa->core_config.size(); i++)
    {
        if (ui->PpsAnalyzerComboBox->currentText() == pps_tab->upa->core_config.at(i).com_port)
        {
            switch (pps_tab->upa->core_config.at(i).core_instance_nr) {
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

