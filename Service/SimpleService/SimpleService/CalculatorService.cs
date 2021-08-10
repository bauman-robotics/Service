using System;

namespace SimpleService
{
    public class CalculatorService : ICalculatorService
    {
        public double Add(double a, double b)
        {
            return a + b;
        }

        public double Sub(double a, double b)
        {
            return a - b;
        }

        public double Div(double a, double b)
        {
            if (b == 0) throw new ArithmeticException("Делить на нуль нельзя");

            return a / b;
        }
    }
}
