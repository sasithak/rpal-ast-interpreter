#include <iostream>
#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

class Test : public CPPUNIT_NS::TestCase
{
    CPPUNIT_TEST_SUITE(Test);
    CPPUNIT_TEST(test_01);
    CPPUNIT_TEST(test_02);
    CPPUNIT_TEST(test_03);
    CPPUNIT_TEST(test_04);
    CPPUNIT_TEST(test_05);
    CPPUNIT_TEST(test_06);
    CPPUNIT_TEST(test_07);
    CPPUNIT_TEST(test_08);
    CPPUNIT_TEST(test_09);
    CPPUNIT_TEST(test_10);
    CPPUNIT_TEST(test_11);
    CPPUNIT_TEST(test_12);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void) {}
    void tearDown(void) {}

protected:
    void test_01(void)
    {
        system("./myrpal tests/test_01 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_01.out") == 0);
    }

    void test_02(void)
    {
        system("./myrpal tests/test_02 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_02.out") == 0);
    }

    void test_03(void)
    {
        system("./myrpal tests/test_03 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_03.out") == 0);
    }

    void test_04(void)
    {
        system("./myrpal tests/test_04 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_04.out") == 0);
    }

    void test_05(void)
    {
        system("./myrpal tests/test_05 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_05.out") == 0);
    }

    void test_06(void)
    {
        system("./myrpal tests/test_06 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_06.out") == 0);
    }

    void test_07(void)
    {
        system("./myrpal tests/test_07 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_07.out") == 0);
    }

    void test_08(void)
    {
        system("./myrpal tests/test_08 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_08.out") == 0);
    }

    void test_09(void)
    {
        system("./myrpal tests/test_09 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_09.out") == 0);
    }

    void test_10(void)
    {
        system("./myrpal tests/test_10 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_10.out") == 0);
    }

    void test_11(void)
    {
        system("./myrpal tests/test_11 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_11.out") == 0);
    }

    void test_12(void)
    {
        system("./myrpal tests/test_12 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_12.out") == 0);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(Test);

int main()

{
    CPPUNIT_NS::TestResult controller;

    CPPUNIT_NS::TestResultCollector result;
    controller.addListener(&result);

    CPPUNIT_NS::BriefTestProgressListener progress;
    controller.addListener(&progress);

    CPPUNIT_NS::TestRunner runner;
    runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
    runner.run(controller);

    return result.wasSuccessful() ? 0 : 1;
}
