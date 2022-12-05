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
    CPPUNIT_TEST(test_13);
    CPPUNIT_TEST(test_14);
    CPPUNIT_TEST(test_15);
    CPPUNIT_TEST(test_16);
    CPPUNIT_TEST(test_17);
    CPPUNIT_TEST(test_18);
    CPPUNIT_TEST(test_19);
    CPPUNIT_TEST(test_20);
    CPPUNIT_TEST(test_21);
    CPPUNIT_TEST(test_22);
    CPPUNIT_TEST(test_23);
    CPPUNIT_TEST(test_24);
    CPPUNIT_TEST(test_25);
    CPPUNIT_TEST(test_26);
    CPPUNIT_TEST(test_27);
    CPPUNIT_TEST(test_28);
    CPPUNIT_TEST(test_29);
    CPPUNIT_TEST(test_30);
    CPPUNIT_TEST(test_31);
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

    void test_13(void)
    {
        system("./myrpal tests/test_13 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_13.out") == 0);
    }

    void test_14(void)
    {
        system("./myrpal tests/test_14 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_14.out") == 0);
    }

    void test_15(void)
    {
        system("./myrpal tests/test_15 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_15.out") == 0);
    }

    void test_16(void)
    {
        system("./myrpal tests/test_16 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_16.out") == 0);
    }

    void test_17(void)
    {
        system("./myrpal tests/test_17 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_17.out") == 0);
    }

    void test_18(void)
    {
        system("./myrpal tests/test_18 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_18.out") == 0);
    }

    void test_19(void)
    {
        system("./myrpal tests/test_19 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_19.out") == 0);
    }

    void test_20(void)
    {
        system("./myrpal tests/test_20 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_20.out") == 0);
    }

    void test_21(void)
    {
        system("./myrpal tests/test_21 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_21.out") == 0);
    }

    void test_22(void)
    {
        system("./myrpal tests/test_22 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_22.out") == 0);
    }

    void test_23(void)
    {
        system("./myrpal tests/test_23 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_23.out") == 0);
    }

    void test_24(void)
    {
        system("./myrpal tests/test_24 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_24.out") == 0);
    }

    void test_25(void)
    {
        system("./myrpal tests/test_25 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_25.out") == 0);
    }

    void test_26(void)
    {
        system("./myrpal tests/test_26 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_26.out") == 0);
    }

    void test_27(void)
    {
        system("./myrpal tests/test_27 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_27.out") == 0);
    }

    void test_28(void)
    {
        system("./myrpal tests/test_28 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_28.out") == 0);
    }

    void test_29(void)
    {
        system("./myrpal tests/test_29 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_29.out") == 0);
    }

    void test_30(void)
    {
        system("./myrpal tests/test_30 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_30.out") == 0);
    }

    void test_31(void)
    {
        system("./myrpal tests/test_31 >output 2>output");
        CPPUNIT_ASSERT(system("diff output tests/out/test_31.out") == 0);
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
