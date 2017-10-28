    //
//  RPCServerCalculator.c
//  
//
//  Created by Nikita-Mac on 28.10.17.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include <math.h>
#include "Calc.h"

double * pm_1_svc(double * operand, struct svc_req * cl)
{
    double temp = *operand;
    static double result = 0;
    result = -temp;
    return &result;
}

double * proc_1_svc(double * operand, struct svc_req * cl)
{
    double temp = *operand;
    static double result = 0;
    result = temp * temp;
    return &result;
}

double * onetox_1_svc(double * operand, struct svc_req * cl)
{
    double temp = *operand;
    static double result = 0;
    result = 1/temp;
    return &result;
}

double * cosinus_1_svc(double * operand, struct svc_req * cl)
{
    double temp = *operand;
    static double result = 0;
    result = cos(temp);
    return &result;
}

double * sinus_1_svc(double * operand, struct svc_req * cl)
{
    double temp = *operand;
    static double result = 0;
    result = sin(temp);
    return &result;
}

double * mysqrt_1_svc(double * operand, struct svc_req * cl)
{
    double temp = *operand;
    static double result = 0;
    result = sqrt(temp);
    return &result;
}

double * myln_1_svc(double * operand, struct svc_req * cl)
{
    double temp = *operand;
    static double result = 0;
    result = log(temp);
    return &result;
}


double * myexp_1_svc(double * operand, struct svc_req * cl)
{
    double temp = *operand;
    static double result = 0;
    result = exp(temp);
    return &result;
}


double * div_1_svc(operands * operand, struct svc_req * cl)
{
    double left = operand->left;
    double right = operand->right;
    static double result = 0;
    result = left / right;
    return &result;
}


double * mul_1_svc(operands * operand, struct svc_req * cl)
{
    double left = operand->left;
    double right = operand->right;
    static double result = 0;
    result = left * right;
    return &result;
}


double * sub_1_svc(operands * operand, struct svc_req * cl)
{
    double left = operand->left;
    double right = operand->right;
    static double result = 0;
    result = left - right;
    return &result;
}


double * add_1_svc(operands * operand, struct svc_req * cl)
{
    double left = operand->left;
    double right = operand->right;
    static double result = 0;
    result = left + right;
    return &result;
}

double * equ_1_svc(operands * operand, struct svc_req * cl)
{
    static double result = 0;
    return &result;
}











