#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <ctime> // почему clock() работает без этого хедера?

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_searchLimit = 0;
    m_primeNumbers = 0;
    m_progressBarStep = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::originalPrimeNumberSearchAlgorithm(const int NUMBERS) noexcept
{
    int checkNumber = 0; // Проверяемое число
    int delitel = 0; // Подборка делителя
    m_primeNumbers = 1; // Количество всех простых чисел
    bool flag = 0; // Бинарный маркер
    int start_time = clock(); // для таймера

    // Цикл перебирает только нечётные числа, пропуская каждое третье
    for(checkNumber = 3; checkNumber <= NUMBERS; checkNumber+=2)
    {
        unsigned int passNum = 3; // Для пропуска каждого третьего делителя (не являющегося простым)
        for(delitel = 3; delitel*delitel <= checkNumber; delitel+=2) // Цикл перебирает делители
        {

            if(checkNumber % delitel == 0) // Проверка на простоту
            {   // Если делитель найден - число составное, выход из цикла
                // и проверка следующего числа checkNumber
                flag = 1;
                break;
            }

            --passNum;
            if(!passNum) // Когда passNum == 0, восстанавливаем счетчик и пропускаем делитель
            {
                passNum = 2;
                delitel += 2;
            }
        }
        if(!flag) // Если делитель не найден - число простое
        {
            ++m_primeNumbers;
            ui->lcdPrimeNumbersFound->display(m_primeNumbers);
        }
        else
            flag = 0;

        // Настройка програсс-бара
        m_progressBarStep = checkNumber;
        ui->progressBar->setValue(m_progressBarStep);

        // Узнаём время c момента начала поиска до текущей итерации
        int end_time = clock();
        ui->lcdTimePassed->display((double)(end_time-start_time) / 1000);
    }
}

// -запретить менять N после нажатия кнопки
void MainWindow::on_startButton_clicked()
{
    setAllButtonsDisabled();

    m_searchLimit = ui->spinBox->value();

    if(ui->originRadioButton->isChecked())
        originalPrimeNumberSearchAlgorithm(m_searchLimit);

    // Здесь будут другие алгоритмы

    setAllButtonsEnabled();



}

void MainWindow::on_radioButton_100000_clicked()
{
    m_searchLimit = 100000;
    ui->spinBox->setValue(m_searchLimit);
    ui->progressBar->setRange(0, m_searchLimit);
    ui->progressBar->reset();
}

void MainWindow::on_radioButton_1000000_clicked()
{
    m_searchLimit = 1000000;
    ui->spinBox->setValue(m_searchLimit);
    ui->progressBar->setRange(0, m_searchLimit);
    ui->progressBar->reset();
}

void MainWindow::on_radioButton_10000000_clicked()
{
    m_searchLimit = 10000000;
    ui->spinBox->setValue(m_searchLimit);
    ui->progressBar->setRange(0, m_searchLimit);
    ui->progressBar->reset();
}

void MainWindow::on_radioButton_100000000_clicked()
{
    m_searchLimit = 100000000;
    ui->spinBox->setValue(m_searchLimit);
    ui->progressBar->setRange(0, m_searchLimit);
    ui->progressBar->reset();
}

void MainWindow::on_resetButton_clicked()
{
    m_primeNumbers = 0;
    m_progressBarStep = 0;
    ui->progressBar->reset();
    ui->lcdPrimeNumbersFound->display(0);
    ui->lcdTimePassed->display(0);
}

void MainWindow::setAllButtonsDisabled() noexcept
{
    ui->startButton->setEnabled(false);

    ui->spinBox->setEnabled(false);

    ui->radioButton_100000->setEnabled(false);
    ui->radioButton_1000000->setEnabled(false);
    ui->radioButton_10000000->setEnabled(false);
    ui->radioButton_100000000->setEnabled(false);

    ui->originRadioButton->setEnabled(false);
    ui->sieveOfEratosthenesRadioButton->setEnabled(false);
    ui->sieveOfSundaramRadioButton->setEnabled(false);
    ui->sieveOfAtkinRadioButton->setEnabled(false);
}

void MainWindow::setAllButtonsEnabled() noexcept
{
    ui->startButton->setEnabled(true);

    ui->spinBox->setEnabled(true);

    ui->radioButton_100000->setEnabled(true);
    ui->radioButton_1000000->setEnabled(true);
    ui->radioButton_10000000->setEnabled(true);
    ui->radioButton_100000000->setEnabled(true);

    ui->originRadioButton->setEnabled(true);
    ui->sieveOfEratosthenesRadioButton->setEnabled(true);
    ui->sieveOfSundaramRadioButton->setEnabled(true);
    ui->sieveOfAtkinRadioButton->setEnabled(true);
}

