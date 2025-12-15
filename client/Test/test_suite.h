#pragma once
#include <iostream>
#include <string>
#include <cassert> // <--- Includi questo

class TestSuite {
public:
    // Lancia tutti i test e stampa i risultati in console
    static void runAllTests();

private:
    // Singoli casi di test
    static void testNodeHierarchy();
    static void testNodeTransformations();
    static void testListManagement();
    static void testCameraViewMatrix();

    // Helper per stampare successo/fallimento
    static void assertCondition(const std::string& testName, bool condition);
};