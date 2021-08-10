using NUnit.Framework;
using SimpleService;
using System;

namespace SimpleService.Tests.Nunit
{
    public class CalculatorServiceTests
    {
        private ICalculatorService _calculatorService;
        [SetUp]
        public void Setup()
        {
            _calculatorService = new CalculatorService();
        }

        /// <summary>
        /// Тестирует сложение
        /// </summary>
        [Test]
        public void AddTest()
        {
            Assert.IsNotNull(_calculatorService);
            Assert.AreNotEqual(_calculatorService, null);
            var result = _calculatorService.Add(1, 2);
            Assert.AreEqual(3, result);
        }

        /// <summary>
        /// Тестирует вычитание 
        /// </summary>
        [Test]
        public void SubTest()
        {
            Assert.IsNotNull(_calculatorService);
            Assert.AreNotEqual(_calculatorService, null);
            var result = _calculatorService.Sub(2, 1);
            Assert.AreEqual(1, result);
        }

        [Test]
        public void DivWhenDividerIsZeroTest()
        {
            var exception = Assert.Throws<ArithmeticException>(() => _calculatorService.Div(1,0));
            Assert.IsInstanceOf<ArithmeticException>(exception);
            Assert.IsTrue(exception is ArithmeticException);
            Assert.AreEqual("Делить на нуль нельзя", exception.Message);
        }
    }
}