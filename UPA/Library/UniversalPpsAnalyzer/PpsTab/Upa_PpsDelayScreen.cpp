#include <Upa_PpsDelayScreen.h>
#include <Upa_PpsTab.h>
#include <ui_Upa_PpsDelayScreen.h>

//Upa_PpsDelayScreen::Upa_PpsDelayScreen(QWidget *parent) :
//    QWidget(parent),
//    ui(new Ui::Upa_PpsDelayScreen)
Upa_PpsDelayScreen::Upa_PpsDelayScreen(Upa_PpsTab *parent) : QWidget()
{
    pps_tab = parent;

    ui = new Ui::Upa_PpsDelayScreen();
    ui->setupUi(this);

    connect(ui->PpsDoneButton, SIGNAL(clicked()), this, SLOT(pps_done_button_clicked()));
    connect(ui->PpsChangeDelaysButton, SIGNAL(clicked()), this, SLOT(pps_change_delays_button_clicked()));
    connect(ui->PpsAnalyzerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(pps_analyzer_changed(int)));

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
    for (int i = 0; i < pps_tab->upa->core_config.size(); i++)
    {
        if (ui->PpsAnalyzerComboBox->currentText() == pps_tab->upa->core_config.at(i).com_port)
        {
            switch (pps_tab->upa->core_config.at(i).core_instance_nr) {
                case 1:
                    ui->PpsDelayPpsRefValue->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    break;
                case 2:
                    ui->PpsDelayPps1Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    break;
                case 3:
                    ui->PpsDelayPps2Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    break;
                case 4:
                    ui->PpsDelayPps3Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    break;
                case 5:
                    ui->PpsDelayPps4Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    break;
                case 6:
                    ui->PpsDelayPps5Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    break;
                case 7:
                    ui->PpsDelayPps6Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    break;
                case 8:
                    ui->PpsDelayPps7Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    break;
                case 9:
                    ui->PpsDelayPps8Value->setText(QString::number(*(pps_tab->pps_offset_delays.at(i))));
                    break;
                default:
                    break;
            }
        }
    }
}

