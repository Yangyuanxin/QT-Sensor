#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::FunCal(int arr[20], int n, int *max, int *min, float *average)
{
    int i = 0;
    float ave = 0.0;
    int _max = 0, _min = 0, sum = 0;

    _min = arr[0];

    for(i=1; i < n; i++)
    {
        if(_max < arr[i])
        {
            _max = arr[i];
            *max = _max;
        }
        if(_min > arr[i])
        {
            _min = arr[i];
            *min = _min;
        }
    }

    for(i=0;i < n; i++)
    {
        sum += arr[i];
    }

    ave = sum / (float)n;
    *average = ave;

}

int MainWindow::SetBacklight(int Level)
{
    int Ret = -1;
    FILE *Fp = NULL;
    char Buf[10] = {0};

    Fp = fopen("/sys/class/backlight/backlight/brightness", "w");
    if(NULL == Fp)
    {
        printf("fopen backlight dev fail!\n");
        return -1;
    }

    snprintf(Buf, sizeof(Buf), "%d", Level);
    Buf[strlen(Buf)] = '\0';

    Ret = fwrite(Buf, strlen(Buf) + 1, 1, Fp);
    if(1 != Ret)
    {
        printf("fwrite fail!\n");
        return -1;
    }

    fflush(Fp);
    fclose(Fp);
    Fp = NULL;

    return 0;
}
