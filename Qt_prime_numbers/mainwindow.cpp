#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <ctime> // why clock() works without this header?
#include <QVector>
#include <cmath>
//============================================================================================
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_searchLimit                  = 0;
    m_primeNumbers                 = 0;
    m_progressBarStep              = 0;
    m_resetButtonClickedAfterStart = false;

    connect(ui->actionSetRussian, SIGNAL(toggled(bool)),
                                this, SLOT(setRussianLanguage(bool))
                                );
    connect(ui->actionSetEnglish, SIGNAL(toggled(bool)),
                                this, SLOT(setEnglishLanguage(bool))
                                );

    ui->spinBox->setButtonSymbols(QSpinBox::PlusMinus); // does not work for some reason (with Qt Designer too)

}
//============================================================================================
MainWindow::~MainWindow()
{
    delete ui;
}
//============================================================================================
void MainWindow::originalPrimeNumberSearchAlgorithm(const size_t NUMBERS) noexcept
{
    size_t checkNumber = 0;
    size_t divider = 0;
    m_primeNumbers = 1;
    bool flag = 0;
    size_t start_time = clock(); // for timer
    size_t k = 0;

    // the cycle iterates over only odd numbers, skipping every third
    for(checkNumber = 3; checkNumber <= NUMBERS; checkNumber+=2)
    {
        size_t passNum = 3; // to skip every third divider (which is not prime)
        for(divider = 3; divider*divider <= checkNumber; divider+=2) // loop iterates over dividers
        {
            if(m_resetButtonClickedAfterStart)
            {
                return;
            }

            if(checkNumber % divider == 0) // prime checking
            {   // if divider was found - it is not prime number, exit the cycle
                // and check next number
                flag = 1;
                break;
            }

            --passNum;
            if(!passNum) // when passNum == 0, renew counter and skipping the divider
            {
                passNum = 2;
                divider += 2;
            }
        }
        if(!flag) // if divider is not found - number is prime
        {
            ++m_primeNumbers;
            ui->lcdPrimeNumbersFound->display(m_primeNumbers);
        }
        else
            flag = 0;

        // progress-bar set
        m_progressBarStep = checkNumber;
        ui->progressBar->setValue(m_progressBarStep);

        // find out the time from the start of the search to the current iteration
        size_t end_time = clock();
        ui->lcdTimePassed->display((double)(end_time-start_time) / 1000);


        if(k % 100000 == 0)
            QApplication::processEvents();
        ++k;
    }
}
//============================================================================================
void MainWindow::sieveOfEratosthenes(size_t const NUMBERS) noexcept
{
    m_primeNumbers = 0;
    int start_time = clock();

    QVector<bool> array(NUMBERS + 1, true);
    array[0] = array[1] = false;

    for(size_t i = 2; i * i <= NUMBERS; ++i) // valid for n < 46340^2 = 2147395600
    {
        if(array[i])
        {
            for(size_t j = i * i; j <= NUMBERS; j += i)
            {
                array[j] = false;
            }
        }
    }

    // counting and output prime numbers of vector
    for(size_t i = 0; i < NUMBERS+1; ++i)
    {
        if(m_resetButtonClickedAfterStart)
        {
            return;
        }

        if(array[i])
        {
            ++m_primeNumbers;
            ui->lcdPrimeNumbersFound->display(m_primeNumbers);

            m_progressBarStep = i;
            ui->progressBar->setValue(m_progressBarStep);

            int end_time = clock();
            ui->lcdTimePassed->display((double)(end_time-start_time) / 1000);
            if(i % 100000 == 0)
                QApplication::processEvents();
        }
    }
}
//============================================================================================
void MainWindow::sieveOfSundaram(size_t NUMBERS) noexcept
{
    NUMBERS /= 2;
    m_primeNumbers = 0;
    int start_time = clock();

    size_t i, j, index;

    QVector<bool> array(NUMBERS+1, true);

    for(i = 1; 2*i*(i+1) < NUMBERS; ++i)
        for(j = i; index = 2*i*j+i+j, index <= NUMBERS; ++j)
            array[index] = false;

    // counting and output prime numbers of vector
    for(size_t i = 0; i < NUMBERS; ++i)
    {
        if(m_resetButtonClickedAfterStart)
        {
            return;
        }

        if(array[i])
        {
            ++m_primeNumbers;
            ui->lcdPrimeNumbersFound->display(m_primeNumbers);

            m_progressBarStep = i * 2;
            ui->progressBar->setValue(m_progressBarStep);

            int end_time = clock();
            ui->lcdTimePassed->display((double)(end_time-start_time) / 1000);
        }
        if(i % 100000 == 0)
            QApplication::processEvents();
    }
}
//============================================================================================
void MainWindow::sieveOfAtkin(size_t const NUMBERS) noexcept
{
    int limit = NUMBERS;
    int sqr_lim;
    QVector<bool> is_prime(limit+1, false);
    int x2, y2;
    int i, j;
    int n;

    int start_time = clock();
    m_primeNumbers = 0; // maybe 2

    // Sieve initialization
    sqr_lim = (int)sqrt((long double)limit);

    is_prime[2] = true;
    is_prime[3] = true;

    // Presumably simple - these are integers with an odd number
    // representations in given square forms.
    // x2 and y2 are the squares i and j (optimization).
    x2 = 0;
    for(i = 1; i <= sqr_lim; ++i)
    {
        x2 += 2 * i - 1;
        y2 = 0;
        for(j = 1; j <= sqr_lim; ++j)
        {
            y2 += 2 * j - 1;

            n = 4 * x2 + y2;
            if ((n <= limit) && (n % 12 == 1 || n % 12 == 5))
                is_prime[n] = !is_prime[n];

            // n = 3 * x2 + y2;
            n -= x2; // optimization
            if ((n <= limit) && (n % 12 == 7))
                is_prime[n] = !is_prime[n];

            // n = 3 * x2 - y2;
            n -= 2 * y2; // optimization
            if ((i > j) && (n <= limit) && (n % 12 == 11))
                is_prime[n] = !is_prime[n];
        }
    }

    // Sift out the multiple squares of primes in the interval [5, sqrt (limit)].
    // (the main stage cannot weed them out)
    for(i = 5; i <= sqr_lim; ++i) {
        if (is_prime[i]) {
            n = i * i;
            for(j = n; j <= limit; j += n)
                is_prime[j] = false;
        }
    }

    // counting and output prime numbers of vector
    for(size_t i = 0; i < NUMBERS; ++i)
    {
        if(m_resetButtonClickedAfterStart)
        {
            return;
        }

        if(is_prime[i])
        {
            ++m_primeNumbers;
            ui->lcdPrimeNumbersFound->display(m_primeNumbers);

            m_progressBarStep = i * 2;
            ui->progressBar->setValue(m_progressBarStep);

            int end_time = clock();
            ui->lcdTimePassed->display((double)(end_time-start_time) / 1000);
        }
        if(i % 100000 == 0)
            QApplication::processEvents();
    }
}
//============================================================================================
void MainWindow::on_startButton_clicked()
{
    m_resetButtonClickedAfterStart = false;
    setAllButtonsDisabled();

    m_searchLimit = ui->spinBox->value();

    if(ui->originRadioButton->isChecked())
        originalPrimeNumberSearchAlgorithm(m_searchLimit);

    if(ui->sieveOfEratosthenesRadioButton->isChecked())
        sieveOfEratosthenes(m_searchLimit);

    if(ui->sieveOfSundaramRadioButton->isChecked())
        sieveOfSundaram(m_searchLimit);

    if(ui->sieveOfAtkinRadioButton->isChecked())
        sieveOfAtkin(m_searchLimit);

    setAllButtonsEnabled();
}
//============================================================================================
void MainWindow::on_radioButton_100000_clicked()
{
    m_searchLimit = 100000;
    ui->spinBox->setValue(m_searchLimit);
    ui->progressBar->setRange(0, m_searchLimit);
    ui->progressBar->reset();
}
//============================================================================================
void MainWindow::on_radioButton_1000000_clicked()
{
    m_searchLimit = 1000000;
    ui->spinBox->setValue(m_searchLimit);
    ui->progressBar->setRange(0, m_searchLimit);
    ui->progressBar->reset();
}
//============================================================================================
void MainWindow::on_radioButton_10000000_clicked()
{
    m_searchLimit = 10000000;
    ui->spinBox->setValue(m_searchLimit);
    ui->progressBar->setRange(0, m_searchLimit);
    ui->progressBar->reset();
}
//============================================================================================
void MainWindow::on_radioButton_100000000_clicked()
{
    m_searchLimit = 100000000;
    ui->spinBox->setValue(m_searchLimit);
    ui->progressBar->setRange(0, m_searchLimit);
    ui->progressBar->reset();
}
//============================================================================================
void MainWindow::on_resetButton_clicked()
{
    m_resetButtonClickedAfterStart = true;
    m_primeNumbers = 0;
    m_progressBarStep = 0;
    ui->progressBar->reset();
    ui->lcdPrimeNumbersFound->display(0);
    ui->lcdTimePassed->display(0);
}
//============================================================================================
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
//============================================================================================
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
//============================================================================================
QMenu& MainWindow::getMainMenu() const
{
    return *(ui->mainMenu);
}
//============================================================================================
void MainWindow::setRussianLanguage(bool signal)
{
    if(!signal)
        return;

    //ui->actionSetEnglish->setEnabled(true);

    ui->actionSetRussian->setChecked(true);
    ui->actionSetEnglish->setChecked(false);

    //ui->actionSetRussian->setEnabled(false);

    ui->mainMenu->setTitle("Меню");
    ui->menuSetLanguage->setTitle("Язык");
    ui->actionExit->setText("Выход");
    ui->groupBoxNNumber->setTitle("Задайте число N:");
    ui->groupBoxAlgorithm->setTitle("Выберите алгоритм:");
    ui->originRadioButton->setText("Стандарт");
    ui->sieveOfEratosthenesRadioButton->setText("Решето Эратосфена");
    ui->sieveOfSundaramRadioButton->setText("Решето Сундарама");
    ui->sieveOfAtkinRadioButton->setText("Решето Аткина");
    ui->timePassedLabel->setText("Времени прошло:");
    ui->primesFoundLabel->setText("Простых чисел найдено:");
    ui->startButton->setText("Старт");
    ui->resetButton->setText("Стоп/Сброс");
}
//============================================================================================
void MainWindow::setEnglishLanguage(bool signal)
{
    if(!signal)
        return;

    //ui->actionSetEnglish->setEnabled(false);

    ui->actionSetRussian->setChecked(false);
    ui->actionSetEnglish->setChecked(true);

    //ui->actionSetRussian->setEnabled(true);

    ui->mainMenu->setTitle("Menu");
    ui->menuSetLanguage->setTitle("Language");
    ui->actionExit->setText("Exit");
    ui->groupBoxNNumber->setTitle("Set the N number:");
    ui->groupBoxAlgorithm->setTitle("Set the algorithm:");
    ui->originRadioButton->setText("Standard");
    ui->sieveOfEratosthenesRadioButton->setText("Sieve of Eratosthenes");
    ui->sieveOfSundaramRadioButton->setText("Sieve of Sundaram");
    ui->sieveOfAtkinRadioButton->setText("Sieve of Atkin");
    ui->timePassedLabel->setText("Time passed:");
    ui->primesFoundLabel->setText("Primes found:");
    ui->startButton->setText("Start");
    ui->resetButton->setText("Stop/Reset");
}
