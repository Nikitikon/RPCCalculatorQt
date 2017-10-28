#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include </Users/nikita-mac/Desktop/WS/Unix/RPCCalc/Calc.h>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Initialization();

    handle = clnt_create(RMACHINE, CALCULATOR_PROG, CALCULATOR_VARS, "tcp");
        if (handle == NULL) {
            qDebug() << "create error";
            exit(0);
        }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Initialization()
{
    connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(DigitButtonPressed()));
    connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(DigitButtonPressed()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(DigitButtonPressed()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(DigitButtonPressed()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(DigitButtonPressed()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(DigitButtonPressed()));
    connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(DigitButtonPressed()));
    connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(DigitButtonPressed()));
    connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(DigitButtonPressed()));
    connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(DigitButtonPressed()));
    connect(ui->pushButtonPi, SIGNAL(clicked()), this, SLOT(DigitButtonPressed()));

    lastNumber = "0";
    resultSoFar = 0.0;
    waitingForOperand = true;

}

void MainWindow::displayNumber()
{
    qreal LCDValue = (lastNumber).toDouble();
    if (LCDValue < qPow(10, 13))
    {
        ui->lcdNumber->display(LCDValue);
    }
}

void MainWindow::DigitButtonPressed()
{

    QPushButton *button = qobject_cast<QPushButton *>(sender());

    if (button->text() == '0' && lastNumber == 0)
        return;

    if (lastNumber.contains('.') && lastNumber.count() >= 13)
        return;

    if (waitingForOperand)
    {
        lastNumber = "0";
        ui->lcdNumber->display(0);
        waitingForOperand = false;
    }

    if (button->text() == "pi")
    {
        lastNumber = Pi;
        displayNumber();
        return;
    }

    if (lastNumber.count() >= 13)
        return;

    if (lastNumber == "0")
        lastNumber = button->text();
    else
        lastNumber += button->text();

    displayNumber();
}



void MainWindow::on_pushButton_div_clicked()
{
    deleteLastDot();
        double operand = lastNumber.toDouble();
        if (!pendingOperator.isEmpty())
        {
            if (!calculate(operand))
            {
                invalidOperation();
                return;
            }

            lastNumber = QString::number(resultSoFar, 'f', 12);
            deleteLastDot();
            displayNumber();
        }
        else
        {
            resultSoFar = operand;
        }

        pendingOperator = "/";
        pendingOperatorForEqual = "/";
        equalOperand = 0;
        waitingForOperand = true;
}

void MainWindow::on_pushButton_com_clicked()
{
    if(waitingForOperand)
    {
         ui->lcdNumber->display(0);
         lastNumber = "0";
    }

    if(lastNumber.contains('.') || lastNumber.count() >= 13)
         return;
    lastNumber += '.';

    ui->lcdNumber->display(lastNumber);

    waitingForOperand = false;
}

void MainWindow::on_pushButtonC_clicked()
{
    if (waitingForOperand)
            return;

        lastNumber = "0";
        ui->lcdNumber->display(0);
        waitingForOperand = true;
}

void MainWindow::on_pushButton_AC_clicked()
{
    waitingForOperand = true;
    lastNumber = "0";
    resultSoFar = 0;
    pendingOperator.clear();
    pendingOperatorForEqual.clear();
    equalOperand = 0;

    ui->lcdNumber->display(0);
}

void MainWindow::on_pushButton_BS_clicked()
{
    if (lastNumber == "0")
        return;

    if (lastNumber.count() == 1 || lastNumber.count() == 2 && lastNumber.contains("-"))
    {
        lastNumber = "0";
        ui->lcdNumber->display(0);
        return;
    }


    lastNumber.remove(lastNumber.count() - 1, lastNumber.count() - 1);


    displayNumber();
}

void MainWindow::invalidOperation()
{
    ui->pushButton_AC->clicked();
    ui->lcdNumber->display("0000");
}

void MainWindow::deleteLastDot()
{
    if (lastNumber.contains("."))
    {
        int index = lastNumber.indexOf('.');
        int count = lastNumber.count();

        int onlyZeroIndex = 0;

        for (int i = count - 1; i > index; i--)
        {
            if (lastNumber[i] == '0')
                onlyZeroIndex += 1;

            if (lastNumber[i] != '0')
                break;
        }

        int deleteCount = count - onlyZeroIndex;
        lastNumber.remove(deleteCount, count);

        count = lastNumber.count();

        if (index == count - 1)
        {
            lastNumber.remove(count- 1, count - 1);
            ui->lcdNumber->display(lastNumber);
        }
    }
}

void MainWindow::on_pushButton_pm_clicked()
{
    deleteLastDot();

        if (lastNumber == "0")
            return;

        if (!lastNumber.contains('-'))
            lastNumber.insert(0, '-');
        else
            lastNumber.remove(0,1);

        displayNumber();
}

void MainWindow::on_pushButton_proc_clicked()
{
    deleteLastDot();

        double variableCalc = lastNumber.toDouble();

        int index = lastNumber.indexOf('.');

        if(index >= 6 || index == -1 && lastNumber.count() >= 6)
            return;

        double result = *proc_1(&variableCalc, handle);

        if (lastNumber.contains('.'))
            lastNumber = QString::number(result, 'f', 12);
        else
            lastNumber = QString::number(result, 'f', 0);

        if (lastNumber.count() > 13)
            result = variableCalc;

        deleteLastDot();
        displayNumber();
        waitingForOperand = true;

}

void MainWindow::on_pushButton_1X_clicked()
{
    deleteLastDot();

        if (lastNumber == "0")
        {
            invalidOperation();
            return;
        }

        double operand = lastNumber.toDouble();
        double result = *onetox_1(&operand, handle);

        if (result < qPow(10, -13))
            lastNumber = "0";
        else
            lastNumber = QString::number(result, 'g', 12);

        if (lastNumber.count() > 13)
            result = operand;

        deleteLastDot();
        displayNumber();
        waitingForOperand = true;
}

void MainWindow::on_pushButton_Cos_clicked()
{
    deleteLastDot();

    double operand = lastNumber.toDouble();
    double result = * cosinus_1(&operand, handle);

    lastNumber = QString::number(result, 'g', 11);
    deleteLastDot();
    displayNumber();
    waitingForOperand = true;
}

void MainWindow::on_pushButton_Sin_clicked()
{
    deleteLastDot();

    double operand = lastNumber.toDouble();
    double result = * sinus_1(&operand, handle);

    if (lastNumber == Pi)
        result = 0;

    lastNumber = QString::number(result, 'g', 11);
    deleteLastDot();
    displayNumber();
    waitingForOperand = true;
}

void MainWindow::on_pushButton_Sqrt_clicked()
{
    deleteLastDot();

    double operand = lastNumber.toDouble();
    if (operand < 0)
    {
        invalidOperation();
        return;
    }

    double result = * mysqrt_1(&operand, handle);

    lastNumber = QString::number(result, 'g', 11);

    if (lastNumber.count() > 13)
        result = operand;

    deleteLastDot();
    displayNumber();
    waitingForOperand = true;
}

void MainWindow::on_pushButton_ln_clicked()
{
    deleteLastDot();
    double operand = lastNumber.toDouble();
    if (operand <= 0)
    {
        invalidOperation();
        return;
    }

    double result = * myln_1(&operand, handle);

    if (result == INFINITY)
            result = operand;

    lastNumber = QString::number(result, 'g', 10);

    if (lastNumber.count() > 13)
        result = operand;

    deleteLastDot();
    displayNumber();
    waitingForOperand = true;

}

void MainWindow::on_pushButton_Exp_clicked()
{
    double operand = lastNumber.toDouble();
    double result = * myexp_1(&operand, handle);

    if (result == INFINITY)
        result = operand;

    lastNumber = QString::number(result, 'g', 11);

    if (lastNumber.count() > 13)
        result = operand;

    deleteLastDot();
    displayNumber();
    waitingForOperand = true;

}

bool MainWindow::calculate(double rightOperand)
{
    double temp = resultSoFar;
    operands operand;
    operand.left = resultSoFar;
    operand.right = rightOperand;

        if (pendingOperator == "+")
        {
                 resultSoFar = *add_1(&operand, handle);
        }
        else
            if (pendingOperator == "-")
            {
                 resultSoFar = * sub_1(&operand, handle);
            }
            else
                if (pendingOperator == "X")
                {
                    resultSoFar = * mul_1(&operand, handle);
                }
                else
                    if (pendingOperator == "/")
                    {
                        if (rightOperand == 0.0)
                        return false;
                        resultSoFar = * div_1(&operand, handle);
                    }

        if (resultSoFar < qPow(10, 13) && resultSoFar > qPow(10, -13) ||
                resultSoFar > -qPow(10, 12) && resultSoFar < -qPow(10, -12) ||
                resultSoFar == 0)
        {
            return true;
        }
        else {
            resultSoFar = temp;
        }
        return true;
}

void MainWindow::on_pushButton_mul_clicked()
{
    deleteLastDot();
    double operand = lastNumber.toDouble();
    if (!pendingOperator.isEmpty())
    {
        if (!calculate(operand))
        {
            invalidOperation();
            return;
        }

        lastNumber = QString::number(resultSoFar, 'f', 12);
        deleteLastDot();
        displayNumber();
    }
    else
    {
        resultSoFar = operand;
    }

    pendingOperator = "X";
    pendingOperatorForEqual = "X";
    equalOperand = 0;
    waitingForOperand = true;
}

void MainWindow::on_pushButton_sub_clicked()
{
    deleteLastDot();
    double operand = lastNumber.toDouble();
    if (!pendingOperator.isEmpty())
    {
        if (!calculate(operand))
        {
            invalidOperation();
            return;
        }

        lastNumber = QString::number(resultSoFar, 'f', 12);
        deleteLastDot();
        displayNumber();
    }
    else
    {
        resultSoFar = operand;
    }

    pendingOperator = "-";
    pendingOperatorForEqual = "-";
    equalOperand = 0;
    waitingForOperand = true;
}

void MainWindow::on_pushButton_add_clicked()
{
    deleteLastDot();
    double operand = lastNumber.toDouble();
    if (!pendingOperator.isEmpty())
    {
        if (!calculate(operand))
        {
            invalidOperation();
            return;
        }

        lastNumber = QString::number(resultSoFar, 'f', 12);
        deleteLastDot();
        displayNumber();
    }
    else
    {
        resultSoFar = operand;
    }

    pendingOperator = "+";
    pendingOperatorForEqual = "+";
    equalOperand = 0;
    waitingForOperand = true;
}

void MainWindow::on_pushButton_equ_clicked()
{
    deleteLastDot();
    if (equalOperand == 0)
        equalOperand = lastNumber.toDouble();
    pendingOperator = pendingOperatorForEqual;
    if (!pendingOperator.isEmpty())
    {
        if (!calculate(equalOperand))
        {
            invalidOperation();
            return;
        }

        lastNumber = QString::number(resultSoFar, 'f', 12);
        deleteLastDot();
        displayNumber();
    }

    pendingOperator.clear();
    waitingForOperand = true;
}
