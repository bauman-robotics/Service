using System;
using System.Collections.Generic;
using System.Text;

namespace SimpleService
{
    public interface ICalculatorService
    {
        double Add(double a, double b);

        double Sub(double a, double b);
    }
}
