#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{   Q_OBJECT
private:
    Ui::MainWindow *ui;

    int m_searchLimit;
    int m_primeNumbers;
    int m_progressBarStep;

    // Algorithms:
    void originalPrimeNumberSearchAlgorithm(const size_t NUMBERS) noexcept;
    void sieveOfEratosthenes(const size_t NUMBERS) noexcept;
    void sieveOfSundaram(size_t NUMBERS) noexcept;
    void sieveOfAtkin(size_t const NUMBERS) noexcept;

    // Controls all buttons except resetButton
    void setAllButtonsDisabled() noexcept;
    void setAllButtonsEnabled() noexcept;

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    QMenu& getMainMenu() const;

public slots:
    void setRussianLanguage(bool signal);
    void setEnglishLanguage(bool signal);

private slots:
    void on_startButton_clicked();
    void on_resetButton_clicked();
    void on_radioButton_100000_clicked();
    void on_radioButton_1000000_clicked();
    void on_radioButton_10000000_clicked();
    void on_radioButton_100000000_clicked();
};
