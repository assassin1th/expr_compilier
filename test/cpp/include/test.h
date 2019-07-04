#ifndef TEST_H_
#define TEST_H_

#include <iostream>
#ifdef __DEBUG__
#define TEST_MSG(stmt, msg)     \
    std::cerr << "msg from file: " << __FILE__ << " line: " << __LINE__ << std::endl;\
    std::cerr << msg << std::endl;\
    stmt;\
    std::cerr << "success executing stmt:" << std::endl << #stmt << std::endl \
              << std::endl;
#else
#define TEST_MSG(stmt, msg) stmt
#endif // __DEBUG__

#endif // TEST_H_
